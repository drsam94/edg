#include "ChoiceAdapter.h"
#include "GodBook.h"

void TTYChoiceAdapter::displayHand(const std::vector<ActionID> &hand) {
    const GodBook &gb = GodBook::instance();
    for (size_t i = 0; i < hand.size(); ++i) {
        out << i << ": " << gb.getAction(hand[i]) << "\n";
    }
}

ActionID TTYChoiceAdapter::chooseAction(const std::vector<ActionID> &hand) {
    out << "Pick an action:\n";
    displayHand(hand);
    // error handling and stuff
    int choice;
    in >> choice;
    if (choice < 0) {
        return ActionID::Unset;
    } else {
        return hand[choice];
    }
}

std::vector<int> TTYChoiceAdapter::getDissentBoostFollowChoice(Role role, bool lead,
    const std::vector<ActionID> &hand) {
    std::vector<int> choices;
    if (lead) {
        // filler
        choices.emplace_back();
    } else {
        out << "Dissent or follow role " << role.str() << "?\n0: Dissent\n1: Follow\n";
        choices.emplace_back();
        in >> choices[0];
    }
    out << "Pick any number of actions to " << (lead ? "boost" : "follow") << " with (-1 to end)\n";
    // TODO: only display legal choices, auto-end when no choices left
    displayHand(hand);
    int input;
    while (true) {
        in >> input;
        if (input == -1) break;
        if (std::find(choices.begin() + 1, choices.end(), input) != choices.end())
            choices.push_back(input);
    }
    return choices;
}
