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

void ChoiceAdapter::displayHand(const std::vector<ActionID> &hand) {
    const GodBook &gb = GodBook::instance();
    for (size_t i = 0; i < hand.size(); ++i) {
        display() << i << ": " << gb.getAction(hand[i]) << "\n";
    }
}

ActionID ChoiceAdapter::chooseAction(const std::vector<ActionID> &hand) {
    display() << "Pick an action:\n";
    displayHand(hand);
    // error handling and stuff
    int choice = getChoice();
    if (choice < 0) {
        return ActionID::Unset;
    } else {
        return hand[choice];
    }
}

std::vector<int> ChoiceAdapter::chooseCardsFromHand(const std::vector<ActionID> &hand,
        int atMost) {
    display() << "Pick cards from your hand (max: " << atMost << ")\n";
    displayHand(hand);
    int input;
    std::vector<int> choices;
    while (atMost < 0 || choices.size() < static_cast<size_t>(atMost)) {
        input = getChoice();
        if (input == -1) break;
        if (std::find(choices.begin() + 1, choices.end(), input) != choices.end())
            choices.push_back(input);
    }
    return choices;
}

std::vector<int> ChoiceAdapter::chooseRole(const RoleState &roles, int atMost) {
    display() << "Choose (" << atMost << ") roles\n";
    size_t i = 0;
    for (Role role : Role::values()) {
        display() << i++ << ": " << role.str() << " (" << static_cast<int>(roles.count(role)) << " left)\n";
    }
    std::vector<int> choices;
    int input;
    while (choices.size() < static_cast<size_t>(atMost)) {
        input = getChoice();
        if (input == -1) break;
        choices.push_back(input);
    }
    return choices;
}

std::vector<int> ChoiceAdapter::chooseOneOfPlanetCards(const std::vector<PlanetID> &planets) {
    display() << "Choice a Planet Card:\n";
    size_t i = 0;
    for (PlanetID planet : planets) {
        display() << i++ << ": " << GodBook::instance().getPlanet(planet) << "\n";
    }
    return { getChoice() };
}

std::vector<int> ChoiceAdapter::chooseOneOfFDPlanets(const std::vector<PlanetState> &planets) {
    display() << "Choose a Planet:\n";
    size_t i = 0;
    for (const PlanetState &state : planets) {
        display() << i++ << ": " << state << "\n";
    }
    return { getChoice() };
}

int TTYChoiceAdapter::getChoice() {
    int input;
    in >> input;
    return input;
}

std::ostream &TTYChoiceAdapter::display() {
    return out;
}
