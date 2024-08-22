/*
**	Author: zerico2005 (2024)
**	Project: LIB-Dekker-Float
**	License: MIT License
**	A copy of the MIT License should be included with
**	this project. If not, see https://opensource.org/license/MIT
*/
#ifndef FLOAT64X2_H
#define FLOAT64X2_H

/**
 * @brief Double-Float64 Dekker Float implementation for C.
 * Source: Creel "Double it Like Dekker" on YouTube.
 *
 * @warning -Ofast may break this library. -O3 compiles okay on gcc and clang.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <fenv.h>

//------------------------------------------------------------------------------
// Float64x2 struct
//------------------------------------------------------------------------------

#include "Float64x2_def.h"

/**
 * @brief Allows for type punning
 */
typedef union Bitwise_Float64x2 {
	Float64x2 float_part;
	struct {
		uint64_t hi;
		uint64_t lo;
	} binary_part;
	#ifdef __cplusplus
		Bitwise_Float64x2() : float_part() {}
	#endif
} Bitwise_Float64x2;

//------------------------------------------------------------------------------
// Float64x2 constants
//------------------------------------------------------------------------------

/* C++20 <numbers> */
#define FLOAT64X2_E          {0x1.5bf0a8b145769p+1,+0x1.4d57ee2b1013ap-53} /**< ~2.718281828 */
#define FLOAT64X2_LOG2E      {0x1.71547652b82fep+0,+0x1.777d0ffda0d24p-56} /**< ~1.442695041 */
#define FLOAT64X2_LOG10E     {0x1.bcb7b1526e50ep-2,+0x1.95355baaafad3p-57} /**< ~0.434294482 */
#define FLOAT64X2_PI         {0x1.921fb54442d18p+1,+0x1.1a62633145c07p-53} /**< ~3.141592654 */
#define FLOAT64X2_INV_PI     {0x1.45f306dc9c883p-2,-0x1.6b01ec5417056p-56} /**< ~0.318309886 */
#define FLOAT64X2_INV_SQRTPI {0x1.20dd750429b6dp-1,+0x1.1ae3a914fed80p-57} /**< ~0.564189584 */
#define FLOAT64X2_LN2        {0x1.62e42fefa39efp-1,+0x1.abc9e3b39803fp-56} /**< ~0.693147181 */
#define FLOAT64X2_LN10       {0x1.26bb1bbb55516p+1,-0x1.f48ad494ea3e9p-53} /**< ~2.302585093 */
#define FLOAT64X2_SQRT2      {0x1.6a09e667f3bcdp+0,-0x1.bdd3413b26456p-54} /**< ~1.414213562 */
#define FLOAT64X2_SQRT3      {0x1.bb67ae8584caap+0,+0x1.cec95d0b5c1e3p-54} /**< ~1.732050808 */
#define FLOAT64X2_INV_SQRT3  {0x1.279a74590331cp-1,+0x1.34863e0792bedp-55} /**< ~0.577350269 */
#define FLOAT64X2_EGAMMA     {0x1.2788cfc6fb619p-1,-0x1.6cb90701fbfabp-58} /**< ~0.577215665 */
#define FLOAT64X2_PHI        {0x1.9e3779b97f4a8p+0,-0x1.f506319fcfd19p-55} /**< ~1.618033989 */

/* <math.h> constants */
#define FLOAT64X2_1_PI       FLOAT64X2_INV_PI /**< ~0.318309886 */
#define FLOAT64X2_PI_2       {0x1.921fb54442d18p+0,+0x1.1a62633145c06p-54} /**< ~1.570796327 */
#define FLOAT64X2_PI_4       {0x1.921fb54442d18p-1,+0x1.1a62633145c06p-55} /**< ~0.785398163 */
#define FLOAT64X2_2_PI       {0x1.45f306dc9c883p-1,-0x1.6b01ec5417056p-55} /**< ~0.636619772 */
#define FLOAT64X2_2_SQRTPI   {0x1.20dd750429b6dp+0,+0x1.1ae3a914fed80p-56} /**< ~1.128379167 */
#define FLOAT64X2_SQRT1_2    {0x1.6a09e667f3bcdp-1,-0x1.bdd3413b26456p-55} /**< ~0.707106781 */

#ifdef __cplusplus
	extern "C" {
#endif

//------------------------------------------------------------------------------
// Float64x2 set functions
//------------------------------------------------------------------------------

inline Float64x2 Float64x2_set_zero(void) {
	Float64x2 ret = {0.0, 0.0};
	return ret;
}

inline Float64x2 Float64x2_set_d_d(const fp64 hi, const fp64 lo) {
	Float64x2 ret = {hi, lo};
	return ret;
}

inline Float64x2 Float64x2_set_d(const fp64 x) {
	Float64x2 ret = {x, 0.0};
	return ret;
}

inline Float64x2 Float64x2_set_u64_u64(const uint64_t hi, const uint64_t lo) {
	Bitwise_Float64x2 ret;
	ret.binary_part.hi = hi;
	ret.binary_part.lo = lo;
	return ret.float_part;
}

inline Float64x2 Float64x2_set_i64_i64(const int64_t hi, const int64_t lo) {
	Bitwise_Float64x2 ret;
	ret.binary_part.hi = (uint64_t)hi;
	ret.binary_part.lo = (uint64_t)lo;
	return ret.float_part;
}

//------------------------------------------------------------------------------
// Float64x2 get functions
//------------------------------------------------------------------------------

inline double Float64x2_get_d(const Float64x2 x) {
	return x.hi;
}

inline void Float64x2_get_d_d(
	const Float64x2 src, fp64* const dst_hi, fp64* const dst_lo
) {
	*dst_hi = src.hi;
	*dst_lo = src.lo;
}

inline void Float64x2_get_u64_u64(
	const Float64x2 src, uint64_t* const dst_hi, uint64_t* const dst_lo
) {
	Bitwise_Float64x2 bitwise_src;
	bitwise_src.float_part = src;
	*dst_hi = bitwise_src.binary_part.hi;
	*dst_lo = bitwise_src.binary_part.lo;
}

inline void Float64x2_get_i64_i64(
	const Float64x2 src, int64_t* const dst_hi, int64_t* const dst_lo
) {
	Bitwise_Float64x2 bitwise_src;
	bitwise_src.float_part = src;
	*dst_hi = (int64_t)bitwise_src.binary_part.hi;
	*dst_lo = (int64_t)bitwise_src.binary_part.lo;
}

//------------------------------------------------------------------------------
// Float64x2 basic arithmetic
//------------------------------------------------------------------------------

/**
 * @brief Negates a Float64x2 value (multiplies by -1.0)
 */
inline Float64x2 Float64x2_negate(Float64x2 x) {
	x.hi = -x.hi;
	x.lo = -x.lo;
	return x;
}

/**
 * @brief Adds two Float64x2 values
 */
inline Float64x2 Float64x2_add(const Float64x2 x, const Float64x2 y) {
	fp64 r_hi = x.hi + y.hi;
	fp64 r_lo = 0.0;
	if (fabs(x.hi) > fabs(y.hi)) {
		r_lo = x.hi - r_hi + y.hi + y.lo + x.lo;
	} else {
		r_lo = y.hi - r_hi + x.hi + x.lo + y.lo;
	}

	Float64x2 c;
	c.hi = r_hi + r_lo;
	c.lo = r_hi - c.hi + r_lo;
	return c;
}

/**
 * @brief Subtracts two Float64x2 values
 */
inline Float64x2 Float64x2_sub(const Float64x2 x, const Float64x2 y) {
	fp64 r_hi = x.hi - y.hi;
	fp64 r_lo = 0.0;
	if (fabs(x.hi) > fabs(y.hi)) {
		r_lo = x.hi - r_hi - y.hi - y.lo + x.lo;
	} else {
		r_lo = -y.hi - r_hi + x.hi + x.lo - y.lo;
	}

	Float64x2 c;
	c.hi = r_hi + r_lo;
	c.lo = r_hi - c.hi + r_lo;
	return c;
}

#if defined(FLOATNX2_BITWISE_SPLIT) || defined(FLOAT64X2_BITWISE_SPLIT)
	/**
	* @brief Splits the mantissa bits of a floating point value via bitwise
	* operations for use in the dekker_mul12 function.
	*/
	inline Float64x2 Float64x2_dekker_split(const fp64 x) {
		Bitwise_Float64x2 r;
		const uint64_t Dekker_Split_Mask = ~((uint64_t)0x3FFFFFF);
		r.float_part.hi = x;
		r.binary_part.hi &= Dekker_Split_Mask;
		r.float_part.lo = x - r.float_part.hi;
		return r.float_part;
	}
#else
	/**
	* @brief Splits the mantissa bits of a floating point value via
	* multiplication for use in the dekker_mul12 function.
	*/
	inline Float64x2 Float64x2_dekker_split(const fp64 x) {
		const fp64 Dekker_Scale = 134217729.0; // (2^ceil(53 / 2) + 1)
		fp64 p = x * Dekker_Scale;
		Float64x2 r;
		r.hi = (x - p) + p;
		r.lo = x - r.hi;
		return r;
	}
#endif

/**
 * @brief Subtracts two Float64x2 values
 */
inline Float64x2 Float64x2_dekker_mul12(const fp64 x, const fp64 y) {
	Float64x2 a = Float64x2_dekker_split(x);
	Float64x2 b = Float64x2_dekker_split(y);
	fp64 p = a.hi * b.hi;
	fp64 q = a.hi * b.lo + a.lo * b.hi;

	Float64x2 r;
	r.hi = p + q;
	r.lo = p - r.hi + q + a.lo * b.lo;
	return r;
}

inline Float64x2 Float64x2_mul(const Float64x2 x, const Float64x2 y) {
	Float64x2 t = Float64x2_dekker_mul12(x.hi, y.hi);
	fp64 c = x.hi * y.lo + x.lo * y.hi + t.lo;

	Float64x2 r;
	r.hi = t.hi + c;
	r.lo = t.hi - r.hi + c;
	return r;
}

inline Float64x2 Float64x2_div(const Float64x2 x, const Float64x2 y) {
	fp64 u = x.hi / y.hi;
	Float64x2 t = Float64x2_dekker_mul12(u, y.hi);
	fp64 l = (x.hi - t.hi - t.lo + x.lo - u * y.lo) / y.hi;

	Float64x2 r;
	r.hi = u + l;
	r.lo = u - r.hi + l;
	return r;
}

/**
 * @brief returns 0 on division by 0
 */
inline Float64x2 Float64x2_div_zero(const Float64x2 x, const Float64x2 y) {
	if (y.hi == 0.0) {
		return Float64x2_set_zero();
	}
	fp64 u = x.hi / y.hi;
	Float64x2 t = Float64x2_dekker_mul12(u, y.hi);
	fp64 l = (x.hi - t.hi - t.lo + x.lo - u * y.lo) / y.hi;

	Float64x2 r;
	r.hi = u + l;
	r.lo = u - r.hi + l;
	return r;
}


inline Float64x2 Float64x2_dekker_square12(const fp64 x) {
	Float64x2 a = Float64x2_dekker_split(x);
	fp64 p = a.hi * a.hi;
	fp64 q = 2.0 * (a.hi * a.lo);

	Float64x2 r;
	r.hi = p + q;
	r.lo = (
		((p - r.hi) + q) +
		(a.lo + a.lo)
	);
	return r;
}

inline Float64x2 Float64x2_square(const Float64x2 x) {
	Float64x2 t = Float64x2_dekker_square12(x.hi);
	fp64 c = ((2.0 * (x.hi * x.lo)) + t.lo);

	Float64x2 r;
	r.hi = (t.hi + c);
	r.lo = ((t.hi - r.hi) + c);
	return r;
}


inline Float64x2 Float64x2_recip(const Float64x2 y) {
	fp64 u = (1.0 / y.hi);
	Float64x2 t = Float64x2_dekker_mul12(u, y.hi);
	fp64 l = ((((1.0 - t.hi) - t.lo) - (u * y.lo)) / y.hi);

	Float64x2 r;
	r.hi = (u + l);
	r.lo = ((u - r.hi) + l);
	return r;
}

//------------------------------------------------------------------------------
// Float64x2 optimized arithmetic
//------------------------------------------------------------------------------


inline Float64x2 Float64x2_add_dx2_d(const Float64x2 x, const fp64 y) {
	fp64 r_hi = x.hi + y;
	fp64 r_lo = 0.0;
	if (fabs(x.hi) > fabs(y)) {
		r_lo = x.hi - r_hi + y + x.lo;
	} else {
		r_lo = y - r_hi + x.hi + x.lo;
	}

	Float64x2 c;
	c.hi = r_hi + r_lo;
	c.lo = r_hi - c.hi + r_lo;
	return c;
}

inline Float64x2 Float64x2_add_d_dx2(const fp64 x, const Float64x2 y) {
	fp64 r_hi = x + y.hi;
	fp64 r_lo = 0.0;
	if (fabs(x) > fabs(y.hi)) {
		r_lo = x - r_hi + y.hi + y.lo;
	} else {
		r_lo = y.hi - r_hi + x + y.lo;
	}

	Float64x2 c;
	c.hi = r_hi + r_lo;
	c.lo = r_hi - c.hi + r_lo;
	return c;
}

/**
 * @brief Adds two fp64 values with the result stored as a Float64x2
 */
inline Float64x2 Float64x2_add_d_d(const fp64 x, const fp64 y) {
	fp64 r_hi = x + y;
	fp64 r_lo = 0.0;
	if (fabs(x) > fabs(y)) {
		r_lo = x - r_hi + y;
	} else {
		r_lo = y - r_hi + x;
	}

	Float64x2 c;
	c.hi = r_hi + r_lo;
	c.lo = r_hi - c.hi + r_lo;
	return c;
}

inline Float64x2 Float64x2_sub_dx2_d(const Float64x2 x, const fp64 y) {
	fp64 r_hi = x.hi - y;
	fp64 r_lo = 0.0;
	if (fabs(x.hi) > fabs(y)) {
		r_lo = x.hi - r_hi - y + x.lo;
	} else {
		r_lo = -y - r_hi + x.hi + x.lo;
	}

	Float64x2 c;
	c.hi = r_hi + r_lo;
	c.lo = r_hi - c.hi + r_lo;
	return c;
}

inline Float64x2 Float64x2_sub_d_dx2(const fp64 x, const Float64x2 y) {
	fp64 r_hi = x - y.hi;
	fp64 r_lo = 0.0;
	if (fabs(x) > fabs(y.hi)) {
		r_lo = x - r_hi - y.hi - y.lo;
	} else {
		r_lo = -y.hi - r_hi + x - y.lo;
	}

	Float64x2 c;
	c.hi = r_hi + r_lo;
	c.lo = r_hi - c.hi + r_lo;
	return c;
}

/**
 * @brief Subtracts two fp64 values with the result stored as a Float64x2
 */
inline Float64x2 Float64x2_sub_d_d(const fp64 x, const fp64 y) {
	fp64 r_hi = x - y;
	fp64 r_lo = 0.0;
	if (fabs(x) > fabs(y)) {
		r_lo = x - r_hi - y;
	} else {
		r_lo = -y - r_hi + x;
	}

	Float64x2 c;
	c.hi = r_hi + r_lo;
	c.lo = r_hi - c.hi + r_lo;
	return c;
}

inline Float64x2 Float64x2_mul_dx2_d(const Float64x2 x, const fp64 y) {
	Float64x2 t = Float64x2_dekker_mul12(x.hi, y);
	fp64 c = ((x.lo * y) + t.lo);

	Float64x2 r;
	r.hi = (t.hi + c);
	r.lo = ((t.hi - r.hi) + c);
	return r;
}

inline Float64x2 Float64x2_mul_d_dx2(const fp64 x, const Float64x2 y) {
	Float64x2 t = Float64x2_dekker_mul12(x, y.hi);
	fp64 c = ((x * y.lo) + t.lo);

	Float64x2 r;
	r.hi = (t.hi + c);
	r.lo = ((t.hi - r.hi) + c);
	return r;
}

/**
 * @brief Multiplies two fp64 values with the result stored as a Float64x2
 */
inline Float64x2 Float64x2_mul_d_d(const fp64 x, const fp64 y) {
	return Float64x2_dekker_mul12(x, y);
}

inline Float64x2 Float64x2_div_dx2_d(const Float64x2 x, const fp64 y) {
	fp64 u = x.hi / y;
	Float64x2 t = Float64x2_dekker_mul12(u, y);
	fp64 l = (((x.hi - t.hi) - t.lo) + x.lo) / y;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

inline Float64x2 Float64x2_div_d_dx2(const fp64 x, const Float64x2 y) {
	fp64 u = x / y.hi;
	Float64x2 t = Float64x2_dekker_mul12(u, y.hi);
	fp64 l = (((x - t.hi) - t.lo) - (u * y.lo)) / y.hi;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

/**
 * @brief Divides two fp64 values with the result stored as a Float64x2
 */
inline Float64x2 Float64x2_div_d_d(const fp64 x, const fp64 y) {
	fp64 u = x / y;
	Float64x2 t = Float64x2_dekker_mul12(u, y);
	fp64 l = ((x - t.hi) - t.lo) / y;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

/**
 * @brief Returns 0 on division by 0
 */
inline Float64x2 Float64x2_div_zero_dx2_d(const Float64x2 x, const fp64 y) {
	if (y == 0.0) {
		return Float64x2_set_zero();
	}
	fp64 u = x.hi / y;
	Float64x2 t = Float64x2_dekker_mul12(u, y);
	fp64 l = (((x.hi - t.hi) - t.lo) + x.lo) / y;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

/**
 * @brief Returns 0 on division by 0
 */

inline Float64x2 Float64x2_div_zero_d_dx2(const fp64 x, const Float64x2 y) {
	if (y.hi == 0.0) {
		return Float64x2_set_zero();
	}
	fp64 u = x / y.hi;
	Float64x2 t = Float64x2_dekker_mul12(u, y.hi);
	fp64 l = (((x - t.hi) - t.lo) - (u * y.lo)) / y.hi;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

/**
 * @brief Returns 0 on division by 0. Divides two fp64 values with the
 * result stored as a Float64x2
 */
inline Float64x2 Float64x2_div_zero_d_d(const fp64 x, const fp64 y) {
	if (y == 0.0) {
		return Float64x2_set_zero();
	}
	fp64 u = x / y;
	Float64x2 t = Float64x2_dekker_mul12(u, y);
	fp64 l = ((x - t.hi) - t.lo) / y;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

/**
 * @brief Squares a fp64 value with the result stored as a Float64x2
 */
inline Float64x2 Float64x2_square_d(const fp64 x) {
	return Float64x2_dekker_square12(x);
}

/**
 * @brief Calculates the reciprocal of a fp64 value with the result stored
 * as a Float64x2
 */

inline Float64x2 Float64x2_recip_d(const fp64 y) {
	fp64 u = (1.0 / y);
	Float64x2 t = Float64x2_dekker_mul12(u, y);
	fp64 l = (((1.0 - t.hi) - t.lo) / y);

	Float64x2 r;
	r.hi = (u + l);
	r.lo = ((u - r.hi) + l);
	return r;
}

//------------------------------------------------------------------------------
// Float64x2 specialized arithmetic
//------------------------------------------------------------------------------

/**
 * @brief Multiplies by a known power of two (such as 2.0, 0.5, etc.) or zero
 */
inline Float64x2 Float64x2_mul_power2_dx2_d(Float64x2 x, const fp64 y) {
	x.hi = x.hi * y;
	x.lo = x.lo * y;
	return x;
}

/**
 * @brief Multiplies by a known power of two (such as 2.0, 0.5, etc.) or zero
 */
inline Float64x2 Float64x2_mul_power2_d_dx2(const fp64 x, Float64x2 y) {
	y.hi = x * y.hi;
	y.lo = x * y.lo;
	return y;
}

/**
 * @brief Multiplies by a known power of two (such as 2.0, 0.5, etc.) or zero.
 * The result is stored as a Float64x2
 */
inline Float64x2 Float64x2_mul_power2_d_d(const fp64 x, const fp64 y) {
	Float64x2 ret;
	ret.hi = x * y;
	ret.lo = 0.0;
	return ret;
}

//------------------------------------------------------------------------------
// Float64x2 bitwise operations
//------------------------------------------------------------------------------

inline Float64x2 Float64x2_bitwise_and(const Float64x2 x, const Float64x2 y) {
	Bitwise_Float64x2 x0, y0;
	x0.float_part = x;
	y0.float_part = y;
	x0.binary_part.hi &= y0.binary_part.hi;
	x0.binary_part.lo &= y0.binary_part.lo;
	return x0.float_part;
}

inline Float64x2 Float64x2_bitwise_andnot(const Float64x2 x, const Float64x2 y) {
	Bitwise_Float64x2 x0, y0;
	x0.float_part = x;
	y0.float_part = y;
	x0.binary_part.hi &= ~y0.binary_part.hi;
	x0.binary_part.lo &= ~y0.binary_part.lo;
	return x0.float_part;
}

inline Float64x2 Float64x2_bitwise_or(const Float64x2 x, const Float64x2 y) {
	Bitwise_Float64x2 x0, y0;
	x0.float_part = x;
	y0.float_part = y;
	x0.binary_part.hi |= y0.binary_part.hi;
	x0.binary_part.lo |= y0.binary_part.lo;
	return x0.float_part;
}

inline Float64x2 Float64x2_bitwise_xor(const Float64x2 x, const Float64x2 y) {
	Bitwise_Float64x2 x0, y0;
	x0.float_part = x;
	y0.float_part = y;
	x0.binary_part.hi ^= y0.binary_part.hi;
	x0.binary_part.lo ^= y0.binary_part.lo;
	return x0.float_part;
}

//------------------------------------------------------------------------------
// Float64x2 comparison functions
//------------------------------------------------------------------------------

inline bool Float64x2_cmpeq(const Float64x2 x, const Float64x2 y) {
	return (x.hi == y.hi && x.lo == y.lo);
}

inline bool Float64x2_cmpneq(const Float64x2 x, const Float64x2 y) {
	return (x.hi != y.hi || x.lo != y.lo);
}

inline bool Float64x2_cmpord(const Float64x2 x, const Float64x2 y) {
	return (!isunordered(x.hi, y.hi) && !isunordered(x.lo, y.lo));
}

inline bool Float64x2_cmpunord(const Float64x2 x, const Float64x2 y) {
	return (isunordered(x.hi, y.hi) || isunordered(x.lo, y.lo));
}

inline bool Float64x2_cmplt(const Float64x2 x, const Float64x2 y) {
	if (x.hi == y.hi) {
		return (x.lo < y.lo);
	}
	return (x.hi < y.hi);
}

inline bool Float64x2_cmple(const Float64x2 x, const Float64x2 y) {
	if (x.hi == y.hi) {
		return (x.lo <= y.lo);
	}
	return (x.hi < y.hi);
}

inline bool Float64x2_cmpgt(const Float64x2 x, const Float64x2 y) {
	if (x.hi == y.hi) {
		return (x.lo > y.lo);
	}
	return (x.hi > y.hi);
}

inline bool Float64x2_cmpge(const Float64x2 x, const Float64x2 y) {
	if (x.hi == y.hi) {
		return (x.lo >= y.lo);
	}
	return (x.hi > y.hi);
}

//------------------------------------------------------------------------------
// Float64x2 math.h comparison functions
//------------------------------------------------------------------------------

inline bool Float64x2_isgreater(const Float64x2 x, const Float64x2 y) {
	return Float64x2_cmpgt(x, y);
}
inline bool Float64x2_isgreaterequal(const Float64x2 x, const Float64x2 y) {
	return Float64x2_cmpge(x, y);
}
inline bool Float64x2_isless(const Float64x2 x, const Float64x2 y) {
	return Float64x2_cmplt(x, y);
}
inline bool Float64x2_islessequal(const Float64x2 x, const Float64x2 y) {
	return Float64x2_cmple(x, y);
}
inline bool Float64x2_islessgreater(const Float64x2 x, const Float64x2 y) {
	return Float64x2_cmplt(x, y) || Float64x2_cmpgt(x, y);
}

//------------------------------------------------------------------------------
// Float64x2 compare to zero functions
//------------------------------------------------------------------------------

inline bool Float64x2_cmpeq_zero(const Float64x2 x) {
	return (x.hi == 0.0 && x.lo == 0.0);
}

inline bool Float64x2_cmpneq_zero(const Float64x2 x) {
	return (x.hi != 0.0 || x.lo != 0.0);
}

inline bool Float64x2_cmplt_zero(const Float64x2 x) {
	if (x.hi == 0.0) {
		return (x.lo < 0.0);
	}
	return (x.hi < 0.0);
}

inline bool Float64x2_cmple_zero(const Float64x2 x) {
	if (x.hi == 0.0) {
		return (x.lo <= 0.0);
	}
	return (x.hi < 0.0);
}

inline bool Float64x2_cmpgt_zero(const Float64x2 x) {
	if (x.hi == 0.0) {
		return (x.lo > 0.0);
	}
	return (x.hi > 0.0);
}

inline bool Float64x2_cmpge_zero(const Float64x2 x) {
	if (x.hi == 0.0) {
		return (x.lo >= 0.0);
	}
	return (x.hi > 0.0);
}

//------------------------------------------------------------------------------
// Float64x2 optimized comparison functions
//------------------------------------------------------------------------------

inline bool Float64x2_cmpeq_dx2_d(const Float64x2 x, const fp64 y) {
	return (
		x.hi == y &&
		x.lo == 0.0
	) ? true : false;
}

inline bool Float64x2_cmpneq_dx2_d(const Float64x2 x, const fp64 y) {
	return (
		x.hi != y ||
		x.lo != 0.0
	) ? true : false;
}

inline bool Float64x2_cmpord_dx2_d(const Float64x2 x, const fp64 y) {
	return (!isunordered(x.hi, y));
}

inline bool Float64x2_cmpunord_dx2_d(const Float64x2 x, const fp64 y) {
	return (isunordered(x.hi, y));
}

inline bool Float64x2_cmplt_dx2_d(const Float64x2 x, const fp64 y) {
	if (x.hi == y) {
		return (x.lo < 0.0);
	}
	return (x.hi < y);
}

inline bool Float64x2_cmple_dx2_d(const Float64x2 x, const fp64 y) {
	if (x.hi == y) {
		return (x.lo <= 0.0);
	}
	return (x.hi < y);
}

inline bool Float64x2_cmpgt_dx2_d(const Float64x2 x, const fp64 y) {
	if (x.hi == y) {
		return (x.lo > 0.0);
	}
	return (x.hi > y);
}

inline bool Float64x2_cmpge_dx2_d(const Float64x2 x, const fp64 y) {
	if (x.hi == y) {
		return (x.lo >= 0.0);
	}
	return (x.hi > y);
}

inline bool Float64x2_cmpeq_d_dx2(const fp64 x, const Float64x2 y) {
	return (
		x == y.hi &&
		0.0 == y.lo
	) ? true : false;
}

inline bool Float64x2_cmpneq_d_dx2(const fp64 x, const Float64x2 y) {
	return (
		x != y.hi ||
		0.0 != y.lo
	) ? true : false;
}

inline bool Float64x2_cmpord_d_dx2(const fp64 x, const Float64x2 y) {
	return (!isunordered(x, y.hi));
}

inline bool Float64x2_cmpunord_d_dx2(const fp64 x, const Float64x2 y) {
	return (isunordered(x, y.hi));
}

inline bool Float64x2_cmplt_d_dx2(const fp64 x, const Float64x2 y) {
	if (x == y.hi) {
		return (0.0 < y.lo);
	}
	return (x < y.hi);
}

inline bool Float64x2_cmple_d_dx2(const fp64 x, const Float64x2 y) {
	if (x == y.hi) {
		return (0.0 <= y.lo);
	}
	return (x < y.hi);
}

inline bool Float64x2_cmpgt_d_dx2(const fp64 x, const Float64x2 y) {
	if (x == y.hi) {
		return (0.0 > y.lo);
	}
	return (x > y.hi);
}

inline bool Float64x2_cmpge_d_dx2(const fp64 x, const Float64x2 y) {
	if (x == y.hi) {
		return (0.0 >= y.lo);
	}
	return (x > y.hi);
}

//------------------------------------------------------------------------------
// Float64x2 max/min functions
//------------------------------------------------------------------------------

inline Float64x2 Float64x2_max(const Float64x2 x, const Float64x2 y) {
	Float64x2 ret;
	ret.hi = fmax(x.hi, y.hi);

	if (x.hi == y.hi) {
		ret.lo = fmax(x.lo, y.lo);
		return ret;
	}
	ret.lo = (x.hi > y.hi) ? x.lo : y.lo;

	return ret;
}

inline Float64x2 Float64x2_min(const Float64x2 x, const Float64x2 y) {
	Float64x2 ret;
	ret.hi = fmin(x.hi, y.hi);

	if (x.hi == y.hi) {
		ret.lo = fmin(x.lo, y.lo);
		return ret;
	}
	ret.lo = (x.hi < y.hi) ? x.lo : y.lo;

	return ret;
}

//------------------------------------------------------------------------------
// Float64x2 rounding functions
//------------------------------------------------------------------------------

/** same as aint(x) */
inline Float64x2 Float64x2_trunc(const Float64x2 x) {
	Float64x2 int_hi = {trunc(x.hi), 0.0};
	Float64x2 int_lo = {trunc(x.lo), 0.0};
	fp64 frac_hi = x.hi - int_hi.hi;
	fp64 frac_lo = x.lo - int_lo.hi;
	// Sum in increasing order
	Float64x2 trunc_all = Float64x2_set_zero();
	trunc_all = (
		Float64x2_cmpge(
			Float64x2_add_d_d(frac_hi, frac_lo),
			Float64x2_set_d(1.0)
		)
	) ? Float64x2_add_dx2_d(trunc_all, 1.0) : trunc_all;
	trunc_all = Float64x2_add(trunc_all, int_lo);
	trunc_all = Float64x2_add(trunc_all, int_hi);
	return trunc_all;
}
inline Float64x2 Float64x2_floor(const Float64x2 x) {
	Float64x2 int_part =  Float64x2_trunc(x);
	return (
		Float64x2_cmplt_zero(x) && Float64x2_cmpneq(int_part, x)
	) ? Float64x2_sub_dx2_d(int_part, 1.0) : int_part;
}
inline Float64x2 Float64x2_ceil(const Float64x2 x) {
	Float64x2 int_part = Float64x2_trunc(x);
	return (
		Float64x2_cmpgt_zero(x) && Float64x2_cmpneq(int_part, x)
	) ? Float64x2_add_dx2_d(int_part, 1.0) : int_part;
}
/** same as nint(x) */
inline Float64x2 Float64x2_round(const Float64x2 x) {
	Float64x2 int_part = Float64x2_trunc(x);
	Float64x2 frac_part = Float64x2_sub(x, int_part);
	if (Float64x2_cmpge_zero(x)) {
		if (Float64x2_cmpge(frac_part, Float64x2_set_d(0.5))) {
			return Float64x2_add_dx2_d(int_part, 1.0);
		}
		return int_part;
	}
	if (Float64x2_cmple(frac_part, Float64x2_set_d(-0.5))) {
		return Float64x2_sub_dx2_d(int_part, 1.0);
	}
	return int_part;
}
inline Float64x2 Float64x2_rint(const Float64x2 x) {
	switch (fegetround()) {
		default:
		case FE_TOWARDZERO:
			return Float64x2_trunc(x);
		case FE_DOWNWARD:
			return Float64x2_floor(x);
		case FE_UPWARD:
			return Float64x2_ceil(x);
		case FE_TONEAREST:
			return Float64x2_round(x);
	}
}
inline long Float64x2_lround(const Float64x2 x) {
	Float64x2 ret = Float64x2_round(x);
	return (long)(ret.hi + ret.lo);
}
inline long Float64x2_lrint(const Float64x2 x) {
	Float64x2 ret = Float64x2_rint(x);
	return (long)(ret.hi + ret.lo);
}
inline long long Float64x2_llround(const Float64x2 x) {
	Float64x2 ret = Float64x2_round(x);
	return (long long)(ret.hi + ret.lo);
}
inline long long Float64x2_llrint(const Float64x2 x) {
	Float64x2 ret = Float64x2_rint(x);
	return (long long)(ret.hi + ret.lo);
}

//------------------------------------------------------------------------------
// Float64x2 math.h functions
//------------------------------------------------------------------------------

inline Float64x2 Float64x2_fabs(const Float64x2 x) {
	return (
		Float64x2_cmplt_zero(x)
	) ? Float64x2_negate(x) : x;
}

inline Float64x2 Float64x2_fdim(const Float64x2 x, const Float64x2 y) {
	return (
		Float64x2_cmple(x, y)
	) ? Float64x2_sub(x, y) : Float64x2_set_zero();
}

inline Float64x2 Float64x2_copysign(const Float64x2 x, const Float64x2 y) {
	return (
		Float64x2_cmplt_zero(x) != Float64x2_cmplt_zero(y)
	) ? Float64x2_negate(x) : x;
}

inline Float64x2 Float64x2_sqrt(const Float64x2 x) {
	fp64 guess = sqrt(x.hi);
	return Float64x2_mul_power2_dx2_d(Float64x2_add_d_dx2(
		guess, Float64x2_div_zero_dx2_d(x, guess)
	), 0.5);
}

inline Float64x2 Float64x2_cbrt(const Float64x2 x) {
	fp64 guess = cbrt(x.hi);
	return Float64x2_div_dx2_d(Float64x2_add(
			Float64x2_mul_power2_d_d(2.0, guess),
			Float64x2_div_zero(x, Float64x2_square_d(guess))
	), 3.0);
}

/**
 * @brief returns the fraction part of a Float64x2 value. int_part may be NULL
 */
inline Float64x2 Float64x2_modf(const Float64x2 x, Float64x2* const int_part) {
	Float64x2 trunc_part = Float64x2_trunc(x);
	if (int_part != NULL) {
		*int_part = trunc_part;
	}
	return Float64x2_sub(x, trunc_part);
}

inline Float64x2 Float64x2_fmod(const Float64x2 x, const Float64x2 y) {
	Float64x2 trunc_part = Float64x2_trunc(Float64x2_div(x, y));
	return Float64x2_sub(x, Float64x2_mul(y, trunc_part));
}

//------------------------------------------------------------------------------
// Float64x2 exponents and logarithms
//------------------------------------------------------------------------------

Float64x2 Float64x2_exp(Float64x2 x);
inline Float64x2 Float64x2_exp2(const Float64x2 x) {
	const Float64x2 mult_val = FLOAT64X2_LN2;
	return Float64x2_exp(Float64x2_mul(x, mult_val));
}
inline Float64x2 Float64x2_exp10(const Float64x2 x) {
	const Float64x2 mult_val = FLOAT64X2_LN10;
	return Float64x2_exp(Float64x2_mul(x, mult_val));
}

Float64x2 Float64x2_log(Float64x2 x);
inline Float64x2 Float64x2_log2(const Float64x2 x) {
	const Float64x2 mult_val = FLOAT64X2_LOG2E;
	return Float64x2_mul(Float64x2_log(x), mult_val);
}
inline Float64x2 Float64x2_log10(const Float64x2 x) {
	const Float64x2 mult_val = FLOAT64X2_LOG10E;
	return Float64x2_mul(Float64x2_log(x), mult_val);
}

//------------------------------------------------------------------------------
// Float64x2 trigonometry
//------------------------------------------------------------------------------

Float64x2 Float64x2_sin(Float64x2 x);
Float64x2 Float64x2_cos(Float64x2 x);
void Float64x2_sincos(
	Float64x2 theta, Float64x2* p_sin, Float64x2* p_cos
);
inline Float64x2 Float64x2_tan(const Float64x2 x) {
	Float64x2 t_sin, t_cos;
	Float64x2_sincos(x, &t_sin, &t_cos);
	return Float64x2_div(t_sin, t_cos);
}

Float64x2 Float64x2_asin(Float64x2 x);
Float64x2 Float64x2_acos(Float64x2 x);
Float64x2 Float64x2_atan(Float64x2 x);
Float64x2 Float64x2_atan2(Float64x2 y, Float64x2 x);

/** @note sinh is inaccurate when x is close to 0, which is why it isn't inlined */
Float64x2 Float64x2_sinh(Float64x2 x);
inline Float64x2 Float64x2_cosh(const Float64x2 x) {
	Float64x2 exp_x = Float64x2_exp(x);
	return Float64x2_mul_power2_dx2_d(Float64x2_add(
		exp_x, Float64x2_recip(exp_x)
	), 0.5);
}
Float64x2 Float64x2_tanh(Float64x2 x);
void Float64x2_sinhcosh(
	Float64x2 theta, Float64x2* p_sinh, Float64x2* p_cosh
);

inline Float64x2 Float64x2_asinh(const Float64x2 x) {
	return Float64x2_log(Float64x2_add(x,
		Float64x2_sqrt(
			Float64x2_sub_dx2_d(Float64x2_square(x), 1.0)
		)
	));
}
inline Float64x2 Float64x2_acosh(const Float64x2 x) {
	return Float64x2_log(Float64x2_add(x,
		Float64x2_sqrt(
			Float64x2_add_dx2_d(Float64x2_square(x), 1.0)
		)
	));
}
inline Float64x2 Float64x2_atanh(const Float64x2 x) {
	return Float64x2_mul_power2_dx2_d(Float64x2_log(Float64x2_div(
			Float64x2_add_d_dx2(1.0, x),
			Float64x2_sub_d_dx2(1.0, x)
	)), 0.5);
}

//------------------------------------------------------------------------------
// Float64x2 string operations
//------------------------------------------------------------------------------

#ifdef __cplusplus
	Float64x2 stringTo_Float64x2(const char* nPtr, char** endPtr = nullptr);
#else
	Float64x2 stringTo_Float64x2(const char* nPtr, char** endPtr);
#endif

#define PRIFloat64x2 "D"
#define PRIfp64x2 PRIFloat64x2

/**
 * @brief snprintf a singular Float64x2/fp64x2.
 * Similar in functionallity to quadmath_snprintf.
 * @note $ not supported. This function ignores additional format specifiers.
 * @return -1 on encoding failure. Otherwise the total length of the
 * string excluding the \0 terminator and ignoring the buffer size.
 */
int Float64x2_snprintf(char* buf, size_t len, const char* format, ...);

#ifdef __cplusplus
	}
#endif

#ifdef __cplusplus
#include "double_Float64.hpp"
#endif

#endif /* FLOAT64X2_H */ 
