// Copyright 2020
// Author: Daniel Schindler <danielmschindler1@googlemail.com>

#ifndef GRAFX_H_
#define GRAFX_H_

#include <gtest/gtest.h>
#include "./logx.h"
// =================================

class Grafx {
  // Class Grafx holds everything related to displaying the game.
 public:
  // Constructor for Grafx Class.
  Grafx();

  // Destructor for Grafx Class. Will clean up the terminal.
  ~Grafx();

  // Preparing/setup of the terminal for drawing with ncurses.
  static void initializeGame();

  // This method takes the user input, then acts accordingly
  // ie: If right arrow is pressed, all tiles will be summed up with their
  // neighboring tiles IF the value of said tiles is identical to one another.
  int processUserInput(const int& key);

  // This method prints the current score and turns taken. Also prints the
  // values of 'board' field.
  void displayGame();

  // This method draws a single tile under given offsets while applying a given
  // color
  static void drawTile(int xoffset, int yoffset, int colorpair);

  // This method draws the entire game board, while colorin each tile with its
  // destined color (dependend on logic board tile's values)
  void drawBoard();

  // This method superimposes the logic board's values over the graphics board
  void fillTile();

  // Playing the game (scanning for user input using 'processUserInput' and
  // acting accordingly).
  // This method also takes care of exiting the game if winlosechecker()
  // returns 2 or 3, OR the player hits the ESC button.
  void play();

  // This method determines the colors used for the individual tiles at
  // given x and y coordinates in the logic field
  int setTileColor(uint_fast8_t x, uint_fast8_t y);

  // var _exiter is just there for exiting the game. Declaring this as a public
  // var so it's easily accessible
  int _exiter{};

 private:
  // =============TESTS==================
  FRIEND_TEST(Grafx, processUserInput);
  // =============TESTS==================

  // Game logic
  Logx _logx;
};

#endif  // GRAFX_H_
