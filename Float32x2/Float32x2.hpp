/*
**	Author: zerico2005 (2024)
**	Project: LIB-Dekker-Float
**	License: MIT License
**	A copy of the MIT License should be included with
**	this project. If not, see https://opensource.org/license/MIT
*/
#ifndef FLOAT32X2_HPP
#define FLOAT32X2_HPP

// #include "Float32x2.h"
// #include "Float32x2_def.h"

#include <cstdarg>
#include <cstdint>
#include <math.h>
#include <cmath>
#include <cfenv>
#include <limits>


typedef float fp32;
typedef double fp64;

// Can be changed to other types for better accuracy
typedef fp32 fp32x2_Math;

/**
 * @brief Double-Float32 Dekker Float implementation.
 * Source: Creel "Double it Like Dekker" on YouTube.
 *
 * @warning -Ofast may break this library. -O3 compiles okay on gcc and clang.
 */
struct Float32x2 {

	fp32 hi;
	fp32 lo;

	/* Arithmetic */

	static inline Float32x2 Dekker_Add(
		const Float32x2& x, const Float32x2& y
	) {
		fp32 r_hi = x.hi + y.hi;
		fp32 r_lo = static_cast<fp32>(0.0);
		if (fabsf(x.hi) > fabsf(y.hi)) {
			r_lo = x.hi - r_hi + y.hi + y.lo + x.lo;
		} else {
			r_lo = y.hi - r_hi + x.hi + x.lo + y.lo;
		}

		Float32x2 c;
		c.hi = r_hi + r_lo;
		c.lo = r_hi - c.hi + r_lo;
		return c;
	}

	static inline Float32x2 Dekker_Sub(
		const Float32x2& x, const Float32x2& y
	) {
		fp32 r_hi = x.hi - y.hi;
		fp32 r_lo = static_cast<fp32>(0.0);
		if (fabsf(x.hi) > fabsf(y.hi)) {
			r_lo = x.hi - r_hi - y.hi - y.lo + x.lo;
		} else {
			r_lo = -y.hi - r_hi + x.hi + x.lo - y.lo;
		}

		Float32x2 c;
		c.hi = r_hi + r_lo;
		c.lo = r_hi - c.hi + r_lo;
		return c;
	}

	static constexpr fp32 Dekker_Scale = 4097.0f; // (2^ceil(24 / 2) + 1)
	
	static inline Float32x2 Dekker_Split(const fp32& x) {
		fp32 p = x * Dekker_Scale;
		Float32x2 r;
		r.hi = (x - p) + p;
		r.lo = x - r.hi;
		return r;
	}
	
	// static constexpr uint64_t Dekker_Split_Mask = ~((uint64_t)0xFFF);

	// static inline Float32x2 Dekker_Split(const fp32& x) {
	// 	Float32x2 r;
	// 	uint64_t temp = (*(uint64_t*)((void*)&x)) & Dekker_Split_Mask;
	// 	r.hi = (*(fp32*)((void*)&temp));
	// 	r.lo = x - r.hi;
	// 	return r;
	// }

	static inline Float32x2 Dekker_Mul12(
		const fp32& x, const fp32& y
	) {
		Float32x2 a = Dekker_Split(x);
		Float32x2 b = Dekker_Split(y);
		fp32 p = a.hi * b.hi;
		fp32 q = a.hi * b.lo + a.lo * b.hi;

		Float32x2 r;
		r.hi = p + q;
		r.lo = p - r.hi + q + a.lo * b.lo;
		return r;
	}

	static inline Float32x2 Dekker_Mul(
		const Float32x2& x, const Float32x2& y
	) {
		Float32x2 t = Dekker_Mul12(x.hi, y.hi);
		fp32 c = x.hi * y.lo + x.lo * y.hi + t.lo;

		Float32x2 r;
		r.hi = t.hi + c;
		r.lo = t.hi - r.hi + c;
		return r;
	}

	static inline Float32x2 Dekker_Div(
		const Float32x2& x, const Float32x2& y
	) {
		Float32x2 u;
		u.hi = x.hi / y.hi;
		Float32x2 t = Dekker_Mul12(u.hi, y.hi);
		fp32 l = (x.hi - t.hi - t.lo + x.lo - u.hi * y.lo) / y.hi;

		Float32x2 r;
		r.hi = u.hi + l;
		r.lo = u.hi - r.hi + l;
		return r;
	}

	static inline Float32x2 Dekker_Sqr12(
		const fp32& x
	) {
		Float32x2 a = Dekker_Split(x);
		fp32 p = a.hi * a.hi;
		fp32 q = static_cast<fp32>(2.0) * a.hi * a.lo;

		Float32x2 r;
		r.hi = p + q;
		r.lo = p - r.hi + q + a.lo * a.lo;
		return r;
	}

	static inline Float32x2 Dekker_Sqr(
		const Float32x2& x
	) {
		Float32x2 t = Dekker_Sqr12(x.hi);
		fp32 c = static_cast<fp32>(2.0) * x.hi * x.lo + t.lo;

		Float32x2 r;
		r.hi = t.hi + c;
		r.lo = t.hi - r.hi + c;
		return r;
	}

	/* Double-Single Arithmetic */

	static inline Float32x2 Dekker_Add_Float32(
		const Float32x2& x, const fp32& y
	) {
		fp32 r_hi = x.hi + y;
		fp32 r_lo = static_cast<fp32>(0.0);
		if (fabsf(x.hi) > fabsf(y)) {
			r_lo = x.hi - r_hi + y + x.lo;
		} else {
			r_lo = y - r_hi + x.hi + x.lo;
		}

		Float32x2 c;
		c.hi = r_hi + r_lo;
		c.lo = r_hi - c.hi + r_lo;
		return c;
	}

	static inline Float32x2 Dekker_Sub_Float32(
		const Float32x2& x, const fp32& y
	) {
		fp32 r_hi = x.hi - y;
		fp32 r_lo = static_cast<fp32>(0.0);
		if (fabsf(x.hi) > fabsf(y)) {
			r_lo = x.hi - r_hi - y + x.lo;
		} else {
			r_lo = -y - r_hi + x.hi + x.lo;
		}

		Float32x2 c;
		c.hi = r_hi + r_lo;
		c.lo = r_hi - c.hi + r_lo;
		return c;
	}

	static inline Float32x2 Dekker_Mul_Float32(
		const Float32x2& x, const fp32& y
	) {
		Float32x2 t = Dekker_Mul12(x.hi, y);
		fp32 c = x.lo * y + t.lo;

		Float32x2 r;
		r.hi = t.hi + c;
		r.lo = t.hi - r.hi + c;
		return r;
	}

	static inline Float32x2 Dekker_Div_Float32(
		const Float32x2& x, const fp32& y
	) {
		Float32x2 u;
		u.hi = x.hi / y;
		Float32x2 t = Dekker_Mul12(u.hi, y);
		fp32 l = (x.hi - t.hi - t.lo + x.lo) / y;

		Float32x2 r;
		r.hi = u.hi + l;
		r.lo = u.hi - r.hi + l;
		return r;
	}

	static inline Float32x2 Float32_Div_Dekker(
		const fp32& x, const Float32x2& y
	) {
		Float32x2 u;
		u.hi = x / y.hi;
		Float32x2 t = Dekker_Mul12(u.hi, y.hi);
		fp32 l = (x - t.hi - t.lo - u.hi * y.lo) / y.hi;

		Float32x2 r;
		r.hi = u.hi + l;
		r.lo = u.hi - r.hi + l;
		return r;
	}

/* Arithmetic */

	inline Float32x2 operator+(const Float32x2 &value) const {
		return Dekker_Add(*this, value);
	}

	inline Float32x2 operator-(const Float32x2 &value) const {
		return Dekker_Sub(*this, value);
	}

	inline Float32x2 operator*(const Float32x2 &value) const {
		return Dekker_Mul(*this, value);
	}

	inline Float32x2 operator/(const Float32x2 &value) const {
		return Dekker_Div(*this, value);
	}

	inline Float32x2 operator+(const fp32 &value) const {
		return Dekker_Add_Float32(*this, value);
	}

	inline Float32x2 operator-(const fp32 &value) const {
		return Dekker_Sub_Float32(*this, value);
	}

	inline Float32x2 operator*(const fp32 &value) const {
		return Dekker_Mul_Float32(*this, value);
	}

	inline Float32x2 operator/(const fp32 &value) const {
		return Dekker_Div_Float32(*this, value);
	}

	inline Float32x2 operator-() const {
		Float32x2 value = *this;
		value.hi = -value.hi;
		value.lo = -value.lo;
		return value;
	}

/* Increment/Decrement */

	inline Float32x2& operator++() {
		*this = Dekker_Add(*this, static_cast<Float32x2>(1.0));
		return *this;
	}

	inline Float32x2& operator--() {
		*this = Dekker_Sub(*this, static_cast<Float32x2>(1.0));
		return *this;
	}

	inline Float32x2 operator++(int) {
		Float32x2 temp = *this;
		*this = Dekker_Add(*this, static_cast<Float32x2>(1.0));
		return temp;
	}

	inline Float32x2 operator--(int) {
		Float32x2 temp = *this;
		*this = Dekker_Sub(*this, static_cast<Float32x2>(1.0));
		return temp;
	}

/* Compound Assignment */

	inline Float32x2& operator+=(const Float32x2 &value) {
		*this = Dekker_Add(*this, value);
		return *this;
	}

	inline Float32x2& operator-=(const Float32x2 &value) {
		*this = Dekker_Sub(*this, value);
		return *this;
	}

	inline Float32x2& operator*=(const Float32x2 &value) {
		*this = Dekker_Mul(*this, value);
		return *this;
	}

	inline Float32x2& operator/=(const Float32x2 &value) {
		*this = Dekker_Div(*this, value);
		return *this;
	}

/* Double-Single Compound Assignment */

	inline Float32x2& operator+=(const fp32 &value) {
		*this = Dekker_Add_Float32(*this, value);
		return *this;
	}

	inline Float32x2& operator-=(const fp32 &value) {
		*this = Dekker_Sub_Float32(*this, value);
		return *this;
	}

	inline Float32x2& operator*=(const fp32 &value) {
		*this = Dekker_Mul_Float32(*this, value);
		return *this;
	}

	inline Float32x2& operator/=(const fp32 &value) {
		*this = Dekker_Div_Float32(*this, value);
		return *this;
	}

/* Comparison */

	inline bool operator==(const Float32x2 &value) const {
		return (
			this->hi == value.hi &&
			this->lo == value.lo
		) ? true : false;
	}
	inline bool operator!=(const Float32x2 &value) const {
		return (
			this->hi != value.hi ||
			this->lo != value.lo
		) ? true : false;
	}

	inline bool operator<(const Float32x2 &value) const {
		if (this->hi == value.hi) {
			return (this->lo < value.lo);
		}
		return (this->hi < value.hi);
	}

	inline bool operator<=(const Float32x2 &value) const {
		if (this->hi == value.hi) {
			return (this->lo <= value.lo);
		}
		return (this->hi <= value.hi);
	}

	inline bool operator>(const Float32x2 &value) const {
		if (this->hi == value.hi) {
			return (this->lo > value.lo);
		}
		return (this->hi > value.hi);
	}

	inline bool operator>=(const Float32x2 &value) const {
		if (this->hi == value.hi) {
			return (this->lo >= value.lo);
		}
		return (this->hi >= value.hi);
	}

/* Constructors */

	Float32x2() = default;

	constexpr inline Float32x2(const fp32& value_hi, const fp32& value_lo) :
		hi(value_hi), lo(value_lo) {}

	constexpr inline Float32x2(const fp32& value) :
		hi(value), lo(0.0f) {}

	constexpr inline Float32x2(const fp64& value) :
		hi((fp32)value), lo((fp32)(value - (fp64)this->hi)) {}

	template<typename fpX>
	constexpr inline Float32x2(const fpX& value) :
		hi((fp32)value), lo((fp32)(value - (fpX)this->hi)) {}

/* Casts */

	constexpr inline operator fp32() const {
		return this->hi;
	}
	constexpr inline operator fp64() const {
		return (fp64)this->hi + (fp64)this->lo;
	}

	template<typename fpX>
	constexpr inline operator fpX() const {
		return (fpX)this->hi + (fpX)this->lo;
	}

};

//------------------------------------------------------------------------------
// Float32x2 Constants
//------------------------------------------------------------------------------

/* C++20 <numbers> */

	constexpr Float32x2 Float32x2_e          = {0x1.5bf0a8p+1f,+0x1.628aeep-24f}; /**< ~2.718281828 */
	constexpr Float32x2 Float32x2_log2e      = {0x1.715476p+0f,+0x1.4ae0c0p-26f}; /**< ~1.442695041 */
	constexpr Float32x2 Float32x2_log10e     = {0x1.bcb7b2p-2f,-0x1.5b235ep-27f}; /**< ~0.434294482 */
	constexpr Float32x2 Float32x2_pi         = {0x1.921fb6p+1f,-0x1.777a5cp-24f}; /**< ~3.141592654 */
	constexpr Float32x2 Float32x2_inv_pi     = {0x1.45f306p-2f,+0x1.b93910p-27f}; /**< ~0.318309886 */
	constexpr Float32x2 Float32x2_inv_sqrtpi = {0x1.20dd76p-1f,-0x1.f7ac92p-26f}; /**< ~0.564189584 */
	constexpr Float32x2 Float32x2_ln2        = {0x1.62e430p-1f,-0x1.05c610p-29f}; /**< ~0.693147181 */
	constexpr Float32x2 Float32x2_ln10       = {0x1.26bb1cp+1f,-0x1.12aabap-25f}; /**< ~2.302585093 */
	constexpr Float32x2 Float32x2_sqrt2      = {0x1.6a09e6p+0f,+0x1.9fcef4p-26f}; /**< ~1.414213562 */
	constexpr Float32x2 Float32x2_sqrt3      = {0x1.bb67aep+0f,+0x1.0b0996p-25f}; /**< ~1.732050808 */
	constexpr Float32x2 Float32x2_inv_sqrt3  = {0x1.279a74p-1f,+0x1.640cc8p-27f}; /**< ~0.577350269 */
	constexpr Float32x2 Float32x2_egamma     = {0x1.2788d0p-1f,-0x1.c824f4p-28f}; /**< ~0.577215665 */
	constexpr Float32x2 Float32x2_phi        = {0x1.9e377ap+0f,-0x1.1a02d6p-26f}; /**< ~1.618033989 */

#if __cplusplus >= 201907L
#include <numbers>
namespace std {
	namespace numbers {
		template<> inline constexpr Float32x2 e_v          <Float32x2> = Float32x2_e         ; /**< ~2.718281828 */
		template<> inline constexpr Float32x2 log2e_v      <Float32x2> = Float32x2_log2e     ; /**< ~1.442695041 */
		template<> inline constexpr Float32x2 log10e_v     <Float32x2> = Float32x2_log10e    ; /**< ~0.434294482 */
		template<> inline constexpr Float32x2 pi_v         <Float32x2> = Float32x2_pi        ; /**< ~3.141592654 */
		template<> inline constexpr Float32x2 inv_pi_v     <Float32x2> = Float32x2_inv_pi    ; /**< ~0.318309886 */
		template<> inline constexpr Float32x2 inv_sqrtpi_v <Float32x2> = Float32x2_inv_sqrtpi; /**< ~0.564189584 */
		template<> inline constexpr Float32x2 ln2_v        <Float32x2> = Float32x2_ln2       ; /**< ~0.693147181 */
		template<> inline constexpr Float32x2 ln10_v       <Float32x2> = Float32x2_ln10      ; /**< ~2.302585093 */
		template<> inline constexpr Float32x2 sqrt2_v      <Float32x2> = Float32x2_sqrt2     ; /**< ~1.414213562 */
		template<> inline constexpr Float32x2 sqrt3_v      <Float32x2> = Float32x2_sqrt3     ; /**< ~1.732050808 */
		template<> inline constexpr Float32x2 inv_sqrt3_v  <Float32x2> = Float32x2_inv_sqrt3 ; /**< ~0.577350269 */
		template<> inline constexpr Float32x2 egamma_v     <Float32x2> = Float32x2_egamma    ; /**< ~0.577215665 */
		template<> inline constexpr Float32x2 phi_v        <Float32x2> = Float32x2_phi       ; /**< ~1.618033989 */
	}
}
#endif

typedef Float32x2 fp32x2;

/* Math functions (Natively implemented) */

	/* Arithmetic */

	inline fp32x2 fmax(fp32x2 x, fp32x2 y) {
		return (x > y) ? x : y;
	}
	// inline fp32x2 fmax(fp32x2 x, fp32x2 y, fp32x2 z) {
	// 	return (x > y) ?
	// 	((x > z) ? x : z) :
	// 	((y > z) ? y : z);
	// }
	inline fp32x2 fmin(fp32x2 x, fp32x2 y) {
		return (x < y) ? x : y;
	}
	// inline fp32x2 fmin(fp32x2 x, fp32x2 y, fp32x2 z) {
	// 	return (x < y) ?
	// 	((x < z) ? x : z) :
	// 	((y < z) ? y : z);
	// }
	inline fp32x2 fabs(fp32x2 x) {
		return (x < static_cast<fp32x2>(0.0)) ? -x : x;
	}
	inline fp32x2 fdim(fp32x2 x, fp32x2 y) {
		return (x > y) ? (x - y) : static_cast<fp32x2>(0.0);
	}
	inline fp32x2 fma(fp32x2 x, fp32x2 y, fp32x2 z) {
		return (x * y) + z;
	}
	inline fp32x2 copysign(fp32x2 x, fp32x2 y) {
		return (
			(x.hi < static_cast<fp32>(0.0)) != (y.hi < (static_cast<fp32>(0.0)))
		) ? -x : x;
	}
	/** @note This function name may change to square() or etc */
	inline fp32x2 sqr(fp32x2 x) {
		return Float32x2::Dekker_Sqr(x);
	}
	inline fp32x2 sqrt(fp32x2 x) {
		if (x == static_cast<fp32x2>(0.0)) {
			return x;
		}
		fp32x2 guess = (fp32x2)sqrt(x.hi);
		return (guess + x / guess) * static_cast<fp32>(0.5);
	}
	inline fp32x2 cbrt(fp32x2 x) {
		if (x == static_cast<fp32x2>(0.0)) {
			return x;
		}
		fp32x2 guess = (fp32x2)cbrt(x.hi);
		return (
			guess * static_cast<fp32>(2.0) + (x) / Float32x2::Dekker_Sqr(guess)
		) / static_cast<fp32>(3.0);
	}
	inline fp32x2 hypot(fp32x2 x, fp32x2 y) {
		return sqrt(
			Float32x2::Dekker_Sqr(x) + Float32x2::Dekker_Sqr(y)
		);
	}
	// inline fp32x2 hypot(fp32x2 x, fp32x2 y, fp32x2 z) {
	// 	return sqrt(
	// 		Float32x2::Dekker_Sqr(x) + Float32x2::Dekker_Sqr(y) + Float32x2::Dekker_Sqr(z)
	// 	);
	// }

	/* Tests */

	inline bool signbit(fp32x2 x) {
		return (x.hi < static_cast<fp32>(0.0)) ? true : false;
	}
	/** Returns true if both x.hi and x.lo are finite */
	inline bool isfinite(fp32x2 x) {
		return (isfinite(x.hi) && isfinite(x.lo));
	}
	/** Returns true if either x.hi or x.lo are infinite */
	inline bool isinf(fp32x2 x) {
		return (isinf(x.hi) || isinf(x.lo));
	}
	/** Returns true if either x.hi or x.lo are nan */
	inline bool isnan(fp32x2 x) {
		return (isnan(x.hi) || isnan(x.lo));
	}
	/** Returns true if both x.hi and x.lo are normal */
	inline bool isnormal(fp32x2 x) {
		return (isnormal(x.hi) && isnormal(x.lo));
	}
	/** Returns true if either {x.hi, y.hi} or {x.lo, y.lo} are unordered */
	inline bool isunordered(fp32x2 x, fp32x2 y) {
		return (isunordered(x.hi, y.hi) || isunordered(x.lo, y.lo));
	}
	inline int fpclassify(fp32x2 x) {
		if (isfinite(x)) { return FP_INFINITE; }
		if (isnan(x)) { return FP_NAN; }
		if (x == static_cast<fp32x2>(0.0)) { return FP_ZERO; }
		if (isnormal(x)) { return FP_NORMAL; }
		return FP_SUBNORMAL;
	}

	/* Comparison */

	inline bool isgreater(fp32x2 x, fp32x2 y) {
		return (x > y);
	}
	inline bool isgreaterequal(fp32x2 x, fp32x2 y) {
		return (x >= y);
	}
	inline bool isless(fp32x2 x, fp32x2 y) {
		return (x < y);
	}
	inline bool islessequal(fp32x2 x, fp32x2 y) {
		return (x <= y);
	}
	inline bool islessgreater(fp32x2 x, fp32x2 y) {
		return (x < y) || (x > y);
	}

	/* Rounding */

	inline fp32x2 trunc(fp32x2 x) {
		fp32x2 int_hi = trunc(x.hi);
		fp32x2 int_lo = trunc(x.lo);
		fp32 frac_hi = x.hi - int_hi.hi;
		fp32 frac_lo = x.lo - int_lo.hi;
		// Sum in increasing order
		fp32x2 trunc_all = static_cast<fp32x2>(0.0);
		trunc_all += (
			(fp32x2)frac_hi + (fp32x2)frac_lo >= static_cast<fp32x2>(1.0)
		) ? static_cast<fp32x2>(1.0) : static_cast<fp32x2>(0.0);
		trunc_all += int_lo;
		trunc_all += int_hi;
		return trunc_all;
	}
	inline fp32x2 floor(fp32x2 x) {
		fp32x2 int_part = trunc(x);
		return (
			x < static_cast<fp32x2>(0.0) && int_part != x
		) ? int_part - static_cast<fp32x2>(1.0) : int_part;
	}
	inline fp32x2 ceil(fp32x2 x) {
		fp32x2 int_part = trunc(x);
		return (
			x > static_cast<fp32x2>(0.0) && int_part != x
		) ? int_part + static_cast<fp32x2>(1.0) : int_part;
	}
	inline fp32x2 round(fp32x2 x) {
		fp32x2 int_part = trunc(x);
		fp32x2 frac_part = x - int_part;
		if (x >= static_cast<fp32x2>(0.0)) {
			if (frac_part >= static_cast<fp32x2>(0.5)) {
				return int_part + static_cast<fp32x2>(1.0);
			}
			return int_part;
		}
		if (frac_part <= static_cast<fp32x2>(-0.5)) {
			return int_part - static_cast<fp32x2>(1.0);
		}
		return int_part;
	}
	inline fp32x2 rint(fp32x2 x) {
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
	inline long lround(fp32x2 x) {
		return (long)round(x);
	}
	inline long lrint(fp32x2 x) {
		return (long)rint(x);
	}
	inline long long llround(fp32x2 x) {
		return (long long)round(x);
	}
	inline long long llrint(fp32x2 x) {
		return (long long)rint(x);
	}

	/* Integer and Remainder */

	inline fp32x2 modf(fp32x2 x, fp32x2* int_part) {
		fp32x2 trunc_part = trunc(x);
		if (int_part != nullptr) {
			*int_part = trunc_part;
		}
		return x - trunc_part;
	}
	inline fp32x2 nearbyint(fp32x2 x) {
		return rint(x);
	}

	/* Float Exponents */

	inline fp32x2 ldexp(fp32x2 x, int  exp) {
		x.hi = ldexp(x.hi, exp);
		x.lo = isfinite(x.hi) ? ldexp(x.lo, exp) : x.hi;
		return x;
	}
	inline fp32x2 scalbn(fp32x2 x, int  exp) {
		x.hi = scalbn(x.hi, exp);
		x.lo = isfinite(x.hi) ? scalbn(x.lo, exp) : x.hi;
		return x;
	}
	inline fp32x2 scalbln(fp32x2 x, long exp) {
		x.hi = scalbln(x.hi, exp);
		x.lo = isfinite(x.hi) ? scalbln(x.lo, exp) : x.hi;
		return x;
	}

/* Math overloads (Casts to other types) */

		/* Arithmetic */
		// inline fp32x2 fmax(fp32x2 x, fp32x2 y) { return (fp32x2)fmax((fp32x2_Math)x, (fp32x2_Math)y); }
		// inline fp32x2 fmin(fp32x2 x, fp32x2 y) { return (fp32x2)fmin((fp32x2_Math)x, (fp32x2_Math)y); }
		// inline fp32x2 fabs(fp32x2 x) { return (fp32x2)fabs((fp32x2_Math)x); }
		// inline fp32x2 fdim(fp32x2 x, fp32x2 y) { return (fp32x2)fdim((fp32x2_Math)x, (fp32x2_Math)y); }
		// inline fp32x2 fma(fp32x2 x, fp32x2 y, fp32x2 z) { return (fp32x2)fma((fp32x2_Math)x, (fp32x2_Math)y, (fp32x2_Math)z); }
		// inline fp32x2 copysign(fp32x2 x, fp32x2 y) { return (fp32x2)copysign((fp32x2_Math)x, (fp32x2_Math)y); }
		// inline fp32x2 sqrt(fp32x2 x) { return (fp32x2)sqrt((fp32x2_Math)x); }
		// inline fp32x2 cbrt(fp32x2 x) { return (fp32x2)cbrt((fp32x2_Math)x); }
		// inline fp32x2 hypot(fp32x2 x, fp32x2 y) { return (fp32x2)hypot((fp32x2_Math)x, (fp32x2_Math)y); }
		/* Trigonometry */
		inline fp32x2  sin (fp32x2 x) { return (fp32x2) sin ((fp32x2_Math)x); }
		inline fp32x2  cos (fp32x2 x) { return (fp32x2) cos ((fp32x2_Math)x); }
		inline fp32x2  tan (fp32x2 x) { return (fp32x2) tan ((fp32x2_Math)x); }
		inline fp32x2 asin (fp32x2 x) { return (fp32x2)asin ((fp32x2_Math)x); }
		inline fp32x2 acos (fp32x2 x) { return (fp32x2)acos ((fp32x2_Math)x); }
		inline fp32x2 atan (fp32x2 x) { return (fp32x2)atan ((fp32x2_Math)x); }
		inline fp32x2  sinh(fp32x2 x) { return (fp32x2) sinh((fp32x2_Math)x); }
		inline fp32x2  cosh(fp32x2 x) { return (fp32x2) cosh((fp32x2_Math)x); }
		inline fp32x2  tanh(fp32x2 x) { return (fp32x2) tanh((fp32x2_Math)x); }
		inline fp32x2 asinh(fp32x2 x) { return (fp32x2)asinh((fp32x2_Math)x); }
		inline fp32x2 acosh(fp32x2 x) { return (fp32x2)acosh((fp32x2_Math)x); }
		inline fp32x2 atanh(fp32x2 x) { return (fp32x2)atanh((fp32x2_Math)x); }
		inline fp32x2 atan2(fp32x2 y, fp32x2 x) { return (fp32x2)atan2((fp32x2_Math)y, (fp32x2_Math)x); }
		inline void sincos(fp32x2 x, fp32x2* p_sin, fp32x2* p_cos) {
			*p_sin = sin(x);
			*p_cos = cos(x);
		}
		inline void sinhcosh(fp32x2 x, fp32x2* p_sinh, fp32x2* p_cosh) {
			*p_sinh = sinh(x);
			*p_cosh = cosh(x);
		}
		/* Logarithms and Exponents */
		inline fp32x2 log  (fp32x2 x) { return (fp32x2)log  ((fp32x2_Math)x); }
		inline fp32x2 log1p(fp32x2 x) { return (fp32x2)log1p((fp32x2_Math)x); }
		inline fp32x2 logb (fp32x2 x) { return (fp32x2)logb ((fp32x2_Math)x); }
		inline fp32x2 log2 (fp32x2 x) { return (fp32x2)log2 ((fp32x2_Math)x); }
		inline fp32x2 log10(fp32x2 x) { return (fp32x2)log10((fp32x2_Math)x); }
		inline fp32x2 exp  (fp32x2 x) { return (fp32x2)exp  ((fp32x2_Math)x); }
		inline fp32x2 expm1(fp32x2 x) { return (fp32x2)expm1((fp32x2_Math)x); }
		inline fp32x2 exp2 (fp32x2 x) { return (fp32x2)exp2 ((fp32x2_Math)x); }
		inline fp32x2 exp10(fp32x2 x) { return (fp32x2)exp((fp32x2_Math)x * (fp32x2_Math)log(static_cast<fp32x2_Math>(10.0))); }
		inline fp32x2 pow(fp32x2 x, fp32x2 y) { return (fp32x2)pow((fp32x2_Math)x, (fp32x2_Math)y); }
		/* Rounding */
		// inline fp32x2 trunc(fp32x2 x) { return (fp32x2)trunc((fp32x2_Math)x); }
		// inline fp32x2 floor(fp32x2 x) { return (fp32x2)floor((fp32x2_Math)x); }
		// inline fp32x2 ceil (fp32x2 x) { return (fp32x2)ceil ((fp32x2_Math)x); }
		// inline fp32x2 rint (fp32x2 x) { return (fp32x2)rint ((fp32x2_Math)x); }
		// inline fp32x2 round(fp32x2 x) { return (fp32x2)round((fp32x2_Math)x); }
		// inline long lrint (fp32x2 x) { return lrint ((fp32x2_Math)x); }
		// inline long lround(fp32x2 x) { return lround((fp32x2_Math)x); }
		// inline long long llrint (fp32x2 x) { return llrint ((fp32x2_Math)x); }
		// inline long long llround(fp32x2 x) { return llround((fp32x2_Math)x); }
		/* Integer and Remainder */
		inline fp32x2 fmod(fp32x2 x, fp32x2 y) { return (fp32x2)fmod((fp32x2_Math)x, (fp32x2_Math)y); }
		// inline fp32x2 modf(fp32x2 x, fp32x2* y) {
		// 	fp32x2_Math y_temp;
		// 	fp32x2 result = modf((fp32x2_Math)x, &y_temp);
		// 	*y = (fp32x2)y_temp;
		// 	return result;
		// }
		// inline fp32x2 nearbyint(fp32x2 x) { return (fp32x2)nearbyint((fp32x2_Math)x); }
		// Incorrect Function // inline fp32x2 nextafter(fp32x2 x) { return (fp32x2)nextafter((fp32x2_Math)x); }
		inline fp32x2 remainder(fp32x2 x, fp32x2 y) { return (fp32x2)remainder((fp32x2_Math)x, (fp32x2_Math)y); }
		inline fp32x2 remquo(fp32x2 x, fp32x2 y, int* quo) { return (fp32x2)remquo((fp32x2_Math)x, (fp32x2_Math)y, quo); }
		/* Float Exponents */
		inline int ilogb(fp32x2 x) { return ilogb((fp32x2_Math)x); }
		inline fp32x2 frexp  (fp32x2 x, int* exp) { return (fp32x2)frexp  ((fp32x2_Math)x, exp); }
		// inline fp32x2 ldexp  (fp32x2 x, int  exp) { return (fp32x2)ldexp  ((fp32x2_Math)x, exp); }
		// inline fp32x2 scalbn (fp32x2 x, int  exp) { return (fp32x2)scalbn ((fp32x2_Math)x, exp); }
		// inline fp32x2 scalbln(fp32x2 x, long exp) { return (fp32x2)scalbln((fp32x2_Math)x, exp); }
		/* Tests */
		// inline bool signbit(fp32x2 x) { return (signbit((fp32x2_Math)x) != 0) ? true : false; }
		// inline bool isfinite(fp32x2 x) { return (isfinite((fp32x2_Math)x) != 0) ? true : false; }
		// inline bool isinf(fp32x2 x) { return (isinf((fp32x2_Math)x) != 0) ? true : false; }
		// inline bool isnan(fp32x2 x) { return (isnan((fp32x2_Math)x) != 0) ? true : false; }
		/* Transcendental Functions */
		inline fp32x2 erf (fp32x2 x) { return (fp32x2)erf ((fp32x2_Math)x); }
		inline fp32x2 erfc(fp32x2 x) { return (fp32x2)erfc((fp32x2_Math)x); }
		inline fp32x2 lgamma(fp32x2 x) { return (fp32x2)lgamma((fp32x2_Math)x); }
		inline fp32x2 tgamma(fp32x2 x) { return (fp32x2)tgamma((fp32x2_Math)x); }

	/* Strings */

		#include "../FloatNx2/FloatNx2_stringTo.hpp"

		inline Float32x2 stringTo_Float32x2(const char* nPtr, char** endPtr = nullptr) {
			internal_double_FloatN_stringTo<Float32x2, fp32> stringTo_func;
			return stringTo_func.stringTo_FloatNx2(nPtr, endPtr);
		}

		/**
		 * @brief Wrapper for stringTo_Float32x2
		 */
		inline std::istream& operator>>(std::istream& stream, Float32x2& value) {
			internal_double_FloatN_stringTo<Float32x2, fp32> func_cin;
			return func_cin.cin_FloatNx2(stream, value);
		}

		#include "../FloatNx2/FloatNx2_snprintf.hpp"

		#define PRIFloat32x2 "D"
		#define PRIfp32x2 PRIFloat32x2

		/**
		 * @brief snprintf a singular Float32x2/fp32x2.
		 * Similar in functionallity to quadmath_snprintf.
		 * @note $ not supported. This function ignore additional
		 * format specifiers.
		 * @return -1 on encoding failure. Otherwise the total length of the
		 * string excluding the \0 terminator and ignoring the buffer size.
		 */
		inline int Float32x2_snprintf(
			char* buf, size_t len,
			const char* format, ...
		) {
			va_list args;
			va_start(args, format);
			internal_double_FloatN_snprintf<Float32x2, fp32> func_snprintf;
			int ret_val = func_snprintf.FloatNx2_snprintf(
				PRIFloat32x2, buf, len,
				format, args
			);
			va_end(args);
			return ret_val;
		}

		/**
		 * @brief Wrapper for Float32x2_snprintf
		 */
		inline std::ostream& operator<<(std::ostream& stream, const Float32x2& value) {
			internal_double_FloatN_snprintf<Float32x2, fp32> func_cout;
			return func_cout.FloatNx2_cout(PRIFloat32x2, stream, value);
		}

#endif /* FLOAT32X2_HPP */