// Copyright 2020
// Author: Daniel Schindler <danielmschindler1@googlemail.com>

#include "random"
#include "./logx.h"
// =================================

Logx::Logx() {
  this->_score = 0;
  this->_turns = 0;
  this->_winstate = false;
  this->_gameoverstate = false;

  // initializing empty logic board
  for (auto & x : this->_board) {
    for (auto & y : x) {
      y = Tile(0);
    }
  }

  // Generate two random tiles
  for (int i = 1; i <= 2; i++) {
    this->setRandomTileOnBoard();
  }
}

Logx::~Logx() = default;

// ============================================================================

void Logx::updateState(int ShiftDirection) {
  // Adjust turn counter
  if (!this->_gameoverstate) this->_turns++;

  // Remove empty spaces between tiles before merging
  this->removeEmptySpacesTiles(ShiftDirection);

  // Do the merging depending on ShiftDirection
  if (ShiftDirection == direction.up) {
    // Iterate from top to bottom over board.
    for (auto & x : this->_board) {
        for (int y = 0; y <= 2; y++) {
            if (x[y].getLastMerge() < this->_turns) {
                if (x[y + 1].getLastMerge() < this->_turns) {
                    this->_score += x[y].merge(&x[y + 1],
                                               this->_turns);
                }
            }
        }
    }
  } else if (ShiftDirection == direction.down) {
    // Iterate from bottom to top over board.
    for (auto & x : this->_board) {
      for (int y = 2; y >= 0; y--) {
        if (x[y].getLastMerge() < this->_turns) {
          if (x[y + 1 - 1].getLastMerge() < this->_turns) {
            this->_score += x[y + 1].merge(&x[y],
                                           this->_turns);
          }
        }
      }
    }
  } else if (ShiftDirection == direction.left) {
    // Iterate from left to right over board.
    for (int x = 0; x <= 2; x++) {
      for (int y = 3; y >= 0; y--) {
        if (this->_board[x][y].getLastMerge() < this->_turns) {
          if (this->_board[x][y + 1 - 1].getLastMerge() < this->_turns) {
            this->_score += this->_board[x + 1][y].merge(&this->_board[x][y],
                                                         this->_turns);
          }
        }
      }
    }
  } else if (ShiftDirection == direction.right) {
    // Iterate from right to left over board.
    for (int x = 3; x >= 1; x--) {
      for (int y = 3; y >= 0; y--) {
        if (this->_board[x][y].getLastMerge() < this->_turns) {
          if (this->_board[x][y - 1].getLastMerge() < this->_turns) {
            this->_score +=this->_board[x][y].merge(&this->_board[x - 1][y],
                                                    this->_turns);
          }
        }
      }
    }
  }

  // Remove empty spaces between tiles after merging
  this->removeEmptySpacesTiles(ShiftDirection);

  // Add a new random tile
  this->setRandomTileOnBoard();

  // Check if board has any free and mergeable tiles left
  this->checkFreeAndMerge();
}

// ============================================================================

bool Logx::checkFreeAndMerge() {
  for (int m = 0; m < 4; m++) {
    for (int n = 0; n < 4; n++) {
      if (this->_board[m][n].getValue() == 0) {
        this->_gameoverstate = false;
        return false;
      } else if (n < 3 && this->_board[n][m].getValue() ==
          this->_board[n + 1][m].getValue()) {
            this->_gameoverstate = false;
            return false;
      } else if (m < 3 && this->_board[n][m].getValue() ==
          this->_board[n][m + 1].getValue()) {
            this->_gameoverstate = false;
            return false;
          }
    }
  }
  this->_gameoverstate = true;
  return true;
}

// ============================================================================

uint32_t Logx::getScore() const {
  return this->_score;
}

int Logx::getTurns() const {
  return this->_turns;
}

bool Logx::getWinState() const {
  return this->_winstate;
}

bool Logx::getGameOverState() const {
  return this->_gameoverstate;
}

Tile* Logx::getTileAtPos(int x, int y) {
  return &this->_board[x][y];
}

// ============================================================================

uint_fast8_t Logx::isGameOver() {
  if (_gameoverstate) {
    return 1;
  } else {
    for (auto & x : _board) {
      for (const auto & y : x) {
        if (y.getValue() == 2048) {
          _winstate = true;
          return 2;
        }
      }
    }
  }
  return 0;
}

// ============================================================================

void Logx::setRandomTileOnBoard() {
  // Create random number generators
  for (auto & x : this->_board) {
    for (const auto & y : x) {
      if (y.getValue() == 0) {
        // place new random tile, because there's space
        std::random_device randomDevice;
        std::mt19937 mersenneTwister(randomDevice());
        std::uniform_real_distribution<double> distProbability(0, 1);
        std::uniform_real_distribution<> distPosition(0, 4);

        int tileValue;
        // Check probabilities 0.9 - 0.1 for value 2 or 4
        if (distProbability(mersenneTwister) <= 0.9) {
          // 1st random tile is 2
          tileValue = 2;
        } else {
          // 1st random tile is 4
          tileValue = 4;
          }
          // Set Tile at random Position
          Tile targetTile = Tile(2);
          int posX = 0;
          int posY = 0;
          while (!targetTile.isEmpty()) {
            posX = distPosition(mersenneTwister);
            posY = distPosition(mersenneTwister);
            targetTile = this->_board[posX][posY];
          }
          this->_board[posX][posY] = Tile(tileValue);
          break;
      }
      break;
    }
  break;
  }
}

// ============================================================================

Tile Logx::getNextTile(int currentX, int currentY, int dir) {
  if (dir == direction.down) {
    for (int y = currentY; y < 4; y++) {
      if (this->_board[currentX][y].getValue() != 0) {
        Tile foundTile = this->_board[currentX][y];
        this->_board[currentX][y].destroy();
        return foundTile;
      }
    }
  } else if (dir == direction.up) {
    for (int y = currentY; y >= 0; y--) {
      if (this->_board[currentX][y].getValue() != 0) {
        Tile foundTile = this->_board[currentX][y];
        this->_board[currentX][y].destroy();
        return foundTile;
      }
    }
  } else if (dir == direction.left) {
    for (int x = currentX; x <= 3; x++) {
      if (this->_board[x][currentY].getValue() != 0) {
        Tile foundTile = this->_board[x][currentY];
        this->_board[x][currentY].destroy();
        return foundTile;
      }
    }
  } else if (dir == direction.right) {
    for (int x = currentX; x >= 0; x--) {
      if (this->_board[x][currentY].getValue() != 0) {
        Tile foundTile = this->_board[x][currentY];
        this->_board[x][currentY].destroy();
        return foundTile;
      }
    }
  }
  return Tile(0);
}

// ============================================================================

void Logx::removeEmptySpacesTiles(int removeDirection) {
  if (removeDirection == direction.up) {
    // Move empty tiles up
    for (int x = 0; x < 4; x++) {
      for (int y = 0; y < 3; y++) {
        if (this->_board[x][y].getValue() == 0) {
          this->_board[x][y] = this->getNextTile(x, y, direction.down);
        }
      }
    }
  } else if (removeDirection == direction.down) {
    // Move empty tiles down
    for (int x = 3; x >= 0; x--) {
      for (int y = 3; y >= 0; y--) {
        if (this->_board[x][y].getValue() == 0) {
          this->_board[x][y] = this->getNextTile(x, y, direction.up);
        }
      }
    }
  } else if (removeDirection == direction.left) {
    for (int x = 3; x >= 0; x--) {
      for (int y = 3; y >= 0; y--) {
        if (this->_board[x][y].getValue() == 0) {
          this->_board[x][y] = this->getNextTile(x, y, direction.left);
        }
      }
    }
  } else if (removeDirection == direction.right) {
    for (int x = 0; x <= 3; x++) {
      for (int y = 0; y <= 3; y++) {
        if (this->_board[x][y].getValue() == 0) {
          this->_board[x][y] = this->getNextTile(x, y, direction.right);
        }
      }
    }
  }
}
