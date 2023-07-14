#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <limits.h>
#include "bigInt.h"

// ����� (_) �Ѱ��� �������� �� ������������
// ����� (_) �ΰ�¥�� �������� �������� ������
// ����� (_) �ΰ�¥�� �Լ����� �� ������� �Լ���

// �Լ��鿡 ���� ������ bigInt.h ���Ͽ� �����
// bigUInt_init, bigUInt_destroy, bigUInt_resize�� ������ ��� �Լ����� �Է��� bigUInt_t**�� ���� (�������� ������)
// �׷��� c++�� ������ ������ �� ������ �¿쿡 �ִ� ������ �ڷ����� bigUInt_t* �� �Ǿ�� �� (bigUInt ������)
// ����:
// 
// bigUInt_t* a = bigUInt_init();
// bigUInt_t* b = bigUInt_init();
// bigUInt_t* c = bigUInt_init();
// c = a + b;
// 
// �̷��� ����� �� �ֵ��� �����Ǿ�� ��
// �״ϱ� �� �Ŀ��� �� �����ڵ��� ��� �۵��ؾ� �ϳĸ�
// 
// operator+ �� 
// bigUInt_t* temp = bigUInt_init();
// bigUInt_add(&a, &b, &temp);
// return temp;
// 
// �̷��� �۵��ؾ� �ϰ�
// 
// operator= ��
// bigUInt_t(&c, &temp);
// return c;
// 
// �̷��� �۵��ؾ� ��

bigUInt_t* bigUInt_init()
{
  // _tmp���ٰ� �޸� �Ҵ��ϰ� �ʱⰪ ������
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
  // �Ű������� ���� num�� free ���Ѽ� �޸𸮸� �ü���� ��ȯ��
  free(num);
  num = NULL;
}

bigUInt_t* bigUInt_resize(bigUInt_t* a, uint64_t size)
{
  // realloc���� �޸� ������ �ø�
  bigUInt_t* _tmp = realloc(a, sizeof(bigUInt_t) + size * 8 + 1);
  if (_tmp == NULL)
  {
    return NULL;
  }
  // ���� �Ҵ���� ������ ������ ������ 0���� �ʱ�ȭ��
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
  
  // ���� �� ���� ��Ʈ�� 1�̸� ������ ������ nums �迭�� �ϳ��� �� �־�� �ϹǷ� �ִ� ���� + 1�� ���̸� �÷���
  if ((_max_len == (*a)->len) && ((*a)->nums[(*a)->len - 1] & 0xF000000000000000))
    *a = bigUInt_resize(*a, _max_len+1);
  if ((_max_len == (*b)->len) && ((*b)->nums[(*b)->len - 1] & 0xF000000000000000))
    *b = bigUInt_resize(*b, _max_len+1);

  // ���� ���̰� �� ���� �ٸ� �� ���� ���̸� ����
  if (_max_len > (*b)->len)
    *b = bigUInt_resize(*b, _max_len);
  if (_max_len > (*a)->len)
    *a = bigUInt_resize(*a, _max_len);
  if (_max_len > (*dest)->len)
    *dest = bigUInt_resize(*dest, _max_len);

  // ������� ���� �Լ� ȣ��
  __bigUInt_add(_max_len, (*a)->nums, (*b)->nums, (*dest)->nums);
}

// -
void bigUInt_sub(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  // a < b �̸� dest�� 0���� ����� ����
  // unsigned ������ ������ �Ǹ� ���۵���
  if (bigUInt_less(a, b))
  {
    for (int i = 0; i < (*dest)->len; i++)
      (*dest)->nums[i] ^= (*dest)->nums[i];
    return;
  }

  // ���� ���̰� �� ���� �ٸ� �� ���� ���̸� ����
  register uint64_t _max_len = MAX_TRIPLE((*a)->len, (*b)->len, (*dest)->len);
  if (_max_len > (*b)->len)
    *b = bigUInt_resize(*b, _max_len);
  if (_max_len > (*a)->len)
    *a = bigUInt_resize(*a, _max_len);
  if (_max_len > (*dest)->len)
    *dest = bigUInt_resize(*dest, _max_len);

  // ������� ���� �Լ� ȣ��
  __bigUInt_sub(_max_len, (*a)->nums, (*b)->nums, (*dest)->nums);
}
// *
void bigUInt_mul(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  uint64_t _n = 0;
  // �˰���� a�� b�� ��Ʈ����Ʈ �ؾ��ϴµ� �׷��� a�� b�� ���� �ٲ�� �����Ƿ� _a�� _b�� �����ؼ� ���
  bigUInt_t* _a = bigUInt_init();
  bigUInt_t* _b = bigUInt_init();
  bigUInt_t* _dest = bigUInt_init();
  bigUInt_assign(&_a, a);
  bigUInt_assign(&_b, b);
  bigUInt_assign(&_dest, dest);
  
  // dest �ʱ�ȭ
  _dest = bigUInt_resize(_dest, 1);
  _dest->nums[0] = 0;

  // ���� �˰���
  while (bigUInt_n_zero(&_b))
  {
    if (_b->nums[0] & 1)
    {
      bigUInt_bit_shl(a, _n, &_a);
      bigUInt_add(&_dest, &_a, &_dest);
    }
    bigUInt_bit_shr(&_b, 1, &_b);
    _n++;
  }
  bigUInt_assign(dest, &_dest);

  // _a�� _b �޸� ��ȯ
  bigUInt_destroy(_a);
  bigUInt_destroy(_b);
  bigUInt_destroy(_dest);
}
// /
bool bigUInt_div_cmp_helper(bigUInt_t** a, bigUInt_t** b, uint64_t len)
{
  for (uint64_t i = len; i > 0; i--)
  {
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i + len - 1 < (*a)->len) __a = (*a)->nums[i + len - 1];
    if (i - 1 < (*b)->len) __b = (*b)->nums[i - 1];
    if (__a == __b) continue;
    if (__a < __b) return 1;
    if (__a > __b) return 0;
  }
  return 0;
}
void bigUInt_div(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);

  bigUInt_t* _a = bigUInt_init();
  bigUInt_assign(&_a, a);
  
  _a = bigUInt_resize(_a, _max_len * 2);
  for (uint64_t i = 0; i < _max_len * 64; i++)
  {
    bigUInt_bit_shl(&_a, 1, &_a);
    if (bigUInt_div_cmp_helper(&_a, b, _max_len))
      continue;
    _a->nums[0] |= 1;
    __bigUInt_sub(_max_len, _a->nums+_max_len, (*b)->nums, _a->nums + _max_len);
  }

  _a = bigUInt_resize(_a, _max_len);
  bigUInt_assign(dest, &_a);

  bigUInt_destroy(_a);
}
// %
void bigUInt_mod(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);

  bigUInt_t* _a = bigUInt_init();
  bigUInt_assign(&_a, a);

  _a = bigUInt_resize(_a, _max_len * 2);
  for (uint64_t i = 0; i < _max_len * 64; i++)
  {
    bigUInt_bit_shl(&_a, 1, &_a);
    if (bigUInt_div_cmp_helper(&_a, b, _max_len))
      continue;
    _a->nums[0] |= 1;
    __bigUInt_sub(_max_len, _a->nums + _max_len, (*b)->nums, _a->nums + _max_len);
  }

  memcpy(_a->nums, _a->nums + _max_len, _max_len * 8);
  _a = bigUInt_resize(_a, _max_len);
  bigUInt_assign(dest, &_a);

  bigUInt_destroy(_a);
}

// ++
void bigUInt_inc(bigUInt_t** a)
{
  // ����ó�� ���� �� ù��° ��Ʈ�� 1�̸� nums�� ���̸� ��ĭ �÷���
  if ((*a)->nums[(*a)->len - 1] & 0xF000000000000000)
    *a = bigUInt_resize(*a, (*a)->len+1);

  // ������� increase �Լ� ȣ��
  __bigUInt_inc((*a)->len, (*a)->nums);
}

// --
void bigUInt_dec(bigUInt_t** a)
{
  // ������� decrease �Լ� ȣ��
  __bigUInt_dec((*a)->len, (*a)->nums);
}

/* Relational Operators */

// ==
bool bigUInt_eq(bigUInt_t** a, bigUInt_t** b)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);
  // �� �� �� �� bigUInt�� len����ŭ �ݺ�
  for (uint64_t i = 0; i < _max_len; i++)
  {
    // �迭 ���̺��� ū i���� ���� �ʵ��� i�� len���� ���� ��쿡�� __a�� __b�� ���� �Ҵ� (i�� len���� ũ�ų� �������� �⺻�� 0�� ���·� ��)
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i < (*a)->len) __a = (*a)->nums[i];
    if (i < (*b)->len) __b = (*b)->nums[i];
    // ���� a�� b�� �ѱ����� �ٸ��� �ٷ� �ߴ��ϰ� 0�� ����
    if (__a != __b)
      return 0;
  }
  // ��� ���Ƽ� �ݺ������� ���ϵ��� ������ 1�� ����
  return 1;
}
bool bigUInt_eq_num(bigUInt_t** a, uint64_t b)
{
  // ù��° nums�� b ���ؼ� �ٸ��� 0 ����
  if ((*a)->nums[0] != b)
    return 0;
  // ������ ���� nums���� ��� 0���� Ȯ��
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    // ���� ���� nums�� �߿� 0�� �ƴ� ���� ������ a != b�̹Ƿ� 0 ����
    if ((*a)->nums[0] & 0xffffffffffffffff)
      return 0;
  }
  // ���� nums���� ��� 0�̸� a==b�̹Ƿ� 1 ����
  return 1;
}

// !=
bool bigUInt_neq(bigUInt_t** a, bigUInt_t** b)
{
  // !(a==b) �ΰ���
  return !bigUInt_eq(a, b);
}
// >
bool bigUInt_greater(bigUInt_t** a, bigUInt_t** b)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);
  // �� �� �� �� bigUInt�� len����ŭ �ݺ�
  for (uint64_t i = _max_len; i > 0; i--)
  {
    // �迭 ���̺��� ū i���� ���� �ʵ��� i�� len���� ���� ��쿡�� __a�� __b�� ���� �Ҵ� (i�� len���� ũ�ų� �������� �⺻�� 0�� ���·� ��)
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i-1 < (*a)->len) __a = (*a)->nums[i-1];
    if (i-1 < (*b)->len) __b = (*b)->nums[i-1];
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
  for (uint64_t i = _max_len; i > 0; i--)
  {
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i-1 < (*a)->len) __a = (*a)->nums[i-1];
    if (i-1 < (*b)->len) __b = (*b)->nums[i-1];
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
  for (uint64_t i = _max_len; i > 0; i--)
  {
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i-1 < (*a)->len) __a = (*a)->nums[i-1];
    if (i-1 < (*b)->len) __b = (*b)->nums[i-1];
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
  for (uint64_t i = _max_len; i > 0; i--)
  {
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i-1 < (*a)->len) __a = (*a)->nums[i-1];
    if (i-1 < (*b)->len) __b = (*b)->nums[i-1];
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
    // ��� ���� �˻� �ϸ鼭 � ��Ʈ�� 1�� ������ 1 ����
    if ((*a)->nums[i] & 0xffffffffffffffff)
      return 1;
  }
  // 1�� ��Ʈ�� �ϳ��� ������ 0 ����
  return 0;
}
// &&
bool bigUInt_and(bigUInt_t** a, bigUInt_t** b)
{
  // ���� ���ҷ�
  return bigUInt_n_zero(a) && bigUInt_n_zero(b);
}
// ||
bool bigUInt_or(bigUInt_t** a, bigUInt_t** b)
{
  // ���� ���ҷ�22
  return bigUInt_n_zero(a) || bigUInt_n_zero(b);
}
// !
bool bigUInt_not(bigUInt_t** a)
{
  // ���� ���ҷ�333
  return !bigUInt_n_zero(a);
}

/* Bitwise Operators */

// &
void bigUInt_bit_and(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    // ��� nums�� ���� & ����
    (*dest)->nums[i] = (*a)->nums[i] & (*b)->nums[i];
  }
}
// |
void bigUInt_bit_or(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    // ��� nums�� ���� | ����
    (*dest)->nums[i] = (*a)->nums[i] | (*b)->nums[i];
  }
}
// ^
void bigUInt_bit_xor(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    // ��� nums�� ���� ^ ����
    (*dest)->nums[i] = (*a)->nums[i] ^ (*b)->nums[i];
  }
}
// ~
void bigUInt_bit_not(bigUInt_t** a, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    // ��� nums�� ���� ~ ����
    (*dest)->nums[i] = ~((*a)->nums[i]);
  } 
}
// <<
void bigUInt_bit_shl(bigUInt_t** a, uint64_t b, bigUInt_t** dest)
{
  // a�� 0�̸� ���ѹݺ��� �Ͼ�� �̸� ����
  if (!bigUInt_n_zero(a)) return;

  bigUInt_t* _a = bigUInt_init();
  bigUInt_assign(&_a, a);

  // b�� �ݺ�
  for (uint64_t i = 0; i < b; i++)
  {
    // ������� shl �Լ� ȣ�� (dest = a << 1);
    __bigUInt_bit_shl(_a->len, _a->nums);
  }
  bigUInt_assign(dest, &_a);
  bigUInt_destroy(_a);
}
// >>
void bigUInt_bit_shr(bigUInt_t** a, uint64_t b, bigUInt_t** dest)
{
  // a�� 0�̸� ���ѹݺ��� �Ͼ�� �̸� ����
  if (!bigUInt_n_zero(a)) return;

  bigUInt_t* _a = bigUInt_init();
  bigUInt_assign(&_a, a);

  for (uint64_t i = 0; i < b; i++)
  {
    // ������� shr �Լ� ȣ�� (dest = a >> 1);
    __bigUInt_bit_shr(_a->len, _a->nums);
  }
  bigUInt_assign(dest, &_a);
  bigUInt_destroy(_a);
}

// =
void bigUInt_assign(bigUInt_t** a, bigUInt_t** b)
{
  // a�� b�� ũ�⸦ ����
  if ((*a)->len < (*b)->len)
  {
    *a = bigUInt_resize(*a, (*b)->len);
  }
  // a���ٰ� b�� ������
  memcpy((*a)->nums, (*b)->nums, (*b)->len * 8);
}

/* utilities */

// int64_t to bigUInt
int itobi(int64_t num, bigUInt_t** dest)
{
  if ((*dest)->len > 1)
  {
    // a ũ�� 1�� ����
    *dest = bigUInt_resize(*dest, 1);
  }
  // a���ٰ� b ����
  (*dest)->nums[0] = num;
}
// bigUInt to string
char* bitostr(bigUInt_t* num)
{
  //
}
// bigUInt to int64_t
int64_t bitoi(bigUInt_t* num)
{
  if (num->len != 1) return 0xffffffffffffffff;
  return num->nums[0];
}

// bigUInt ��� �Լ�
void putbi(bigUInt_t** bi)
{
  //
}

// bigUInt ��� ������ printf ������
int bi_printf(const char* format, ...)
{
  //
}
// bigUInt ������ �Է¹޾� ������ִ� �Լ�
int bi_formulaf(bigUInt_t** dest, const char* format, ...)
{
  //
}

int main()
{

  bigUInt_t* a = bigUInt_init();
  bigUInt_t* b = bigUInt_init();

  a->nums[0] = 51231;
  b->nums[0] = 10;
  bigUInt_mod(&a, &b, &a);
  printf("%llu\n", a->nums[0]);

  return 0;
}
