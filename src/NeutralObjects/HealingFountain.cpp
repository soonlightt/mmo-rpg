#include "NeutralObjects/HealingFountain.h"

#include "Strategies/FullHealStrategy.h"

HealingFountain::HealingFountain(int x, int y)
    : NeutralObject(x, y, std::make_unique<FullHealStrategy>()) {}

std::string HealingFountain::getType() const { return "HealingFountain"; }

char HealingFountain::getSymbol() const { return '+'; }

std::string HealingFountain::getColoredSymbol() const { return "\033[92m+\033[0m"; }