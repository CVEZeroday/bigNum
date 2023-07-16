#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "bigInt.h"

// 언더바 (_) 한개인 변수들은 다 지역변수들임
// 언더바 (_) 두개짜리 변수들은 레지스터 변수임
// 언더바 (_) 두개짜리 함수들은 다 어셈블리어 함수임

// 함수들에 대한 설명은 bigInt.h 파일에 써놓음
// bigUInt_init, bigUInt_destroy, bigUInt_resize를 제외한 모든 함수들은 입력을 bigUInt_t**로 받음 (포인터의 포인터)
// 그래서 c++로 연산자 구현할 때 연산자 좌우에 있는 변수의 자료형은 bigUInt_t* 가 되어야 함 (bigUInt 포인터)
// 예시:
// 
// bigUInt_t* a = bigUInt_init();
// bigUInt_t* b = bigUInt_init();
// bigUInt_t* c = bigUInt_init();
// c = a + b;
// 
// 이렇게 사용할 수 있도록 구현되어야 함
// 그니까 위 식에서 각 연산자들이 어떻게 작동해야 하냐면
// 
// operator+ 는 
// bigUInt_t* temp = bigUInt_init();
// bigUInt_add(&a, &b, &temp);
// return temp;
// 
// 이렇게 작동해야 하고
// 
// operator= 는
// bigUInt_t(&c, &temp);
// return c;
// 
// 이렇게 작동해야 함

bigUInt_t* bigUInt_init()
{
  // _tmp에다가 메모리 할당하고 초기값 지정함
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
  // 매개변수로 받은 num을 free 시켜서 메모리를 운영체제에 반환함
  free(num);
  num = NULL;
}

bigUInt_t* bigUInt_resize(bigUInt_t* a, uint64_t size)
{
  // realloc으로 메모리 사이즈 늘림
  bigUInt_t* _tmp = realloc(a, sizeof(bigUInt_t) + size * 8 + 1);
  if (_tmp == NULL)
  {
    return NULL;
  }
  // 새로 할당받은 공간에 쓰레기 값들을 0으로 초기화함
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
  
  // 만약 맨 위쪽 비트가 1이면 덧셈을 했을때 nums 배열이 하나가 더 있어야 하므로 최대 길이 + 1로 길이를 늘려줌
  if ((_max_len == (*a)->len) && ((*a)->nums[(*a)->len - 1] & 0x8000000000000000))
    *a = bigUInt_resize(*a, ++_max_len);
  if ((_max_len == (*b)->len) && ((*b)->nums[(*b)->len - 1] & 0x8000000000000000))
    *b = bigUInt_resize(*b, ++_max_len);

  // 가장 길이가 긴 수에 다른 두 수의 길이를 맞춤
  if (_max_len > (*b)->len)
    *b = bigUInt_resize(*b, _max_len);
  if (_max_len > (*a)->len)
    *a = bigUInt_resize(*a, _max_len);
  if (_max_len > (*dest)->len)
    *dest = bigUInt_resize(*dest, _max_len);

  // 어셈블리어 덧셈 함수 호출
  __bigUInt_add(_max_len, (*a)->nums, (*b)->nums, (*dest)->nums);
}

// -
void bigUInt_sub(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  // a < b 이면 dest를 0으로 만들고 리턴
  // unsigned 정수라서 음수가 되면 오작동함
  if (bigUInt_less(a, b))
  {
    for (int i = 0; i < (*dest)->len; i++)
      (*dest)->nums[i] ^= (*dest)->nums[i];
    return;
  }

  // 가장 길이가 긴 수에 다른 두 수의 길이를 맞춤
  register uint64_t _max_len = MAX_TRIPLE((*a)->len, (*b)->len, (*dest)->len);
  if (_max_len > (*b)->len)
    *b = bigUInt_resize(*b, _max_len);
  if (_max_len > (*a)->len)
    *a = bigUInt_resize(*a, _max_len);
  if (_max_len > (*dest)->len)
    *dest = bigUInt_resize(*dest, _max_len);

  // 어셈블리어 뺄셈 함수 호출
  __bigUInt_sub(_max_len, (*a)->nums, (*b)->nums, (*dest)->nums);
}
// *
void bigUInt_mul(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  uint64_t _n = 0;
  // 알고리즘상 a와 b를 비트시프트 해야하는데 그러면 a와 b의 값이 바뀌어 버리므로 _a와 _b에 복사해서 계산
  bigUInt_t* _a = bigUInt_init();
  bigUInt_t* _b = bigUInt_init();
  bigUInt_t* _dest = bigUInt_init();
  bigUInt_assign(&_a, a);
  bigUInt_assign(&_b, b);
  bigUInt_assign(&_dest, dest);
  
  // dest 초기화
  _dest = bigUInt_resize(_dest, 1);
  _dest->nums[0] = 0;

  // 곱셈 알고리즘
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

  // _a와 _b 메모리 반환
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
  // 덧셈처럼 만약 맨 첫번째 비트가 1이면 nums의 길이를 한칸 늘려줌
  if ((*a)->nums[(*a)->len - 1] & 0x8000000000000000)
    *a = bigUInt_resize(*a, (*a)->len+1);

  // 어셈블리어 increase 함수 호출
  __bigUInt_inc((*a)->len, (*a)->nums);
}

// --
void bigUInt_dec(bigUInt_t** a)
{
  // 어셈블리어 decrease 함수 호출
  __bigUInt_dec((*a)->len, (*a)->nums);
}

/* Relational Operators */

// ==
bool bigUInt_eq(bigUInt_t** a, bigUInt_t** b)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);
  // 둘 중 더 긴 bigUInt의 len값만큼 반복
  for (uint64_t i = 0; i < _max_len; i++)
  {
    // 배열 길이보다 큰 i값이 들어가지 않도록 i가 len보다 작은 경우에만 __a와 __b에 값을 할당 (i가 len보다 크거나 같아지면 기본값 0인 상태로 비교)
    register uint64_t __a = 0;
    register uint64_t __b = 0;
    if (i < (*a)->len) __a = (*a)->nums[i];
    if (i < (*b)->len) __b = (*b)->nums[i];
    // 만약 a와 b가 한군데라도 다르면 바로 중단하고 0을 리턴
    if (__a != __b)
      return 0;
  }
  // 모두 같아서 반복문에서 리턴되지 않으면 1을 리턴
  return 1;
}
bool bigUInt_eq_num(bigUInt_t** a, uint64_t b)
{
  // 첫번째 nums와 b 비교해서 다르면 0 리턴
  if ((*a)->nums[0] != b)
    return 0;
  // 같으면 뒤쪽 nums들이 모두 0인지 확인
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    // 만약 뒤쪽 nums들 중에 0이 아닌 값이 있으면 a != b이므로 0 리턴
    if ((*a)->nums[0] & 0xffffffffffffffff)
      return 0;
  }
  // 뒤쪽 nums들이 모두 0이면 a==b이므로 1 리턴
  return 1;
}

// !=
bool bigUInt_neq(bigUInt_t** a, bigUInt_t** b)
{
  // !(a==b) 인거임
  return !bigUInt_eq(a, b);
}
// >
bool bigUInt_greater(bigUInt_t** a, bigUInt_t** b)
{
  uint64_t _max_len = MAX((*a)->len, (*b)->len);
  // 둘 중 더 긴 bigUInt의 len값만큼 반복
  for (uint64_t i = _max_len; i > 0; i--)
  {
    // 배열 길이보다 큰 i값이 들어가지 않도록 i가 len보다 작은 경우에만 __a와 __b에 값을 할당 (i가 len보다 크거나 같아지면 기본값 0인 상태로 비교)
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
    // 모든 수를 검사 하면서 어떤 비트든 1이 있으면 1 리턴
    if ((*a)->nums[i] & 0xffffffffffffffff)
      return 1;
  }
  // 1인 비트가 하나도 없으면 0 리턴
  return 0;
}
// &&
bool bigUInt_and(bigUInt_t** a, bigUInt_t** b)
{
  // 설명 안할래
  return bigUInt_n_zero(a) && bigUInt_n_zero(b);
}
// ||
bool bigUInt_or(bigUInt_t** a, bigUInt_t** b)
{
  // 설명 안할래22
  return bigUInt_n_zero(a) || bigUInt_n_zero(b);
}
// !
bool bigUInt_not(bigUInt_t** a)
{
  // 설명 안할래333
  return !bigUInt_n_zero(a);
}

/* Bitwise Operators */

// &
void bigUInt_bit_and(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    // 모든 nums에 대해 & 연산
    (*dest)->nums[i] = (*a)->nums[i] & (*b)->nums[i];
  }
}
// |
void bigUInt_bit_or(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    // 모든 nums에 대해 | 연산
    (*dest)->nums[i] = (*a)->nums[i] | (*b)->nums[i];
  }
}
// ^
void bigUInt_bit_xor(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    // 모든 nums에 대해 ^ 연산
    (*dest)->nums[i] = (*a)->nums[i] ^ (*b)->nums[i];
  }
}
// ~
void bigUInt_bit_not(bigUInt_t** a, bigUInt_t** dest)
{
  for (uint64_t i = 0; i < (*a)->len; i++)
  {
    // 모든 nums에 대해 ~ 연산
    (*dest)->nums[i] = ~((*a)->nums[i]);
  } 
}
// <<
void bigUInt_bit_shl(bigUInt_t** a, uint64_t b, bigUInt_t** dest)
{
  // a가 0이면 무한반복이 일어나서 이를 방지
  if (!bigUInt_n_zero(a)) return;

  bigUInt_t* _a = bigUInt_init();
  bigUInt_assign(&_a, a);

  // b번 반복
  for (uint64_t i = 0; i < b; i++)
  {
    if (_a->nums[_a->len - 1] & 0x8000000000000000)
      _a = bigUInt_resize(_a, _a->len + 1);
    // 어셈블리어 shl 함수 호출 (dest = a << 1);
    __bigUInt_bit_shl(_a->len, _a->nums);
  }
  bigUInt_assign(dest, &_a);
  bigUInt_destroy(_a);
}
// >>
void bigUInt_bit_shr(bigUInt_t** a, uint64_t b, bigUInt_t** dest)
{
  // a가 0이면 무한반복이 일어나서 이를 방지
  if (!bigUInt_n_zero(a)) return;

  bigUInt_t* _a = bigUInt_init();
  bigUInt_assign(&_a, a);

  for (uint64_t i = 0; i < b; i++)
  {
    // 어셈블리어 shr 함수 호출 (dest = a >> 1);
    __bigUInt_bit_shr(_a->len, _a->nums);
  }
  bigUInt_assign(dest, &_a);
  bigUInt_destroy(_a);
}

// =
void bigUInt_assign(bigUInt_t** a, bigUInt_t** b)
{
  // a와 b의 크기를 맞춤
  if ((*a)->len < (*b)->len)
  {
    *a = bigUInt_resize(*a, (*b)->len);
  }
  // a에다가 b값 복사함
  memcpy((*a)->nums, (*b)->nums, (*b)->len * 8);
}

/* utilities */

// int64_t to bigUInt
int itobi(int64_t num, bigUInt_t** dest)
{
  if ((*dest)->len > 1)
  {
    // a 크기 1로 설정
    *dest = bigUInt_resize(*dest, 1);
  }
  // a에다가 b 대입
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

  // 연산 결과 128비트 (덧셈)
  itobi(18446744073709551615, &a);
  itobi(100, &b);
  bigUInt_mul(&a, &b, &a);
  char* str = 0;
  str = bitostr(a);
  printf("%s\n", str);
  // 27,670,116,110,564,327,423

  // 연산 결과 64비트 (덧셈)
  itobi(9223372036854775808, &a);
  itobi(100000000000000, &b);
  bigUInt_bit_shr(&a, 3, &a);
  str = 0;
  str = bitostr(a);
  printf("%s\n", str);
  // 9223472036854775808

  // 연산 결과 128비트 (곱셈)
  itobi(9223372036854775808, &a);
  itobi(9223372036854775808, &b);
  bigUInt_mul(&a, &b, &a);
  str = 0;
  str = bitostr(a);
  printf("%s\n", str);
  // 8.5070591730234615865843651857942e+37

  // 연산 결과 64비트 (곱셈)
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