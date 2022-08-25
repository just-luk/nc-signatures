#include <data.hpp>
#include <decoder.hpp>
#include <encoder.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <stdint.h>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <recoder.hpp>
#include <stdlib.h>
#include <assert.h>
#include <random>

#include <boneh.hpp>
#include <li.hpp>
#include <zhang.hpp>
#include <catalano.hpp>
#include <chang.hpp>

using namespace mcl::bls12;

typedef Chang sigScheme;
typedef G1 sigType;

std::vector<uint8_t> readFile(const char *fileName)
{
    // open the file:
    std::streampos fileSize;
    std::ifstream file(fileName, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    std::vector<uint8_t> fileData(fileSize);
    file.read((char *)&fileData[0], fileSize);
    return fileData;
}

int main(int argc, char **argv)
{
    srand(unsigned(time(NULL)));
    initPairing();
    std::vector<uint8_t> fileData = readFile("../logo.png");

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <pieceCount>" << std::endl;
        return 1;
    }
    int pieceCount = strtol(argv[1], NULL, 10);
    int pieceSize = ceil((float)fileData.size() / (float)pieceCount);
    assert(pieceSize >= pieceCount);
    int codedPieceCount = pieceCount * 2;
    int droppedPieceCount = pieceCount;

    // systematic encoder
    
    sigScheme scheme(pieceSize, "logo.png");
    // sigScheme scheme("node1", "logo.png");
    // Fr fid;
    // fid.setRand();
    // sigScheme scheme(pieceCount, pieceSize, fid);
    FullRLNCEncoder<sigScheme, sigType> encoder(fileData, pieceCount, scheme, true);

    std::vector<CodedPiece<sigType>> codedPieces(codedPieceCount);
    for (int i = 0; i < codedPieceCount; i++)
    {
        codedPieces[i] = encoder.getCodedPiece();
    }

    std::random_shuffle(codedPieces.begin(), codedPieces.end());
    std::vector<CodedPiece<sigType>> droppedPieces(codedPieces.begin(), codedPieces.end() - droppedPieceCount);

    FullRLNCRecoder<sigScheme, sigType> recoder(droppedPieces, scheme);
    int recodedPieceCount = droppedPieces.size() * 2;
    std::vector<CodedPiece<sigType>> recodedPieces(recodedPieceCount);
    for (int i = 0; i < recodedPieceCount; i++)
    {
        recodedPieces[i] = recoder.getCodedPiece();
    }

    std::random_shuffle(recodedPieces.begin(), recodedPieces.end());
    std::vector<CodedPiece<sigType>> droppedPiecesAgain(recodedPieces.begin(), recodedPieces.end() - recodedPieces.size() / 2);

    FullRLNCDecoder<sigScheme, sigType> decoder(pieceCount, scheme);
    for (int i = 0; i < pieceCount; i++)
    {
        decoder.addPiece(droppedPiecesAgain[i]);
    }

    std::vector<uint8_t>
        decodedData = decoder.getData();
    if (decodedData != fileData)
    {
        std::cout << "[DECODER] ERROR Incorrect decoding!" << std::endl;
    }
    else
    {
        std::cout << "[DECODER] Correct decoding and verification!" << std::endl;
    }
}