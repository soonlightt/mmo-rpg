#pragma once

class Health {
 private:
  int value;
  int maxValue;

 public:
  explicit Health(int initialValue = 100);

  int getValue() const;
  int getMaxValue() const;

  void decrease(int amount);
  void increase(int amount);
  bool isZero() const;
};