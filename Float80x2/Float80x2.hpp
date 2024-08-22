/*
**	Author: zerico2005 (2024)
**	Project: LIB-Dekker-Float
**	License: MIT License
**	A copy of the MIT License should be included with
**	this project. If not, see https://opensource.org/license/MIT
*/
#ifndef FLOAT80X2_HPP
#define FLOAT80X2_HPP

#include <cstdint>
#include <math.h>
#include <cmath>
#include <cfenv>

typedef float fp32;
typedef double fp64;

#include "Float80.hpp"

// Can be changed to other types for better accuracy
typedef fp80 fp80x2_Math;

/**
 * @brief Double-Float80 Dekker Float implementation.
 * Source: Creel "Double it Like Dekker" on YouTube.
 */
struct Float80x2 {
	
	fp80 hi;
	fp80 lo;

	/* Arithmetic */

	static inline Float80x2 Dekker_Add(
		const Float80x2& x, const Float80x2& y
	) {
		fp80 r_hi = x.hi + y.hi;
		fp80 r_lo = static_cast<fp80>(0.0);
		if (fabsl(x.hi) > fabsl(y.hi)) {
			r_lo = x.hi - r_hi + y.hi + y.lo + x.lo;
		} else {
			r_lo = y.hi - r_hi + x.hi + x.lo + y.lo;
		}

		Float80x2 c;
		c.hi = r_hi + r_lo;
		c.lo = r_hi - c.hi + r_lo;
		return c;
	}

	static inline Float80x2 Dekker_Sub(
		const Float80x2& x, const Float80x2& y
	) {
		fp80 r_hi = x.hi - y.hi;
		fp80 r_lo = static_cast<fp80>(0.0);
		if (fabsl(x.hi) > fabsl(y.hi)) {
			r_lo = x.hi - r_hi - y.hi - y.lo + x.lo;
		} else {
			r_lo = -y.hi - r_hi + x.hi + x.lo - y.lo;
		}

		Float80x2 c;
		c.hi = r_hi + r_lo;
		c.lo = r_hi - c.hi + r_lo;
		return c;
	}

	static constexpr fp80 Dekker_Scale = 4294967297.0; // (2^ceil(64 / 2) + 1)
	
	static inline Float80x2 Dekker_Split(const fp80& x) {
		fp80 p = x * Dekker_Scale;
		Float80x2 r;
		r.hi = (x - p) + p;
		r.lo = x - r.hi;
		return r;
	}
	
	// static constexpr uint64_t Dekker_Split_Mask = ~((uint64_t)0xFFFFFFFF);

	// inline Float80x2 Dekker_Split(const fp80& x) const {
	// 	Float80x2 r;
	// 	r.hi = x;
	// 	uint32_t* bit_mask = (uint32_t*)((void*)&r.hi);
	// 	*bit_mask = 0x00000000;
	// 	r.lo = x - r.hi;
	// 	return r;
	// }

	static inline Float80x2 Dekker_Mul12(
		const fp80& x, const fp80& y
	) {
		Float80x2 a = Dekker_Split(x);
		Float80x2 b = Dekker_Split(y);
		fp80 p = a.hi * b.hi;
		fp80 q = a.hi * b.lo + a.lo * b.hi;

		Float80x2 r;
		r.hi = p + q;
		r.lo = p - r.hi + q + a.lo * b.lo;
		return r;
	}

	static inline Float80x2 Dekker_Mul(
		const Float80x2& x, const Float80x2& y
	) {
		Float80x2 t = Dekker_Mul12(x.hi, y.hi);
		fp80 c = x.hi * y.lo + x.lo * y.hi + t.lo;

		Float80x2 r;
		r.hi = t.hi + c;
		r.lo = t.hi - r.hi + c;
		return r;
	}

	static inline Float80x2 Dekker_Div(
		const Float80x2& x, const Float80x2& y
	) {
		Float80x2 u;
		u.hi = x.hi / y.hi;
		Float80x2 t = Dekker_Mul12(u.hi, y.hi);
		fp80 l = (x.hi - t.hi - t.lo + x.lo - u.hi * y.lo) / y.hi;

		Float80x2 r;
		r.hi = u.hi + l;
		r.lo = u.hi - r.hi + l;
		return r;
	}

	static inline Float80x2 Dekker_Sqr12(
		const fp80& x
	) {
		Float80x2 a = Dekker_Split(x);
		fp80 p = a.hi * a.hi;
		fp80 q = static_cast<fp80>(2.0) * a.hi * a.lo;

		Float80x2 r;
		r.hi = p + q;
		r.lo = p - r.hi + q + a.lo * a.lo;
		return r;
	}

	static inline Float80x2 Dekker_Sqr(
		const Float80x2& x
	) {
		Float80x2 t = Dekker_Sqr12(x.hi);
		fp80 c = static_cast<fp80>(2.0) * x.hi * x.lo + t.lo;

		Float80x2 r;
		r.hi = t.hi + c;
		r.lo = t.hi - r.hi + c;
		return r;
	}

	/* Double-Single Arithmetic */

	static inline Float80x2 Dekker_Add_Float80(
		const Float80x2& x, const fp80& y
	) {
		fp80 r_hi = x.hi + y;
		fp80 r_lo = static_cast<fp80>(0.0);
		if (fabsl(x.hi) > fabsl(y)) {
			r_lo = x.hi - r_hi + y + x.lo;
		} else {
			r_lo = y - r_hi + x.hi + x.lo;
		}

		Float80x2 c;
		c.hi = r_hi + r_lo;
		c.lo = r_hi - c.hi + r_lo;
		return c;
	}

	static inline Float80x2 Dekker_Sub_Float80(
		const Float80x2& x, const fp80& y
	) {
		fp80 r_hi = x.hi - y;
		fp80 r_lo = static_cast<fp80>(0.0);
		if (fabsl(x.hi) > fabsl(y)) {
			r_lo = x.hi - r_hi - y + x.lo;
		} else {
			r_lo = -y - r_hi + x.hi + x.lo;
		}

		Float80x2 c;
		c.hi = r_hi + r_lo;
		c.lo = r_hi - c.hi + r_lo;
		return c;
	}

	static inline Float80x2 Dekker_Mul_Float80(
		const Float80x2& x, const fp80& y
	) {
		Float80x2 t = Dekker_Mul12(x.hi, y);
		fp80 c = x.lo * y + t.lo;

		Float80x2 r;
		r.hi = t.hi + c;
		r.lo = t.hi - r.hi + c;
		return r;
	}

	static inline Float80x2 Dekker_Div_Float80(
		const Float80x2& x, const fp80& y
	) {
		Float80x2 u;
		u.hi = x.hi / y;
		Float80x2 t = Dekker_Mul12(u.hi, y);
		fp80 l = (x.hi - t.hi - t.lo + x.lo) / y;

		Float80x2 r;
		r.hi = u.hi + l;
		r.lo = u.hi - r.hi + l;
		return r;
	}

	static inline Float80x2 Float80_Div_Dekker(
		const fp80& x, const Float80x2& y
	) {
		Float80x2 u;
		u.hi = x / y.hi;
		Float80x2 t = Dekker_Mul12(u.hi, y.hi);
		fp80 l = (x - t.hi - t.lo - u.hi * y.lo) / y.hi;

		Float80x2 r;
		r.hi = u.hi + l;
		r.lo = u.hi - r.hi + l;
		return r;
	}

/* Arithmetic */

	inline Float80x2 operator+(const Float80x2 &value) const {
		return Dekker_Add(*this, value);
	}

	inline Float80x2 operator-(const Float80x2 &value) const {
		return Dekker_Sub(*this, value);
	}

	inline Float80x2 operator*(const Float80x2 &value) const {
		return Dekker_Mul(*this, value);
	}

	inline Float80x2 operator/(const Float80x2 &value) const {
		return Dekker_Div(*this, value);
	}

	inline Float80x2 operator+(const fp80 &value) const {
		return Dekker_Add_Float80(*this, value);
	}

	inline Float80x2 operator-(const fp80 &value) const {
		return Dekker_Sub_Float80(*this, value);
	}

	inline Float80x2 operator*(const fp80 &value) const {
		return Dekker_Mul_Float80(*this, value);
	}

	inline Float80x2 operator/(const fp80 &value) const {
		return Dekker_Div_Float80(*this, value);
	}

	inline Float80x2 operator-() const {
		Float80x2 value = *this;
		value.hi = -value.hi;
		value.lo = -value.lo;
		return value;
	}

/* Increment/Decrement */

	inline Float80x2& operator++() {
		*this = Dekker_Add(*this, static_cast<Float80x2>(1.0));
		return *this;
	}

	inline Float80x2& operator--() {
		*this = Dekker_Sub(*this, static_cast<Float80x2>(1.0));
		return *this;
	}

	inline Float80x2 operator++(int) {
		Float80x2 temp = *this;
		*this = Dekker_Add(*this, static_cast<Float80x2>(1.0));
		return temp;
	}

	inline Float80x2 operator--(int) {
		Float80x2 temp = *this;
		*this = Dekker_Sub(*this, static_cast<Float80x2>(1.0));
		return temp;
	}

/* Compound Assignment */

	inline Float80x2& operator+=(const Float80x2 &value) {
		*this = Dekker_Add(*this, value);
		return *this;
	}

	inline Float80x2& operator-=(const Float80x2 &value) {
		*this = Dekker_Sub(*this, value);
		return *this;
	}

	inline Float80x2& operator*=(const Float80x2 &value) {
		*this = Dekker_Mul(*this, value);
		return *this;
	}

	inline Float80x2& operator/=(const Float80x2 &value) {
		*this = Dekker_Div(*this, value);
		return *this;
	}

/* Double-Single Compound Assignment */

	inline Float80x2& operator+=(const fp80 &value) {
		*this = Dekker_Add_Float80(*this, value);
		return *this;
	}

	inline Float80x2& operator-=(const fp80 &value) {
		*this = Dekker_Sub_Float80(*this, value);
		return *this;
	}

	inline Float80x2& operator*=(const fp80 &value) {
		*this = Dekker_Mul_Float80(*this, value);
		return *this;
	}

	inline Float80x2& operator/=(const fp80 &value) {
		*this = Dekker_Div_Float80(*this, value);
		return *this;
	}

/* Comparison */

	inline bool operator==(const Float80x2 &value) const {
		return (
			this->hi == value.hi &&
			this->lo == value.lo
		) ? true : false;
	}
	inline bool operator!=(const Float80x2 &value) const {
		return (
			this->hi != value.hi ||
			this->lo != value.lo
		) ? true : false;
	}

	inline bool operator<(const Float80x2 &value) const {
		if (this->hi == value.hi) {
			return (this->lo < value.lo);
		}
		return (this->hi < value.hi);
	}

	inline bool operator<=(const Float80x2 &value) const {
		if (this->hi == value.hi) {
			return (this->lo <= value.lo);
		}
		return (this->hi <= value.hi);
	}

	inline bool operator>(const Float80x2 &value) const {
		if (this->hi == value.hi) {
			return (this->lo > value.lo);
		}
		return (this->hi > value.hi);
	}

	inline bool operator>=(const Float80x2 &value) const {
		if (this->hi == value.hi) {
			return (this->lo >= value.lo);
		}
		return (this->hi >= value.hi);
	}

/* Constructors */

	Float80x2() = default;

	constexpr inline Float80x2(const fp80& value_hi, const fp80& value_lo) :
		hi(value_hi), lo(value_lo) {}

	constexpr inline Float80x2(const fp32& value) :
		hi((fp32)value), lo(static_cast<fp80>(0.0)) {}

	constexpr inline Float80x2(const fp64& value) :
		hi((fp64)value), lo(static_cast<fp80>(0.0)) {}

	constexpr inline Float80x2(const fp80& value) :
		hi(value), lo(static_cast<fp80>(0.0)) {}

	template<typename fpX>
	constexpr inline Float80x2(const fpX& value) :
		hi((fp80)value), lo((fp80)(value - (fpX)this->hi)) {}

/* Casts */

	constexpr inline operator fp32() const {
		return (fp32)this->hi;
	}
	constexpr inline operator fp64() const {
		return (fp64)this->hi;
	}
	constexpr inline operator fp80() const {
		return this->hi;
	}

	template<typename fpX>
	constexpr inline operator fpX() const {
		return (fpX)this->hi + (fpX)this->lo;
	}

};

//------------------------------------------------------------------------------
// Float80x2 Constants
//------------------------------------------------------------------------------

/* C++20 <numbers> */

	constexpr Float80x2 Float80x2_e          = {0xa.df85458a2bb4a9bp-2L,-0xa.04753bfb185861cp-67L}; /**< ~2.718281828 */
	constexpr Float80x2 Float80x2_log2e      = {0xb.8aa3b295c17f0bcp-3L,-0x8.2f0025f2dc582eep-68L}; /**< ~1.442695041 */
	constexpr Float80x2 Float80x2_log10e     = {0xd.e5bd8a937287195p-5L,+0xd.56eaabeb4cf70c9p-71L}; /**< ~0.434294482 */
	constexpr Float80x2 Float80x2_pi         = {0xc.90fdaa22168c235p-2L,-0xe.ce675d1fc8f8cbbp-68L}; /**< ~3.141592654 */
	constexpr Float80x2 Float80x2_inv_pi     = {0xa.2f9836e4e44152ap-5L,-0xf.62a0b82b2c88fc9p-75L}; /**< ~0.318309886 */
	constexpr Float80x2 Float80x2_inv_sqrtpi = {0x9.06eba8214db688dp-4L,+0xe.3a914fed7fd8688p-69L}; /**< ~0.564189584 */
	constexpr Float80x2 Float80x2_ln2        = {0xb.17217f7d1cf79acp-4L,-0xd.871319ff0342543p-70L}; /**< ~0.693147181 */
	constexpr Float80x2 Float80x2_ln10       = {0x9.35d8dddaaa8ac17p-2L,-0xa.d494ea3e967aeb9p-69L}; /**< ~2.302585093 */
	constexpr Float80x2 Float80x2_sqrt2      = {0xb.504f333f9de6484p-3L,+0xb.2fb1366ea957d3ep-68L}; /**< ~1.414213562 */
	constexpr Float80x2 Float80x2_sqrt3      = {0xd.db3d742c265539ep-3L,-0xd.a8bd28f8747c477p-68L}; /**< ~1.732050808 */
	constexpr Float80x2 Float80x2_inv_sqrt3  = {0x9.3cd3a2c8198e269p-4L,+0xc.7c0f257d92be831p-72L}; /**< ~0.577350269 */
	constexpr Float80x2 Float80x2_egamma     = {0x9.3c467e37db0c7a5p-4L,-0xb.90701fbfab4d2a5p-70L}; /**< ~0.577215665 */
	constexpr Float80x2 Float80x2_phi        = {0xc.f1bbcdcbfa53e0bp-3L,-0xc.633f9fa31237cbfp-72L}; /**< ~1.618033989 */

#if __cplusplus >= 201907L
#include <numbers>
namespace std {
	namespace numbers {
		template<> inline constexpr Float80x2 e_v          <Float80x2> = Float80x2_e         ; /**< ~2.718281828 */
		template<> inline constexpr Float80x2 log2e_v      <Float80x2> = Float80x2_log2e     ; /**< ~1.442695041 */
		template<> inline constexpr Float80x2 log10e_v     <Float80x2> = Float80x2_log10e    ; /**< ~0.434294482 */
		template<> inline constexpr Float80x2 pi_v         <Float80x2> = Float80x2_pi        ; /**< ~3.141592654 */
		template<> inline constexpr Float80x2 inv_pi_v     <Float80x2> = Float80x2_inv_pi    ; /**< ~0.318309886 */
		template<> inline constexpr Float80x2 inv_sqrtpi_v <Float80x2> = Float80x2_inv_sqrtpi; /**< ~0.564189584 */
		template<> inline constexpr Float80x2 ln2_v        <Float80x2> = Float80x2_ln2       ; /**< ~0.693147181 */
		template<> inline constexpr Float80x2 ln10_v       <Float80x2> = Float80x2_ln10      ; /**< ~2.302585093 */
		template<> inline constexpr Float80x2 sqrt2_v      <Float80x2> = Float80x2_sqrt2     ; /**< ~1.414213562 */
		template<> inline constexpr Float80x2 sqrt3_v      <Float80x2> = Float80x2_sqrt3     ; /**< ~1.732050808 */
		template<> inline constexpr Float80x2 inv_sqrt3_v  <Float80x2> = Float80x2_inv_sqrt3 ; /**< ~0.577350269 */
		template<> inline constexpr Float80x2 egamma_v     <Float80x2> = Float80x2_egamma    ; /**< ~0.577215665 */
		template<> inline constexpr Float80x2 phi_v        <Float80x2> = Float80x2_phi       ; /**< ~1.618033989 */
	}
}
#endif

typedef Float80x2 fp80x2;

/* Math functions (Natively implemented) */

	/* Arithmetic */
	
	inline fp80x2 fmax(fp80x2 x, fp80x2 y) {
		return (x > y) ? x : y;
	}
	// inline fp80x2 fmax(fp80x2 x, fp80x2 y, fp80x2 z) {
	// 	return (x > y) ?
	// 	((x > z) ? x : z) :
	// 	((y > z) ? y : z);
	// }
	inline fp80x2 fmin(fp80x2 x, fp80x2 y) {
		return (x < y) ? x : y;
	}
	// inline fp80x2 fmin(fp80x2 x, fp80x2 y, fp80x2 z) {
	// 	return (x < y) ?
	// 	((x < z) ? x : z) :
	// 	((y < z) ? y : z);
	// }
	inline fp80x2 fabs(fp80x2 x) {
		return (x < static_cast<fp80x2>(0.0)) ? -x : x;
	}
	inline fp80x2 fdim(fp80x2 x, fp80x2 y) {
		return (x > y) ? (x - y) : static_cast<fp80x2>(0.0);
	}
	inline fp80x2 fma(fp80x2 x, fp80x2 y, fp80x2 z) {
		return (x * y) + z;
	}
	inline fp80x2 copysign(fp80x2 x, fp80x2 y) {
		return (
			(x.hi < static_cast<fp80>(0.0)) != (y.hi < (static_cast<fp80>(0.0)))
		) ? -x : x;
	}
	/** @note This function name may change to square() or etc */
	inline fp80x2 sqr(fp80x2 x) {
		return Float80x2::Dekker_Sqr(x);
	}
	inline fp80x2 sqrt(fp80x2 x) {
		if (x == static_cast<fp80x2>(0.0)) {
			return x;
		}
		fp80x2 guess = (fp80x2)sqrt(x.hi);
		return (guess + x / guess) * static_cast<fp80>(0.5);
	}
	inline fp80x2 cbrt(fp80x2 x) {
		if (x == static_cast<fp80x2>(0.0)) {
			return x;
		}
		fp80x2 guess = (fp80x2)cbrt(x.hi);
		return (
			guess * static_cast<fp80>(2.0) + (x) / Float80x2::Dekker_Sqr(guess)
		) / static_cast<fp80>(3.0);
	}
	inline fp80x2 hypot(fp80x2 x, fp80x2 y) {
		return sqrt(
			Float80x2::Dekker_Sqr(x) + Float80x2::Dekker_Sqr(y)
		);
	}
	// inline fp80x2 hypot(fp80x2 x, fp80x2 y, fp80x2 z) {
	// 	return sqrt(
	// 		Float80x2::Dekker_Sqr(x) + Float80x2::Dekker_Sqr(y) + Float80x2::Dekker_Sqr(z)
	// 	);
	// }

	/* Tests */

	inline bool signbit(fp80x2 x) {
		return (x.hi < static_cast<fp80>(0.0)) ? true : false;
	}
	/** Returns true if both x.hi and x.lo are finite */
	inline bool isfinite(fp80x2 x) {
		return (isfinite(x.hi) && isfinite(x.lo));
	}
	/** Returns true if either x.hi or x.lo are infinite */
	inline bool isinf(fp80x2 x) {
		return (isinf(x.hi) || isinf(x.lo));
	}
	/** Returns true if either x.hi or x.lo are nan */
	inline bool isnan(fp80x2 x) {
		return (isnan(x.hi) || isnan(x.lo));
	}
	/** Returns true if both x.hi and x.lo are normal */
	inline bool isnormal(fp80x2 x) {
		return (isnormal(x.hi) && isnormal(x.lo));
	}
	/** Returns true if either {x.hi, y.hi} or {x.lo, y.lo} are unordered */
	inline bool isunordered(fp80x2 x, fp80x2 y) {
		return (isunordered(x.hi, y.hi) || isunordered(x.lo, y.lo));
	}
	inline int fpclassify(fp80x2 x) {
		if (isfinite(x)) { return FP_INFINITE; }
		if (isnan(x)) { return FP_NAN; }
		if (x == static_cast<fp80x2>(0.0)) { return FP_ZERO; }
		if (isnormal(x)) { return FP_NORMAL; }
		return FP_SUBNORMAL;
	}

	/* Comparison */

	inline bool isgreater(fp80x2 x, fp80x2 y) {
		return (x > y);
	}
	inline bool isgreaterequal(fp80x2 x, fp80x2 y) {
		return (x >= y);
	}
	inline bool isless(fp80x2 x, fp80x2 y) {
		return (x < y);
	}
	inline bool islessequal(fp80x2 x, fp80x2 y) {
		return (x <= y);
	}
	inline bool islessgreater(fp80x2 x, fp80x2 y) {
		return (x < y) || (x > y);
	}

	/* Rounding */

	inline fp80x2 trunc(fp80x2 x) {
		fp80x2 int_hi = trunc(x.hi);
		fp80x2 int_lo = trunc(x.lo);
		fp80 frac_hi = x.hi - int_hi.hi;
		fp80 frac_lo = x.lo - int_lo.hi;
		// Sum in increasing order
		fp80x2 trunc_all = static_cast<fp80x2>(0.0);
		trunc_all += (
			(fp80x2)frac_hi + (fp80x2)frac_lo >= static_cast<fp80x2>(1.0)
		) ? static_cast<fp80x2>(1.0) : static_cast<fp80x2>(0.0);
		trunc_all += int_lo;
		trunc_all += int_hi;
		return trunc_all;
	}
	inline fp80x2 floor(fp80x2 x) {
		fp80x2 int_part = trunc(x);
		return (
			x < static_cast<fp80x2>(0.0) && int_part != x
		) ? int_part - static_cast<fp80x2>(1.0) : int_part;
	}
	inline fp80x2 ceil(fp80x2 x) {
		fp80x2 int_part = trunc(x);
		return (
			x > static_cast<fp80x2>(0.0) && int_part != x
		) ? int_part + static_cast<fp80x2>(1.0) : int_part;
	}
	inline fp80x2 round(fp80x2 x) {
		fp80x2 int_part = trunc(x);
		fp80x2 frac_part = x - int_part;
		if (x >= static_cast<fp80x2>(0.0)) {
			if (frac_part >= static_cast<fp80x2>(0.5)) {
				return int_part + static_cast<fp80x2>(1.0);
			}
			return int_part;
		}
		if (frac_part <= static_cast<fp80x2>(-0.5)) {
			return int_part - static_cast<fp80x2>(1.0);
		}
		return int_part;
	}
	inline fp80x2 rint(fp80x2 x) {
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
	inline long lround(fp80x2 x) {
		return (long)round(x);
	}
	inline long lrint(fp80x2 x) {
		return (long)rint(x);
	}
	inline long long llround(fp80x2 x) {
		return (long long)round(x);
	}
	inline long long llrint(fp80x2 x) {
		return (long long)rint(x);
	}

	/* Integer and Remainder */

	inline fp80x2 modf(fp80x2 x, fp80x2* int_part) {
		fp80x2 trunc_part = trunc(x);
		if (int_part != nullptr) {
			*int_part = trunc_part;
		}
		return x - trunc_part;
	}
	inline fp80x2 nearbyint(fp80x2 x) {
		return rint(x);
	}

	/* Float Exponents */

	inline fp80x2 ldexp(fp80x2 x, int exp) {
		x.hi = ldexp(x.hi, exp);
		x.lo = isfinite(x.hi) ? ldexp(x.lo, exp) : x.hi;
		return x;
	}
	inline fp80x2 scalbn(fp80x2 x, int exp) {
		x.hi = scalbn(x.hi, exp);
		x.lo = isfinite(x.hi) ? scalbn(x.lo, exp) : x.hi;
		return x;
	}
	inline fp80x2 scalbln(fp80x2 x, long exp) {
		x.hi = scalbln(x.hi, exp);
		x.lo = isfinite(x.hi) ? scalbln(x.lo, exp) : x.hi;
		return x;
	}

/* Math overloads (Casts to other types) */

		/* Arithmetic */
		// inline fp80x2 fmax(fp80x2 x, fp80x2 y) { return (fp80x2)fmax((fp80x2_Math)x, (fp80x2_Math)y); }
		// inline fp80x2 fmin(fp80x2 x, fp80x2 y) { return (fp80x2)fmin((fp80x2_Math)x, (fp80x2_Math)y); }
		// inline fp80x2 fabs(fp80x2 x) { return (fp80x2)fabs((fp80x2_Math)x); }
		// inline fp80x2 fdim(fp80x2 x, fp80x2 y) { return (fp80x2)fdim((fp80x2_Math)x, (fp80x2_Math)y); }
		// inline fp80x2 fma(fp80x2 x, fp80x2 y, fp80x2 z) { return (fp80x2)fma((fp80x2_Math)x, (fp80x2_Math)y, (fp80x2_Math)z); }
		// inline fp80x2 copysign(fp80x2 x, fp80x2 y) { return (fp80x2)copysign((fp80x2_Math)x, (fp80x2_Math)y); }
		// inline fp80x2 sqrt(fp80x2 x) { return (fp80x2)sqrt((fp80x2_Math)x); }
		// inline fp80x2 cbrt(fp80x2 x) { return (fp80x2)cbrt((fp80x2_Math)x); }
		// inline fp80x2 hypot(fp80x2 x, fp80x2 y) { return (fp80x2)hypot((fp80x2_Math)x, (fp80x2_Math)y); }
		/* Trigonometry */
		inline fp80x2  sin (fp80x2 x) { return (fp80x2) sin ((fp80x2_Math)x); }
		inline fp80x2  cos (fp80x2 x) { return (fp80x2) cos ((fp80x2_Math)x); }
		inline fp80x2  tan (fp80x2 x) { return (fp80x2) tan ((fp80x2_Math)x); }
		inline fp80x2 asin (fp80x2 x) { return (fp80x2)asin ((fp80x2_Math)x); }
		inline fp80x2 acos (fp80x2 x) { return (fp80x2)acos ((fp80x2_Math)x); }
		inline fp80x2 atan (fp80x2 x) { return (fp80x2)atan ((fp80x2_Math)x); }
		inline fp80x2  sinh(fp80x2 x) { return (fp80x2) sinh((fp80x2_Math)x); }
		inline fp80x2  cosh(fp80x2 x) { return (fp80x2) cosh((fp80x2_Math)x); }
		inline fp80x2  tanh(fp80x2 x) { return (fp80x2) tanh((fp80x2_Math)x); }
		inline fp80x2 asinh(fp80x2 x) { return (fp80x2)asinh((fp80x2_Math)x); }
		inline fp80x2 acosh(fp80x2 x) { return (fp80x2)acosh((fp80x2_Math)x); }
		inline fp80x2 atanh(fp80x2 x) { return (fp80x2)atanh((fp80x2_Math)x); }
		inline fp80x2 atan2(fp80x2 y, fp80x2 x) { return (fp80x2)atan2((fp80x2_Math)y, (fp80x2_Math)x); }
		inline void sincos(fp80x2 x, fp80x2* p_sin, fp80x2* p_cos) {
			*p_sin = sin(x);
			*p_cos = cos(x);
		}
		/* Logarithms and Exponents */
		inline fp80x2 log  (fp80x2 x) { return (fp80x2)log  ((fp80x2_Math)x); }
		inline fp80x2 log1p(fp80x2 x) { return (fp80x2)log1p((fp80x2_Math)x); }
		inline fp80x2 logb (fp80x2 x) { return (fp80x2)logb ((fp80x2_Math)x); }
		inline fp80x2 log2 (fp80x2 x) { return (fp80x2)log2 ((fp80x2_Math)x); }
		inline fp80x2 log10(fp80x2 x) { return (fp80x2)log10((fp80x2_Math)x); }
		inline fp80x2 exp  (fp80x2 x) { return (fp80x2)exp  ((fp80x2_Math)x); }
		inline fp80x2 expm1(fp80x2 x) { return (fp80x2)expm1((fp80x2_Math)x); }
		inline fp80x2 exp2 (fp80x2 x) { return (fp80x2)exp2 ((fp80x2_Math)x); }
		inline fp80x2 pow(fp80x2 x, fp80x2 y) { return (fp80x2)pow((fp80x2_Math)x, (fp80x2_Math)y); }
		/* Rounding */
		// inline fp80x2 trunc(fp80x2 x) { return (fp80x2)trunc((fp80x2_Math)x); }
		// inline fp80x2 floor(fp80x2 x) { return (fp80x2)floor((fp80x2_Math)x); }
		// inline fp80x2 ceil (fp80x2 x) { return (fp80x2)ceil ((fp80x2_Math)x); }
		// inline fp80x2 rint (fp80x2 x) { return (fp80x2)rint ((fp80x2_Math)x); }
		// inline fp80x2 round(fp80x2 x) { return (fp80x2)round((fp80x2_Math)x); }
		// inline long lrint (fp80x2 x) { return lrint ((fp80x2_Math)x); }
		// inline long lround(fp80x2 x) { return lround((fp80x2_Math)x); }
		// inline long long llrint (fp80x2 x) { return llrint ((fp80x2_Math)x); }
		// inline long long llround(fp80x2 x) { return llround((fp80x2_Math)x); }
		/* Integer and Remainder */
		inline fp80x2 fmod(fp80x2 x, fp80x2 y) { return (fp80x2)fmod((fp80x2_Math)x, (fp80x2_Math)y); }
		// inline fp80x2 modf(fp80x2 x, fp80x2* y) {
		// 	fp80x2_Math y_temp;
		// 	fp80x2 result = modf((fp80x2_Math)x, &y_temp);
		// 	*y = (fp80x2)y_temp;
		// 	return result;
		// }
		// inline fp80x2 nearbyint(fp80x2 x) { return (fp80x2)nearbyint((fp80x2_Math)x); }
		// Incorrect Function // inline fp80x2 nextafter(fp80x2 x, fp80x2 y) { return (fp80x2)nextafter((fp80x2_Math)x, (fp80x2_Math)y); }
		inline fp80x2 remainder(fp80x2 x, fp80x2 y) { return (fp80x2)remainder((fp80x2_Math)x, (fp80x2_Math)y); }
		inline fp80x2 remquo(fp80x2 x, fp80x2 y, int* quo) { return (fp80x2)remquo((fp80x2_Math)x, (fp80x2_Math)y, quo); }
		/* Float Exponents */
		inline int ilogb(fp80x2 x) { return ilogb((fp80x2_Math)x); }
		inline fp80x2 frexp  (fp80x2 x, int* exp) { return (fp80x2)frexp  ((fp80x2_Math)x, exp); }
		// inline fp80x2 ldexp  (fp80x2 x, int  exp) { return (fp80x2)ldexp  ((fp80x2_Math)x, exp); }
		// inline fp80x2 scalbn (fp80x2 x, int  exp) { return (fp80x2)scalbn ((fp80x2_Math)x, exp); }
		// inline fp80x2 scalbln(fp80x2 x, long exp) { return (fp80x2)scalbln((fp80x2_Math)x, exp); }
		/* Tests */
		// inline bool signbit(fp80x2 x) { return (signbit((fp80x2_Math)x) != 0) ? true : false; }
		// inline bool isfinite(fp80x2 x) { return (isfinite((fp80x2_Math)x) != 0) ? true : false; }
		// inline bool isinf(fp80x2 x) { return (isinf((fp80x2_Math)x) != 0) ? true : false; }
		// inline bool isnan(fp80x2 x) { return (isnan((fp80x2_Math)x) != 0) ? true : false; }
		/* Transcendental Functions */
		inline fp80x2 erf (fp80x2 x) { return (fp80x2)erf ((fp80x2_Math)x); }
		inline fp80x2 erfc(fp80x2 x) { return (fp80x2)erfc((fp80x2_Math)x); }
		inline fp80x2 lgamma(fp80x2 x) { return (fp80x2)lgamma((fp80x2_Math)x); }
		inline fp80x2 tgamma(fp80x2 x) { return (fp80x2)tgamma((fp80x2_Math)x); }

	/* Strings */

		#include "../FloatNx2/FloatNx2_stringTo.hpp"

		inline Float80x2 stringTo_Float80x2(const char* nPtr, char** endPtr = nullptr) {
			internal_double_FloatN_stringTo<Float80x2, fp80> stringTo_func;
			return stringTo_func.stringTo_FloatNx2(nPtr, endPtr);
		}

		/**
		 * @brief Wrapper for stringTo_Float80x2
		 */
		inline std::istream& operator>>(std::istream& stream, Float80x2& value) {
			internal_double_FloatN_stringTo<Float80x2, fp80> func_cin;
			return func_cin.cin_FloatNx2(stream, value);
		}

		#include "../FloatNx2/FloatNx2_snprintf.hpp"

		#define PRIFloat80x2 "D"
		#define PRIfp80x2 PRIFloat80x2

		/**
		 * @brief snprintf a singular Float80x2/fp80x2.
		 * Similar in functionallity to quadmath_snprintf.
		 * @note $ not supported. This function ignore additional
		 * format specifiers.
		 * @return -1 on encoding failure. Otherwise the total length of the
		 * string excluding the \0 terminator and ignoring the buffer size.
		 */
		inline int Float80x2_snprintf(
			char* buf, size_t len,
			const char* format, ...
		) {
			va_list args;
			va_start(args, format);
			internal_double_FloatN_snprintf<Float80x2, fp80> func_snprintf;
			int ret_val = func_snprintf.FloatNx2_snprintf(
				PRIFloat80x2, buf, len,
				format, args
			);
			va_end(args);
			return ret_val;
		}

		/**
		 * @brief Wrapper for Float80x2_snprintf
		 */
		inline std::ostream& operator<<(std::ostream& stream, const Float80x2& value) {
			internal_double_FloatN_snprintf<Float80x2, fp80> func_cout;
			return func_cout.FloatNx2_cout(PRIFloat80x2, stream, value);
		}

#endif /* FLOAT80X2_HPP */