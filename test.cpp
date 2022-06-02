#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <sha256.h>
#include <pbcbind/pbc.hpp>
#include <kodo/finite_field.hpp>
#include <kodo/block/decoder.hpp>
#include <kodo/block/encoder.hpp>
#include <kodo/block/generator/random_uniform.hpp>
#include <fifi/utils.hpp>
#define N 10

using namespace pbc;
using namespace std;

uint32_t get_value(kodo::finite_field field, const uint8_t* elements,
                          std::size_t index)
{
        return fifi::get_value(fifi::finite_field::prime2325, elements, index);
}

string random_string(int len)
{
     string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

     random_device rd;
     mt19937 generator(rd());

     shuffle(str.begin(), str.end(), generator);

     return str.substr(0, len);        
}

void element_printf(const char* str, Element a) {
   printf(str, a.to_str().c_str());
}

// TODO Speed up aggregate hashing
Element aggregate_hash(Element vec[], int len, string id, PairingPtr pairing) {
   Element tmp, hash_val;
   tmp.init_g1(pairing);
   hash_val.init_g1(pairing);
   vector<unsigned char> hash(picosha2::k_digest_size);
   tmp.set_one();
   string m, message;
   for(int i = 0; i < len; i++) {
      m = id + to_string(i);
      picosha2::hash256(m.begin(), m.end(), hash.begin(), hash.end());
      message = string(hash.begin(), hash.end());
      hash_val = Element::hash_to_point(pairing, hash_val.type(), message);
      tmp = tmp * hash_val.pow(vec[i]);
   }
   return tmp;
}

Element sign(Element v[], int len, Element alpha, string id, PairingPtr pairing) {
   Element hash_digest = aggregate_hash(v, len, id, pairing);
   return hash_digest.pow(alpha);
}

int verify(Element h, Element u, Element y[], int len, Element sig, string id, PairingPtr pairing) {
   Element hash_out = aggregate_hash(y, len, id, pairing);
   return e(sig, h) == e(hash_out, u);
}

int main(int argc, char** argv) {
   srand(static_cast<uint32_t>(time(0)));
   PairingPtr pairing;
   string type_a_param;
   if(argc == 1) {
      printf("Please provide the file to encode!\n");
      return 0;
   }
   string id = argv[1] + random_string(10);
   // ifstream f("./pbc/param/a.param", ios::in);
   ifstream f("./pbc/param/2325.param", ios::in);
   ifstream input_file(argv[1], ios::in);
   type_a_param = string(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
   string test_file = string(istreambuf_iterator<char>(input_file), istreambuf_iterator<char>());
   printf("Length of file is %lu bytes\n", test_file.length());
   
   pairing = Pairing::init_from_param_str(type_a_param);
   
   Element h, public_key, secret_key;
   Element vector1[N], vector2[N];
   h.init_g2(pairing);
   public_key.init_g2(pairing);
   secret_key.init_zr(pairing);
   
   for(int i = 0; i < N; i++) {
      vector1[i].init_zr(pairing);
      vector1[i].random();
      vector2[i].init_zr(pairing);
      vector2[i].random();
   }
   
   // generate private key
   secret_key.random();
   element_printf("Private Key\nalpha = %s\n\n", secret_key);
   
   // generate public key
   h.random();
   public_key = h.pow(secret_key);
   element_printf("Public Key\nh = %s\n", h);
   element_printf("u = %s\n\n", public_key);
   Element sig1 = sign(vector1, N, secret_key, id, pairing);
   element_printf("signature of vector1 = %s\n", sig1);
   Element sig2 = sign(vector2, N, secret_key, id, pairing);
   element_printf("signature of vector2 = %s\n", sig2);
   
   if(verify(h, public_key, vector1, N, sig1, id, pairing)) {
     printf("\nVector 1 verified! \U00002705\n");
   }
   if(!verify(h, public_key, vector2, N, sig1, id, pairing)) {
     printf("\nVector 2 doesn't verify under vector 1 signature! \U00002705\n");
   }
   if(verify(h, public_key, vector2, N, sig2, id, pairing)) {
     printf("\nVector 2 verified! \U00002705\n");
   }
   if(!verify(h, public_key, vector1, N, sig2, id, pairing)) {
     printf("\nVector 1 doesn't verify under vector 2 signature! \U00002705\n\n");
   }
   
   mpz_class t = pairing->g1_order();
   gmp_printf("Order = %Zd\n", t.get_mpz_t());
   
   auto field = kodo::finite_field::prime2325;
   
   kodo::block::encoder encoder(field);
   kodo::block::decoder decoder(field);
   // number of vectors
   auto symbols = 10;

   // Pick the size of each vector in bytes AKA N
   auto symbol_bytes = 1400;
   
   encoder.configure(symbols, symbol_bytes);
   decoder.configure(symbols, symbol_bytes);

   kodo::block::generator::random_uniform generator(field);
   generator.configure(encoder.symbols());

   // storage for each vector
   std::vector<uint8_t> symbol(encoder.symbol_bytes());

   // coefficients
   std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());
   
   std::vector<uint8_t> data_in(encoder.block_bytes());
   
   std::generate(data_in.begin(), data_in.end(), rand);

   encoder.set_symbols_storage(data_in.data());
   auto acc_field = encoder.field();
   std::vector<uint8_t> data_out(decoder.block_bytes());
   decoder.set_symbols_storage(data_out.data());


    // Lose packets with 10% probability
   auto loss_propability = 10;

   while (!decoder.is_complete())
   {
       {
           printf("coded symbol");

           // Draw a seed
           auto seed = rand();
           generator.set_seed(seed);

           generator.generate(coefficients.data());

           // Encode a symbol into the symbol buffer
           encoder.encode_symbol(symbol.data(), coefficients.data());
           
           // 350 bc there are 4 bits in a byte
           for(int i = 0; i < 350; i++) {
           	uint32_t test_out = get_value(field, symbol.data(), i);
           	printf("(%u)", test_out);
           }

           // Drop packet based on loss probability
           if (rand() % 100 < loss_propability)
           {
               printf(" - lost\n");
               continue;
           }

	   // decode using seed
           generator.set_seed(seed);
           generator.generate(coefficients.data());
           decoder.decode_symbol(symbol.data(), coefficients.data());
           printf(" - decoded, rank now %lu\n", decoder.rank());
       }
   }

   // Check if we properly decoded the data
   if (data_out == data_in)
   {
       printf("Data decoded correctly\n");
   }
   return 0;
}
