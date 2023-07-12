#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include "bigInt.h"

bigUInt_t* bigUInt_init()
{
  bigUInt_t* _tmp = malloc(sizeof(bigUInt_t) + 9);
  if (_tmp == NULL)
  {
    return NULL;
  }
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

bigUInt_t* bigUInt_resize(bigUInt_t* a, uint64_t size)
{
  bigUInt_t* _tmp = realloc(a, sizeof(bigUInt_t) + size * 8 + 1);
  if (_tmp == NULL)
  {
    return NULL;
  }
  memset(_tmp->nums + _tmp->len, 0, (size - _tmp->len) * 8);
  _tmp->len = size;
  return _tmp;
  //for (uint64_t i = _len; i <= size; i++)
  //  a->nums[i] = 0;
}

/* Arithmetic Operators */

// +
int bigUInt_add(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  register uint64_t _max_len = MAX_TRIPLE((*a)->len, (*b)->len, (*dest)->len);
  if (_max_len > (*b)->len)
    *b = bigUInt_resize(*b, _max_len);
  if (_max_len > (*a)->len)
    *a = bigUInt_resize(*a, _max_len);
  if (_max_len > (*dest)->len)
    *dest = bigUInt_resize(*dest, _max_len);
  __bigUInt_add(_max_len, (*a)->nums, (*b)->nums, (*dest)->nums);
}

int main()
{
  bigUInt_t* a = bigUInt_init();
  bigUInt_t* b = bigUInt_init();
  bigUInt_t* c = bigUInt_init();

  a->nums[0] = ULLONG_MAX;
  b->nums[0] = 2;
 
  a = bigUInt_resize(a, 2);
  b = bigUInt_resize(b, 2);

  a->nums[1] = 1;
  b->nums[1] = 2;

  bigUInt_add(&a, &b, &c); // c = a + b
  printf("%llu\n", c->nums[0]);
  printf("%llu\n", c->nums[1]);
  
  bigUInt_destroy(a);
  bigUInt_destroy(b);
  bigUInt_destroy(c);
  return 0;
}