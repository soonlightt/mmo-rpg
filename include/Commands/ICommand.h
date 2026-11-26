#pragma once

#include <string>

class ICommand {
 public:
  virtual ~ICommand() = default;

  virtual bool execute() = 0;
  virtual bool undo() = 0;
  virtual std::string getName() const = 0;
};