#include "Units/Support/Dazzle.h"

Dazzle::Dazzle(int x, int y) : Support(x, y, Health(100), Armor(10), Attack(20)) {}

std::string Dazzle::getType() const { return "Dazzle"; }

char Dazzle::getSymbol() const { return 'D'; }

std::unique_ptr<Unit> Dazzle::clone() const { return std::make_unique<Dazzle>(*this); }