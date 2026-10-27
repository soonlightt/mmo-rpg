#include "Units/Carry/Lifestealer.h"

Lifestealer::Lifestealer(int x, int y) : Carry(x, y, Health(130), Armor(10), Attack(25)) {}

std::string Lifestealer::getType() const { return "Lifestealer"; }

char Lifestealer::getSymbol() const { return 'L'; }

std::unique_ptr<Unit> Lifestealer::clone() const { return std::make_unique<Lifestealer>(*this); }