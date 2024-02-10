#ifndef INCLUDE_SRC_GAMESTATE_HPP_
#define INCLUDE_SRC_GAMESTATE_HPP_

// I'll admit I haven't exactly split the concerns well between board and
// gamestate but it's fine
#include "chess.hpp"
#include "common.hpp"
#include <memory>
// Still not enough to undo a move I guess but it's fine
struct Move {
  ChessPiece piece;
  ChessColor color;
  char from[3] = "xx";
  char to[3] = "xx";
};
class GameState {
public:
  GameState(std::unique_ptr<ChessBoard> board);
  std::vector<std::pair<int, int>> possibleMoves() const {
    return m_possibleMoves;
  };
  std::vector<Piece *> *getPieces() const { return m_board->getPieces(); }
  void handleInput(int x, int y);

private:
  std::unique_ptr<ChessBoard> m_board;
  std::vector<Move> m_moves;
  std::vector<std::pair<int, int>> m_possibleMoves;
  bool m_hasSelection = false;
  ChessColor m_toMove = WHITE;
};

#endif // INCLUDE_SRC_GAMESTATE_HPP_
