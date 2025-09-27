#pragma once

#include "player.h"
#include "dealer.h"
#include "deck.h"
#include <vector>
#include <map>
#include <unordered_set>

enum class State : std::uint8_t{ PREFLOP, FLOP, TURN, RIVER };

class Game {
public:
  Game(std::vector<Player> players) {
    players_ = players;
  } 

  void play_one_game() {
    dealer_.shuffle();
    // check playing
    state_ = State::PREFLOP;
    std::vector<Player> playing = get_playing();
    check_playing_init();

    state_ = State::FLOP;
    dealer_.deal_flop();
    run_betting_round();
    
    state_ = State::TURN;
    dealer_.deal_turn();
    run_betting_round();

    state_ = State::RIVER;
    dealer_.deal_river();
    run_betting_round();

    // Showdown
    determine_winner();
    pot_.clear();
  }

  void determine_winner() {
    Player* best = nullptr;
    std::vector<Player> playing = get_playing();
    int highest_hand = -1;
    for (auto &p : playing) {
      int hand = static_cast<int>(p.cards()[0].rank());
      if (highest_hand < hand) {
        best = &p;
        highest_hand = hand;
      }
    }
    for (auto& [n, v] : pot_) {
      best->chips_mut()[v] += n;
    }
  }

  void run_betting_round() {
    int highest_bet = 0;
    for (size_t i = 0; i < players_.size(); i++) {
      Player& p = players_[i];
      if (!p.is_playing()) continue;
      auto active_before = get_active_before(i);
      Action decision = p.decide(active_before, highest_bet);
      if (decision == Action::FOLD) {
        p.set_playing(false);
      } else if (decision == Action::CALL) {
        auto [n, v] = dealer_.take_input_chips(p);
        pot_[v] += n;
      } else if (decision == Action::RAISE) {
        auto [n, v] = dealer_.take_input_chips(p);
        highest_bet = n * v;
        pot_[v] += n;
      }
    }
  }

  auto get_playing() -> std::vector<Player> {
    std::vector<Player> playing;
    for (auto& p : players_) {
      if (p.is_playing()) {
        playing.push_back(p);
      }
    }
    return playing;
  }

  void check_playing_init() {
    for (auto& p : players_) {
      if (p.is_playing()) {
        dealer_.deal_hole_cards(p);
        dealer_.take_chips(p, 1, 50);
        pot_[50]++;
      }
    }
  }

  auto get_pot_total() -> int {
    int sum = 0;
    for (auto [k, v] : pot_) {
      sum += k * v;
    }
    return sum;
  }

  std::vector<int> get_active_before(size_t current_idx) const {
    std::vector<int> ids;
    for (size_t i = 0; i < current_idx; i++) {
      if (players_[i].is_playing()) {
        ids.push_back(static_cast<int>(i));
      }
    }
    return ids;
  }

private:
  std::vector<Player> players_;
  Dealer dealer_;
  std::vector<Card> community_;
  std::map<int, int, std::greater<int>> pot_;
  State state_;
};