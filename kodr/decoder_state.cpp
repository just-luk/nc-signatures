#include <mcl/bn256.hpp>
#include <decoder_state.hpp>
#include <matrix.hpp>
#include <data.hpp>
#include <iostream>
#include <stdexcept>

DecoderState::DecoderState(Matrix cfs, Matrix pieces) {
    coeffs = cfs;
    coded = pieces;
    pieceCount = cfs.rows;
}

DecoderState::DecoderState(uint p) {
    pieceCount = p;
    Matrix cfs(std::vector<std::vector<Fr>>(1));
    Matrix cd(std::vector<std::vector<Fr>>(1));
    coeffs = cfs;
    coded = cd;
}

DecoderState::DecoderState() {};

int min(int a, int b) {
    if(a <= b) {
		return a;
	}
	return b;
}

void DecoderState::clean_forward() {
    int rows = coeffs.rows;
    int cols = coeffs.cols;
    int boundary = min(rows, cols);
    for(int i = 0; i < boundary; i++) {
        if(coeffs.data[i][i].isZero()) {
            int pivot = i+1;
            bool non_zero_col = false;
            for(; pivot < rows; pivot++) {
                if(!coeffs.data[pivot][i].isZero()) {
					non_zero_col = true;
					break;
				}
            }
            
            if(!non_zero_col) {
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

        for(int j = i + 1; j < rows; j++) {
            if(coeffs.data[j][i].isZero()) {
				continue;
			}
            Fr quotient = coeffs.data[j][i] / coeffs.data[i][i];
            for(int k = i; k < cols; k++) {
                coeffs.data[j][k] -= coeffs.data[i][k] * quotient;
            }

            for(int k = 0; k < coded.data[0].size(); k++) {
                coded.data[j][k] -= coded.data[i][k] * quotient;
            }
        }
    }
}

void DecoderState::clean_backward() {
    int rows = coeffs.rows;
    int cols = coeffs.cols;
    int boundary = min(rows, cols);
    for(int i = boundary - 1; i >= 0; i--) {
		if(coeffs.data[i][i].isZero()) {
			continue;
		}

        for(int j = 0; j < i; j++) {
			if(coeffs.data[j][i].isZero()) {
				continue;
			}

            Fr quotient = coeffs.data[j][i] / coeffs.data[i][i];
            for(int k = i; k < cols; k++) {
                coeffs.data[j][k] -= coeffs.data[i][k] * quotient;
            }

            for(int k = 0; k < coded.data[0].size(); k++) {
                coded.data[j][k] -= coded.data[i][k] * quotient;
            }
        }

        if(coeffs.data[i][i].isOne()) {
			continue;
		}

        Fr inv = 1;
        inv = inv / coeffs.data[i][i];
        coeffs.data[i][i] = 1;
        for(int j = i + 1; j < cols; j++) {
			if(coeffs.data[i][j].isZero()) {
				continue;
			}

			coeffs.data[i][j] *= inv;
		}

        for(int j = 0; j < coded.data[0].size(); j++) {
			coded.data[i][j] *= inv;
		}
    }
}

void DecoderState::remove_zero_rows() {
    int cols = coeffs.data[0].size();
    for(int i = 0; i < coeffs.data.size(); i++) {
        bool yes = true;
        for(int j = 0; j < cols; j++) {
            if(!coeffs.data[i][j].isZero()) {
                yes = false;
                break;
            }
        }
        if(!yes) {
            continue;
        }

        coeffs.data.erase(coeffs.data.begin() + i);
        coded.data.erase(coded.data.begin() + i);

        i--;
    }
}

void DecoderState::Rref() {
    clean_forward();
    clean_backward();
    remove_zero_rows();
}

uint DecoderState::Rank() {
    return coeffs.rows;
}

Matrix DecoderState::CoeffMatrix() {
    return coeffs;
}

Matrix DecoderState::CodedMatrix() {
    return coded;
}

void DecoderState::AddPiece(CodedPiece a, bool isFirst) {
    if(isFirst) {
        coeffs.data[0] = a.codingVector;
        coded.data[0] = a.piece;
        coeffs.cols = a.codingVector.size();
        coded.cols = a.piece.size();
    } else {
        coeffs.data.push_back(a.codingVector);
        coded.data.push_back(a.piece);
        coeffs.rows++;
        coded.rows++;
    }
}

std::vector<Fr> DecoderState::GetPiece(uint idx) {
    if(idx >= pieceCount) {
        throw std::out_of_range("Index out of bounds");
    }
    if(idx >= coeffs.rows) {
        throw std::runtime_error("Piece not yet decoded");
    }
    if(Rank() >= pieceCount) {
        return coded.data[idx];
    }
    bool decoded = true;
    for (int i = 0; i < coeffs.cols; i++)
    {
        switch(i) {
            case 0:
                if(!coeffs.data[idx][i].isOne()) {
                    decoded = false;
                    goto OUT;
                }
            default:
                if(coeffs.data[idx][i].isZero()) {
                    decoded = false;
                    goto OUT;
                }
        }
    }
OUT:
    if(!decoded) {
        throw std::runtime_error("Piece not yet decoded");
    }
    return coded.data[idx];
}