#include <data.hpp>
#include <matrix.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <recoder.hpp>

FullRLNCRecoder::FullRLNCRecoder(std::vector<CodedPiece> ps)
{
    this->pieces = ps;
    this->pieceCount = ps.size();
}

FullRLNCRecoder::FullRLNCRecoder()
{
    this->pieceCount = 0;
};

void FullRLNCRecoder::addPiece(CodedPiece piece)
{
    this->pieces.push_back(piece);
    this->pieceCount++;
}

CodedPiece FullRLNCRecoder::getCodedPiece()
{
    std::vector<Fr> coefficients = generateCodingVector(this->pieceCount);
    int size = this->pieces[0].piece.size();
    std::vector<Fr> pc(this->pieces[0].dataLen(), 0);
    std::vector<G1> sigs(this->pieceCount);

    for (int i = 0; i < this->pieceCount; i++)
    {
        sigs[i] = this->pieces[i].signature;
        pc = multiply(pc, this->pieces[i].flatten(), coefficients[i]);
    }

    G1 signature;
    G1::mulVec(signature, sigs.data(), coefficients.data(), sigs.size());

    std::vector<Fr> recodedPiece(pc.begin(), pc.begin() + size);
    std::vector<Fr> recodedVec(pc.begin() + size, pc.end());
    return CodedPiece(recodedPiece, recodedVec, signature);
}