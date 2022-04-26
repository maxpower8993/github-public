// Copyright 2020
// Author: Daniel Schindler <danielmschindler1@googlemail.com>

#include "./grafx.h"
#include <ncurses.h>
// =================================

Grafx::Grafx() {
  this->_logx = Logx();
}

Grafx::~Grafx() {
  endwin();
}

// ============================================================================

void Grafx::initializeGame() {
  // ready terminal
  initscr();
  cbreak();
  noecho();
  curs_set(false);
  nodelay(stdscr, true);
  keypad(stdscr, true);
  start_color();
}

// ============================================================================

void Grafx::displayGame() {
  // =======PRINT GAME TITLE AND STATISTICS==========
  mvprintw(0, 10, "-~| 2 0 4 8 EARLY ACCESS SEASON PASS EDITION |~-");
  mvprintw(1, 10, "SCORE: [%d]", this->_logx.getScore());
  mvprintw(2, 10, "TURNS: [%d]", this->_logx.getTurns());
  mvprintw(1, 35, "WON?: [%d]", this->_logx.getWinState());
  mvprintw(2, 35, "GAMEOVER?: [%d]", this->_logx.getGameOverState());
  mvprintw(3, 10, "PRESS n FOR NEW GAME || PRESS esc TO EXIT");

  // =======DEFINING COLORS==========
  init_pair(1, COLOR_BLACK, COLOR_BLACK);       // 0
  init_pair(2, COLOR_BLUE, COLOR_BLUE);         // 2
  init_pair(3, COLOR_YELLOW, COLOR_YELLOW);     // 4
  init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA);   // 8
  init_pair(5, COLOR_GREEN, COLOR_GREEN);       // 16
  init_pair(6, COLOR_RED, COLOR_RED);           // 32
  init_pair(7, COLOR_CYAN, COLOR_CYAN);         // 64
  init_pair(8, COLOR_WHITE, COLOR_WHITE);       // 128
  init_pair(9, 90, 90);                         // 256
  init_pair(10, 100, 100);                      // 512
  init_pair(11, 110, 110);                      // 1024
  init_pair(12, 120, 120);                      // 2048

  // =======DRAWING COLORED TILES============
  this->Grafx::drawBoard();
  // ======FILLING COLORED TILES WITH VALUES=======
  this->Grafx::fillTile();
  refresh();
}
// ============================================================================
void Grafx::drawTile(int xoffset, int yoffset, int colorpair) {
  attron(COLOR_PAIR(colorpair));
  for (int x = (10 + xoffset); x < (20 + xoffset); x++) {
    for (int y = (5 + yoffset); y < (10 + yoffset); y++) {
      mvprintw(y, x, " ");
    }
  }
  attroff(COLOR_PAIR(colorpair));
}
// ============================================================================
void Grafx::fillTile() {
  for (int x = 0; x < 4; x++) {
    mvprintw(7, 14+11*x, "%d", this->_logx.getTileAtPos(x, 0)->getValue());
    mvprintw(13, 14+11*x, "%d", this->_logx.getTileAtPos(x, 1)->getValue());
    mvprintw(19, 14+11*x, "%d", this->_logx.getTileAtPos(x, 2)->getValue());
    mvprintw(25, 14+11*x, "%d", this->_logx.getTileAtPos(x, 3)->getValue());
  }

  // FOR DEBUGGING: prints the logic board in the upper left corner

  // for (int x = 0; x < 4; x++) {
  //   for (int y = 0; y < 4; y++) {
  //     mvprintw(y, x, "%d", this->_logx.getTileAtPos(x, y)->getValue());
  //   }
  // }
}
// ============================================================================

void Grafx::drawBoard() {
  for (uint8_t x = 0; x < 4; x++) {
    for (uint8_t y = 0; y < 4; y++) {
      this->Grafx::drawTile(11*x, 6*y, this->Grafx::setTileColor(x, y));
    }
  }
}

// ============================================================================

int Grafx::processUserInput(const int& key) {
  switch (key) {
    case 27:  // ESC
      this->_exiter = 1;
      return 0;
    case KEY_UP:
      this->_logx.updateState(direction.up);
      return 1;
    case KEY_DOWN:
      this->_logx.updateState(direction.down);
      return 2;
    case KEY_LEFT:
      this->_logx.updateState(direction.left);
      return 3;
    case KEY_RIGHT:
      this->_logx.updateState(direction.right);
      return 4;
    case 'n':  // NEW GAME
      clear();                              // clear terminal completely
      // Get new instance of Logx, so the entire game will be reset
      // to zero / false
      this->_logx = Logx();
      return 5;
    case 'u':  // UNDO
      return 6;
  }return -1;  // Return -1 if garbage input
}

// ============================================================================

int Grafx::setTileColor(uint_fast8_t x, uint_fast8_t y) {
  // uint_fast8_t, because x and y are always < 4
  if (this->_logx.getTileAtPos(x, y)->getValue() == 0) {
    return 1;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() == 2) {
    return 2;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() == 4) {
    return 3;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() == 8) {
    return 4;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() == 16) {
    return 5;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() == 32) {
    return 6;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() == 64) {
    return 7;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() == 128) {
    return 8;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() == 256) {
    return 9;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() == 512) {
    return 10;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() == 1024) {
    return 11;
  }
  if (this->_logx.getTileAtPos(x, y)->getValue() >= 2048) {
    return 12;
  }
  return 0;
}

// ============================================================================

void Grafx::play() {
  this->Grafx::displayGame();
  refresh();
  int key = getch();
  this->Grafx::processUserInput(key);
  if (this->_logx.isGameOver() == 1) {
    mvprintw(30, 15, ")-: GAME OVER :-(");
    mvprintw(31, 10, "Press 'n' for new game or 'esc' to exit");
  } else if (this->_logx.isGameOver() == 2) {
      mvprintw(30, 25, "(-: GAME WON :-)");
      mvprintw(31, 13, "Press 'n' for new game or 'esc' to exit");
      mvprintw(32, 13, "        OR continue playing");
  }
}
