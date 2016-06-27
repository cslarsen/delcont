#include <assert.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"

struct continuation {
  jmp_buf env;
  size_t size;
  char* stack;
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

int reify(char* start, struct continuation* p)
{
  char* rsp = getrsp();
  assert(rsp < start);
  p->size = start - rsp;
  p->stack = malloc(p->size);
  memcpy(p->stack, rsp, p->size);
  return setjmp(p->env);
}

void reinstate(char* start, struct continuation* p, int value)
{
  // TODO: This really should be in assembly, because we overwrite our current
  // stack frame here, so the longjump will use incorrect values for p->env
  // etc. We work around that here by using static variables.

  static jmp_buf env;
  static int retval;

  memcpy(env, p->env, sizeof(jmp_buf));
  retval = value;

  printf("reverse copy %zu bytes to %p\n", p->size, start);
  memcpy(start - p->size, p->stack, p->size);

  printf("longjump w/value %d\n", retval);
  longjmp(env, retval);
}

// The function we'll jump back to
void show(const char* name, int number)
{
  printf("%s %d\n", name, number);
}

void test()
{
  // Notice that we jump back (*down* the stack) to reify() several times with
  // new return values
  show("    the current value is", reify(root, cont));
}

void calltest()
{
  // This is just to show that we are traversing back up the stack after
  // calling reinstate. Otherwise we would just have done a simple jump.
  root = getrbp();
  printf("  call test() w/start set to %p\n", root);
  test();
  printf("  test() returned\n");
}

int main()
{
  printf("allocating continuation\n");
  cont = create_cont();

  int age = 5;
  calltest();

  do {
    reinstate(root, cont, age--);
  } while (age>0);
}
