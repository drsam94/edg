#include "ChoiceAdapter.h"
#include "GodBook.h"

ActionID TTYChoiceAdapter::chooseAction(const std::vector<ActionID> &hand) {
    const GodBook &gb = GodBook::instance();
    out << "Pick an action:\n";
    for (size_t i = 0; i < hand.size(); ++i) {
        out << i << ": " << gb.getAction(hand[i]) << "\n";
    }
    // error handling and stuff
    int choice;
    in >> choice;
    if (choice < 0) {
        return ActionID::Unset;
    } else {
        return hand[choice];
    }
}
