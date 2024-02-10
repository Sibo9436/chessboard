#include "chess.hpp"
#include "common.hpp"
#include <algorithm>
#include <iostream>
#include <utility>

ChessBoard::ChessBoard() {

  for (unsigned int w = 0; w < 2; w++) {
    for (unsigned int x = 0; x < 8; x++) {
      m_pieces.push_back(new Piece{x, (w ? 1u : 6u), PAWN, w ? WHITE : BLACK});
    }
    unsigned int y = w ? 0 : 7;
    m_pieces.push_back(new Piece{4, y, KING, w ? WHITE : BLACK});
    m_pieces.push_back(new Piece{3, y, QUEEN, w ? WHITE : BLACK});
    m_pieces.push_back(new Piece{2, y, BISHOP, w ? WHITE : BLACK});
    m_pieces.push_back(new Piece{5, y, BISHOP, w ? WHITE : BLACK});
    m_pieces.push_back(new Piece{6, y, KNIGHT, w ? WHITE : BLACK});
    m_pieces.push_back(new Piece{1, y, KNIGHT, w ? WHITE : BLACK});
    m_pieces.push_back(new Piece{0, y, ROOK, w ? WHITE : BLACK});
    m_pieces.push_back(new Piece{7, y, ROOK, w ? WHITE : BLACK});
  }
}
Piece *ChessBoard::pick(int x, int y) const {
  // std::cout << "Trying to select piece " << x << " " << y << "\n";
  for (auto *piece : m_pieces) {
    if (piece->x == x && piece->y == y) {
      return piece;
    }
  }
  // horrendous
  return nullptr;
}

// Returns the nth matching element
bool ChessBoard::select(ChessPiece type, ChessColor color, int n) {
  int count = 0;
  for (auto *piece : m_pieces) {
    if (count < n) {
      if (piece->type == type && piece->color == color) {
        m_selected = piece;
        count++;
      }
    } else {
      return true;
    }
  }
  return count > 0;
}

bool ChessBoard::select(int x, int y) {
  m_selected = pick(x, y);
  return !!m_selected;
  ;
}

void ChessBoard::placeSelected(int x, int y) {
  if (!m_selected) {
    return;
  }
  Piece *tgt = pick(x, y);
  if (tgt != nullptr) {
    if (tgt->color == m_selected->color) {
      return;
    }
    m_pieces.erase(std::find(m_pieces.begin(), m_pieces.end(), tgt));
  }
  m_selected->x = std::clamp(x, 0, 7);
  m_selected->y = std::clamp(y, 0, 7);
}

void ChessBoard::moveSelected(int x, int y) {
  if (!m_selected) {
    return;
  }
  x += m_selected->x;
  y += m_selected->y;
  placeSelected(x, y);
  // TODO: rimuovere
  //  m_selected = nullptr;
}

std::vector<std::pair<int, int>> ChessBoard::possibleMoves() const {
  if (!m_selected) {
    return {};
  }
  std::vector<std::pair<int, int>> result;
  switch (m_selected->type) {
    // These might be extracted later
  case KING: {
    for (unsigned int y = (m_selected->y == 0) ? 0u : m_selected->y - 1u;
         y <= std::min(7u, m_selected->y + 1); y++) {
      for (unsigned int x = (m_selected->x == 0) ? 0u : m_selected->x - 1u;
           x <= std::min(7u, m_selected->x + 1); x++) {
        Piece *tgt = pick(x, y);
        if ((y != m_selected->y || x != m_selected->x) &&
            (!tgt || tgt->color != m_selected->color)) {
          result.push_back(std::make_pair(x, y));
        }
      }
    }
    break;
  }
  case KNIGHT: {
    std::vector<std::pair<int, int>> possibleKnights;
    // There's probably a better way I guess
    for (int y : {-2, 2}) {
      for (int x : {-1, 1}) {
        int ny = (int)m_selected->y + y;
        int nx = (int)m_selected->x + x;
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
          possibleKnights.push_back(std::make_pair(nx, ny));
        }
      }
    }
    for (int x : {-2, 2}) {
      for (int y : {-1, 1}) {
        int ny = (int)m_selected->y + y;
        int nx = (int)m_selected->x + x;
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
          possibleKnights.push_back(std::make_pair(nx, ny));
        }
      }
    }
    for (auto move : possibleKnights) {
      Piece *tgt = pick(move.first, move.second);
      if (!tgt || tgt->color != m_selected->color) {
        result.push_back(move);
      }
    }
    break;
  }
    // It's kind of cheating but who cares
  case BISHOP: {
    result = possibleBishop();
    break;
  }
  case ROOK: {
    result = possibleRook();
    break;
  }
  case QUEEN: {
    auto b = possibleBishop();
    auto r = possibleRook();
    result.insert(result.end(), r.begin(), r.end());
    result.insert(result.end(), b.begin(), b.end());
    break;
  }

  case PAWN: {
    if (m_selected->y == 0 || m_selected->y == 7)
      break;
    int dir = ((m_selected->color == BLACK) ? -1 : 1);
    int ly = m_selected->y + dir;
    Piece *front = pick(m_selected->x, ly);
    Piece *left = pick(std::max(0, (int)m_selected->x - 1), ly);
    Piece *right = pick(std::min(7, (int)m_selected->x + 1), ly);
    Piece *ffront = pick(m_selected->x, ly + dir);
    if (!front) {
      result.push_back(std::make_pair(m_selected->x, ly));
    }
    if (left && left->color != m_selected->color) {
      result.push_back(std::make_pair(std::max(0, (int)m_selected->x - 1), ly));
    }
    if (right && right->color != m_selected->color) {
      result.push_back(std::make_pair(std::max(0, (int)m_selected->x + 1), ly));
    }
    // check if pawn is on starting rows, this could be done better 4 sure
    if (!ffront && ((m_selected->y == 1 && m_selected->color == WHITE) ||
                    (m_selected->y == 6 && m_selected->color == BLACK))) {
      result.push_back(std::make_pair(m_selected->x, ly + dir));
    }
    break;
  }
  }
  return result;
}

std::vector<std::pair<int, int>> ChessBoard::possibleBishop() const {
  int directions[][4] = {{-1, -1}, {1, 1}, {-1, 1}, {2, -1}};
  std::vector<std::pair<int, int>> result;
  for (auto direction : directions) {
    int x = m_selected->x + direction[0];
    int y = m_selected->y + direction[1];
    Piece *tgt;
    while (x >= 0 && x < 8 && y >= 0 && y < 8 &&
           ((tgt = pick(x, y)) == nullptr || tgt->color != m_selected->color)) {
      result.push_back(std::make_pair(x, y));
      x += direction[0];
      y += direction[1];
    }
  }
  return result;
}

std::vector<std::pair<int, int>> ChessBoard::possibleRook() const {
  int directions[][4] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
  std::vector<std::pair<int, int>> result;
  for (auto direction : directions) {
    int x = m_selected->x + direction[0];
    int y = m_selected->y + direction[1];
    Piece *tgt;
    while (x >= 0 && x < 8 && y >= 0 && y < 8 &&
           ((tgt = pick(x, y)) == nullptr || tgt->color != m_selected->color)) {
      result.push_back(std::make_pair(x, y));
      x += direction[0];
      y += direction[1];
    }
  }
  return result;
}
