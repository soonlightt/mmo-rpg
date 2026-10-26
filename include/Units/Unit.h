#pragma once

#include <memory>
#include <string>

#include "Attributes/Armor.h"
#include "Attributes/Attack.h"
#include "Attributes/Health.h"

class Unit {
 protected:
  int x;
  int y;
  Health health;
  Armor armor;
  Attack attack;

 public:
  Unit(int startX, int startY, const Health& h, const Armor& arm, const Attack& att);
  virtual ~Unit() = default;

  int getX() const;
  int getY() const;
  void setPosition(int newX, int newY);

  const Health& getHealth() const;
  const Armor& getArmor() const;
  const Attack& getAttack() const;

  virtual void takeDamage(int inComingDamage);
  virtual bool isAlive() const;

  virtual std::string getType() const = 0;
  virtual char getSymbol() const = 0;
  virtual std::unique_ptr<Unit> clone() const = 0;
};