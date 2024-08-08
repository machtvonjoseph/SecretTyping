//#include <iostream>

class A
{
  int a_memb;
public:
  void foo();
};

class B
{
public:
  virtual void bar();
  virtual void qux();
};

class C : public B
{
public:
  void bar() override;
};

void A::foo()
{
  std::cout << "Hello this is foo" << std::endl;
}


void myFUNC()
{
  A a;
  a.foo();
}

void B::bar()
{
  std::cout << "This is B's implementation of bar" << std::endl;
}

void B::qux()
{
  std::cout << "This is B's implementation of qux" << std::endl;
}
void C::bar()
{
  std::cout << "This is C's implementation of bar" << std::endl;
}

int main(){
    B* b = new B();
    b->bar();
}