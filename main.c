#include "bst.h"
#include <stdio.h>

int main() {
  Tree t = createTree();
  link x;
  insert(t, 99);
  imprime(t, "oi");
  insert(t, 88);
  imprime(t, "oi");
  insert(t, 32);
  imprime(t, "oi");
  insert(t, 77);
  imprime(t, "oi");

  return 0;
}