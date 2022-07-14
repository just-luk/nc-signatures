#include <iostream>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <time.h>

using namespace mcl::bls12;

void Combine(G1 &out, std::vector <G1> &sigs, std::vector <Fr> &coeffs) {
    assert(sigs.size() == coeffs.size());
    G1::mulVec(out, sigs.data(), coeffs.data(), sigs.size());
}

void AggregateHash(G1 &P, std::vector <Fr> &vec, std::vector <G1> &generators, const std::string &id) {
    int n = generators.size();
    int m = vec.size() - n;
    G1 hashedID;
    for (int i = 0; i < m; i++) {
        if (!vec[i + n].isZero()) {
            hashAndMapToG1(hashedID, id + std::to_string(i));
        }
    }
    G1::mulVec(P, generators.data(), vec.data(), n);
    P += hashedID;
}

void KeyGen(Fr &alpha, G2 &pub, G2 &h, std::vector <G1> &generators) {
    mapToG2(h, rand());
    alpha.setRand();
    G2::mul(pub, h, alpha); // pub = h * alpha
    for (int i = 0; i < generators.size(); i++) {
        generators[i].x.setRand();
        generators[i].y.setRand();
    }
}

void Sign(G1 &sign, const Fr &secret, std::vector <Fr> &vector, std::vector <G1> &generators, const std::string &id) {
    AggregateHash(sign, vector, generators, id);
    G1::mul(sign, sign, secret);
}

bool Verify(const G1 &sign, const G2 &u, const G2 &h, std::vector <Fr> &vector, std::vector <G1> &generators,
            const std::string &id) {
    Fp12 e1, e2;
    G1 hashed;
    AggregateHash(hashed, vector, generators, id);
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
    std::vector <G1> generators(100);
    KeyGen(alpha, u, h, generators);
    std::cout << "secret key " << std::endl;
    std::cout << "\talpha = " << alpha << std::endl;
    std::cout << "public key " << std::endl;
    std::cout << "\th = " << h << std::endl;
    std::cout << "\tu = " << u << std::endl;

    // create random vector
    std::vector <Fr> vec1(102);
    std::vector <Fr> vec2(102);
    for (int i = 0; i < vec1.size(); i++) {
        vec1[i].setRand();
        vec2[i].setRand();
    }
    vec1[100] = 1, vec1[101] = 0;
    vec2[100] = 0, vec2[101] = 1;

    // sign
    G1 sign1;
    Sign(sign1, alpha, vec1, generators, id);
    std::cout << "signature 1 = " << sign1 << std::endl;

    G1 sign2;
    Sign(sign2, alpha, vec2, generators, id);
    std::cout << "signature 2 = " << sign2 << std::endl;

    // verify
    bool ok = Verify(sign1, u, h, vec1, generators, id);
    std::cout << "verify sig 1 " << (ok ? "ok" : "ng") << std::endl;

    ok = Verify(sign2, u, h, vec2, generators, id);
    std::cout << "verify sig 2 " << (ok ? "ok" : "ng") << std::endl;
}
