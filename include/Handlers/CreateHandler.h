#pragma once
#include <iostream>
#include <sstream>

#include "Handlers/Handler.h"

class CreateHandler : public Handler {
 public:
  bool handle(const std::string& input, GameFacade& facade) override;
};