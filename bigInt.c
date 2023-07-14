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
  if (size > _tmp->len)
  {
    memset(_tmp->nums + _tmp->len, 0, (size - _tmp->len) * 8);
  }
  _tmp->len = size;
  return _tmp;
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
  __bigUInt_sub(_max_len, (*a)->nums, (*b)->nums, (*dest)->nums);
}
// *
// NOT DONE YET.
void bigUInt_mul(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
    // (*a)->nums[0] & 0x1
    // if((*b)->nums[0] & 0x1 == 1)
    // dest = a + b;
    // a << 1;
    // else if((*b)->nums[0] & 0x1 == 0)
    // a << 1;


    bigUInt_add(a, b, dest); 
    bigUInt_bit_shl(a, 1);
   
}
// /
// NOT DONE YET.
void bigUInt_div(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{

}
// %
// NOT DONE YET.
void bigUInt_mod(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{

}

// ++
void bigUInt_inc(bigUInt_t** a)
{
  if ((*a)->nums[(*a)->len - 1] & 0xF000000000000000)
    *a = bigUInt_resize(*a, (*a)->len+1);
  __bigUInt_inc((*a)->len, (*a)->nums);
}

// --
void bigUInt_dec(bigUInt_t** a)
{
  __bigUInt_dec((*a)->len, (*a)->nums);
}

/* Relational Operators */

// ==
bool bigUInt_eq(bigUInt_t** a, bigUInt_t** b)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);
  for (uint64_t i = 0; i < _max_len; i++)
  {
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i < (*a)->len) __a = (*a)->nums[i];
    if (i < (*b)->len) __b = (*b)->nums[i];
    if (__a != __b)
      return 0;
  }
  return 1;
}
// !=
bool bigUInt_neq(bigUInt_t** a, bigUInt_t** b)
{
  return !bigUInt_eq(a, b);
}
// >
bool bigUInt_greater(bigUInt_t** a, bigUInt_t** b)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);
  for (uint64_t i = 0; i < _max_len; i++)
  {
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i < (*a)->len) __a = (*a)->nums[i];
    if (i < (*b)->len) __b = (*b)->nums[i];
    if (__a == __b) continue;
    if (__a > __b) return 1;
    if (__a < __b) return 0;
  }
  return 0;
}
// <
bool bigUInt_less(bigUInt_t** a, bigUInt_t** b)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);
  for (uint64_t i = 0; i < _max_len; i++)
  {
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i < (*a)->len) __a = (*a)->nums[i];
    if (i < (*b)->len) __b = (*b)->nums[i];
    if (__a == __b) continue;
    if (__a < __b) return 1;
    if (__a > __b) return 0;
  }
  return 0;
}
// >=
bool bigUInt_greater_eq(bigUInt_t** a, bigUInt_t** b)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);
  for (uint64_t i = 0; i < _max_len; i++)
  {
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i < (*a)->len) __a = (*a)->nums[i];
    if (i < (*b)->len) __b = (*b)->nums[i];
    if (__a == __b) continue;
    if (__a > __b) return 1;
    if (__a < __b) return 0;
  }
  return 1;
}
// <=
bool bigUInt_less_eq(bigUInt_t** a, bigUInt_t** b)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);
  for (uint64_t i = 0; i < _max_len; i++)
  {
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i < (*a)->len) __a = (*a)->nums[i];
    if (i < (*b)->len) __b = (*b)->nums[i];
    if (__a == __b) continue;
    if (__a < __b) return 1;
    if (__a > __b) return 0;
  }
  return 1;
}

/* Logical Operators */

// return 0 if zero, 1 if not zero
bool bigUInt_n_zero(bigUInt_t** a)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    if ((*a)->nums[i] & ULLONG_MAX)
      return 1;
  }
  return 0;
}
// &&
bool bigUInt_and(bigUInt_t** a, bigUInt_t** b)
{
  return bigUInt_n_zero(a) && bigUInt_n_zero(b);
}
// ||
bool bigUInt_or(bigUInt_t** a, bigUInt_t** b)
{
  return bigUInt_n_zero(a) || bigUInt_n_zero(b);
}
// !
bool bigUInt_not(bigUInt_t** a)
{
  return !bigUInt_n_zero(a);
}

/* Bitwise Operators */

// &
void bigUInt_bit_and(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    (*dest)->nums[i] = (*a)->nums[i] & (*b)->nums[i];
  }
}
// |
void bigUInt_bit_or(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    (*dest)->nums[i] = (*a)->nums[i] | (*b)->nums[i];
  }
}
// ^
void bigUInt_bit_xor(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    (*dest)->nums[i] = (*a)->nums[i] ^ (*b)->nums[i];
  }
}
// ~
void bigUInt_bit_not(bigUInt_t** a, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    (*dest)->nums[i] = ~((*a)->nums[i]);
  } 
}
// <<
// NOT DONE YET.
void bigUInt_bit_shl(bigUInt_t** a, uint64_t b)
{
}
// >>
// NOT DONE YET.
void bigUInt_bit_shr(bigUInt_t** a, uint64_t b)
{
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
  if ((*a)->len > 1)
  {
    *a = bigUInt_resize(*a, 1);
  }
  memset((*a)->nums, 0, (*a)->len * 8);
  (*a)->nums[0] = b;
}

int main()
{
  /*
  unsigned long long a, b, c, res, carry = 0, n;
  a = 13; b = 11, n=4;
  res = b;
  a <<= n;
  c = a ^ b;
  for(int i=0; i<=3; i++){
    if ((res & 0x1) == 1)
    {
      c >>= 1;
      if (carry == 1)
      {
        carry <<= ((n << 1) - 1);
        c ^= carry;
      }
      a = c >> n;
      b = c ^ (a << n);
    }
    if((res & 0x2) == 0) res = 0;
    if ((a + res) & (1 << n)) carry = 1;
    else carry = 0;
    a >>= 1;
    c = (a << n) ^ b;
  }


  printf("%llu\n", a);
  printf("%llu\n", b);


  printf("%llu\n", c);
  */

  bigUInt_t* a = bigUInt_init();
  bigUInt_t* b = bigUInt_init();
  a = bigUInt_resize(a, 3);
  b = bigUInt_resize(b, 2);
  a->nums[0] = 0b0101;
  a->nums[1] = 0b1001;
  b->nums[0] = 0b0101;
  b->nums[1] = 0b1001;
  bigUInt_sub(&a, &b, &a);
  printf("%llu\n", a->nums[0]);
  printf("%llu\n", a->nums[1]);

  return 0;
}
