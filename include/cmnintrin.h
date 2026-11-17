#ifndef GRIMACE_CMNINTRIN_H
#define GRIMACE_CMNINTRIN_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Counts the contiguous one bits at the top of a 32-bit value.
 *
 * Windows CE 4.1 and later.
 *
 * @param value Value to examine.
 * @return Number of leading one bits, 32 if all bits are set.
 */
unsigned int _CountLeadingOnes(long value);
/**
 * Counts the contiguous one bits at the top of a 64-bit value.
 *
 * Windows CE 4.1 and later.
 *
 * @param value Value to examine.
 * @return Number of leading one bits, 64 if all bits are set.
 */
unsigned int _CountLeadingOnes64(long long value);
/**
 * Counts the contiguous bits below the sign bit of a 32-bit value that match
 * it.
 *
 * Useful for normalising fixed-point values. Windows CE 4.1 and later.
 *
 * @param value Value to examine.
 * @return Number of bits after the sign bit that equal it.
 */
unsigned int _CountLeadingSigns(long value);
/**
 * Counts the contiguous bits below the sign bit of a 64-bit value that match
 * it.
 *
 * Windows CE 4.1 and later.
 *
 * @param value Value to examine.
 * @return Number of bits after the sign bit that equal it.
 */
unsigned int _CountLeadingSigns64(long long value);
/**
 * Counts the contiguous zero bits at the top of a 32-bit value.
 *
 * The compiler can emit the ARM CLZ instruction for this when targeting ARMv5
 * or later. Windows CE 4.1 and later.
 *
 * @param value Value to examine.
 * @return Number of leading zero bits, 32 if value is zero.
 */
unsigned int _CountLeadingZeros(long value);
/**
 * Counts the contiguous zero bits at the top of a 64-bit value.
 *
 * Windows CE 4.1 and later.
 *
 * @param value Value to examine.
 * @return Number of leading zero bits, 64 if value is zero.
 */
unsigned int _CountLeadingZeros64(long long value);
/**
 * Counts the one bits in a 32-bit value.
 *
 * Windows CE 4.1 and later.
 *
 * @param value Value to examine.
 * @return Number of bits set.
 */
unsigned int _CountOneBits(long value);
/**
 * Counts the one bits in a 64-bit value.
 *
 * Windows CE 4.1 and later.
 *
 * @param value Value to examine.
 * @return Number of bits set.
 */
unsigned int _CountOneBits64(long long value);
/**
 * Multiplies two signed 32-bit values and returns the high 32 bits of the
 * 64-bit product.
 *
 * Useful for overflow checks and fixed-point multiplication. Windows CE 4.1
 * and later.
 *
 * @param first First factor.
 * @param second Second factor.
 * @return High-order 32 bits of the product.
 */
long _MulHigh(long first, long second);
/**
 * Multiplies two unsigned 32-bit values and returns the high 32 bits of the
 * 64-bit product.
 *
 * Useful for overflow checks and fixed-point multiplication. Windows CE 4.1
 * and later.
 *
 * @param first First factor.
 * @param second Second factor.
 * @return High-order 32 bits of the product.
 */
unsigned long _MulUnsignedHigh(unsigned long first, unsigned long second);
/**
 * Tests whether two doubles are unordered, that is, either is a NaN.
 *
 * The other IEEE comparison results (less, equal, greater) can be tested with
 * the usual operators; this covers the remaining case. Windows CE 4.1 and
 * later.
 *
 * @param first First value.
 * @param second Second value.
 * @return Nonzero if either value is a NaN, otherwise 0.
 */
int _isunordered(double first, double second);
/**
 * Tests whether two floats are unordered, that is, either is a NaN.
 *
 * Windows CE 4.1 and later.
 *
 * @param first First value.
 * @param second Second value.
 * @return Nonzero if either value is a NaN, otherwise 0.
 */
int _isunorderedf(float first, float second);

#ifdef __cplusplus
}
#endif

#if !defined(RC_INVOKED) && !defined(GRIMACE_ALL_DECLARATIONS)
#include <grimace/aliases.h>
#include <grimace/unavailable.h>
#include <grimace/lazy.h>
#endif

#endif
