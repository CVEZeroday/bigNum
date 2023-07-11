#include <malloc.h>
#include "bigInt.h"

bigInt bigInt_init()
{
  bigInt_t* _tmp = (bigInt_t*)malloc(sizeof(bigInt_t));
  _tmp->len = 0;
  _tmp->head = 0;
  return _tmp;
}

bigInt bigInt_destroy(bigInt num)
{
  free(num);
  return NULL;
}

/* Arithmetic Operators */

// +
int bigInt_add(bigInt a, bigInt b, bigInt dest)
{
  __asm__ __volatile__
    (
    "adds x0, %1, %2\n"
    "str x0, [%1]\n"
    "cset %0, cs\n"
    : "=r" (overflowed)
    : "r" (&a), "r" (&b)
    : "x0", "cc"
  );
}
