#include <data.hpp>
#include <matrix.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <recoder.hpp>
#include <boneh.hpp>
#include <li.hpp>
#include <zhang.hpp>
#include <catalano.hpp>
#include <chang.hpp>

template <typename T, typename S>
FullRLNCRecoder<T, S>::FullRLNCRecoder(std::vector<CodedPiece<S>> ps, T sig)
{
    this->pieces = ps;
    this->sig = sig;
    this->pieceCount = ps.size();
}

template <typename T, typename S>
FullRLNCRecoder<T, S>::FullRLNCRecoder(T sig)
{
    this->sig = sig;
    this->pieceCount = 0;
}

template <typename T, typename S>
FullRLNCRecoder<T, S>::FullRLNCRecoder(){};

template <typename T, typename S>
void FullRLNCRecoder<T, S>::addPiece(CodedPiece<S> piece)
{
    if (!sig.Verify(piece))
    {
        std::cout << "Piece not verified" << std::endl;
        return;
    }
    this->pieces.push_back(piece);
    this->pieceCount++;
}

template <typename T, typename S>
void FullRLNCRecoder<T, S>::clear()
{
    this->pieces.clear();
    this->pieceCount = 0;
}

template <typename T, typename S>
CodedPiece<S> FullRLNCRecoder<T, S>::getCodedPiece()
{
    std::vector<Fr> coefficients = generateCodingVector(this->pieceCount);
    int size = this->pieces[0].piece.size();
    std::vector<Fr> pc(this->pieces[0].dataLen(), 0);
    std::vector<S> sigs(this->pieceCount);

    for (int i = 0; i < this->pieceCount; i++)
    {
        sigs[i] = this->pieces[i].signature;
        pc = multiply(pc, this->pieces[i].flatten(), coefficients[i]);
    }

    S signature = sig.Combine(sigs, coefficients);

    std::vector<Fr> recodedPiece(pc.begin(), pc.begin() + size);
    std::vector<Fr> recodedVec(pc.begin() + size, pc.end());
    return CodedPiece<S>(recodedPiece, recodedVec, signature);
}

template class FullRLNCRecoder<Boneh, G1>;
template class FullRLNCRecoder<Li, G1>;
template class FullRLNCRecoder<Zhang, G1>;
template class FullRLNCRecoder<Catalano, CatSignature>;
template class FullRLNCRecoder<Chang, G1>;