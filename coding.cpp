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

using namespace mcl::bls12;

typedef Boneh sigScheme;

std::string random_string(std::size_t length)
{
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string random_string;

    for (int i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

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
    
    sigScheme boneh(pieceSize, "logo.png");
    // sigScheme li("node1", "logo.png");
    FullRLNCEncoder<sigScheme> encoder(fileData, pieceCount, boneh, true);

    std::vector<CodedPiece> codedPieces(codedPieceCount);
    for (int i = 0; i < codedPieceCount; i++)
    {
        codedPieces[i] = encoder.getCodedPiece();
    }

    std::random_shuffle(codedPieces.begin(), codedPieces.end());
    std::vector<CodedPiece> droppedPieces(codedPieces.begin(), codedPieces.end() - droppedPieceCount);

    FullRLNCRecoder<sigScheme> recoder(droppedPieces, boneh);
    int recodedPieceCount = droppedPieces.size() * 2;
    std::vector<CodedPiece> recodedPieces(recodedPieceCount);
    for (int i = 0; i < recodedPieceCount; i++)
    {
        recodedPieces[i] = recoder.getCodedPiece();
    }

    std::random_shuffle(recodedPieces.begin(), recodedPieces.end());
    std::vector<CodedPiece> droppedPiecesAgain(recodedPieces.begin(), recodedPieces.end() - recodedPieces.size() / 2);

    FullRLNCDecoder<sigScheme> decoder(pieceCount, boneh);
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