#include <data.hpp>
#include <decoder.hpp>
#include <encoder.hpp>
#include <fstream>
#include <iostream>
#include <math.h>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <recoder.hpp>
#include <stdlib.h>
#include <random>

using namespace mcl::bls12;

std::vector<unsigned char> readFile(const char *fileName) {
    // open the file:
    std::streampos fileSize;
    std::ifstream file(fileName, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    std::vector<unsigned char> fileData(fileSize);
    file.read((char *) &fileData[0], fileSize);
    return fileData;
}

void KeyGen(Fr &alpha, G2 &pub, G2 &h, std::vector <G1> &generators) {
    mapToG2(h, rand());
    alpha.setRand();
    G2::mul(pub, h, alpha);
    for (int i = 0; i < generators.size(); i++) {
        generators[i].x.setRand();
        generators[i].y.setRand();
    }
}

bool Verify(const G1 &sign, const G2 &u, const G2 &h, std::vector <G1> gens, std::vector <Fr> &vector,
            std::vector <Fr> &codingVec,
            const std::string &id) {
    Fp12 e1, e2;
    G1 hashed;
    AggregateHash(hashed, vector, codingVec, gens, id);
    pairing(e1, sign, h);   // e1 = e(signature, h)
    pairing(e2, hashed, u); // e2 = e(hashed, u)
    return e1 == e2;
}

int main(int argc, char **argv) {
    srand(unsigned(time(NULL)));
    initPairing();
    std::vector<unsigned char> fileData = readFile("logo.png");

    std::string identifier = "logo.png";
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " <pieceCount>" << std::endl;
        return 1;
    }
    int pieceCount = strtol(argv[1], NULL, 10);
    int codedPieceCount = pieceCount * 2;
    int droppedPieceCount = pieceCount / 2;

    Fr alpha;
    G2 u, h;
    std::vector < G1 > generators(ceil((float) fileData.size() / (float) pieceCount));

    KeyGen(alpha, u, h, generators);
    std::cout << "secret key " << std::endl;
    std::cout << "\talpha = " << alpha << std::endl;
    std::cout << "public key " << std::endl;
    std::cout << "\th = " << h << std::endl;
    std::cout << "\tu = " << u << std::endl;
    std::cout << "\tpieceCount = " << pieceCount << std::endl;

    FullRLNCEncoder encoder(fileData, pieceCount, identifier, alpha, generators, true);

    std::vector <CodedPiece> codedPieces(codedPieceCount);
    for (int i = 0; i < codedPieceCount; i++) {
        codedPieces[i] = encoder.getCodedPiece();
        bool verified = Verify(codedPieces[i].signature, u, h, generators, codedPieces[i].piece,
                               codedPieces[i].codingVector,
                               identifier);
        if (!verified) {
            std::cout << "[ENCODER] ERROR not verified" << std::endl;
        }
    }
    std::cout << "byteLength = " << codedPieces[0].fullLen() << std::endl;

    std::random_shuffle(codedPieces.begin(), codedPieces.end());
    std::vector <CodedPiece> droppedPieces(codedPieces.begin(), codedPieces.end() - droppedPieceCount);

    FullRLNCRecoder recoder(droppedPieces);
    int recodedPieceCount = droppedPieces.size() * 2;
    std::vector <CodedPiece> recodedPieces(recodedPieceCount);
    for (int i = 0; i < recodedPieceCount; i++) {
        recodedPieces[i] = recoder.getCodedPiece();
        bool verified = Verify(recodedPieces[i].signature, u, h, generators, recodedPieces[i].piece,
                               recodedPieces[i].codingVector,
                               identifier);
        if (!verified) {
            std::cout << "[RECODER] ERROR not verified" << std::endl;
        }
    }

    std::random_shuffle(recodedPieces.begin(), recodedPieces.end());
    std::vector <CodedPiece> droppedPiecesAgain(recodedPieces.begin(), recodedPieces.end() - recodedPieces.size() / 2);

    FullRLNCDecoder decoder(pieceCount);
    decoder.addPiece(droppedPiecesAgain[0], true);
    for (int i = 1; i < pieceCount; i++) {
        decoder.addPiece(droppedPiecesAgain[i], false);
    }

    std::vector<unsigned char> decodedData = decoder.getData();
    if (decodedData != fileData) {
        std::cout << "[DECODER] ERROR Incorrect decoding!" << std::endl;
    }
}