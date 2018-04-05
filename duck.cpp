#include <iostream>

class Flyable {
public:
  // abstract method fly
  virtual void fly() = 0;
};

class Duck {
public:
  // abstract method quack
  virtual void quack() = 0;

  // virtual method push with default implementation
  virtual void push() {
    quack();
  }
};

class Mallard : public Duck, public Flyable {
public:
  void quack() {
    std::cout << "Quack!" << std::endl;
  }

  void fly() {
    std::cout << "Heading south!" << std::endl;
  }

  // Overriding Flyable::push
  void push() {
    quack();
    fly();
  }
};

class RubberDuck : public Duck {
public:
  void quack() {
    std::cout << "Squeak!" << std::endl;
  }
};

