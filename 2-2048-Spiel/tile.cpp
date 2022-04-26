// Copyright 2020
// Author: Daniel Schindler <danielmschindler1@googlemail.com>

#include "./tile.h"
// =================================

Tile::Tile() {
  this->_value = 0;
  this->_lastMerge = 0;
}

Tile::Tile(int value) {
  this->_value = value;
  this->_lastMerge = 0;
}

Tile::~Tile() = default;

int Tile::merge(Tile *other, int turn) {
  // Check if tiles values are identical
  if (this->getValue() != other->getValue()) return 0;

  // Check if a tile is 0
  if (this->getValue() == 0 || other->getValue() == 0) return 0;

  // Combine values of tiles
  this->_value = this->_value + other->_value;

  // Set turn to this turn
  this->_lastMerge = turn;

  // Destroy the other tile
  other->destroy();

  return this->_value;
}

void Tile::destroy() {
  // Reset values
  this->_value = 0;
  this->_lastMerge = 0;
}

int Tile::getValue() const {
  return this->_value;
}

bool Tile::isEmpty() const {
  return this->getValue() == 0 && this->getLastMerge() == 0;
}

int Tile::getLastMerge() const {
    return this->_lastMerge;
}
