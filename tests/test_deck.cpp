#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "deck.h"

TEST_CASE("new deck has 52 cards") {
  Deck d;
  REQUIRE(d.size() == 52);
}