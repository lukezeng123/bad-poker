#pragma once

#include <cstdint>
#include <string>
enum class Suit : std::uint8_t { Clubs, Diamonds, Spades, Hearts};
enum class Rank : uint8_t { Two=2, Three, Four, Five, Six, Seven, Eight,
  Nine, Ten, Jack, Queen, King, Ace };

class Card {
public:
  Card(Rank r, Suit s) : rank_(r), suit_(s) {}
  Rank rank() const noexcept { return rank_; }
  Suit suit() const noexcept { return suit_; }
private:
  Rank rank_;
  Suit suit_;
};