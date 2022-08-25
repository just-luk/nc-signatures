#include <data.hpp>
#include <decoder.hpp>
#include <decoder_state.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>
#include <vector>
#include <boneh.hpp>
#include <li.hpp>
#include <zhang.hpp>
#include <catalano.hpp>
#include <chang.hpp>

template <typename T, typename S>
FullRLNCDecoder<T, S>::FullRLNCDecoder(int pieceCount, T sig)
{
    expected = pieceCount;
    useful = 0;
    received = 0;
    this->sig = sig;
    state = DecoderState<S>(pieceCount);
}

template <typename T, typename S>
FullRLNCDecoder<T, S>::FullRLNCDecoder(){};

template <typename T, typename S>
int FullRLNCDecoder<T, S>::PieceLength()
{
    if (received > 0)
    {
        return state.CodedMatrix().cols;
    }
    return 0;
}

template <typename T, typename S>
bool FullRLNCDecoder<T, S>::IsDecoded() { return useful >= expected; }

template <typename T, typename S>
int FullRLNCDecoder<T, S>::Required() { return expected - useful; }

template <typename T, typename S>
void FullRLNCDecoder<T, S>::addPiece(CodedPiece<S> piece)
{
    if (IsDecoded() || !sig.Verify(piece))
    {
        return;
    }
    state.AddPiece(piece);
    received++;
    if (received <= 1)
    {
        useful++;
        return;
    }

    state.Rref();
    useful = state.Rank();
}

template <typename T, typename S>
std::vector<Fr> FullRLNCDecoder<T, S>::getPiece(int i) { return state.GetPiece(i); }

template <typename T, typename S>
std::vector<uint8_t> FullRLNCDecoder<T, S>::getData()
{
    if (!IsDecoded())
    {
        throw std::runtime_error("More useful pieces are required!");
    }
    std::vector<uint8_t> pieces;
    std::vector<Fr> tempPiece;
    std::vector<uint8_t> tempVec;
    std::string tempString;
    for (int i = 0; i < useful; i++)
    {
        tempPiece = getPiece(i);
        for (int j = 0; j < tempPiece.size(); j++)
        {
            tempString = tempPiece[j].getStr(mcl::IoSerialize);
            tempVec = std::vector<uint8_t>(tempString.begin(), tempString.end());
            pieces.push_back(tempVec[0]);
        }
    }
    int len = pieces.size() - 1;
    while (pieces[len] == 0)
    {
        len--;
    }
    pieces.resize(len + 1);
    return pieces;
}

template class FullRLNCDecoder<Boneh, G1>;
template class FullRLNCDecoder<Li, G1>;
template class FullRLNCDecoder<Zhang, G1>;
template class FullRLNCDecoder<Catalano, CatSignature>;
template class FullRLNCDecoder<Chang, G1>;