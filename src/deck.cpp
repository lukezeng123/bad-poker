#include "deck.h"
#include <array>
#include <algorithm>
#include <stdexcept>


Deck::Deck() { build_standard(); }

void Deck::build_standard() {
  cards_.clear();
  cards_.reserve(52);
  const std::array<Suit, 4> suits{
    Suit::Clubs, Suit::Diamonds, Suit::Hearts, Suit::Spades
  };
  for (Suit s : suits) {
    for (int r = static_cast<int>(Rank::Two);
         r <= static_cast<int>(Rank::Ace); ++r) {
      cards_.emplace_back(static_cast<Rank>(r), s);
    }
  }
  next_ = 0;
}

void Deck::shuffle() {
  std::shuffle(cards_.begin(), cards_.end(), rng_);
  next_ = 0;
}

Card Deck::draw() {
  if (next_ >= cards_.size())
    throw std::out_of_range("Deck::draw: empty deck");
  return cards_[next_++];
}

std::size_t Deck::size() const {
  return cards_.size() - next_;
}