#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

inline bool parseCoords(const std::string& token, int& outX, int& outY) {
  char comma = '\0';
  std::istringstream ss(token);
  int rawX = 0;
  int rawY = 0;
  if (!(ss >> rawX >> comma >> rawY) || comma != ',') {
    return false;
  }
  if (rawX < 1 || rawY < 1) {
    return false;
  }
  outX = rawX - 1;
  outY = rawY - 1;
  return true;
}