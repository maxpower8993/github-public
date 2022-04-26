// Copyright 2020
// Author: Daniel Schindler <danielmschindler1@googlemail.com>

#ifndef LOGX_H_
#define LOGX_H_

#include <gtest/gtest.h>
#include "./tile.h"
#include "./direction.h"
// =================================

class Logx {
  // Class Logx holds everything related to the underlying game logic.
 public:
  // Constructor for Logx Class. This initializes the entire logic board
  // and game states
  Logx();

  // Destructor for Logx Class.
  ~Logx();

  // This method calculates the upcoming state of the board according to
  // the detected user input coming from "processUserInput()" and compute next
  // game steps
  void updateState(int shiftDirection);

  // This method checks if there are no free and mergeable tiles left,
  // then set var _gameoverstate = true
  bool checkFreeAndMerge();

  // Getter for var _score
  uint32_t getScore() const;

  // Getter for var _turns
  int getTurns() const;

  // Getter for var _winstate
  bool getWinState() const;

  // Getter for var _gameOverState
  bool getGameOverState() const;

  // This method checks the game board after every turn if LOSE or WIN
  // conditions are being met, and display a message informing the player
  // on how to restart, exit or continue the game
  uint_fast8_t isGameOver();

  // Getter for specific board value
  Tile* getTileAtPos(int x, int y);

  // Set a random Tile on Board, chance of 90% value 2, 10% value 4 specs.
  void setRandomTileOnBoard();

  // Get next Tile in direction from the current position
  Tile getNextTile(int currentX, int currentY, int dir);

  // Remove empty Tiles in a direction
  void removeEmptySpacesTiles(int removeDirection);

 private:
  // ===========TESTS=============
  FRIEND_TEST(Logx, Constructor);
  FRIEND_TEST(Logx, updateStateUp);
  FRIEND_TEST(Logx, updateStateDown);
  FRIEND_TEST(Logx, updateStateLeft);
  FRIEND_TEST(Logx, updateStateRight);
  FRIEND_TEST(Logx, isGameWon);
  FRIEND_TEST(Logx, isGameLost);
  FRIEND_TEST(Logx, getTileAtPos);
  FRIEND_TEST(Logx, checkFreeAndMerge);
  FRIEND_TEST(Logx, getters);
  FRIEND_TEST(Logx, setRandomTileOnBoard);
  FRIEND_TEST(Logx, removeEmptySpacesTiles);
  // ===========TESTS=============

  // 'var score' saves the current score achieved by the player.
  // ie: 1024
  uint32_t _score;

  // 'var turns' saves the amount of turns taken by the player.
  // ie: 33
  uint16_t _turns;

  // 'var winstate' is set either True or False, depending on the player's
  // performance. If 2048 is reached on a tile, winstate will be set True, else
  // False.
  bool _winstate;

  // 'var gameoverstate' is set either True or False. True when there's no free
  // space for a new field or the highest possible score of 3.932.100 is reached
  // else False.
  bool _gameoverstate;

  // 'board' holds the current state of the playing field.
  Tile _board[4][4] = {{ Tile() }};
};

#endif  // LOGX_H_
