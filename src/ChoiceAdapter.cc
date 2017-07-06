#include "ChoiceAdapter.h"
#include "GodBook.h"

std::vector<int> ChoiceAdapter::getDissentBoostFollowChoice(Role role, bool lead,
            const std::vector<ActionID> &hand) {
    if (!lead) {
        return composedAlternatives("Dissent",
                &ChoiceAdapter::nullChoice, std::make_tuple(),
                "Follow", &ChoiceAdapter::chooseCardsFromHand,
                std::make_tuple(std::cref(hand), -1)) ;
    } else {
        return chooseCardsFromHand(hand, -1);
    }
}

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

std::vector<int> TTYChoiceAdapter::chooseCardsFromHand(const std::vector<ActionID> &hand,
        int atMost) {
    out << "Pick cards from your hand (max: " << atMost << ")\n";
    displayHand(hand);
    int input;
    std::vector<int> choices;
    while (atMost < 0 || choices.size() < static_cast<size_t>(atMost)) {
        in >> input;
        if (input == -1) break;
        if (std::find(choices.begin() + 1, choices.end(), input) != choices.end())
            choices.push_back(input);
    }
    return choices;
}

std::vector<int> TTYChoiceAdapter::chooseRole(const RoleState &roles, int atMost) {
    out << "Choose (" << atMost << ") roles\n";
    size_t i = 0;
    for (Role role : Role::values()) {
        out << i++ << ": " << role.str() << " (" << static_cast<int>(roles.count(role)) << " left)\n";
    }
    std::vector<int> choices;
    int input;
    while (choices.size() < static_cast<size_t>(atMost)) {
        in >> input;
        if (input == -1) break;
        choices.push_back(input);
    }
    return choices;
}
