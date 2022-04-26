// Copyright 2020
// Author: Daniel Schindler <danielmschindler1@googlemail.com>

#ifndef TILE_H_
#define TILE_H_

#include <gtest/gtest.h>
// =================================

class Tile {
  // Class Tile represents the individual (integer fields on the board)
 public:
  // Constructor for Tile Class.
  Tile();

  // Constructor for specific value.
  explicit Tile(int value);

  // Destructor for Tile Class.
  ~Tile();

  // Merges another tile into this one (addition if values are right)
  int merge(Tile *other, int turn);

  // This method "destroys" a tile by setting it to 0
  void destroy();

  // This method returns the value of a given field. Returns an int
  int getValue() const;

  // This method checks if a certain field is empty
  // returns true or false
  bool isEmpty() const;

  // This method returns the turn number of the last merge
  int getLastMerge() const;

 private:
  // ===========TESTS=============
  FRIEND_TEST(Tile, Constructor);
  FRIEND_TEST(Tile, ConstructorInt);
  FRIEND_TEST(Tile, merge);
  FRIEND_TEST(Tile, destroy);
  FRIEND_TEST(Tile, getValue);
  FRIEND_TEST(Tile, isEmpty);
  FRIEND_TEST(Tile, getLastMerge);
  // ===========TESTS=============

  // Value of the field
  // ie: 1024
  int _value;

  // To remember in which turn the Tile was merged last time
  int _lastMerge;
};

#endif  // TILE_H_
