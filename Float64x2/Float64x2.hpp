/*
**	Author: zerico2005 (2024)
**	Project: LIB-Dekker-Float
**	License: MIT License
**	A copy of the MIT License should be included with
**	this project. If not, see https://opensource.org/license/MIT
*/

#ifndef FLOAT64X2_HPP
#define FLOAT64X2_HPP

/**
 * @brief Double-Float64 Dekker Float implementation for C++.
 *
 * @warning -Ofast may break this library. -O3 compiles okay on gcc and clang.
 */

#include "Float64x2_def.h"

#include "../LDF/LDF_arithmetic.hpp"
#include "Float64x2_input_limits.hpp"

#include <cstdint>
#include <limits>
#include <cmath>
#include <cfenv>
#include <type_traits>

//------------------------------------------------------------------------------
// Float64x2 LDF Type Information
//------------------------------------------------------------------------------

#include "../LDF/LDF_type_info.hpp"
namespace LDF {
	template<> struct LDF_Type_Info<Float64x2> {
		static constexpr int FloatBase_Count = 2;
		static constexpr bool to_string_implemented = true;
		static constexpr bool from_string_implemented = true;
		static constexpr bool arithmetic_implemented = true;
		static constexpr bool numeric_limits_implemented = true;
		static constexpr bool bitwise_implemented = true;
		static constexpr bool constants_implemented = true;
		static constexpr bool basic_C99_math_implemented = true;
		static constexpr bool accurate_C99_math_implemented = false; // erfc is very inaccurate when x > 0
	};
}

//------------------------------------------------------------------------------
// Float64x2 String Operations
//------------------------------------------------------------------------------

#include "Float64x2_string.h"

#if __cplusplus >= 200809L
/**
 * @brief Wrapper for stringTo_Float64x2
 */
inline Float64x2 operator""_FP64X2(const char* str, std::size_t) {
	return stringTo_Float64x2(str, nullptr);
}
#endif

#include <istream>
/**
 * @brief Wrapper for stringTo_Float64x2
 */
std::istream& operator>>(std::istream& stream, Float64x2& value);

#include <ostream>
/**
 * @brief Wrapper for Float64x2_snprintf
 */
std::ostream& operator<<(std::ostream& stream, const Float64x2& value);


//------------------------------------------------------------------------------
// Float64x2 Comparison
//------------------------------------------------------------------------------

/* Comparison */

inline constexpr bool operator==(const Float64x2& x, const Float64x2& y) {
	return (x.hi == y.hi && x.lo == y.lo);
}
inline constexpr bool operator!=(const Float64x2& x, const Float64x2& y) {
	return (x.hi != y.hi || x.lo != y.lo);
}
inline constexpr bool operator< (const Float64x2& x, const Float64x2& y) {
	return (x.hi < y.hi || (x.hi == y.hi && x.lo <  y.lo));
}
inline constexpr bool operator<=(const Float64x2& x, const Float64x2& y) {
	return (x.hi < y.hi || (x.hi == y.hi && x.lo <= y.lo));
}
inline constexpr bool operator> (const Float64x2& x, const Float64x2& y) {
	return (x.hi > y.hi || (x.hi == y.hi && x.lo >  y.lo));
}
inline constexpr bool operator>=(const Float64x2& x, const Float64x2& y) {
	return (x.hi > y.hi || (x.hi == y.hi && x.lo >= y.lo));
}

/* Optimized Comparison */

inline constexpr bool operator==(const Float64x2& x, const fp64 y) {
	return (x.hi == y && x.lo == static_cast<fp64>(0.0));
}
inline constexpr bool operator!=(const Float64x2& x, const fp64 y) {
	return (x.hi != y || x.lo != static_cast<fp64>(0.0));
}
inline constexpr bool operator< (const Float64x2& x, const fp64 y) {
	return (x.hi < y || (x.hi == y && x.lo <  static_cast<fp64>(0.0)));
}
inline constexpr bool operator<=(const Float64x2& x, const fp64 y) {
	return (x.hi < y || (x.hi == y && x.lo <= static_cast<fp64>(0.0)));
}
inline constexpr bool operator> (const Float64x2& x, const fp64 y) {
	return (x.hi > y || (x.hi == y && x.lo >  static_cast<fp64>(0.0)));
}
inline constexpr bool operator>=(const Float64x2& x, const fp64 y) {
	return (x.hi > y || (x.hi == y && x.lo >= static_cast<fp64>(0.0)));
}

inline constexpr bool operator==(const fp64 x, const Float64x2& y) {
	return (x == y.hi && x == y.lo);
}
inline constexpr bool operator!=(const fp64 x, const Float64x2& y) {
	return (x != y.hi || x != y.lo);
}
inline constexpr bool operator< (const fp64 x, const Float64x2& y) {
	return (x < y.hi || (x == y.hi && static_cast<fp64>(0.0) <  y.lo));
}
inline constexpr bool operator<=(const fp64 x, const Float64x2& y) {
	return (x < y.hi || (x == y.hi && static_cast<fp64>(0.0) <= y.lo));
}
inline constexpr bool operator> (const fp64 x, const Float64x2& y) {
	return (x > y.hi || (x == y.hi && static_cast<fp64>(0.0) >  y.lo));
}
inline constexpr bool operator>=(const fp64 x, const Float64x2& y) {
	return (x > y.hi || (x == y.hi && static_cast<fp64>(0.0) >= y.lo));
}

/* Compare to Zero */

/** @brief Assumes that if x.hi is zero then x.lo is also zero */
inline constexpr bool isequal_zero(const Float64x2& x) {
	return (x.hi == static_cast<fp64>(0.0));
}
/** @brief Assumes that if x.hi is zero then x.lo is also zero */
inline constexpr bool isnotequal_zero(const Float64x2& x) {
	return (x.hi != static_cast<fp64>(0.0));
}
/** @brief Assumes that if x.hi is zero then x.lo is also zero */
inline constexpr bool isless_zero(const Float64x2& x) {
	return (x.hi < static_cast<fp64>(0.0));
}
/** @brief Assumes that if x.hi is zero then x.lo is also zero */
inline constexpr bool islessequal_zero(const Float64x2& x) {
	return (x.hi <= static_cast<fp64>(0.0));
}
/** @brief Assumes that if x.hi is zero then x.lo is also zero */
inline constexpr bool isgreater_zero(const Float64x2& x) {
	return (x.hi > static_cast<fp64>(0.0));
}
/** @brief Assumes that if x.hi is zero then x.lo is also zero */
inline constexpr bool isgreaterequal_zero(const Float64x2& x) {
	return (x.hi >= static_cast<fp64>(0.0));
}

//------------------------------------------------------------------------------
// Float64x2 Dekker Split
//------------------------------------------------------------------------------

#if defined(FLOATNX2_BITWISE_SPLIT) || defined(FLOAT64X2_BITWISE_SPLIT)
	/**
	 * @brief Splits the mantissa bits of a floating point value via bitwise
	 * operations for use in the dekker_mul12 function.
	 */
	inline Float64x2 dekker_split(const fp64 x) {
		Bitwise_Float64x2 r;
		constexpr uint64_t Dekker_Split_Mask = ~((uint64_t)0x3FFFFFF);
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
	inline Float64x2 dekker_split(const fp64 x) {
		constexpr fp64 Dekker_Scale = 134217729.0; // (2^ceil(53 / 2) + 1)
		fp64 p = x * Dekker_Scale;
		Float64x2 r;
		r.hi = (x - p) + p;
		r.lo = x - r.hi;
		return r;
	}
#endif

//------------------------------------------------------------------------------
// Float64x2 Basic Arithmetic
//------------------------------------------------------------------------------

template <> inline
Float64x2 LDF::add<Float64x2, Float64x2, Float64x2>
(const Float64x2& x, const Float64x2& y) {
	fp64 r_hi = x.hi + y.hi;
	fp64 r_lo = static_cast<fp64>(0.0);
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

template <> inline
Float64x2 LDF::sub<Float64x2, Float64x2, Float64x2>
(const Float64x2& x, const Float64x2& y) {
	fp64 r_hi = x.hi - y.hi;
	fp64 r_lo = static_cast<fp64>(0.0);
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

/**
 * @brief Multiplies two fp64 values with result stored as a Float64x2
 */
template <> inline
Float64x2 LDF::mul<Float64x2, fp64, fp64>
(const fp64& x, const fp64& y) {
	Float64x2 a = dekker_split(x);
	Float64x2 b = dekker_split(y);
	fp64 p = a.hi * b.hi;
	fp64 q = a.hi * b.lo + a.lo * b.hi;

	Float64x2 r;
	r.hi = p + q;
	r.lo = p - r.hi + q + a.lo * b.lo;
	return r;
}

template <> inline
Float64x2 LDF::mul<Float64x2, Float64x2, Float64x2>
(const Float64x2& x, const Float64x2& y) {
	Float64x2 t = LDF::mul<Float64x2, fp64, fp64>(x.hi, y.hi);
	fp64 c = x.hi * y.lo + x.lo * y.hi + t.lo;

	Float64x2 r;
	r.hi = t.hi + c;
	r.lo = t.hi - r.hi + c;
	return r;
}

template <> inline
Float64x2 LDF::div<Float64x2, Float64x2, Float64x2>
(const Float64x2& x, const Float64x2& y) {
	fp64 u = x.hi / y.hi;
	Float64x2 t = LDF::mul<Float64x2, fp64, fp64>(u, y.hi);
	fp64 l = (x.hi - t.hi - t.lo + x.lo - u * y.lo) / y.hi;

	Float64x2 r;
	r.hi = u + l;
	r.lo = u - r.hi + l;
	return r;
}

/**
 * @brief Squares a fp64 value with the result stored as a Float64x2
 */
template <> inline
Float64x2 LDF::square<Float64x2, fp64>
(const fp64& x) {
	Float64x2 a = dekker_split(x);
	fp64 p = a.hi * a.hi;
	fp64 q = static_cast<fp64>(2.0) * (a.hi * a.lo);

	Float64x2 r;
	r.hi = p + q;
	r.lo = ((p - r.hi) + q) + (a.lo * a.lo);
	return r;
}

template <> inline
Float64x2 LDF::square<Float64x2, Float64x2>
(const Float64x2& x) {
	Float64x2 t = LDF::square<Float64x2, fp64>(x.hi);
	fp64 c = (static_cast<fp64>(2.0) * (x.hi * x.lo)) + t.lo;

	Float64x2 r;
	r.hi = t.hi + c;
	r.lo = (t.hi - r.hi) + c;
	return r;
}

template <> inline
Float64x2 LDF::recip<Float64x2, Float64x2>
(const Float64x2& y) {
	fp64 u = static_cast<fp64>(1.0) / y.hi;
	Float64x2 t = LDF::mul<Float64x2, fp64, fp64>(u, y.hi);
	fp64 l = (((static_cast<fp64>(1.0) - t.hi) - t.lo) - (u * y.lo)) / y.hi;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

//------------------------------------------------------------------------------
// Float64x2 optimized arithmetic
//------------------------------------------------------------------------------

template <> inline
Float64x2 LDF::add<Float64x2, Float64x2, fp64>
(const Float64x2& x, const fp64& y) {
	fp64 r_hi = x.hi + y;
	fp64 r_lo = static_cast<fp64>(0.0);
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

template <> inline
Float64x2 LDF::add<Float64x2, fp64, Float64x2>
(const fp64& x, const Float64x2& y) {
	fp64 r_hi = x + y.hi;
	fp64 r_lo = static_cast<fp64>(0.0);
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
template <> inline
Float64x2 LDF::add<Float64x2, fp64, fp64>
(const fp64& x, const fp64& y) {
	fp64 r_hi = x + y;
	fp64 r_lo = static_cast<fp64>(0.0);
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

template <> inline
Float64x2 LDF::sub<Float64x2, Float64x2, fp64>
(const Float64x2& x, const fp64& y) {
	fp64 r_hi = x.hi - y;
	fp64 r_lo = static_cast<fp64>(0.0);
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

template <> inline
Float64x2 LDF::sub<Float64x2, fp64, Float64x2>
(const fp64& x, const Float64x2& y) {
	fp64 r_hi = x - y.hi;
	fp64 r_lo = static_cast<fp64>(0.0);
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
template <> inline
Float64x2 LDF::sub<Float64x2, fp64, fp64>
(const fp64& x, const fp64& y) {
	fp64 r_hi = x - y;
	fp64 r_lo = static_cast<fp64>(0.0);
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

template <> inline
Float64x2 LDF::mul<Float64x2, Float64x2, fp64>
(const Float64x2& x, const fp64& y) {
	Float64x2 t = LDF::mul<Float64x2, fp64, fp64>(x.hi, y);
	fp64 c = (x.lo * y) + t.lo;

	Float64x2 r;
	r.hi = t.hi + c;
	r.lo = (t.hi - r.hi) + c;
	return r;
}

template <> inline
Float64x2 LDF::mul<Float64x2, fp64, Float64x2>
(const fp64& x, const Float64x2& y) {
	Float64x2 t = LDF::mul<Float64x2, fp64, fp64>(x, y.hi);
	fp64 c = (x * y.lo) + t.lo;

	Float64x2 r;
	r.hi = t.hi + c;
	r.lo = (t.hi - r.hi) + c;
	return r;
}

template <> inline
Float64x2 LDF::div<Float64x2, Float64x2, fp64>
(const Float64x2& x, const fp64& y) {
	fp64 u = x.hi / y;
	Float64x2 t = LDF::mul<Float64x2, fp64, fp64>(u, y);
	fp64 l = (((x.hi - t.hi) - t.lo) + x.lo) / y;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

template <> inline
Float64x2 LDF::div<Float64x2, fp64, Float64x2>
(const fp64& x, const Float64x2& y) {
	fp64 u = x / y.hi;
	Float64x2 t = LDF::mul<Float64x2, fp64, fp64>(u, y.hi);
	fp64 l = (((x - t.hi) - t.lo) - (u * y.lo)) / y.hi;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

/**
 * @brief Divides two fp64 values with the result stored as a Float64x2
 */
template <> inline
Float64x2 LDF::div<Float64x2, fp64, fp64>
(const fp64& x, const fp64& y) {
	fp64 u = x / y;
	Float64x2 t = LDF::mul<Float64x2, fp64, fp64>(u, y);
	fp64 l = ((x - t.hi) - t.lo) / y;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

/**
 * @brief Calculates the reciprocal of a fp64 value with the result stored
 * as a Float64x2
 */

template <> inline
Float64x2 LDF::recip<Float64x2, fp64>
(const fp64& y) {
	fp64 u = static_cast<fp64>(1.0) / y;
	Float64x2 t = LDF::mul<Float64x2, fp64, fp64>(u, y);
	fp64 l = ((static_cast<fp64>(1.0) - t.hi) - t.lo) / y;

	Float64x2 r;
	r.hi = u + l;
	r.lo = (u - r.hi) + l;
	return r;
}

//------------------------------------------------------------------------------
// Float64x2 specialized arithmetic
//------------------------------------------------------------------------------

/**
 * @brief Multiplies by a known power of two (such as 2.0, 0.5, etc.) or zero
 */
template <> inline constexpr
Float64x2 LDF::mul_pwr2<Float64x2, Float64x2, fp64>
(const Float64x2& x, const fp64& y) {
	return {x.hi * y, x.lo * y};
}

/**
 * @brief Multiplies by a known power of two (such as 2.0, 0.5, etc.) or zero
 */
template <> inline constexpr
Float64x2 LDF::mul_pwr2<Float64x2, fp64, Float64x2>
(const fp64& x, const Float64x2& y) {
	return {x * y.hi, x * y.hi};
}

/**
 * @brief Multiplies by a known power of two (such as 2.0, 0.5, etc.) or zero.
 * The result is stored as a Float64x2
 */
template <> inline constexpr
Float64x2 LDF::mul_pwr2<Float64x2, fp64, fp64>
(const fp64& x, const fp64& y) {
	return {x * y, static_cast<fp64>(0.0)};
}

//------------------------------------------------------------------------------
// Float64x2 bitwise operators
//------------------------------------------------------------------------------

template <> inline
Float64x2 LDF::bitwise_not<Float64x2>
(const Float64x2& x) {
	Float64x2 ret = x;
	uint64_t* binary_part = reinterpret_cast<uint64_t*>(&ret);
	binary_part[0] = ~binary_part[0];
	binary_part[1] = ~binary_part[1];
	return ret;
}

template <> inline
Float64x2 LDF::bitwise_and<Float64x2, Float64x2>
(const Float64x2& x, const Float64x2& y) {
	Float64x2 ret = x;
	uint64_t* ret_bin = reinterpret_cast<uint64_t*>(&ret);
	const uint64_t* y_bin = reinterpret_cast<const uint64_t*>(&y);
	ret_bin[0] &= y_bin[0];
	ret_bin[1] &= y_bin[1];
	return ret;
}

template <> inline
Float64x2 LDF::bitwise_andnot<Float64x2, Float64x2>
(const Float64x2& x, const Float64x2& y) {
	Float64x2 ret = x;
	uint64_t* ret_bin = reinterpret_cast<uint64_t*>(&ret);
	const uint64_t* y_bin = reinterpret_cast<const uint64_t*>(&y);
	ret_bin[0] &= ~y_bin[0];
	ret_bin[1] &= ~y_bin[1];
	return ret;
}

template <> inline
Float64x2 LDF::bitwise_or<Float64x2, Float64x2>
(const Float64x2& x, const Float64x2& y) {
	Float64x2 ret = x;
	uint64_t* ret_bin = reinterpret_cast<uint64_t*>(&ret);
	const uint64_t* y_bin = reinterpret_cast<const uint64_t*>(&y);
	ret_bin[0] |= y_bin[0];
	ret_bin[1] |= y_bin[1];
	return ret;
}

template <> inline
Float64x2 LDF::bitwise_xor<Float64x2, Float64x2>
(const Float64x2& x, const Float64x2& y) {
	Float64x2 ret = x;
	uint64_t* ret_bin = reinterpret_cast<uint64_t*>(&ret);
	const uint64_t* y_bin = reinterpret_cast<const uint64_t*>(&y);
	ret_bin[0] ^= y_bin[0];
	ret_bin[1] ^= y_bin[1];
	return ret;
}

template <> inline
Float64x2 LDF::bitwise_and<Float64x2, fp64>
(const Float64x2& x, const fp64& y) {
	Float64x2 ret = x;
	uint64_t* ret_bin = reinterpret_cast<uint64_t*>(&ret);
	const uint64_t* y_bin = reinterpret_cast<const uint64_t*>(&y);
	ret_bin[0] &= *y_bin;
	ret_bin[1] &= *y_bin;
	return x;
}

template <> inline
Float64x2 LDF::bitwise_andnot<Float64x2, fp64>
(const Float64x2& x, const fp64& y) {
	Float64x2 ret = x;
	uint64_t* ret_bin = reinterpret_cast<uint64_t*>(&ret);
	const uint64_t* y_bin = reinterpret_cast<const uint64_t*>(&y);
	ret_bin[0] &= ~(*y_bin);
	ret_bin[1] &= ~(*y_bin);
	return x;
}

template <> inline
Float64x2 LDF::bitwise_or<Float64x2, fp64>
(const Float64x2& x, const fp64& y) {
	Float64x2 ret = x;
	uint64_t* ret_bin = reinterpret_cast<uint64_t*>(&ret);
	const uint64_t* y_bin = reinterpret_cast<const uint64_t*>(&y);
	ret_bin[0] |= *y_bin;
	ret_bin[1] |= *y_bin;
	return x;
}

template <> inline
Float64x2 LDF::bitwise_xor<Float64x2, fp64>
(const Float64x2& x, const fp64& y) {
	Float64x2 ret = x;
	uint64_t* ret_bin = reinterpret_cast<uint64_t*>(&ret);
	const uint64_t* y_bin = reinterpret_cast<const uint64_t*>(&y);
	ret_bin[0] ^= *y_bin;
	ret_bin[1] ^= *y_bin;
	return x;
}

//------------------------------------------------------------------------------
// Float64x2 Arithmetic Operator Overloads
//------------------------------------------------------------------------------

inline constexpr Float64x2 operator-(const Float64x2& x) {
	return {-x.hi, -x.lo};
}

inline Float64x2 operator+(const Float64x2& x, const Float64x2& y) {
	return LDF::add<Float64x2, Float64x2, Float64x2>(x, y);
}
inline Float64x2 operator-(const Float64x2& x, const Float64x2& y) {
	return LDF::sub<Float64x2, Float64x2, Float64x2>(x, y);
}
inline Float64x2 operator*(const Float64x2& x, const Float64x2& y) {
	return LDF::mul<Float64x2, Float64x2, Float64x2>(x, y);
}
inline Float64x2 operator/(const Float64x2& x, const Float64x2& y) {
	return LDF::div<Float64x2, Float64x2, Float64x2>(x, y);
}

inline Float64x2 operator+(const Float64x2& x, const fp64 y) {
	return LDF::add<Float64x2, Float64x2, fp64>(x, y);
}
inline Float64x2 operator-(const Float64x2& x, const fp64 y) {
	return LDF::sub<Float64x2, Float64x2, fp64>(x, y);
}
inline Float64x2 operator*(const Float64x2& x, const fp64 y) {
	return LDF::mul<Float64x2, Float64x2, fp64>(x, y);
}
inline Float64x2 operator/(const Float64x2& x, const fp64 y) {
	return LDF::div<Float64x2, Float64x2, fp64>(x, y);
}

inline Float64x2 operator+(const fp64 x, const Float64x2& y) {
	return LDF::add<Float64x2, fp64, Float64x2>(x, y);
}
inline Float64x2 operator-(const fp64 x, const Float64x2& y) {
	return LDF::sub<Float64x2, fp64, Float64x2>(x, y);
}
inline Float64x2 operator*(const fp64 x, const Float64x2& y) {
	return LDF::mul<Float64x2, fp64, Float64x2>(x, y);
}
inline Float64x2 operator/(const fp64 x, const Float64x2& y) {
	return LDF::div<Float64x2, fp64, Float64x2>(x, y);
}

inline Float64x2 square(const Float64x2& x) {
	return LDF::square<Float64x2, Float64x2>(x);
}
inline Float64x2 recip(const Float64x2& x) {
	return LDF::recip<Float64x2, Float64x2>(x);
}

inline constexpr Float64x2 mul_pwr2(const Float64x2& x, const fp64 y) {
	return LDF::mul_pwr2<Float64x2, Float64x2, fp64>(x, y);
}
inline constexpr Float64x2 mul_pwr2(const fp64 x, const Float64x2& y) {
	return LDF::mul_pwr2<Float64x2, fp64, Float64x2>(x, y);
}

// #if __cplusplus >= 201304L
// 	#define FLOAT64X2_RELAXED_CONSTEXPR constexpr
// #else
// 	#define FLOAT64X2_RELAXED_CONSTEXPR
// #endif

/* Compound Assignment */

inline Float64x2& operator+=(Float64x2& x, const Float64x2& y) {
	x = x + y;
	return x;
}
inline Float64x2& operator-=(Float64x2& x, const Float64x2& y) {
	x = x - y;
	return x;
}
inline Float64x2& operator*=(Float64x2& x, const Float64x2& y) {
	x = x * y;
	return x;
}
inline Float64x2& operator/=(Float64x2& x, const Float64x2& y) {
	x = x / y;
	return x;
}

inline Float64x2& operator+=(Float64x2& x, const fp64 y) {
	x = x + y;
	return x;
}
inline Float64x2& operator-=(Float64x2& x, const fp64 y) {
	x = x - y;
	return x;
}
inline Float64x2& operator*=(Float64x2& x, const fp64 y) {
	x = x * y;
	return x;
}
inline Float64x2& operator/=(Float64x2& x, const fp64 y) {
	x = x / y;
	return x;
}

/* Increment/Decrement */

inline Float64x2& operator++(Float64x2& x) {
	x += static_cast<fp64>(1.0);
	return x;
}

inline Float64x2& operator--(Float64x2& x) {
	x -= static_cast<fp64>(1.0);
	return x;
}

inline Float64x2 operator++(Float64x2& x, int) {
	Float64x2 temp = x;
	x += static_cast<fp64>(1.0);
	return temp;
}

inline Float64x2 operator--(Float64x2& x, int) {
	Float64x2 temp = x;
	x -= static_cast<fp64>(1.0);
	return temp;
}

//------------------------------------------------------------------------------
// Float64x2 Limits
//------------------------------------------------------------------------------

namespace std {
	template <>
	class numeric_limits<Float64x2> {
	public:
		static constexpr bool is_signed = true;
		static constexpr bool is_integer = false;
		static constexpr bool is_exact = false;
		static constexpr bool has_infinity = std::numeric_limits<fp64>::has_infinity;
		static constexpr bool has_quiet_NaN = std::numeric_limits<fp64>::has_quiet_NaN;
		static constexpr bool has_signaling_NaN = std::numeric_limits<fp64>::has_signaling_NaN;
		static constexpr std::float_denorm_style has_denorm = std::numeric_limits<fp64>::has_denorm;
		static constexpr bool is_bounded = true;
		static constexpr int digits = 106; // 2 * (52 mantissa bits + 1 implicit bit)
		#if __cplusplus >= 201103L
		static constexpr int digits10 = 31; // floor(mantissa bits * log10(2))
		#endif
		/**
		 * @brief Dekker floats can represent FLT_MAX + FLT_MIN exactly, which
		 * is why an absurd amount of digits may be required.
		 */
		static constexpr int max_digits10 =
			std::numeric_limits<fp64>::max_exponent10 - std::numeric_limits<fp64>::min_exponent10 + std::numeric_limits<fp64>::digits10 + 1;
		static constexpr int radix = std::numeric_limits<fp64>::radix;
		static constexpr int min_exponent   = std::numeric_limits<fp64>::min_exponent   + 53;
		static constexpr int min_exponent10 = std::numeric_limits<fp64>::min_exponent10 + 16 /* ceil(53 * log10(2)) */;
		static constexpr int max_exponent   = std::numeric_limits<fp64>::max_exponent  ;
		static constexpr int max_exponent10 = std::numeric_limits<fp64>::max_exponent10;
		static constexpr bool traps = std::numeric_limits<fp64>::traps;
		inline static constexpr Float64x2 min() {
			/** 
			 * @remarks libQD lists this as the minimum value that is fully
			 * normalized, although I am not sure if this is the best
			 * definition to use for min()
			 */
			return {std::numeric_limits<fp64>::min() * static_cast<fp64>(0x1.0p+53), std::numeric_limits<fp64>::min()};
			// return {std::numeric_limits<fp64>::min(), static_cast<fp64>(0.0)};
		}
	
		inline static constexpr Float64x2 max() {
			return {
				std::numeric_limits<fp64>::max(),
				std::numeric_limits<fp64>::max() * static_cast<fp64>(0x1.0p-54)
			};
		}
		#if __cplusplus >= 201103L
		inline static constexpr Float64x2 lowest() { return -max(); }
		#endif
		/** @brief `2^-105 == 2^-52 * 2^-53` */
		inline static constexpr Float64x2 epsilon() {
			return {
				static_cast<fp64>(0x1.0p-105),
				static_cast<fp64>(0.0)
			};
		}
		inline static constexpr Float64x2 round_error() {
			return {
				static_cast<fp64>(0.5),
				static_cast<fp64>(0.0)
			};
		}
		inline static constexpr Float64x2 infinity() {
			return {
				std::numeric_limits<fp64>::infinity(),
				static_cast<fp64>(0.0)
			};
		}
		inline static constexpr Float64x2 quiet_NaN() {
			return {
				std::numeric_limits<fp64>::quiet_NaN(),
				static_cast<fp64>(0.0)
			};
		}
		inline static constexpr Float64x2 signaling_NaN() {
			return {
				std::numeric_limits<fp64>::signaling_NaN(),
				static_cast<fp64>(0.0)
			};
		}
		inline static constexpr Float64x2 denorm_min() {
			return {
				std::numeric_limits<fp64>::denorm_min(),
				static_cast<fp64>(0.0)
			};
		}
	};
}

//------------------------------------------------------------------------------
// Float64x2 Constants
//------------------------------------------------------------------------------

#include "Float64x2_constants.hpp"

#if __cplusplus >= 201907L
#include <numbers>
namespace std {
	namespace numbers {
		template<> inline constexpr Float64x2 e_v          <Float64x2> = LDF::const_e         <Float64x2>(); /**< ~2.718281828 */
		template<> inline constexpr Float64x2 log2e_v      <Float64x2> = LDF::const_log2e     <Float64x2>(); /**< ~1.442695041 */
		template<> inline constexpr Float64x2 log10e_v     <Float64x2> = LDF::const_log10e    <Float64x2>(); /**< ~0.434294482 */
		template<> inline constexpr Float64x2 pi_v         <Float64x2> = LDF::const_pi        <Float64x2>(); /**< ~3.141592654 */
		template<> inline constexpr Float64x2 inv_pi_v     <Float64x2> = LDF::const_inv_pi    <Float64x2>(); /**< ~0.318309886 */
		template<> inline constexpr Float64x2 inv_sqrtpi_v <Float64x2> = LDF::const_inv_sqrtpi<Float64x2>(); /**< ~0.564189584 */
		template<> inline constexpr Float64x2 ln2_v        <Float64x2> = LDF::const_ln2       <Float64x2>(); /**< ~0.693147181 */
		template<> inline constexpr Float64x2 ln10_v       <Float64x2> = LDF::const_ln10      <Float64x2>(); /**< ~2.302585093 */
		template<> inline constexpr Float64x2 sqrt2_v      <Float64x2> = LDF::const_sqrt2     <Float64x2>(); /**< ~1.414213562 */
		template<> inline constexpr Float64x2 sqrt3_v      <Float64x2> = LDF::const_sqrt3     <Float64x2>(); /**< ~1.732050808 */
		template<> inline constexpr Float64x2 inv_sqrt3_v  <Float64x2> = LDF::const_inv_sqrt3 <Float64x2>(); /**< ~0.577350269 */
		template<> inline constexpr Float64x2 egamma_v     <Float64x2> = LDF::const_egamma    <Float64x2>(); /**< ~0.577215665 */
		template<> inline constexpr Float64x2 phi_v        <Float64x2> = LDF::const_phi       <Float64x2>(); /**< ~1.618033989 */
	}
}
#endif

//------------------------------------------------------------------------------
// Float64x2 Math Functions
//------------------------------------------------------------------------------

/* Floating Point Classify */

	/** @brief Returns true if x is negative */
	inline constexpr bool signbit(const Float64x2& x) {
		return std::signbit(x.hi);
	}

	/** @brief Returns true if x is finite */
	inline constexpr bool isfinite(const Float64x2& x) {
		return std::isfinite(x.hi);
	}

	/** @brief Returns true if x is +-infinity */
	inline constexpr bool isinf(const Float64x2& x) {
		return std::isinf(x.hi);
	}

	/** @brief Returns true if x is any kind of NaN */
	inline constexpr bool isnan(const Float64x2& x) {
		return std::isnan(x.hi);
	}

	/** @brief Returns true if x is normal */
	inline constexpr bool isnormal(const Float64x2& x) {
		return (
			std::isnormal(x.hi) &&
			(std::isnormal(x.lo) || x.lo == static_cast<fp64>(0.0))
		);
	}
	
	/** @brief Returns true if x and y are unordered */
	inline constexpr bool isunordered(const Float64x2& x, const Float64x2& y) {
		return std::isunordered(x.hi, y.hi);
	}

	/** @brief Returns true if x and y are unordered */
	inline constexpr bool isunordered(
		const Float64x2& x, const Float64x2& y, const Float64x2& z
	) {
		return std::isunordered(std::isunordered(x.hi, y.hi), z.hi);
	}

	inline constexpr int fpclassify(const Float64x2& x) {
		return
			isinf(x)             ? FP_INFINITE :
			isnan(x)             ? FP_NAN      :
			isequal_zero(x)      ? FP_ZERO     :
			isnormal(x)          ? FP_NORMAL   :
			FP_SUBNORMAL;
	}

/* Comparison */

	inline constexpr bool isless(const Float64x2& x, const Float64x2& y) {
		return (std::isless(x.hi, y.hi) || (x.hi == y.hi && std::isless(x.lo, y.lo)));
	}
	inline constexpr bool islessequal(const Float64x2& x, const Float64x2& y) {
		return (std::isless(x.hi, y.hi) || (x.hi == y.hi && std::islessequal(x.lo, y.lo)));
	}
	inline constexpr bool isgreater(const Float64x2& x, const Float64x2& y) {
		return (std::isgreater(x.hi, y.hi) || (x.hi == y.hi && std::isgreater(x.lo, y.lo)));
	}
	inline constexpr bool isgreaterequal(const Float64x2& x, const Float64x2& y) {
		return (std::isgreater(x.hi, y.hi) || (x.hi == y.hi && std::isgreaterequal(x.lo, y.lo)));
	}
	inline constexpr bool islessgreater(const Float64x2& x, const Float64x2& y) {
		return (std::islessgreater(x.hi, y.hi) || std::islessgreater(x.lo, y.lo));
	}

/* fmax and fmin */

	/**
	 * @brief Returns the fmax of x and y. Correctly handling NaN and signed zeros.
	 * You may use std::max as a faster alternative.
	 */
	inline constexpr Float64x2 fmax(const Float64x2& x, const Float64x2& y) {
		return
			isless(x, y) ? y :
			isless(y, x) ? x :
			isnan(x) ? y :
			isnan(y) ? x :
			signbit(x) ? y : x;
	}

	/**
	 * @brief Returns the fmin of x and y. Correctly handling NaN and signed zeros.
	 * You may use std::min as a faster alternative.
	 */
	inline constexpr Float64x2 fmin(const Float64x2& x, const Float64x2& y) {
		return
			isgreater(x, y) ? y :
			isgreater(y, x) ? x :
			isnan(x) ? y :
			isnan(y) ? x :
			signbit(x) ? x : y;
	}

	inline constexpr Float64x2 fmax(const Float64x2& x, const Float64x2& y, const Float64x2& z) {
		return fmax(fmax(x, y), z);
	}
	inline constexpr Float64x2 fmin(const Float64x2& x, const Float64x2& y, const Float64x2& z) {
		return fmin(fmin(x, y), z);
	}

/* Arithmetic */

	inline constexpr Float64x2 fabs(const Float64x2& x) {
		return signbit(x) ? -x : x;
	}
	inline Float64x2 fdim(const Float64x2& x, const Float64x2& y) {
		return islessequal(x, y) ? static_cast<Float64x2>(0.0) : (x - y);
	}

	/**
	 * @brief `(x * y) + z` Performs a Fused-Multiply-Add operation, avoiding
	 * rounding errors.
	 * @note Uses Float64x4 for calculations, which may cause this function to
	 * run slowly.
	 */
	Float64x2 fma(const Float64x2& x, const Float64x2& y, const Float64x2& z);

	inline constexpr Float64x2 copysign(const Float64x2& x, const Float64x2& y) {
		return (signbit(x) != signbit(y)) ? -x : x;
	}
	inline Float64x2 sqrt(const Float64x2& x) {
		if (isless_zero(x)) {
			std::feraiseexcept(FE_INVALID);
			return std::numeric_limits<Float64x2>::quiet_NaN();
		}
		if (isequal_zero(x)) {
			return x;
		}
		fp64 guess = sqrt(x.hi);
		return mul_pwr2((guess + x / guess), static_cast<fp64>(0.5));
	}
	inline Float64x2 cbrt(const Float64x2& x) {
		if (isequal_zero(x)) {
			return x;
		}
		fp64 guess = cbrt(x.hi);
		return (
			(static_cast<fp64>(2.0) * guess) + (x / LDF::square<Float64x2>(guess))
		) / static_cast<fp64>(3.0);
	}

	#if 1
	
	/**
	 * @brief Calculates sqrt(x^2 + y^2) without overflows/underflows.
	 * The lower part of the Float64x2 may be rounded to zero when handling the
	 * overflow case.
	 */
	Float64x2 hypot(const Float64x2& x, const Float64x2& y);

	/**
	 * @brief Calculates sqrt(x^2 + y^2 + z^2) without overflows/underflows.
	 * The lower part of the Float64x2 may be rounded to zero when handling the
	 * overflow case.
	 */
	Float64x2 hypot(const Float64x2& x, const Float64x2& y, const Float64x2& z);
	
	#else

	/** @note Naive implementation of hypot, may overflow for large inputs */
	inline Float64x2 hypot(const Float64x2& x, const Float64x2& y) {
		return sqrt(
			square(x) + square(y)
		);
	}
	/** @note Naive implementation of hypot, may overflow for large inputs */
	inline Float64x2 hypot(const Float64x2& x, const Float64x2& y, const Float64x2& z) {
		return sqrt(
			square(x) + square(y) + square(z)
		);
	}
	#endif

	/**
	 * @brief Returns the next representable value after `x` in the direction
	 * of `y`. If `x == y`, `x` is returned.
	 */
	inline Float64x2 nextafter(const Float64x2& x, const Float64x2& y) {
		if (x == y) {
			return x;
		}
		if (isunordered(x, y)) {
			return isnan(x) ? x : y;
		}
		Float64x2 ret = x;
		if (isless(x, y)) {
			ret.lo = nextafter(x.lo, std::numeric_limits<fp64>::infinity());
		} else {
			ret.lo = nextafter(x.lo, -std::numeric_limits<fp64>::infinity());
		}
		ret += static_cast<fp64>(0.0); // renormilize
		return ret;
	}

	/**
	 * @brief Returns the next representable value after `x` in the direction
	 * of `y`. If `x == y`, `x` is returned.
	 * @note `long double` may or may not have greater precision than Float64x2
	 * depending on the platform, this may lead to unexpected results. Prefer
	 * using nextafter instead.
	 */
	inline Float64x2 nexttoward(const Float64x2& x, const long double y) {
		if (x == static_cast<Float64x2>(y) && static_cast<long double>(x) == y) {
			return x;
		}
		if (isnan(x)) {
			return x;
		}
		if (isnan(y)) {
			return std::numeric_limits<Float64x2>::quiet_NaN();
		}
		Float64x2 ret = x;
		if (x < static_cast<Float64x2>(y) || static_cast<long double>(x) < y) {
			ret.lo = nextafter(x.lo, std::numeric_limits<fp64>::infinity());
		} else {
			ret.lo = nextafter(x.lo, -std::numeric_limits<fp64>::infinity());
		}
		ret += static_cast<fp64>(0.0); // renormilize
		return ret;
	}

/* Trigonometry */

	Float64x2 sin(const Float64x2& x);
	Float64x2 cos(const Float64x2& x);

	/**
	 * @brief Simultaneously calculates `sin(x)` and `cos(x)`. Faster than
	 * calculating `sin(x)` and `cos(x)` individually.
	 */
	void sincos(const Float64x2& x, Float64x2& p_sin , Float64x2& p_cos );

	/**
	 * @brief Calculates `tan(x)` to Float64x2 precision.
	 * @remarks Calls `sincos` and does `sin / cos` to calculate `tan(x)`
	 */
	inline Float64x2 tan(const Float64x2& x) {
		Float64x2 sin_val, cos_val;
		sincos(x, sin_val, cos_val);
		return sin_val / cos_val;
		// Float64x4 ret = static_cast<Float64x4>(x);
		// return static_cast<Float64x2>(tan(ret));
	}

	Float64x2 asin (const Float64x2& x);
	Float64x2 acos (const Float64x2& x);
	Float64x2 atan (const Float64x2& x);
	Float64x2  sinh(const Float64x2& x);
	Float64x2  cosh(const Float64x2& x);

	/**
	 * @brief Simultaneously calculates `sinh(x)` and `cosh(x)`. Faster than
	 * calculating `sinh(x)` and `cosh(x)` individually.
	 */
	void sinhcosh(const Float64x2& x, Float64x2& p_sinh, Float64x2& p_cosh);
	Float64x2  tanh(const Float64x2& x);
	Float64x2 asinh(const Float64x2& x);
	Float64x2 acosh(const Float64x2& x);
	Float64x2 atanh(const Float64x2& x);

	/**
	 * @brief Calculates double-argument `arctan`
	 * @note atan2(y, 1.0) == atan(y)
	 */
	Float64x2 atan2(const Float64x2& y, const Float64x2& x);

/* Special Trigonometry */

	/** @brief Calculates sin(pi * x) */
	Float64x2 sinpi(const Float64x2& x);

	/** @brief Calculates cos(pi * x) */
	Float64x2 cospi(const Float64x2& x);

	/** @brief Calculates tan(pi * x) */
	Float64x2 tanpi(const Float64x2& x);

	/** @brief Calculates 1/pi * asin(x) */
	Float64x2 asinpi(const Float64x2& x);

	/** @brief Calculates 1/pi * acos(x) */
	Float64x2 acospi(const Float64x2& x);

	/** @brief Calculates 1/pi * atan(x) */
	Float64x2 atanpi(const Float64x2& x);

	/** @brief Calculates 1/pi * atan2(y, x) */
	Float64x2 atan2pi(const Float64x2& y, const Float64x2& x);

/* Logarithms and Exponents */

	Float64x2 log(const Float64x2& x);
	
	/**
	 * @brief Calculates `log(x + 1.0)` without losing precision when x is
	 * close to zero.
	 * @note Accurate to at least 94bits for all inputs. Uses log(x + 1.0) when
	 * |x| > 2^-10
	 */
	Float64x2 log1p(const Float64x2& x);

	/** @note Naive implementation of log2(x) */
	inline Float64x2 log2(const Float64x2& x) {
		return log(x) * LDF::const_log2e<Float64x2>();
	}

	/** @note Naive implementation of log10(x) */
	inline Float64x2 log10(const Float64x2& x) {
		return log(x) * LDF::const_log10e<Float64x2>();
	}

	/**
	 * @brief Calculates log(|x|) in base FLT_RADIX
	 * @note Naive implementation of logb(x)
	 */
	inline Float64x2 logb(const Float64x2& x) {
		return log(fabs(x)) / log(static_cast<Float64x2>(std::numeric_limits<fp64>::radix));
	}

	Float64x2 exp(const Float64x2& x);

	/**
	 * @brief Calculates `exp(x) - 1.0` without losing precision when x is
	 * close to zero.
	 * @note Uses a naive implementation of `expm1(x)` when `|x| > 0.5 * ln(2)`
	 */
	Float64x2 expm1(const Float64x2& x);

	/** @note Naive implementation of exp2(x). Calls `exp(x * ln(2))` */
	inline Float64x2 exp2(const Float64x2& x) {
		return exp(x * LDF::const_ln2<Float64x2>());
	}

	/** @note Naive implementation of exp10(x). Calls `exp(x * ln(10))` */
	inline Float64x2 exp10(const Float64x2& x) {
		return exp(x * LDF::const_ln10<Float64x2>());
	}

/* Power functions */

	/**
	 * @brief `x ^ y` Calculates `x` raised to the power of `y` handling
	 * special values such as `zero ^ zero`, `zero ^ -inf`, `+1.0 ^ NaN`, and etc.
	 */
	Float64x2 pow(const Float64x2& x, const Float64x2& y);

	/**
	 * @brief `x ^ y` Calculates `x` raised to the power of `y` handling
	 * special values such as `zero ^ zero`, `zero ^ -inf`, `+1.0 ^ NaN`, and etc.
	 */
	inline Float64x2 pow(const Float64x2& x, const fp64 y) {
		return pow(x, static_cast<Float64x2>(y));
	}

	/**
	 * @brief `x ^ y` Calculates `x` raised to the power of `y` handling
	 * special values such as `zero ^ zero`, `zero ^ -inf`, `+1.0 ^ NaN`, and etc.
	 */
	inline Float64x2 pow(const fp64 x, const Float64x2& y) {
		return pow(static_cast<Float64x2>(x), y);
	}

	/**
	 * @brief `x ^ y` The ieee-754 2008 `powr` function is the same as `pow`
	 * except it does not handle special values such as `zero ^ zero`,
	 * `zero ^ -inf`, `+1.0 ^ NaN`, and etc.
	 */
	inline Float64x2 powr(const Float64x2& x, const Float64x2& y) {
		return exp(y * log(x));
	}

	/**
	 * @brief `x ^ y` The ieee-754 2008 `powr` function is the same as `pow`
	 * except it does not handle special values such as `zero ^ zero`,
	 * `zero ^ -inf`, `+1.0 ^ NaN`, and etc.
	 */
	inline Float64x2 powr(const Float64x2& x, const fp64 y) {
		return exp(y * log(x));
	}
	
	/**
	 * @brief `x ^ y` The ieee-754 2008 `powr` function is the same as `pow`
	 * except it does not handle special values such as `zero ^ zero`,
	 * `zero ^ -inf`, `+1.0 ^ NaN`, and etc.
	 */
	inline Float64x2 powr(const fp64 x, const Float64x2& y) {
		return exp(y * log(static_cast<Float64x2>(x)));
	}
	
	/**
	 * @brief `x ^ n` The ieee-754 2008 `pown` function raises `x` to the power
	 * of an integer in `O(log(n))` time
	 */
	Float64x2 pown(const Float64x2& x, int n);

	/**
	 * @brief `x ^ 1/n` The ieee-754 2008 `rootn` function calculates 
	 * the integer root of `x`
	 * @note Naive implementation of `rootn(x, n)`, calculates
	 * `pow(x, 1 / n)` when `|n| > 3` or `pow(x, +inf)` when `n == 0`
	 */
	Float64x2 rootn(const Float64x2& x, int n);
	
/* Rounding */

	inline Float64x2 trunc(const Float64x2& x) {
		Float64x2 int_hi = trunc(x.hi);
		Float64x2 int_lo = trunc(x.lo);
		fp64 frac_hi = x.hi - int_hi.hi;
		fp64 frac_lo = x.lo - int_lo.hi;
		// Sum in increasing order
		Float64x2 trunc_all = (
			LDF::add<Float64x2>(frac_hi, frac_lo) >= static_cast<fp64>(1.0)
		) ? static_cast<Float64x2>(1.0) : static_cast<Float64x2>(0.0);
		trunc_all += int_lo;
		trunc_all += int_hi;
		return trunc_all;
	}
	inline Float64x2 floor(const Float64x2& x) {
		Float64x2 int_part = trunc(x);
		return (
			isless_zero(x) && int_part != x
		) ? int_part - static_cast<fp64>(1.0) : int_part;
	}
	inline Float64x2 ceil(const Float64x2& x) {
		Float64x2 int_part = trunc(x);
		return (
			isgreater_zero(x) && int_part != x
		) ? int_part + static_cast<fp64>(1.0) : int_part;
	}
	inline Float64x2 round(const Float64x2& x) {
		Float64x2 int_part = trunc(x);
		Float64x2 frac_part = x - int_part;
		if (isgreaterequal_zero(x)) {
			if (frac_part >= static_cast<fp64>(0.5)) {
				return int_part + static_cast<fp64>(1.0);
			}
			return int_part;
		}
		if (frac_part <= static_cast<fp64>(-0.5)) {
			return int_part - static_cast<fp64>(1.0);
		}
		return int_part;
	}
	inline Float64x2 rint(const Float64x2& x) {
		switch (fegetround()) {
			default:
			case FE_TOWARDZERO:
				return trunc(x);
			case FE_DOWNWARD:
				return floor(x);
			case FE_UPWARD:
				return ceil(x);
			case FE_TONEAREST:
				return round(x);
		}
	}
	inline long lround(const Float64x2& x) {
		return static_cast<long>(round(x));
	}
	inline long lrint(const Float64x2& x) {
		return static_cast<long>(rint(x));
	}
	inline long long llround(const Float64x2& x) {
		return static_cast<long long>(round(x));
	}
	inline long long llrint(const Float64x2& x) {
		return static_cast<long long>(rint(x));
	}

/* Integer and Remainder */

	/**
	 * @brief Returns the remainder of x / y (rounded towards zero)
	 * @note Naive implementation of fmod
	 */
	inline Float64x2 fmod(const Float64x2& x, const Float64x2& y) {
		Float64x2 trunc_part = trunc(x / y);
		return x - y * trunc_part;
	}

	/**
	 * @brief Extracts the fractional and integer part of a Float64x2 value.
	 * @note Naive implementation of modf
	 */
	inline Float64x2 modf(const Float64x2& x, Float64x2& int_part) {
		Float64x2 trunc_part = trunc(x);
		int_part = trunc_part;
		return x - trunc_part;
	}
	inline Float64x2 nearbyint(const Float64x2& x) {
		return rint(x);
	}

	/**
	 * @brief Returns the remainder of x / y (rounded to nearest)
	 */
	inline Float64x2 remainder(const Float64x2& x, const Float64x2& y) {
		Float64x2 round_part = round(x / y);
		return x - y * round_part;
	}

	/**
	 * @brief Returns the remainder of x / y (rounded to nearest) in addition
	 * to returning the quotient value used.
	 */
	inline Float64x2 remquo(const Float64x2& x, const Float64x2& y, int& quo) {
		Float64x2 q = round(x / y);
		Float64x2 r = x - y * q;
		quo = static_cast<int>(q.hi + q.lo);
		return r;
	}

/* Float Exponents */

	/**
	 * @brief Extracts the exponent of a Float64x2 value to compute the
	 * binary logarithm.
	 */
	inline int ilogb(const Float64x2& x) {
		return ilogb(x.hi);
	}

	/**
	 * @brief Returns a normalized Float64x2 value and the exponent in
	 * the form [0.5, 1.0) * 2^expon
	 */
	inline Float64x2 frexp(const Float64x2& x, int* expon) {
		Float64x2 ret = {
			frexp(x.hi, expon),
			static_cast<fp64>(0.0)
		};
		if (isfinite(x)) {
			ret.lo = ldexp(x.lo, -(*expon));
		}
		return ret;
	}

	/**
	 * @brief Multiplies a Float64x2 value by 2^expon
	 */
	inline Float64x2 ldexp(const Float64x2& x, int expon) {
		Float64x2 ret = {
			ldexp(x.hi, expon),
			static_cast<fp64>(0.0)
		};
		if (isfinite(x)) {
			ret.lo = ldexp(x.lo, expon);
		}
		return ret;
	}

	/**
	 * @brief Multiplies a Float64x2 value by FLT_RADIX^expon
	 */
	inline Float64x2 scalbn(const Float64x2& x, int expon) {
		Float64x2 ret = {
			scalbn(x.hi, expon),
			static_cast<fp64>(0.0)
		};
		if (isfinite(x)) {
			ret.lo = scalbn(x.lo, expon);
		}
		return ret;
	}

	/**
	 * @brief Multiplies a Float64x2 value by FLT_RADIX^expon
	 */
	inline Float64x2 scalbln(const Float64x2& x, long expon) {
		Float64x2 ret = {
			scalbln(x.hi, expon),
			static_cast<fp64>(0.0)
		};
		if (isfinite(x)) {
			ret.lo = scalbln(x.lo, expon);
		}
		return ret;
	}

/* C99 <math.h> Transcendental Functions */

	Float64x2 erf(const Float64x2& x);

	/**
	 * @brief Calculates `1 - erf(x)` without losing precision when `x > 0`.
	 * @note Result will round to zero when `x > ~27.226017`
	 */
	Float64x2 erfc(const Float64x2& x);

	Float64x2 tgamma(const Float64x2& x);

	/** 
	 * @note naive implementation of lgamma(x)
	 */
	inline Float64x2 lgamma(const Float64x2& x) {
		return log(fabs(tgamma(x)));
	}

/* Mathematical Special Functions */

	/**
	 * @brief calls `(tgamma(x) * tgamma(y)) / tgamma(x + y)`
	 * @note naive implementation of beta(x, y)
	 */
	inline Float64x2 beta(const Float64x2& x, const Float64x2& y) {
		return (tgamma(x) * tgamma(y)) / tgamma(x + y);
	}

	/** @brief Calls `-incgamma(0.0, -x)` */
	Float64x2 expint(const Float64x2& x);

	Float64x2 riemann_zeta(const Float64x2& x);

/* Bessel Functions */

	/** @brief regular modified cylindrical Bessel function */
	Float64x2 cyl_bessel_i(const Float64x2& nu, const Float64x2& x);

	/** @brief cylindrical Bessel functions (of the first kind) */
	Float64x2 cyl_bessel_j(const Float64x2& nu, const Float64x2& x);

	/** @brief irregular modified cylindrical Bessel functions  */
	Float64x2 cyl_bessel_k(const Float64x2& nu, const Float64x2& x);

	/** @brief Bessel function of the second kind. */
	Float64x2 cyl_neumann(const Float64x2& nu, const Float64x2& x);


	/** @brief regular modified cylindrical Bessel function */
	Float64x2 cyl_bessel_i(int nu, const Float64x2& x);

	/** @brief cylindrical Bessel functions (of the first kind) */
	Float64x2 cyl_bessel_j(int nu, const Float64x2& x);

	/** @brief irregular modified cylindrical Bessel functions  */
	Float64x2 cyl_bessel_k(int nu, const Float64x2& x);

	/** @brief Bessel function of the second kind. */
	Float64x2 cyl_neumann(int nu, const Float64x2& x);

	/**
	 * @brief spherical Bessel function of the first kind of n and x.
	 * Calls `sqrt(pi / 2x) * cyl_bessel_j(n + 0.5, x)`
	 * @note naive implementation of sph_bessel(n, x)
	 */
	inline Float64x2 sph_bessel(unsigned int n, const Float64x2& x) {
		return sqrt(LDF::const_pi2<Float64x2>() / x) * cyl_bessel_j(
			static_cast<Float64x2>(n) + static_cast<fp64>(0.5), x
		);
	}

	/**
	 * @brief spherical Bessel function of the second kind.
	 * Calls `sqrt(pi / 2x) * cyl_neumann(n + 0.5, x)`
	 * @note naive implementation of sph_neumann(n, x)
	 */
	inline Float64x2 sph_neumann(unsigned int n, const Float64x2& x) {
		return sqrt(LDF::const_pi2<Float64x2>() / x) * cyl_neumann(
			static_cast<Float64x2>(n) + static_cast<fp64>(0.5), x
		);
	}

/* Additional Functions */

	/**
	 * @brief Calulates the incomplete gamma function.
	 * @warning The name of this function may change in the future.
	 */
	Float64x2 incgamma(const Float64x2& s, const Float64x2& z);

#endif /* FLOAT64X2_HPP */
