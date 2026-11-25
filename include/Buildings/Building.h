#pragma once

#include <string>

class Building {
 private:
  static int nextId;
  int id;
  std::string type;
  int x;
  int y;

 public:
  Building(const std::string& type, int x, int y);

  int getId() const;
  std::string getType() const;
  int getX() const;
  int getY() const;
};