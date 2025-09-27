#pragma once

#include <cstdint>
#include <vector>
#include <random>
#include <algorithm>
#include "card.h"

class Deck {
public:
  Deck();
  void shuffle();
  Card draw();
  std::size_t size() const;
  bool empty() const { return size() == 0; };

private:
  std::vector<Card> cards_;
  size_t next_ = 0;
  void build_standard();
  std::mt19937 rng_{std::random_device{}()};
};