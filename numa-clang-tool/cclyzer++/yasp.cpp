#include <iostream>
#include <cstdlib>
using namespace std;

char *global_var;

int main() {
  char* x;
  char *y = new char[8];
  x= y;
  return 0;
}