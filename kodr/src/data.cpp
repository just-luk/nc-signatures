#include <data.hpp>
#include <math.h>
#include <mcl/bls12_381.hpp>
#include <vector>

using namespace mcl::bls12;

std::vector<Fr> multiply(std::vector<Fr> piece1, std::vector<Fr> piece2, Fr by)
{
    for (int i = 0; i < piece1.size(); i++)
    {
        piece1[i] += piece2[i] * by;
    }
    return piece1;
}

CodedPiece::CodedPiece(std::vector<Fr> p, std::vector<Fr> v, G1 s, bool isSys)
{
    piece = p;
    codingVector = v;
    signature = s;
    isSystematic = isSys;
}

CodedPiece::CodedPiece(std::vector<uint8_t> &bytes, const int &pieceSize, const int &codingVectorSize)
{
    piece.resize(pieceSize);
    codingVector.resize(codingVectorSize);
    std::vector<uint8_t> tempArr(32);
    std::string tempString;

    for (int i = 0; i < pieceSize; i++)
    {
        tempArr = std::vector<uint8_t>(bytes.begin() + i * 32, bytes.begin() + (i + 1) * 32);
        tempString = std::string(tempArr.begin(), tempArr.end());
        piece[i].setStr(tempString, mcl::IoSerialize);
    }
    for (int i = pieceSize; i < pieceSize + codingVectorSize; i++)
    {
        tempArr = std::vector<uint8_t>(bytes.begin() + i * 32, bytes.begin() + (i + 1) * 32);
        tempString = std::string(tempArr.begin(), tempArr.end());
        codingVector[i - pieceSize].setStr(tempString, mcl::IoSerialize);
    }
    tempArr = std::vector<uint8_t>(bytes.begin() + (pieceSize + codingVectorSize) * 32, bytes.end());
    tempString = std::string(tempArr.begin(), tempArr.end());
    signature.setStr(tempString, mcl::IoSerialize);
    isSystematic = false;
}

CodedPiece::CodedPiece(){};

int CodedPiece::dataLen() { return piece.size() + codingVector.size(); }

int CodedPiece::fullLen()
{
    return dataLen() * 32 + 32;
}

std::vector<Fr> CodedPiece::flatten()
{
    std::vector<Fr> ret(dataLen());
    std::copy(piece.begin(), piece.end(), ret.begin());
    std::copy(codingVector.begin(), codingVector.end(), ret.begin() + piece.size());
    return ret;
}

std::vector<uint8_t> CodedPiece::toBytes()
{
    std::vector<uint8_t> ret(fullLen());
    std::vector<uint8_t> tempArr(32);
    std::string tempString;
    for (int i = 0; i < piece.size(); i++)
    {
        tempString = piece[i].getStr(mcl::IoSerialize);
        tempArr = std::vector<uint8_t>(tempString.begin(), tempString.end());
        std::copy(tempArr.begin(), tempArr.end(), ret.begin() + i * 32);
    }
    for (int i = 0; i < codingVector.size(); i++)
    {
        tempString = codingVector[i].getStr(mcl::IoSerialize);
        tempArr = std::vector<uint8_t>(tempString.begin(), tempString.end());
        std::copy(tempArr.begin(), tempArr.end(), ret.begin() + (piece.size() + i) * 32);
    }
    tempString = signature.getStr(mcl::IoSerialize);
    tempArr = std::vector<uint8_t>(tempString.begin(), tempString.end());
    std::copy(tempArr.begin(), tempArr.end(), ret.begin() + dataLen() * 32);
    return ret;
}

std::vector<Fr> generateCodingVector(int n)
{
    std::vector<Fr> ret(n);
    for (int i = 0; i < n; i++)
    {
        ret[i].setRand();
    }
    return ret;
}

std::vector<Fr> generateSystematicVector(int idx, int n)
{
    std::vector<Fr> ret(n, 0);
    ret[idx] = 1;
    return ret;
}

std::vector<std::vector<Fr>>
OriginalPiecesWithCountAndSize(std::vector<uint8_t> data, int pieceCount, int pieceSize)
{
    data.resize(pieceSize * pieceCount, 0);
    std::vector<std::vector<Fr>> ret(pieceCount, std::vector<Fr>(pieceSize));
    for (int i = 0; i < pieceCount; i++)
    {
        copy(data.begin() + i * pieceSize, data.begin() + (i + 1) * pieceSize,
             ret[i].begin());
    }
    return ret;
}

std::vector<std::vector<Fr>>
OriginalPiecesFromDataAndPieceCount(std::vector<uint8_t> data, int pieceCount)
{
    int size = data.size();
    int pieceSize = ceil((float)size / (float)pieceCount);
    return OriginalPiecesWithCountAndSize(data, pieceCount, pieceSize);
}

std::vector<std::vector<Fr>>
OriginalPiecesFromDataAndPieceSize(std::vector<uint8_t> data, int pieceSize)
{
    int size = data.size();
    int pieceCount = ceil((float)size / (float)pieceSize);
    return OriginalPiecesWithCountAndSize(data, pieceCount, pieceSize);
}