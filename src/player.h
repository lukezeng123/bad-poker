#pragma once

#include "card.h"
#include <vector>
#include <map>
#include <cstdint>

enum class Action : std::uint8_t {FOLD, RAISE, CALL, CHECK, END};

class Dealer;
class Player {
public:
  Player(int id, std::map<int, int, std::greater<int>> chips, bool playing=true)
    : id_(id), chips_(std::move(chips)), playing_(playing) {}
  bool is_playing() const noexcept { return playing_; }
  void set_playing(bool b) noexcept { playing_ = b;}
  
  auto raise(int denom, int num) -> void {
    action_ = Action::RAISE;
    input_ = {denom, num};
  }

  auto fold() -> void {
    input_ = {0,0};
    action_ = Action::FOLD;
    playing_ = false;
  }

  auto call(std::pair<int, int> c) -> void {
    raise(c.first,c.second);
    action_ = Action::CALL; 
  }

  auto check() -> void {
    input_ = {0,0};
    action_ = Action::CHECK;
  }

  auto get_action() -> Action {
    return action_;
  }

  Action decide(const std::vector<int>& active_before, int highest_bet) {
    if (!active_before.empty()) {
      if (highest_bet > 50) {
        fold();
        return Action::FOLD;
      } else if (highest_bet != 0){
        call({1, highest_bet});
        return Action::CALL;
      } else {
        check();
        return Action::CHECK;
      }
    } else {
      raise(1, 50);
      return Action::RAISE;
    }
  }
  std::pair<int,int> input() const noexcept { return input_; }

  const std::vector<Card>& cards() const noexcept { return cards_; }
  std::vector<Card>& cards_mut() noexcept {return cards_; }
  std::map<int,int, std::greater<int>>& chips_mut() noexcept { return chips_; }
  const std::map<int,int, std::greater<int>>& chips() const noexcept { return chips_; }

  int id() const noexcept { return id_; }

private:
  friend class Dealer;
  int id_;
  std::vector<Card> cards_;
  std::pair<int, int> input_{0,0};
  Action action_ = Action::CHECK;
  std::map<int, int, std::greater<int>> chips_;
  bool playing_ = true;
};