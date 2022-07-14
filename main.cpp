#include <iostream>
#include <mcl/bls12_381.hpp>
#include <time.h>

using namespace mcl::bls12;

void Combine(G1 &out, std::vector<G1> &sigs, std::vector<Fr> &coeffs) {
  assert(sigs.size() == coeffs.size());
  G1::mulVec(out, sigs.data(), coeffs.data(), sigs.size());
}

void AggregateHash(G1 &P, std::vector<Fr> &vec, const std::string &id) {
  G1 hashedID;
  hashAndMapToG1(hashedID, id);
  std::vector<G1> hashVec(vec.size(), hashedID);
  G1::mulVecMT(P, hashVec.data(), vec.data(), vec.size(), 4);
}

void KeyGen(Fr &alpha, G2 &pub, G2 &h) {
  mapToG2(h, rand());
  alpha.setRand();
  G2::mul(pub, h, alpha); // pub = h * alpha
}

void Sign(G1 &sign, const Fr &secret, std::vector<Fr> &vector,
          const std::string &id) {
  AggregateHash(sign, vector, id);
  G1::mul(sign, sign, secret);
}

bool Verify(const G1 &sign, const G2 &u, const G2 &h, std::vector<Fr> &vector,
            const std::string &id) {
  Fp12 e1, e2;
  G1 hashed;
  AggregateHash(hashed, vector, id);
  pairing(e1, sign, h);   // e1 = e(sign, Q)
  pairing(e2, hashed, u); // e2 = e(Hm, sQ)
  return e1 == e2;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  std::string id = argc == 1 ? "hello mcl" : argv[1];

  // setup parameter
  initPairing();

  // generate secret key and public key
  Fr alpha;
  G2 u, h;
  KeyGen(alpha, u, h);
  std::cout << "secret key " << std::endl;
  std::cout << "\talpha = " << alpha << std::endl;
  std::cout << "public key " << std::endl;
  std::cout << "\th = " << h << std::endl;
  std::cout << "\tu = " << u << std::endl;

  // create random vector
  std::vector<Fr> vec(1000);
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i].setRand();
  }

  // sign
  G1 sign;
  Sign(sign, alpha, vec, id);
  std::cout << "signature = " << sign << std::endl;

  // verify
  bool ok = Verify(sign, u, h, vec, id);
  std::cout << "verify " << (ok ? "ok" : "ng") << std::endl;
}
