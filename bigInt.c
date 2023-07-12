#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <limits.h>
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
void bigUInt_add(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  register uint64_t _max_len = MAX_TRIPLE((*a)->len, (*b)->len, (*dest)->len);

  if ((_max_len == (*a)->len) && ((*a)->nums[(*a)->len - 1] & 0xF000000000000000))
    *a = bigUInt_resize(*a, _max_len+1);
  if ((_max_len == (*b)->len) && ((*b)->nums[(*b)->len - 1] & 0xF000000000000000))
    *b = bigUInt_resize(*b, _max_len+1);

  if (_max_len > (*b)->len)
    *b = bigUInt_resize(*b, _max_len);
  if (_max_len > (*a)->len)
    *a = bigUInt_resize(*a, _max_len);
  if (_max_len > (*dest)->len)
    *dest = bigUInt_resize(*dest, _max_len);
  __bigUInt_add(_max_len, (*a)->nums, (*b)->nums, (*dest)->nums);
}

// -
// !! DON'T USE !!
// NOT DONE YET.
void bigUInt_sub(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  register uint64_t _max_len = MAX_TRIPLE((*a)->len, (*b)->len, (*dest)->len);
  if (_max_len > (*b)->len)
    *b = bigUInt_resize(*b, _max_len);
  if (_max_len > (*a)->len)
    *a = bigUInt_resize(*a, _max_len);
  if (_max_len > (*dest)->len)
    *dest = bigUInt_resize(*dest, _max_len);
//  __bigUInt_sub(_max_len, (*a)->nums, (*b)->nums, (*dest)->nums);
}

// ++
void bigUInt_inc(bigUInt_t** a)
{
  if ((*a)->nums[(*a)->len - 1] & 0xF000000000000000)
    *a = bigUInt_resize(*a, (*a)->len+1);
  __bigUInt_inc((*a)->len, (*a)->nums);
}

// --
// NOT DONE YET.
void bigUInt_dec(bigUInt_t** a)
{
  __bigUInt_dec((*a)->len, (*a)->nums);
}

// <<
// NOT DONE YET.
void bigUInt_bit_shl(bigUInt_t** a, uint64_t b)
{
  for (; b > 0; b--)
    __bigUInt_shl((*a)->len, (*a)->nums);
}
// >>
// NOT DONE YET.
void bigUInt_bit_shr(bigUInt_t** a, uint64_t b)
{
  for (; b > 0; b--)
    __bigUInt_shr((*a)->len, (*a)->nums);
}

// =
void bigUInt_assign(bigUInt_t** a, bigUInt_t** b)
{
  memset((*a)->nums, 0, (*a)->len * 8);
  if ((*a)->len < (*b)->len)
  {
    *a = bigUInt_resize(*a, (*b)->len);
  }
  memcpy((*a)->nums, (*b)->nums, (*b)->len * 8);
}

void bigUInt_assign_num(bigUInt_t** a, uint64_t b)
{
  memset((*a)->nums, 0, (*a)->len * 8);
  (*a)->nums[0] = b;
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
