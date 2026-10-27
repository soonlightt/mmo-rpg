#include "Units/Midder/ArcWarden.h"

ArcWarden::ArcWarden(int x, int y) : Midder(x, y, Health(125), Armor(10), Attack(35)) {}

std::string ArcWarden::getType() const { return "ArcWarden"; }

char ArcWarden::getSymbol() const { return 'A'; }

std::unique_ptr<Unit> ArcWarden::clone() const { return std::make_unique<ArcWarden>(*this); }