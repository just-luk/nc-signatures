#include <data.hpp>
#include <encoder.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <string>
#include <boneh.hpp>

template <typename T>
FullRLNCEncoder<T>::FullRLNCEncoder(std::vector<std::vector<Fr>> pieces, T sig, bool generateSystematic)
{
    this->sig = sig;
    this->pieces = pieces;
    this->useSystematic = generateSystematic;
    this->pieceIndex = 0;
}

template <typename T>
FullRLNCEncoder<T>::FullRLNCEncoder(std::vector<uint8_t> data,
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

template <typename T>
FullRLNCEncoder<T>::FullRLNCEncoder(){};

template <typename T>int 
FullRLNCEncoder<T>::PieceCount() { return pieces.size(); }

template <typename T>int 
FullRLNCEncoder<T>::PieceSize() { return pieces[0].size(); }

template <typename T>int 
FullRLNCEncoder<T>::DecodableLen() { return PieceCount() * CodedPieceLen(); }

template <typename T>int 
FullRLNCEncoder<T>::CodedPieceLen() { return PieceCount() + PieceSize(); }

template <typename T>CodedPiece 
FullRLNCEncoder<T>::getCodedPiece()
{
    std::vector<Fr> codingVec;
    std::vector<Fr> piece;
    G1 signature;
    CodedPiece cp;
    if (useSystematic && pieceIndex < PieceCount())
    {
        codingVec = generateSystematicVector(pieceIndex, PieceCount());
        piece = pieces[pieceIndex];
        pieceIndex++;
        signature = sig.Sign(piece, codingVec, pieceIndex - 1);
        cp = CodedPiece(piece, codingVec, signature, true);
    }
    else
    {
        codingVec = generateCodingVector(PieceCount());
        piece = std::vector<Fr>(PieceSize(), 0);
        for (int i = 0; i < PieceCount(); i++)
        {
            piece = multiply(piece, pieces[i], codingVec[i]);
        }
        signature = sig.Sign(piece, codingVec, -1);
        cp = CodedPiece(piece, codingVec, signature, false);
    }
    return cp;
}

template class FullRLNCEncoder<Boneh>;