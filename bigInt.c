#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
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
  if ((_max_len == (*a)->len) && ((*a)->nums[(*a)->len - 1] & 0x8000000000000000))
    *a = bigUInt_resize(*a, ++_max_len);
  if ((_max_len == (*b)->len) && ((*b)->nums[(*b)->len - 1] & 0x8000000000000000))
    *b = bigUInt_resize(*b, ++_max_len);

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
// Division function only for binary to decimal (fast)
void bigUInt_div_by_10(bigUInt_t** a, char* mod)
{
  register uint64_t len = (*a)->len;
  *a = bigUInt_resize(*a, len + 1);
  for (uint64_t i = 0; i < len * 64; i++)
  {
    bigUInt_bit_shl(a, 1, a);

    if ((*a)->nums[len] < 10)
      continue;
    (*a)->nums[0] |= 1;
    (*a)->nums[len] -= 10;
  } 
  *mod = (*a)->nums[len];
  *a = bigUInt_resize(*a, len);
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
  if ((*a)->nums[(*a)->len - 1] & 0x8000000000000000)
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
    if (_a->nums[_a->len - 1] & 0x8000000000000000)
      _a = bigUInt_resize(_a, _a->len + 1);
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

// bigUInt to string, must free() returned pointer after use
char* bitostr(bigUInt_t* num)
{
  uint64_t _size = (uint64_t)ceil(BIN2DEC_COEFFICIENT * num->len);
  uint64_t _cnt = _size;
  char* _str = calloc(_size, sizeof(char));
  char _mod = 0;

  bigUInt_t* _tmp = bigUInt_init();
  bigUInt_assign(&_tmp, &num);

  while (bigUInt_n_zero(&_tmp))
  {
    bigUInt_div_by_10(&_tmp, &_mod);
    _str[--_cnt] = _mod + 0x30;
  }
  if (_str == 0 || _str + _cnt == 0) return NULL;
  memcpy(_str, _str + _cnt, _size - _cnt);
  _str = realloc(_str, _size - _cnt + 1);
  _str[_size - _cnt] = '\0';

  return _str;
}

// bigUInt to int64_t
int64_t bitoi(bigUInt_t* num)
{
  if (num->len != 1) return 0xffffffffffffffff;
  return num->nums[0];
}

int main()
{
  //DEBUG();
  bigUInt_t* a = bigUInt_init();
  bigUInt_t* b = bigUInt_init();
  itobi(18446744073709551615, &a);
  itobi(9223372036854775808, &b);
  for (int i = 0; i < 1000000000; i++)
    bigUInt_add(&a, &b, &a);

  char* str;
  printf("%s\n", str = bitostr(a));
  free(str);

  bigUInt_destroy(a);
  bigUInt_destroy(b);
  
  return 0;
}

#ifdef DEBUG_MODE
void __debug()
{
  bigUInt_t* a = bigUInt_init();
  bigUInt_t* b = bigUInt_init();

  // ���� ��� 128��Ʈ (����)
  itobi(18446744073709551615, &a);
  itobi(100, &b);
  bigUInt_mul(&a, &b, &a);
  char* str = 0;
  str = bitostr(a);
  printf("%s\n", str);
  // 27,670,116,110,564,327,423

  // ���� ��� 64��Ʈ (����)
  itobi(9223372036854775808, &a);
  itobi(100000000000000, &b);
  bigUInt_bit_shr(&a, 3, &a);
  str = 0;
  str = bitostr(a);
  printf("%s\n", str);
  // 9223472036854775808

  // ���� ��� 128��Ʈ (����)
  itobi(9223372036854775808, &a);
  itobi(9223372036854775808, &b);
  bigUInt_mul(&a, &b, &a);
  str = 0;
  str = bitostr(a);
  printf("%s\n", str);
  // 8.5070591730234615865843651857942e+37

  // ���� ��� 64��Ʈ (����)
  itobi(100000000000, &a);
  itobi(1000, &b);
  bigUInt_mul(&a, &b, &a);
  str = 0;
  str = bitostr(a);
  printf("%s\n", str);
  // 100000000000000

  free(str);
  /*
  bigUInt_t* a = bigUInt_init();
  char mod;
  a = bigUInt_resize(a, 2);
  a->nums[0] = 1231325615;
  a->nums[1] = 1253252353525325;
  bigUInt_div_by_10(&a, &mod);
  bigUInt_div_by_10(&a, &mod);
  bigUInt_div_by_10(&a, &mod);
  */
}
#endif