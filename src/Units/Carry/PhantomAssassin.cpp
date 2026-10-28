#include "Units/Carry/PhantomAssassin.h"

PhantomAssassin::PhantomAssassin(int x, int y) : Carry(x, y, Health(120), Armor(15), Attack(30)) {}

std::string PhantomAssassin::getType() const { return "PhantomAssassin"; }

char PhantomAssassin::getSymbol() const { return 'P'; }

std::unique_ptr<Unit> PhantomAssassin::clone() const {
  return std::make_unique<PhantomAssassin>(*this);
}