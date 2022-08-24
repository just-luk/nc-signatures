#include <data.hpp>
#include <encoder.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include <boneh.hpp>
#include <li.hpp>
#include <zhang.hpp>
#include <catalano.hpp>

template <typename T, typename S>
FullRLNCEncoder<T, S>::FullRLNCEncoder(std::vector<std::vector<Fr>> pieces, T sig, bool generateSystematic)
{
    this->sig = sig;
    this->pieces = pieces;
    this->useSystematic = generateSystematic;
    this->pieceIndex = 0;
}

template <typename T, typename S>
FullRLNCEncoder<T, S>::FullRLNCEncoder(std::vector<uint8_t> data,
                                 int pieceCountOrSize, T sig, bool generateSystematic, bool fromSize)
{
    this->sig = sig;
    this->useSystematic = generateSystematic;
    this->pieceIndex = 0;
    if (fromSize)
    {
        this->pieces = OriginalPiecesFromDataAndPieceSize(data, pieceCountOrSize);
    }
    else
    {
        this->pieces = OriginalPiecesFromDataAndPieceCount(data, pieceCountOrSize);
    }
}

template <typename T, typename S>
FullRLNCEncoder<T, S>::FullRLNCEncoder(){};

template <typename T, typename S>int 
FullRLNCEncoder<T, S>::PieceCount() { return pieces.size(); }

template <typename T, typename S>int 
FullRLNCEncoder<T, S>::PieceSize() { return pieces[0].size(); }

template <typename T, typename S>int 
FullRLNCEncoder<T, S>::DecodableLen() { return PieceCount() * CodedPieceLen(); }

template <typename T, typename S>int 
FullRLNCEncoder<T, S>::CodedPieceLen() { return PieceCount() + PieceSize(); }

template <typename T, typename S> CodedPiece<S> 
FullRLNCEncoder<T, S>::getCodedPiece()
{
    std::vector<Fr> codingVec;
    std::vector<Fr> piece;
    S signature;
    if (useSystematic && pieceIndex < PieceCount())
    {
        codingVec = generateSystematicVector(pieceIndex, PieceCount());
        piece = pieces[pieceIndex];
        pieceIndex++;
    }
    else
    {
        codingVec = generateCodingVector(PieceCount());
        piece = std::vector<Fr>(PieceSize(), 0);
        for (int i = 0; i < PieceCount(); i++)
        {
            piece = multiply(piece, pieces[i], codingVec[i]);
        }
    }
    signature = sig.Sign(piece, codingVec);
    return CodedPiece<S>(piece, codingVec, signature);;
}

template class FullRLNCEncoder<Boneh, G1>;
template class FullRLNCEncoder<Li, G1>;
template class FullRLNCEncoder<Zhang, G1>;
template class FullRLNCEncoder<Catalano, CatSignature>;