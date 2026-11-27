#pragma once
#include <iostream>
#include <sstream>

#include "Handlers/Handler.h"

class AttackHandler : public Handler {
 public:
  bool handle(const std::string& input, GameFacade& facade) override;
};