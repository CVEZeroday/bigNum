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

/* bigInt structure definition */

typedef struct bigInt_s {
  uint64_t len;
  int64_t head;
  uint64_t nums[];
} bigInt_t;
#define bigInt bigInt_t*

bigInt bigInt_init();
bigInt bigInt_destroy(bigInt num);

/* Arithmetic Operators */

// +
int bigInt_add(bigInt a, bigInt b, bigInt dest);
// -
int bigInt_sub(bigInt a, bigInt b, bigInt dest);
// *
int bigInt_mul(bigInt a, bigInt b, bigInt dest);
// /
int bigInt_div(bigInt a, bigInt b, bigInt dest);
// %
int bigInt_mod(bigInt a, bigInt b, bigInt dest);
// ++
int bigInt_inc(bigInt a);
// --
int bigInt_dec(bigInt a);

/* Relational Operators */

// ==
bool bigInt_eq(bigInt a, bigInt b);
// !=
bool bigInt_neq(bigInt a, bigInt b);
// >
bool bigInt_greater(bigInt a, bigInt b);
// <
bool bigInt_less(bigInt a, bigInt b);
// >=
bool bigInt_greater_eq(bigInt a, bigInt b);
// <=
bool bigInt_less_eq(bigInt a, bigInt b);

/* Logical Operators */

// &&
bool bigInt_and(bigInt a, bigInt b);
// ||
bool bigInt_or(bigInt a, bigInt b);
// !
bool bigInt_not(bigInt a);

/* Bitwise Operators */

// &
int bigInt_bit_and(bigInt a, bigInt b, bigInt dest);
// |
int bigInt_bit_or(bigInt a, bigInt b, bigInt dest);
// ^
int bigInt_bit_xor(bigInt a, bigInt b, bigInt dest);
// ~
int bigInt_bit_not(bigInt a, bigInt b, bigInt dest);
// <<
int bigInt_bit_shl(bigInt a, bigInt b, bigInt dest);
// >>
int bigInt_bit_shr(bigInt a, bigInt b, bigInt dest);

int itobi(int64_t num, bigInt dest);
char* bitostr(bigInt num);
int64_t bitoi(bigInt num);

int bi_printf(const char* format, ...);
int bi_formulaf(bigInt dest, const char* format, ...);

#ifdef __aarch64__
#define ADD_OVERFLOW(a, b, overflowed) \
__asm__ __volatile__ (  \
                      "adds x0, %1, %2\n" \
                      "str x0, [%1]\n"  \
                      "cset %0, cs\n" \
                      : "=r" (overflowed) \
                      : "r" (&a), "r" (&b)  \
                      : "x0", "cc"  \
                      );
#define SHL_OVERFLOW(a, b, overflowed) \
__asm__ __volatile__ (  \
""
)
#endif
