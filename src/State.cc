#include "State.h"
#include<random>

std::mt19937 GameState::rng{std::random_device{}()};

void PlayerState::draw(int cards) {
    do {
        while (!deck.empty() && cards-- > 0) {
            hand.push_back(deck.back());
            deck.pop_back();
        }
        if (deck.empty()) {
            std::shuffle(discard.begin(), discard.end(), GameState::rng);
            deck.swap(discard);
        }
    } while (cards && !deck.empty());
}
