#include "Units/Support/WitchDoctor.h"

WitchDoctor::WitchDoctor(int x, int y) : Support(x, y, Health(100), Armor(10), Attack(20)) {}

std::string WitchDoctor::getType() const { return "WitchDoctor"; }

char WitchDoctor::getSymbol() const { return 'W'; }

std::unique_ptr<Unit> WitchDoctor::clone() const { return std::make_unique<WitchDoctor>(*this); }