#pragma once

#include <memory>
#include <string>

class GameFacade;

class Handler {
 protected:
  std::unique_ptr<Handler> nextHandler;

 public:
  virtual ~Handler() = default;

  Handler* setNext(std::unique_ptr<Handler> next) {
    nextHandler = std::move(next);
    return nextHandler.get();
  }

  virtual bool handle(const std::string& input, GameFacade& facade) {
    if (nextHandler) {
      return nextHandler->handle(input, facade);
    }
    return false;
  }
};