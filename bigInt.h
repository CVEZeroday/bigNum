#pragma GCC optimize ("O2")

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#define BIN2DEC_COEFFICIENT 19.26591972
// log10(2) * 64
//#define uint64_t unsigned long long

/**
 * ENVIRONMENT DETECTION
 * Supported Architecture and Environment
 * Windows: MSVC only
 * Linux: GCC only
 * supported architecture: AMD64, i386, aarch64
**/
#if defined(__linux__) && defined(__GNUC__)
  #ifdef __amd64__
    #define LINUX_X64
  #endif
  #ifdef __i386__
    #define LINUX_X86
  #endif
  #ifdef __aarch64__
    #define LINUX_AARCH64
  #endif
#elif defined(_WIN32) && defined(_MSC_VER)
  #ifdef __amd64__
    #define WINDOWS_X64
  #endif
  #ifdef __i386__
    #define WINDOWS_X86
  #endif
  #ifdef __aarch64__
    #define WINDOWS_AARCH64
  #endif
#else
  #define NOT_SUPPORTED_ARCHITECTURE
#endif

// �ִ�, �ּڰ� ���ϴ� ��ũ��
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX_TRIPLE(a, b, c) MAX((a), MAX((b), (c)))
#define MIN_TRIPLE(a, b, c) MIN((a), MIN((b), (c)))

/* bigUInt_s structure definition */

#ifdef __cplusplus
extern "C" {
#endif

// bigUInt ����ü ����
// len �� nums �迭�� �����̰�, nums�� �޸𸮰� �󸶸�ŭ �Ҵ�ǳĿ� ���� ���������� ���̰� ����
// nums[0] ���� nums[1], nums[2], ... �� ����ɼ��� ���� �ڸ�����
typedef struct bigUInt_s {
  uint64_t len;
  uint64_t nums[];
} bigUInt_t;

#define bigUInt bigUInt_t*


// �Ʒ��� �Լ� ������Ÿ�Ե���
// bigUInt�� ���� �޸� �Ҵ��ϴ� �Լ�
bigUInt_t* bigUInt_init();
// bigUInt�� �Ҵ��ߴ� �޸� free�ϴ� �Լ�
void bigUInt_destroy(bigUInt_t* num);
// bigUInt ���� �ø��ų� ���̱�
bigUInt_t* bigUInt_resize(bigUInt_t* a, uint64_t size);

/* Arithmetic Operators */

// +
// dest = a + b
void bigUInt_add(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest);
// -
// dest = a - b
void bigUInt_sub(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest);
// *
// dest = a * b
void bigUInt_mul(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest);
// /
// dest = a / b
bool bigUInt_div_cmp_helper(bigUInt_t** a, bigUInt_t** b, uint64_t len);
void bigUInt_div(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest);
// %
// dest = a % b
void bigUInt_mod(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest);
// ++
// a++
void bigUInt_inc(bigUInt_t** a);
// --
// a--
void bigUInt_dec(bigUInt_t** a);

/* Relational Operators */

// ==
// return a == b
bool bigUInt_eq(bigUInt_t** a, bigUInt_t** b);
// !=
// return a != b
bool bigUInt_neq(bigUInt_t** a, bigUInt_t** b);
// >
// return a > b
bool bigUInt_greater(bigUInt_t** a, bigUInt_t** b);
// <
// return a < b
bool bigUInt_less(bigUInt_t** a, bigUInt_t** b);
// >=
// return a >= b
bool bigUInt_greater_eq(bigUInt_t** a, bigUInt_t** b);
// <=
// return a <= b
bool bigUInt_less_eq(bigUInt_t** a, bigUInt_t** b);

/* Logical Operators */

// a�� 0�̸� 0 ��ȯ, �ƴϸ� 1 ��ȯ
bool bigUInt_n_zero(bigUInt_t** a);
// &&
// return a && b
bool bigUInt_and(bigUInt_t** a, bigUInt_t** b);
// ||
// return a || b
bool bigUInt_or(bigUInt_t** a, bigUInt_t** b);
// !
// return !a
bool bigUInt_not(bigUInt_t** a);

/* Bitwise Operators */

// &
// dest = a & b
void bigUInt_bit_and(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest);
// |
// dest = a | b
void bigUInt_bit_or(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest);
// ^
// dest = a ^ b
void bigUInt_bit_xor(bigUInt_t** a, bigUInt_t** b, bigUInt_t** dest);
// ~
// dest = ~a
void bigUInt_bit_not(bigUInt_t** a, bigUInt_t** dest);
// <<
// dest = a << b
// b�� 64��Ʈ ������ (bigUInt �ƴ�)
void bigUInt_bit_shl(bigUInt_t** a, uint64_t b, bigUInt_t** dest);
// >>
// dest = a >> b
// b�� 64��Ʈ ������ (bigUInt �ƴ�)
void bigUInt_bit_shr(bigUInt_t** a, uint64_t b, bigUInt_t** dest);

// =
// a = b
void bigUInt_assign(bigUInt_t** a, bigUInt_t** b);
// a = b
// b�� 64��Ʈ ������ (bigUInt �ƴ�)

/* utilities */

// int64_t to bigUInt
int itobi(int64_t num, bigUInt_t** dest);
// bigUInt to string
char* bitostr(bigUInt_t* num);
// bigUInt to int64_t
int64_t bitoi(bigUInt_t* num);

// bigUInt ��� ������ printf ������
int bi_printf(const char* format, ...);

#ifdef __cplusplus
}
#endif