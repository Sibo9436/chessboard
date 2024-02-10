#include "gamestate.hpp"
#include <algorithm>

GameState::GameState(std::unique_ptr<ChessBoard> board)
    : m_board(std::move(board)) {}
void GameState::handleInput(int x, int y) {
  if (!m_hasSelection) {
    auto optSelected = m_board->select(x, y);
    if (!optSelected || optSelected.value() != m_toMove) {
      return;
    }
    m_hasSelection = true;
    m_possibleMoves = m_board->possibleMoves();
  } else {
    auto pos = std::make_pair(x, y);
    if (std::find(m_possibleMoves.begin(), m_possibleMoves.end(), pos) !=
        m_possibleMoves.end()) {
      m_board->placeSelected(x, y);
    }
  }
}
