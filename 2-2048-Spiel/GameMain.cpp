// Copyright 2020
// Author: Daniel Schindler <danielmschindler1@googlemail.com>

#include "./grafx.h"
// =================================

int main() {
  // creating new object "grafx" from class Grafx.
  Grafx grafx;

  // Initializing game (setting up terminal).
  Grafx::initializeGame();

  // Launch game (listening for key input and acting accordingly).
  // play() serves as the game loop and takes care of terminating the game
  // once the requirements are met.
  while (true) {
    grafx.play();
    if (grafx._exiter == 1) {
      break;
    }
  }
}
