#pragma once

#include <chrono>
#include <string>

enum class UnitEventType { Created, Damaged, Destroyed };

struct UnitEvent {
  UnitEventType type;
  std::string unitName;
  int unitID;
  int damage;
  std::chrono::system_clock::time_point timestamp;
};