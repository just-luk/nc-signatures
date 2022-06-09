#include <pbc/pbc.h>
#define N 10

// TODO Replace hash with hashes of identifiers
// Also, add SHA256 when hashing
static void aggregate_hash(element_ptr a_hash, element_t vector[], int len, pairing_t pairing) {
  element_t temp, hash;
  element_init_G1(temp, pairing);
  element_init_G1(hash, pairing);
  element_from_hash(hash, "hashofmessage", 13);
  element_pow_zn(a_hash, hash, vector[0]);
  for(int i = 1; i < len; i++) {
    element_pow_zn(temp, hash, vector[i]);
    element_mul(a_hash, a_hash, temp);
  }
}

static void sign(element_ptr sig, element_t vec[], int len, element_t sk, pairing_t pair) {
  element_t hashed;
  element_init_G1(hashed, pair);
  aggregate_hash(hashed, vec, len, pair);
  element_pow_zn(sig, hashed, sk);
}

static int verify(element_t h, element_t u, element_t y[], int len, element_t sig, pairing_t pairing) {
  element_t part1, part2, hashed;
  element_init_G1(hashed, pairing);
  element_init_GT(part1, pairing);
  element_init_GT(part2, pairing);
  aggregate_hash(hashed, y, len, pairing);
  element_pairing(part1, sig, h);
  element_pairing(part2, hashed, u);
  return !element_cmp(part1, part2);
}

int main(void) {
  pairing_t pairing;
  element_t h;
  element_t public_key, sig1, sig2;
  element_t secret_key;
  element_t vector1[N], vector2[N];
  pbc_param_t par;
  // change to proper generatorion
  pbc_param_init_a_gen(par, 160, 512);z
  // pbc_param_out_str(stdout, par);
  pairing_init_pbc_param(pairing, par);

  element_init_G2(h, pairing);
  element_init_G2(public_key, pairing);
  element_init_G1(sig1, pairing);
  element_init_G1(sig2, pairing);
  element_init_Zr(secret_key, pairing);
  
  for(int i = 0; i < N; i++) {
    element_init_Zr(vector1[i], pairing);
    element_random(vector1[i]);
    element_init_Zr(vector2[i], pairing);
    element_random(vector2[i]);
  }

  // generate private key
  element_random(secret_key);
  element_printf("private key\nalpha = %B\n\n", secret_key);

  // generate public key
  element_random(h);
  element_pow_zn(public_key, h, secret_key);
  element_printf("public key\n h = %B\n u = %B\n\n", h, public_key);
  // find signature of both vectors
  sign(sig1, vector1, N, secret_key, pairing);
  element_printf("signature of vector 1 = %B\n", sig1);
  sign(sig2, vector2, N, secret_key, pairing);
  element_printf("signature of vector 2 = %B\n", sig2);
  
  // check that the signatures work
  if(verify(h, public_key, vector1, N, sig1, pairing)) {
    printf("\n\U00002705 Vector 1 verified!\n");
  }
  if(!verify(h, public_key, vector2, N, sig1, pairing)) {
    printf("\n\U00002705 Vector 2 doesn't verify under vector 1 signature!\n");
  }
  if(verify(h, public_key, vector2, N, sig2, pairing)) {
    printf("\n\U00002705 Vector 2 verified!\n");
  }
  if(!verify(h, public_key, vector1, N, sig2, pairing)) {
    printf("\n\U00002705 Vector 1 doesn't verify under vector 2 signature!\n\n");
  }

  return 0;
}
