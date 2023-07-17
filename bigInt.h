#pragma GCC optimize ("O2")

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#define DEBUG_MODE 1
#define DEBUG() if(DEBUG_MODE) __debug();

#define KARATSUBA_THRESHOLD 8
#define BIN2DEC_COEFFICIENT 19.26591972
// log10(2) * 64

//#define uint64_t unsigned long long


extern void __bigUInt_add(uint64_t, void*, void*, void*);
extern void __bigUInt_sub(uint64_t, void*, void*, void*);
extern void __bigUInt_inc(uint64_t, void*);
extern void __bigUInt_dec(uint64_t, void*);
extern void __bigUInt_bit_shl(uint64_t, void*);
extern void __bigUInt_bit_shr(uint64_t, void*);

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


#ifdef __cplusplus
extern "C" {
#endif

/* bigUInt_s structure definition */

// bigUInt 구조체 정의
// len 은 nums 배열의 길이이고, nums는 메모리가 얼마만큼 할당되냐에 따라 가변적으로 길이가 변함
// nums[0] 부터 nums[1], nums[2], ... 로 진행될수록 높은 자릿수임
typedef struct bigUInt_s {
  uint64_t len;
  uint64_t nums[];
} bigUInt_t;

// 최댓값, 최솟값 구하는 매크로
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX_TRIPLE(a, b, c) MAX((a), MAX((b), (c)))
#define MIN_TRIPLE(a, b, c) MIN((a), MIN((b), (c)))

// 아래는 함수 프로토타입들임
// bigUInt에 새로 메모리 할당하는 함수
bigUInt_t* bigUInt_init();
// bigUInt에 할당했던 메모리 free하는 함수
void bigUInt_destroy(bigUInt_t* num);
// bigUInt 길이 늘리거나 줄이기
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

// a가 0이면 0 반환, 아니면 1 반환
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
// b는 64비트 정수임 (bigUInt 아님)
void bigUInt_bit_shl(bigUInt_t** a, uint64_t b, bigUInt_t** dest);
// >>
// dest = a >> b
// b는 64비트 정수임 (bigUInt 아님)
void bigUInt_bit_shr(bigUInt_t** a, uint64_t b, bigUInt_t** dest);

// =
// a = b
void bigUInt_assign(bigUInt_t** a, bigUInt_t** b);

/* utilities */

// int64_t to bigUInt
void bigUInt_assign_num(bigUInt_t** dest, int64_t num);
// bigUInt to string
char* bigUInt_toString(bigUInt_t* num);
// bigUInt to uint64_t
uint64_t bigUInt_toUInt(bigUInt_t* num);

#ifdef DEBUG_MODE
void __debug();
#endif

#ifdef __cplusplus
}
#endif
