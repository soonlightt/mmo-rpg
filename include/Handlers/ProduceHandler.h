#pragma once
#include <iostream>
#include <sstream>

#include "Handlers/Handler.h"

class ProduceHandler : public Handler {
 public:
  bool handle(const std::string& input, GameFacade& facade) override;
};