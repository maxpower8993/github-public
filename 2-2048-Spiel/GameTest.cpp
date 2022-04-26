// Copyright 2020
// Author: Daniel Schindler <danielmschindler1@googlemail.com>

#include <gtest/gtest.h>
#include <ncurses.h>
#include "./logx.h"
#include "./grafx.h"
// =================================

// =============================LOGX TESTS=====================================

TEST(Logx, Constructor) {
  Logx logx;

  ASSERT_EQ(0, logx._score);
  ASSERT_EQ(0, logx._turns);
  ASSERT_FALSE(logx._winstate);
  ASSERT_FALSE(logx._gameoverstate);
}

TEST(Logx, updateStateUp) {
  Logx logx;

  logx._board[0][0] = Tile(512);
  logx._board[0][1] = Tile(512);
  logx._board[3][0] = Tile(2);
  logx._board[3][1] = Tile(4);
  logx._board[3][2] = Tile(256);
  logx._board[3][3] = Tile(256);

  logx.updateState(direction.up);

  ASSERT_EQ(1024, logx._board[0][0].getValue());
  ASSERT_EQ(1, logx._board[0][0].getLastMerge());

  ASSERT_EQ(2, logx._board[3][0].getValue());
  ASSERT_EQ(0, logx._board[3][0].getLastMerge());
  ASSERT_EQ(4, logx._board[3][1].getValue());
  ASSERT_EQ(0, logx._board[3][1].getLastMerge());
  ASSERT_EQ(512, logx._board[3][2].getValue());
  ASSERT_EQ(1, logx._board[3][2].getLastMerge());
}

TEST(Logx, updateStateDown) {
  Logx logx;
  // Resetting board
  for (auto & x : logx._board) {
     for (int y = 0; y < 4; y++) {
      x[y].destroy();
    }
  }

  logx._board[0][0] = Tile(512);
  logx._board[0][1] = Tile(512);
  logx._board[3][0] = Tile(2);
  logx._board[3][1] = Tile(4);
  logx._board[3][2] = Tile(256);
  logx._board[3][3] = Tile(256);

  logx.updateState(direction.down);

  ASSERT_EQ(1024, logx._board[0][3].getValue());
  ASSERT_EQ(1, logx._board[0][3].getLastMerge());
  ASSERT_EQ(512, logx._board[3][3].getValue());
  ASSERT_EQ(1, logx._board[3][3].getLastMerge());
}

TEST(Logx, updateStateLeft) {
  Logx logx;
  // Resetting board
  for (auto & x : logx._board) {
     for (int y = 0; y < 4; y++) {
      x[y].destroy();
    }
  }

  logx._board[0][0] = Tile(512);
  logx._board[0][1] = Tile(512);
  logx._board[3][0] = Tile(2);
  logx._board[3][1] = Tile(4);
  logx._board[3][2] = Tile(256);
  logx._board[3][3] = Tile(256);

  logx.updateState(direction.left);

  ASSERT_EQ(512, logx._board[0][0].getValue());
  ASSERT_EQ(2, logx._board[1][0].getValue());
  ASSERT_EQ(4, logx._board[1][1].getValue());
  ASSERT_EQ(256, logx._board[0][2].getValue());
  ASSERT_EQ(256, logx._board[0][3].getValue());
}

TEST(Logx, updateStateRight) {
  Logx logx;
  // Resetting board
  for (auto & x : logx._board) {
     for (int y = 0; y < 4; y++) {
      x[y].destroy();
    }
  }

  logx._board[0][0] = Tile(512);
  logx._board[0][1] = Tile(512);
  logx._board[3][0] = Tile(2);
  logx._board[3][1] = Tile(4);
  logx._board[3][2] = Tile(256);
  logx._board[3][3] = Tile(256);

  logx.updateState(direction.right);

  ASSERT_EQ(512, logx._board[2][0].getValue());
  ASSERT_EQ(512, logx._board[2][1].getValue());
  ASSERT_EQ(256, logx._board[3][2].getValue());
  ASSERT_EQ(256, logx._board[3][3].getValue());
}

TEST(Logx, isGameWon) {
  Logx logx;

  logx._board[2][3] = Tile(8);
  logx._board[1][2] = Tile(2048);
  ASSERT_EQ(2, logx.isGameOver());
  ASSERT_TRUE(logx._winstate);      // WIN CHECK
}

TEST(Logx, isGameLost) {
  Logx logx;
  int pseudovalue = 1;
  // filling the board with gargabe values, so no merge is possible
  for (auto & x : logx._board) {
    for (int y = 0; y < 4; y++) {
      x[y] = Tile(pseudovalue);
      pseudovalue++;
    }
  }

  ASSERT_TRUE(logx.checkFreeAndMerge());
  ASSERT_TRUE(logx._gameoverstate);
}

TEST(Logx, getTileAtPos) {
  Logx logx;

  logx._board[2][3] = Tile(2048);

  Tile* tileGot = logx.getTileAtPos(2, 3);

  ASSERT_EQ(tileGot, &logx._board[2][3]);
  ASSERT_EQ(2048, tileGot->getValue());
  ASSERT_EQ(0, tileGot->getLastMerge());
  ASSERT_FALSE(tileGot->isEmpty());
}

TEST(Logx, checkFreeAndMerge) {
  Logx logx;

  // Resetting board
  for (auto & x : logx._board) {
     for (int y = 0; y < 4; y++) {
      x[y].destroy();
    }
  }

  logx._board[0][0] = Tile(512);
  logx._board[0][1] = Tile(512);
  logx._board[3][0] = Tile(2);
  logx._board[3][1] = Tile(4);
  logx._board[3][2] = Tile(256);
  logx._board[3][3] = Tile(256);

  ASSERT_EQ(false, logx.checkFreeAndMerge());
}

TEST(Logx, getters) {
  Logx logx;

  logx._score = 99999999;
  logx._turns = 999;
  logx._winstate = true;
  logx._gameoverstate = false;

  ASSERT_EQ(99999999, logx.getScore());
  ASSERT_EQ(999, logx.getTurns());
  ASSERT_EQ(true, logx.getWinState());
  ASSERT_EQ(false, logx.getGameOverState());
}

TEST(Logx, setRandomTileOnBoard) {
  Logx logx;

  for (auto & x : logx._board) {
    for (int y = 0; y < 4; y++) {
      x[y].destroy();
    }
  }

  logx.setRandomTileOnBoard();

  bool randdetected = false;
  for (auto & x : logx._board) {
    for (int y = 0; y < 4; y++) {
      if (x[y].getValue() == 2 || x[y].getValue() == 4) {
        randdetected = true;
        break;
      }
    }
  }
  ASSERT_EQ(true, randdetected);
}

TEST(Logx, removeEmptySpacesTiles) {
  Logx logx;

  // Resetting board
  for (auto & x : logx._board) {
     for (int y = 0; y < 4; y++) {
      x[y].destroy();
    }
  }

  logx._board[0][0] = Tile(512);
  logx._board[0][1] = Tile(512);
  logx._board[3][0] = Tile(2);
  logx._board[3][1] = Tile(4);
  logx._board[3][2] = Tile(256);
  logx._board[3][3] = Tile(256);

  logx.updateState(direction.up);
  logx.removeEmptySpacesTiles(direction.up);

  ASSERT_EQ(0, logx._board[0][1].getValue());
}

// =============================LOGX TESTS END=================================

// =============================TILE TESTS=====================================

TEST(Tile, Constructor) {
  Tile tile;

  ASSERT_EQ(0, tile._value);
  ASSERT_EQ(0, tile._lastMerge);
}

TEST(Tile, ConstructorInt) {
  Tile tile = Tile(1024);

  ASSERT_EQ(1024, tile._value);
  ASSERT_EQ(0, tile._lastMerge);
}

TEST(Tile, merge) {
  Tile tile1 = Tile(256);
  Tile tile2 = Tile(256);
  Tile tile3 = Tile(256);

  // Test merge of equal tiles
  tile1.merge(&tile2, 15);

  ASSERT_EQ(512, tile1.getValue());
  ASSERT_EQ(15, tile1.getLastMerge());
  ASSERT_EQ(0, tile2.getValue());
  ASSERT_EQ(0, tile2.getLastMerge());

  // Test merge of unequal tiles (512 and 256)
  tile1.merge(&tile3, 16);
  ASSERT_EQ(512, tile1.getValue());
  ASSERT_EQ(15, tile1.getLastMerge());
  ASSERT_EQ(256, tile3.getValue());
  ASSERT_EQ(0, tile3.getLastMerge());
}

TEST(Tile, destroy) {
  Tile tile = Tile(512);

  tile.destroy();

  ASSERT_EQ(0, tile.getValue());
  ASSERT_EQ(0, tile._lastMerge);
}

TEST(Tile, getValue) {
  Tile field1 = Tile(1024);
  Tile field2 = Tile(2048);

  ASSERT_EQ(1024, field1.getValue());
  ASSERT_EQ(2048, field2.getValue());
}

TEST(Tile, isEmpty) {
  Tile field1 = Tile(0);
  Tile field2 = Tile(2048);
  Tile field3 = Tile(0);
  field3._lastMerge = 1;

  ASSERT_TRUE(field1.isEmpty());
  ASSERT_FALSE(field2.isEmpty());
  ASSERT_FALSE(field3.isEmpty());
}

TEST(Tile, getLastMerge) {
  Tile field1 = Tile(1024);
  Tile field2 = Tile(2048);
  field2._lastMerge = 3;

  ASSERT_EQ(0, field1.getLastMerge());
  ASSERT_EQ(3, field2.getLastMerge());
}

// =============================TILE TESTS END=================================

// =========================DIRECTION TESTS====================================

TEST(Direction, Struct) {
  ASSERT_EQ(0, direction.up);
  ASSERT_EQ(1, direction.down);
  ASSERT_EQ(2, direction.left);
  ASSERT_EQ(3, direction.right);
}
// =========================DIRECTION TESTS END================================

// ============================GRAFX TESTS=====================================

TEST(Grafx, processUserInput) {
  Grafx grafx;

  ASSERT_EQ(0, grafx.processUserInput(27));  // ESC
  ASSERT_EQ(1, grafx.processUserInput(KEY_UP));
  ASSERT_EQ(2, grafx.processUserInput(KEY_DOWN));
  ASSERT_EQ(3, grafx.processUserInput(KEY_LEFT));
  ASSERT_EQ(4, grafx.processUserInput(KEY_RIGHT));
  ASSERT_EQ(5, grafx.processUserInput('n'));
  ASSERT_EQ(6, grafx.processUserInput('u'));
  ASSERT_EQ(-1, grafx.processUserInput('a'));  // Garbage input test
}
// ============================GRAFX TESTS END=================================
