#include "Units/Midder/Invoker.h"

Invoker::Invoker(int x, int y) : Midder(x, y, Health(110), Armor(15), Attack(45)) {}

std::string Invoker::getType() const { return "Invoker"; }

char Invoker::getSymbol() const { return 'I'; }

std::unique_ptr<Unit> Invoker::clone() const { return std::make_unique<Invoker>(*this); }