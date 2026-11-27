#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

inline bool parseCoords(const std::string& token, int& outX, int& outY) {
  std::string cleaned = token;
  std::replace(cleaned.begin(), cleaned.end(), ',', ' ');
  std::istringstream iss(cleaned);
  return static_cast<bool>(iss >> outX >> outY);
}