#pragma once

#include <memory>
#include <string>

#include "Units/Midder/Midder.h"

class ArcWarden : public Midder {
 public:
  ArcWarden(int x, int y);
  ~ArcWarden() override = default;

  std::string getType() const override;
  char getSymbol() const override;
  std::unique_ptr<Unit> clone() const override;
};