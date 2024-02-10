#ifndef INCLUDE_CHESS_CHESS_HPP_
#define INCLUDE_CHESS_CHESS_HPP_

#include "common.hpp"
#include <optional>
#include <vector>
struct Piece {
  unsigned int x, y;
  ChessPiece type;
  ChessColor color;
};
class ChessBoard {
public:
  ChessBoard();
  std::vector<Piece *> *getPieces() { return &m_pieces; }
  std::optional<ChessColor> select(int x, int y);
  bool findAndSelect(ChessPiece type, ChessColor color, int n);
  void placeSelected(int x, int y);
  void moveSelected(int x, int y);
  std::vector<std::pair<int, int>> possibleMoves() const;

private:
  Piece *pick(int x, int y) const;
  std::vector<Piece *> m_pieces;
  Piece *m_selected = nullptr;
  std::vector<std::pair<int, int>> possibleRook() const;
  std::vector<std::pair<int, int>> possibleBishop() const;
};

#endif // INCLUDE_CHESS_CHESS_HPP_
