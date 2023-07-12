#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include "bigInt.h"

bigUInt_t* bigUInt_init()
{
  bigUInt_t* _tmp = malloc(sizeof(bigUInt_t) + 8);
  _tmp->len = 1;
  _tmp->nums[0] = 0;
  return _tmp;
}

//bigUInt_t* bigUInt_binary(bigUInt)

void bigUInt_destroy(bigUInt_t* num)
{
  free(num);
  num = NULL;
}

void bigUInt_resize(bigUInt_t* a, uint64_t size)
{
  realloc(a, sizeof(*a) + (sizeof(size) + 1) * 8);
  memset(a->nums + a->len, 0, (size - a->len));
  a->len = size;
  //for (uint64_t i = _len; i <= size; i++)
  //  a->nums[i] = 0;
}

/* Arithmetic Operators */

// +
int bigUInt_add(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest)
{
  bigUInt_t* _a = a;
  bigUInt_t* _b = b;
  register uint64_t _max_len = MAX_TRIPLE(_a->len, _b->len, dest->len);
  if (_max_len > _b->len) bigUInt_resize(_b, _max_len);
  if (_max_len > _a->len) bigUInt_resize(_a, _max_len);
  if (_max_len > dest->len) bigUInt_resize(dest, _max_len);
  __bigUInt_add(_max_len, _a->nums, _b->nums, dest->nums);
}

int main()
{
  bigUInt_t* a = bigUInt_init();
  bigUInt_t* b = bigUInt_init();
  a->nums[0] = 40;
  b->nums[0] = 15;
  bigUInt_t* c = bigUInt_init();
  printf("%llu\n", a->nums[0]);
  bigUInt_resize(a, 3);
  bigUInt_add(a, b, c); // c = a + b
  printf("%llu\n", c->nums[0]);
  
  bigUInt_destroy(a);
  bigUInt_destroy(b);
  bigUInt_destroy(c);
  return 0;
}