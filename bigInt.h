#pragma GCC optimize ("O2")

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#define uint64_t unsigned long long

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

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX_TRIPLE(a, b, c) MAX((a), MAX((b), (c)))
#define MIN_TRIPLE(a, b, c) MIN((a), MIN((b), (c)))

/* bigUInt_t* structure definition */

typedef struct bigUInt_s {
  uint64_t len;
  uint64_t nums[];
} bigUInt_t;

bigUInt_t* bigUInt_init();
void bigUInt_destroy(bigUInt_t* num);
void bigUInt_resize(bigUInt_t* a, long long size);

/* Arithmetic Operators */

// +
int bigUInt_add(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);
// -
int bigUInt_sub(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);
// *
int bigUInt_mul(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);
// /
int bigUInt_div(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);
// %
int bigUInt_mod(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);
// ++
int bigUInt_inc(bigUInt_t* a);
// --
int bigUInt_dec(bigUInt_t* a);

/* Relational Operators */

// ==
bool bigUInt_eq(bigUInt_t* a, bigUInt_t* b);
// !=
bool bigUInt_neq(bigUInt_t* a, bigUInt_t* b);
// >
bool bigUInt_greater(bigUInt_t* a, bigUInt_t* b);
// <
bool bigUInt_less(bigUInt_t* a, bigUInt_t* b);
// >=
bool bigUInt_greater_eq(bigUInt_t* a, bigUInt_t* b);
// <=
bool bigUInt_less_eq(bigUInt_t* a, bigUInt_t* b);

/* Logical Operators */

// &&
bool bigUInt_and(bigUInt_t* a, bigUInt_t* b);
// ||
bool bigUInt_or(bigUInt_t* a, bigUInt_t* b);
// !
bool bigUInt_not(bigUInt_t* a);

/* Bitwise Operators */

// &
int bigUInt_bit_and(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);
// |
int bigUInt_bit_or(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);
// ^
int bigUInt_bit_xor(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);
// ~
int bigUInt_bit_not(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);
// <<
int bigUInt_bit_shl(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);
// >>
int bigUInt_bit_shr(bigUInt_t* a, bigUInt_t* b, bigUInt_t* dest);

int itobi(int64_t num, bigUInt_t* dest);
char* bitostr(bigUInt_t* num);
int64_t bitoi(bigUInt_t* num);

int bi_printf(const char* format, ...);
int bi_formulaf(bigUInt_t* dest, const char* format, ...);

/* flag */
#define CF 0b0000000000000001
#define OF 0b0000100000000000