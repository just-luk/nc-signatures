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
#define N 10

using namespace pbc;
using namespace std;
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
   ifstream f("./pbc/param/a.param", ios::in);
   ifstream input_file(argv[1], ios::in);
   type_a_param = string(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
   
   string test_file = string(istreambuf_iterator<char>(input_file), istreambuf_iterator<char>());
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
     printf("\n\U00002705 Vector 1 verified!\n");
   }
   if(!verify(h, public_key, vector2, N, sig1, id, pairing)) {
     printf("\n\U00002705 Vector 2 doesn't verify under vector 1 signature!\n");
   }
   if(verify(h, public_key, vector2, N, sig2, id, pairing)) {
     printf("\n\U00002705 Vector 2 verified!\n");
   }
   if(!verify(h, public_key, vector1, N, sig2, id, pairing)) {
     printf("\n\U00002705 Vector 1 doesn't verify under vector 2 signature!\n\n");
   }
   
   printf("Length of file: %lu\n", test_file.length());
   string compressed = sig1.to_bytes_compressed();
   int len = compressed.size();
   size_t symbols = 10;
   size_t header_size = sizeof(bool) + sizeof(uint32_t) + len + id.size();
   size_t symbol_bytes = 1400 - header_size;
   
   kodo::finite_field field = kodo::finite_field::binary8;
   kodo::block::encoder encoder(field);
   kodo::block::decoder decoder(field);
   kodo::block::generator::random_uniform generator(field);
   encoder.configure(symbols, symbol_bytes);
   decoder.configure(symbols, symbol_bytes);
   generator.configure(encoder.symbols());
   
   vector<uint8_t> data_in(test_file.begin(), test_file.end());
   vector<uint8_t> coefficients(generator.max_coefficients_bytes());
   vector<uint8_t> symbol(encoder.symbol_bytes() + header_size);
   vector<uint8_t> data_out(data_in.size());
   encoder.set_symbols_storage(data_in.data());
   decoder.set_symbols_storage(data_out.data());
   uint32_t seed = rand();
   
   bool is_systematic = false;
   generator.set_seed(seed);
   memcpy(symbol.data(), &is_systematic, sizeof(bool));
   memcpy(symbol.data() + sizeof(bool), &seed, sizeof(uint32_t));
   memcpy(symbol.data() + sizeof(bool) + sizeof(uint32_t), &compressed, len);
   memcpy(symbol.data() + sizeof(bool) + sizeof(uint32_t) + len, &id, id.size());
   generator.generate(coefficients.data());
   encoder.encode_symbol(symbol.data() + header_size,
                                      coefficients.data());
   for (int i = 0; i < symbol.size(); i++) {
        printf("%d ",symbol[i]);
   }
   printf("\n");
   return 0;
}
