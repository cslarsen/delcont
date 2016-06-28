#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"

struct continuation {
  size_t size;
  char* stack;
  void* rip;
  void* rsp;
  void* rbp;
  // TODO: Add more registers (see setjmp)
};

static char* root = NULL;
static struct continuation* cont = NULL;

struct continuation* create_cont()
{
  struct continuation *p = malloc(sizeof(struct continuation));
  p->size = 0;
  p->stack = NULL;
  return p;
}

// The function we'll jump back to
void show(const char* name, int number)
{
  printf("%s %d\n", name, number);
}

void test(int initial)
{
  // We jump _down_ the stack to reify() several times with new return values
  show("  the value is", reify(root, cont, initial));
}

void calltest(int initial)
{
  // Mark the extent of the continuation (root)
  root = getrbp();
  printf("calling test() w/initial value %d and root %p\n", initial, root);

  test(initial);
  printf("test() returned\n", initial);
}

int main()
{
  printf("allocating continuation\n");
  cont = create_cont();

  int age = 10;
  calltest(age);

  // we've now made a "function" out of the continuation; we are now at liberty
  // to call it as many times as we want, each time returning through all
  // functions involved between root and reify.
  do {
    reinstate(root, cont, --age);
  } while (age>0);

  printf("done\n");
  return 0;
}
