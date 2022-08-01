#include <data.hpp>
#include <matrix.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <recoder.hpp>
#include <boneh.hpp>

template <typename T>
FullRLNCRecoder<T>::FullRLNCRecoder(std::vector<CodedPiece> ps, T sig)
{
    this->pieces = ps;
    this->sig = sig;
    this->pieceCount = ps.size();
}

template <typename T>
FullRLNCRecoder<T>::FullRLNCRecoder(T sig)
{
    this->sig = sig;
    this->pieceCount = 0;
}

template <typename T>
FullRLNCRecoder<T>::FullRLNCRecoder(){};

template <typename T>
void FullRLNCRecoder<T>::addPiece(CodedPiece piece)
{
    if (!sig.Verify(piece))
    {
        return;
    }
    this->pieces.push_back(piece);
    this->pieceCount++;
}

template <typename T>
void FullRLNCRecoder<T>::clear()
{
    this->pieces.clear();
    this->pieceCount = 0;
}

template <typename T>
CodedPiece FullRLNCRecoder<T>::getCodedPiece()
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

    G1 signature = sig.Combine(sigs, coefficients);

    std::vector<Fr> recodedPiece(pc.begin(), pc.begin() + size);
    std::vector<Fr> recodedVec(pc.begin() + size, pc.end());
    CodedPiece cp = CodedPiece(recodedPiece, recodedVec, signature, false);
    return cp;
}

template class FullRLNCRecoder<Boneh>;