#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define uint64_t unsigned long long

/**
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
    #define ADD_OVERFLOW(a, b, overflowed) \
__asm__ __volatile__ (  \
    "adds x0, %1, %2\n" \
    "str x0, [%1]\n"  \
    "cset %0, cs\n" \
    : "=r" (overflowed) \
    : "r" (&a), "r" (&b)  \
    : "x0", "cc"  \
);
    #define 
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

int volatile main()
{
  uint64_t a = 0xffffff7fffffffff;
  uint64_t b = 0x7fffffffffffffff;
  bool of;

  clock_t start = clock();
  for (int i = 0; i < 500000000; i++)
  {
    ADD_OVERFLOW(a, b, of);
  }

  // Record the end time
    clock_t end = clock();

    // Calculate the duration
    double duration = (double)(end - start) / CLOCKS_PER_SEC;

  printf("Duration: %.5f seconds\n", duration);

  return 0;
}
