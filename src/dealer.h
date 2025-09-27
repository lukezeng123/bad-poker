#pragma once


#include "player.h"
#include "game.h"
#include "deck.h"
#include <vector>

class Dealer {
public:
  explicit Dealer(Deck d = Deck()) : deck_(std::move(d)) {}
  void shuffle() {deck_.shuffle();}

  void deal_hole_cards(Player& p) {
    p.cards_mut().push_back(deck_.draw());
    p.cards_mut().push_back(deck_.draw());
  }

  std::vector<Card> deal_flop(bool burn=true) {
    if (burn) deck_.draw();
    return {deck_.draw(), deck_.draw(), deck_.draw()};
  }

  Card deal_turn(bool burn=true) {
    if (burn) deck_.draw();
    return deck_.draw();
  }

  Card deal_river(bool burn=true) {
    if (burn) deck_.draw();
    return deck_.draw();
  }

  auto take_chips(Player& p, int denom, int num) -> bool {
    auto& chips = p.chips_mut();
    auto it = chips.find(denom);
    if (it == chips.end() || it->second < num) return false;
    it->second -= num;
    return true;
  }


  auto take_input_chips(Player& p) -> std::pair<int,int> {
    int val = p.input_.first;
    int num = p.input_.second;
    p.chips_.find(val)->second -= num;
    p.input_ = {0,0};
    return std::pair<int,int>{val, num};
  }

private:
  Deck deck_;
};