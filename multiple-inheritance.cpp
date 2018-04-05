#include <iostream>

using namespace std;

class A {
public:
  int a;

  A() : a(1) {}

  virtual void m() = 0;
};

class B : public A {
public:
  int b;

  B() : b(1) {
    this->a = 2;
  }

  virtual void m() { cout << "B" << endl; }
};

class C : public A {
public:
  int c;

  C() : c(1) {
    this->a = 3;
  }

  virtual void m() { cout << "C" << endl; }
};

class D : public B, public C {
public:
  int d;

  D() : d(1) {}
};

int main() {
  D* x = new D();
  B* y = x;
  C* z = x;

  A* u = z;
  
  cout << "y->a = " << y->a << endl; // 2
  cout << "u->a = " << u->a << endl; // 2
  cout << "z->a = " << z->a << endl; // 3

  u->m();
  
  cout << "x = " << (void*) x << endl;
  cout << "y = " << (void*) y << endl;
  cout << "u = " << (void*) u << endl;
  cout << "z = " << (void*) z << endl;

  delete x;

  return 0;
}
