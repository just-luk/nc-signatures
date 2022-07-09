#pragma once

#include <data.hpp>
#include <decoder.hpp>
#include <decoder_state.hpp>
#include <mcl/bn256.hpp>
#include <stdexcept>

#ifndef DECODER_HPP
#define DECODER_HPP

typedef struct FullRLNCDecoder {
  int expected, useful, received;
  DecoderState state;
  FullRLNCDecoder(int pieceCount);
  int PieceLength();
  bool IsDecoded();
  int Required();
  void AddPiece(CodedPiece piece, bool isFirst);
  std::vector<Fr> GetPiece(int i);
  std::vector<std::vector<Fr>> GetPieces();
} FullRLNCDecoder;

#endif