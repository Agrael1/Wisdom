
#ifndef SM_BUILTIN_POPCNT_H
#define SM_BUILTIN_POPCNT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>
#include <assert.h>

//
// popcount() algorithm
//
// See: http://www.cnblogs.com/Martinium/articles/popcount.html
// See: https://en.wikipedia.org/wiki/Hamming_weight
// See: https://stackoverflow.com/questions/757059/position-of-least-significant-bit-that-is-set
//

static inline unsigned int __native_popcnt(unsigned int x)
{
    x -= ((x >> 1) & 0x55555555U);
    x = (((x >> 2) & 0x33333333U) + (x & 0x33333333U));
    x = (((x >> 4) + x) & 0x0F0F0F0FU);
    x += (x >> 8);
    x += (x >> 16);
    x = x & 0x0000003FU;
    assert(x >= 0 && x <= 32);
    return x;
}

static inline unsigned int __native_popcnt_slow(unsigned int x)
{
    x = (x & 0x55555555UL) + ((x >> 1) & 0x55555555UL);
    x = (x & 0x33333333UL) + ((x >> 2) & 0x33333333UL);
    x = (x & 0x0F0F0F0FUL) + ((x >> 4) & 0x0F0F0F0FUL);
    x = (x & 0x00FF00FFUL) + ((x >> 8) & 0x00FF00FFUL);
    x = (x & 0x0000FFFFUL) + ((x >> 16) & 0x0000FFFFUL);
    assert(x >= 0 && x <= 32);
    return x;
}

static inline unsigned int __native_hakmem_popcnt(unsigned int x)
{
    unsigned int tmp;
    tmp = x - ((x >> 1) & 033333333333) - ((x >> 2) & 011111111111);
    return (((tmp + (tmp >> 3)) & 030707070707) % 63);
}

static inline unsigned int __native_popcnt64(uint64_t x)
{
#if 1
    x -= ((x >> 1) & 0x55555555U);
    x = (((x >> 2) & 0x33333333U) + (x & 0x33333333U));
    x = (((x >> 4) + x) & 0x0F0F0F0FU);
    x += (x >> 8);
    x += (x >> 16);
    x += (x >> 32);
    x = x & 0x0000007FU;
    assert(x >= 0 && x <= 64);
    return (unsigned int)x;
#elif 0
    x = (x & 0x5555555555555555ULL) + ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x & 0x0F0F0F0F0F0F0F0FULL) + ((x >> 4) & 0x0F0F0F0F0F0F0F0FULL);
    x = (x & 0x00FF00FF00FF00FFULL) + ((x >> 8) & 0x00FF00FF00FF00FFULL);
    x = (x & 0x0000FFFF0000FFFFULL) + ((x >> 16) & 0x0000FFFF0000FFFFULL);
    x = (x & 0x00000000FFFFFFFFULL) + ((x >> 32) & 0x00000000FFFFFFFFULL);
    assert(x >= 0 && x <= 64);
    return (unsigned int)x;
#else
    unsigned int high, low;
    unsigned int n1, n2;
    high = (unsigned int)(x & 0x00000000FFFFFFFFULL);
    low = (unsigned int)((x & 0xFFFFFFFF00000000ULL) >> 32);
    n1 = __native_popcnt(high);
    n2 = __native_popcnt(low);
    return (n1 + n2);
#endif
}

static inline unsigned int __native_clz(unsigned int x)
{
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return (32 - __native_popcnt(x));
}

static inline unsigned int __native_clzll(uint64_t x)
{
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    x |= (x >> 32);
    return (64 - __native_popcnt64(x));
}

static inline unsigned int __native_ctz(unsigned int x)
{
    return __native_popcnt((x & -(int)x) - 1);
}

static inline unsigned int __native_ctzll(uint64_t x)
{
    return __native_popcnt64((x & -(int64_t)x) - 1);
}

#endif // SM_BUILTIN_POPCNT_H


/**************************************************************************************

 _BitScanForward (VC) = __builtin_ctz (gcc) = bsf (asm)
 _BitScanReverse (VC) = __builtin_clz (gcc) = bsr (asm)

  On ARM it would be the CLZ (count leading zeroes) instruction.

 See: https://msdn.microsoft.com/en-us/library/wfd9z0bb.aspx
 See: https://msdn.microsoft.com/en-us/library/fbxyd7zd.aspx

 See: http://www.cnblogs.com/gleam/p/5025867.html

 ¡ª int __builtin_ctz (unsigned int x);
   int __builtin_ctzll (unsigned long long x);

    Returns the number of trailing 0-bits in x, starting at the least significant bit position.
    If x is 0, the result is undefined.   (MSB)

 ¡ª int __builtin_clz (unsigned int x);
   int __builtin_clzll (unsigned long long x);

    Returns the number of leading 0-bits in x, starting at the most significant bit position.
    If x is 0, the result is undefined.   (LSB)

 See: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html

 ¡ª int __builtin_ffs (int x);
   int __builtin_ffsll (long long x);

    Returns one plus the index of the least significant 1-bit of x, or if x is zero, returns zero.

 See: https://blog.stephencleary.com/2010/10/implementing-gccs-builtin-functions.html
 See: http://www.cnblogs.com/miloyip/p/4517053.html

 int __builtin_ffs(int x) {
     if (x == 0)
         return 0;
     return __builtin_ctz((unsigned int)x) + 1;
 }

 How to use MSVC intrinsics to get the equivalent of this GCC code?

 See: https://stackoverflow.com/questions/355967/how-to-use-msvc-intrinsics-to-get-the-equivalent-of-this-gcc-code

***************************************************************************************/

#ifndef SM_BITSCAN_FORWARD_H
#define SM_BITSCAN_FORWARD_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <assert.h>

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_IA64) || defined(__amd64__) || defined(__x86_64__)
#define __IS_X86_64 1
#endif // _WIN64

#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#include <intrin.h> // For _BitScanForward(), _BitScanForward64()
#pragma intrinsic(_BitScanForward)
#if __IS_X86_64
#pragma intrinsic(_BitScanForward64)
#endif // _WIN64
#endif // _MSC_VER

// #include <xmmintrin.h>  // For MMX, SSE instructions
#include <emmintrin.h> // For SSE2 instructions, __SSE2__ | -msse2

//
// See: http://www.cnblogs.com/zyl910/archive/2012/08/27/intrin_table_gcc.html
//
// #include <avxintrin.h>    // __AVX__  | -mavx     AVX:  Advanced Vector Extensions
// #include <avx2intrin.h>   // __AVX2__ | -mavx2    AVX2: Advanced Vector Extensions 2
//

// Get the index of the first bit on set to 1.
#if defined(_MSC_VER) || defined(__ICL) || defined(__INTEL_COMPILER)
// _MSC_VER

#define __BitScanForward(index, mask) \
    _BitScanForward((unsigned long*)&(index), (unsigned long)(mask))

#if __IS_X86_64
#define __BitScanForward64(index, mask) \
    _BitScanForward64((unsigned long*)&(index), (unsigned long long)(mask))
#else
#define __BitScanForward64(index, mask) \
    __BitScanForward(index, mask)
#endif // _WIN64

#elif (defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)))) || defined(__clang__) || defined(__MINGW32__) || defined(__CYGWIN__)
// __GNUC__

#define __BitScanForward(index, mask) \
    __builtin_BitScanForward((unsigned long*)&(index), (unsigned long)mask)

#if __IS_X86_64
#define __BitScanForward64(index, mask) \
    __builtin_BitScanForward64((unsigned long*)&(index), (unsigned long long)mask)
#else
#define __BitScanForward64(index, mask) \
    __BitScanForward(index, mask)
#endif // __x86_64__

#else
// Not support
#define __BitScanForward(index, mask) \
    __builtin_BitScanForward((unsigned long*)&(index), (unsigned long)mask)

#define __BitScanForward64(index, mask) \
    __builtin_BitScanForward64((unsigned long*)&(index), (unsigned long long)mask)

// #error "The compiler does not support BitScanForward()."
#endif // BitScanForward()

static inline unsigned char
__builtin_BitScanForward(unsigned long* index, unsigned long mask)
{
    // assert(index != nullptr);
    unsigned int trailing_zeros;
#if defined(__has_builtin_ctz)
    trailing_zeros = __builtin_ctz((unsigned int)mask);
#else
    trailing_zeros = __native_ctz((unsigned int)mask);
#endif
    *index = trailing_zeros;
    return (unsigned char)(mask != 0);
}

static inline unsigned char
__builtin_BitScanForward64(unsigned long* index, unsigned long long mask)
{
    // assert(index != nullptr);
    unsigned int trailing_zeros;
#if defined(__has_builtin_ctzll)
    trailing_zeros = __builtin_ctzll((unsigned long long)mask);
#else
    trailing_zeros = __native_ctzll((unsigned long long)mask);
#endif
    *index = trailing_zeros;
    return (unsigned char)(mask != 0);
}

#undef __IS_X86_64

#endif // SM_BITSCAN_FORWARD_H