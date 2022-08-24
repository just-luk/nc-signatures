#include <data.hpp>
#include <decoder_state.hpp>
#include <iostream>
#include <matrix.hpp>
#include <mcl/bls12_381.hpp>
#include <vector>
#include <stdexcept>
#include <catalano.hpp>

template <typename S>
DecoderState<S>::DecoderState(Matrix cfs, Matrix pieces)
{
    coeffs = cfs;
    coded = pieces;
    pieceCount = cfs.rows;
}

template <typename S>
DecoderState<S>::DecoderState(int p)
{
    pieceCount = p;
    Matrix cfs(std::vector<std::vector<Fr>>(1));
    Matrix cd(std::vector<std::vector<Fr>>(1));
    coeffs = cfs;
    coded = cd;
}

template <typename S>
DecoderState<S>::DecoderState(){};

int min(int a, int b)
{
    if (a <= b)
    {
        return a;
    }
    return b;
}

template <typename S>
void DecoderState<S>::clean_forward()
{
    int rows = coeffs.rows;
    int cols = coeffs.cols;
    int boundary = min(rows, cols);
    for (int i = 0; i < boundary; i++)
    {
        if (coeffs.data[i][i].isZero())
        {
            int pivot = i + 1;
            bool non_zero_col = false;
            for (; pivot < rows; pivot++)
            {
                if (!coeffs.data[pivot][i].isZero())
                {
                    non_zero_col = true;
                    break;
                }
            }

            if (!non_zero_col)
            {
                continue;
            }

            {
                std::vector<Fr> tmp = coeffs.data[i];
                coeffs.data[i] = coeffs.data[pivot];
                coeffs.data[pivot] = tmp;
            }

            {
                std::vector<Fr> tmp = coded.data[i];
                coded.data[i] = coded.data[pivot];
                coded.data[pivot] = tmp;
            }
        }

        for (int j = i + 1; j < rows; j++)
        {
            if (coeffs.data[j][i].isZero())
            {
                continue;
            }
            Fr quotient = coeffs.data[j][i] / coeffs.data[i][i];
            for (int k = i; k < cols; k++)
            {
                coeffs.data[j][k] -= coeffs.data[i][k] * quotient;
            }

            for (int k = 0; k < coded.data[0].size(); k++)
            {
                coded.data[j][k] -= coded.data[i][k] * quotient;
            }
        }
    }
}

template <typename S>
void DecoderState<S>::clean_backward()
{
    int rows = coeffs.rows;
    int cols = coeffs.cols;
    int boundary = min(rows, cols);
    for (int i = boundary - 1; i >= 0; i--)
    {
        if (coeffs.data[i][i].isZero())
        {
            continue;
        }

        for (int j = 0; j < i; j++)
        {
            if (coeffs.data[j][i].isZero())
            {
                continue;
            }

            Fr quotient = coeffs.data[j][i] / coeffs.data[i][i];
            for (int k = i; k < cols; k++)
            {
                coeffs.data[j][k] -= coeffs.data[i][k] * quotient;
            }

            for (int k = 0; k < coded.data[0].size(); k++)
            {
                coded.data[j][k] -= coded.data[i][k] * quotient;
            }
        }

        if (coeffs.data[i][i].isOne())
        {
            continue;
        }

        Fr inv = 1;
        inv = inv / coeffs.data[i][i];
        coeffs.data[i][i] = 1;
        for (int j = i + 1; j < cols; j++)
        {
            if (coeffs.data[i][j].isZero())
            {
                continue;
            }

            coeffs.data[i][j] *= inv;
        }

        for (int j = 0; j < coded.data[0].size(); j++)
        {
            coded.data[i][j] *= inv;
        }
    }
}

template <typename S>
void DecoderState<S>::remove_zero_rows()
{
    int cols = coeffs.data[0].size();
    for (int i = 0; i < coeffs.data.size(); i++)
    {
        bool yes = true;
        for (int j = 0; j < cols; j++)
        {
            if (!coeffs.data[i][j].isZero())
            {
                yes = false;
                break;
            }
        }
        if (!yes)
        {
            continue;
        }

        coeffs.data.erase(coeffs.data.begin() + i);
        coeffs.rows--;
        coded.data.erase(coded.data.begin() + i);
        coded.rows--;

        i--;
    }
}

template <typename S>
void DecoderState<S>::Rref()
{
    clean_forward();
    clean_backward();
    remove_zero_rows();
}

template <typename S>
int DecoderState<S>::Rank() { return coeffs.rows; }

template <typename S>
Matrix DecoderState<S>::CoeffMatrix() { return coeffs; }

template <typename S>
Matrix DecoderState<S>::CodedMatrix() { return coded; }

template <typename S>
void DecoderState<S>::AddPiece(CodedPiece<S> a)
{
    if (coeffs.cols == 0)
    {
        coeffs.data[0] = a.codingVector;
        coded.data[0] = a.piece;
        coeffs.cols = a.codingVector.size();
        coded.cols = a.piece.size();
    }
    else
    {
        coeffs.data.push_back(a.codingVector);
        coded.data.push_back(a.piece);
        coeffs.rows++;
        coded.rows++;
    }
}

template <typename S>
std::vector<Fr> DecoderState<S>::GetPiece(int idx)
{
    if (idx >= pieceCount)
    {
        throw std::out_of_range("Index out of bounds");
    }
    if (idx >= coeffs.rows)
    {
        throw std::runtime_error("Piece not yet decoded");
    }
    if (Rank() >= pieceCount)
    {
        return coded.data[idx];
    }
    bool decoded = true;
    for (int i = 0; i < coeffs.cols; i++)
    {
        switch (i)
        {
        case 0:
            if (!coeffs.data[idx][i].isOne())
            {
                decoded = false;
                goto OUT;
            }
        default:
            if (coeffs.data[idx][i].isZero())
            {
                decoded = false;
                goto OUT;
            }
        }
    }
OUT:
    if (!decoded)
    {
        throw std::runtime_error("Piece not yet decoded");
    }
    return coded.data[idx];
}

template class DecoderState<G1>;
template class DecoderState<CatSignature>;