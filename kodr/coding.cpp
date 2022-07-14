#include <data.hpp>
#include <decoder.hpp>
#include <encoder.hpp>
#include <fstream>
#include <iostream>
#include <math.h>
#include <mcl/bls12_381.hpp>
#include <recoder.hpp>

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

void KeyGen(Fr &alpha, G2 &pub, G2 &h) {
    mapToG2(h, rand());
    alpha.setRand();
    G2::mul(pub, h, alpha);
}

bool Verify(const G1 &sign, const G2 &u, const G2 &h, std::vector <Fr> &vector,
            const std::string &id) {
    Fp12 e1, e2;
    G1 hashed;
    AggregateHash(hashed, vector, id);
    pairing(e1, sign, h);   // e1 = e(sign, Q)
    pairing(e2, hashed, u); // e2 = e(Hm, sQ)
    return e1 == e2;
}

int main() {
    srand(unsigned(time(NULL)));
    initPairing();
    std::vector<unsigned char> fileData = readFile("logo.png");

    Fr alpha;
    G2 u, h;
    KeyGen(alpha, u, h);
    std::cout << "secret key " << std::endl;
    std::cout << "\talpha = " << alpha << std::endl;
    std::cout << "public key " << std::endl;
    std::cout << "\th = " << h << std::endl;
    std::cout << "\tu = " << u << std::endl;

    int pieceCount = 64;
    int codedPieceCount = pieceCount * 2;
    int droppedPieceCount = pieceCount / 2;

    FullRLNCEncoder encoder(fileData, pieceCount, "logo.png123", alpha, false);

    std::vector <CodedPiece> codedPieces(codedPieceCount);
    for (int i = 0; i < codedPieceCount; i++) {
        codedPieces[i] = encoder.getCodedPiece();
        bool verified = Verify(codedPieces[i].signature, u, h, codedPieces[i].piece, "logo.png123");
        if (!verified) {
            std::cout << "ERROR not verified" << std::endl;
        }
    }

    std::random_shuffle(codedPieces.begin(), codedPieces.end());

    std::vector <CodedPiece> droppedPieces(codedPieces.begin(),
                                           codedPieces.end() - droppedPieceCount);
    FullRLNCRecoder recoder(droppedPieces);
    int recodedPieceCount = droppedPieces.size() * 2;
    std::vector <CodedPiece> recodedPieces(recodedPieceCount);
    for (int i = 0; i < recodedPieceCount; i++) {
        recodedPieces[i] = recoder.getCodedPiece();
        bool verified = Verify(recodedPieces[i].signature, u, h, recodedPieces[i].piece, "logo.png123");
        if (!verified) {
            std::cout << "ERROR not verified" << std::endl;
        }
    }

    std::random_shuffle(recodedPieces.begin(), recodedPieces.end());
    std::vector <CodedPiece> droppedPiecesAgain(
            recodedPieces.begin(), recodedPieces.end() - recodedPieces.size() / 2);

    FullRLNCDecoder decoder(pieceCount);
    decoder.AddPiece(droppedPiecesAgain[0], true);
    for (int i = 1; i < pieceCount; i++) {
        decoder.AddPiece(droppedPiecesAgain[i], false);
    }

    std::vector<unsigned char> decodedData = decoder.getData();
    if (decodedData != fileData) {
        std::cout << "ERROR Incorrect decoding!" << std::endl;
    }
    // std::cout << std::endl << "decoded pieces " << std::endl;
    // for (int i = 0; i < decodedPieces.size(); i++) {
    //   for (int j = 0; j < decodedPieces[0].size(); j++) {
    //     std::cout << "[" << decodedPieces[i][j] << "] ";
    //   }
    //   std::cout << std::endl;
    // }
}