#include <data.hpp>
#include <matrix.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <recoder.hpp>

FullRLNCRecoder::FullRLNCRecoder(std::vector<CodedPiece> ps, Signature *sig)
{
    this->pieces = ps;
    this->sig = sig;
    this->pieceCount = ps.size();
}

FullRLNCRecoder::FullRLNCRecoder(Signature *sig)
{
    this->sig = sig;
    this->pieceCount = 0;
}

FullRLNCRecoder::FullRLNCRecoder(){};

void FullRLNCRecoder::addPiece(CodedPiece piece)
{
    if (!sig->Verify(piece))
    {
        return;
    }
    this->pieces.push_back(piece);
    this->pieceCount++;
}

void FullRLNCRecoder::clear()
{
    this->pieces.clear();
    this->pieceCount = 0;
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