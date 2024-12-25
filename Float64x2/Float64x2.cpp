/*
**	Author: zerico2005 (2024)
**	Project: LIB-Dekker-Float
**	License: MIT License
**	A copy of the MIT License should be included with
**	this project. If not, see https://opensource.org/license/MIT
*/

/*
**	Portions of this file were part of the libQD and libDDFUN library
**
**	libQD is licensed under a modifed BSD license that can be found below:
**	https://www.davidhbailey.com/dhbsoftware/LBNL-BSD-License.docx
**	Or alternatively from this website:
**	https://www.davidhbailey.com/dhbsoftware/
**	A copy of the LBNL-BSD-License can also be found at:
**	LIB-Dekker-Float/libQD/LBNL-BSD-License.txt
**
**	libDDFUN is licensed under a limited BSD license that can be found below:
**	https://www.davidhbailey.com/dhbsoftware/DHB-License.docx
**	Or alternatively from this website:
**	https://www.davidhbailey.com/dhbsoftware/
**	A copy of the Limited-BSD-License can also be found at:
**	LIB-Dekker-Float/libDDFUN/DISCLAIMER_and_Limited-BSD-License.txt
*/

#include <limits>
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>

#include <cassert>

// TEMP //
	#include <cfenv>
	#include <iostream>
// TEMP //

#include "Float64x2_def.h"
#include "Float64x2.h"
#include "Float64x2.hpp"
#include "Float64x2_string.h"

#include "../Float64x4/Float64x4.hpp"

#include "Float64x2_input_limits.hpp"

#include "Float64x2_LUT.hpp"

//------------------------------------------------------------------------------
// Float64x2 math.h functions
//------------------------------------------------------------------------------

// static constexpr Float64x2 inv_fact[] = {
// /*  3! */ { 1.66666666666666657e-01,  9.25185853854297066e-18},
// /*  4! */ { 4.16666666666666644e-02,  2.31296463463574266e-18},
// /*  5! */ { 8.33333333333333322e-03,  1.15648231731787138e-19},
// /*  6! */ { 1.38888888888888894e-03, -5.30054395437357706e-20},
// /*  7! */ { 1.98412698412698413e-04,  1.72095582934207053e-22},
// /*  8! */ { 2.48015873015873016e-05,  2.15119478667758816e-23},
// /*  9! */ { 2.75573192239858925e-06, -1.85839327404647208e-22},
// /* 10! */ { 2.75573192239858883e-07,  2.37677146222502973e-23},
// /* 11! */ { 2.50521083854417202e-08, -1.44881407093591197e-24},
// /* 12! */ { 2.08767569878681002e-09, -1.20734505911325997e-25},
// /* 13! */ { 1.60590438368216133e-10,  1.25852945887520981e-26},
// /* 14! */ { 1.14707455977297245e-11,  2.06555127528307454e-28},
// /* 15! */ { 7.64716373181981641e-13,  7.03872877733453001e-30},
// /* 16! */ { 4.77947733238738525e-14,  4.39920548583408126e-31},
// /* 17! */ { 2.81145725434552060e-15,  1.65088427308614326e-31},
// };

/**
 * @remarks The original inv_fact went from 3! to 17!
 */
static constexpr Float64x2 inv_fact[] = {
// /*  0! */ {0x1.0000000000000p+0  ,+0x0.0000000000000p+0  },
// /*  1! */ {0x1.0000000000000p+0  ,+0x0.0000000000000p+0  },
// /*  2! */ {0x1.0000000000000p-1  ,+0x0.0000000000000p+0  },
/*  3! */ {0x1.5555555555555p-3  ,+0x1.5555555555555p-57 },
/*  4! */ {0x1.5555555555555p-5  ,+0x1.5555555555555p-59 },
/*  5! */ {0x1.1111111111111p-7  ,+0x1.1111111111111p-63 },
/*  6! */ {0x1.6c16c16c16c17p-10 ,-0x1.f49f49f49f49fp-65 },
/*  7! */ {0x1.a01a01a01a01ap-13 ,+0x1.a01a01a01a01ap-73 },
/*  8! */ {0x1.a01a01a01a01ap-16 ,+0x1.a01a01a01a01ap-76 },
/*  9! */ {0x1.71de3a556c734p-19 ,-0x1.c154f8ddc6c00p-73 },
/* 10! */ {0x1.27e4fb7789f5cp-22 ,+0x1.cbbc05b4fa99ap-76 },
/* 11! */ {0x1.ae64567f544e4p-26 ,-0x1.c062e06d1f209p-80 },
/* 12! */ {0x1.1eed8eff8d898p-29 ,-0x1.2aec959e14c06p-83 },
/* 13! */ {0x1.6124613a86d09p-33 ,+0x1.f28e0cc748ebep-87 },
/* 14! */ {0x1.93974a8c07c9dp-37 ,+0x1.05d6f8a2efd1fp-92 },
/* 15! */ {0x1.ae7f3e733b81fp-41 ,+0x1.1d8656b0ee8cbp-97 },
/* 16! */ {0x1.ae7f3e733b81fp-45 ,+0x1.1d8656b0ee8cbp-101},
/* 17! */ {0x1.952c77030ad4ap-49 ,+0x1.ac981465ddc6cp-103},
/* 18! */ {0x1.6827863b97d97p-53 ,+0x1.eec01221a8b0bp-107},
/* 19! */ {0x1.2f49b46814157p-57 ,+0x1.2650f61dbdcb4p-112},
/* 20! */ {0x1.e542ba4020225p-62 ,+0x1.ea72b4afe3c2fp-120},
/* 21! */ {0x1.71b8ef6dcf572p-66 ,-0x1.d043ae40c4647p-120},
/* 22! */ {0x1.0ce396db7f853p-70 ,-0x1.aebcdbd20331cp-124},
/* 23! */ {0x1.761b41316381ap-75 ,-0x1.3423c7d91404fp-130},
/* 24! */ {0x1.f2cf01972f578p-80 ,-0x1.9ada5fcc1ab14p-135},
/* 25! */ {0x1.3f3ccdd165fa9p-84 ,-0x1.58ddadf344487p-139},
/* 26! */ {0x1.88e85fc6a4e5ap-89 ,-0x1.71c37ebd16540p-143},
/* 27! */ {0x1.d1ab1c2dccea3p-94 ,+0x1.054d0c78aea14p-149},
/* 28! */ {0x1.0a18a2635085dp-98 ,+0x1.b9e2e28e1aa54p-153},
/* 29! */ {0x1.259f98b4358adp-103,+0x1.eaf8c39dd9bc5p-157},
/* 30! */ {0x1.3932c5047d60ep-108,+0x1.832b7b530a627p-162},
/* 31! */ {0x1.434d2e783f5bcp-113,+0x1.0b87b91be9affp-167},
/* 32! */ {0x1.434d2e783f5bcp-118,+0x1.0b87b91be9affp-172},
/* 33! */ {0x1.3981254dd0d52p-123,-0x1.2b1f4c8015a2fp-177},
/* 34! */ {0x1.2710231c0fd7ap-128,+0x1.3f8a2b4af9d6bp-184},
/* 35! */ {0x1.0dc59c716d91fp-133,+0x1.419e3fad3f031p-188},
/* 36! */ {0x1.df983290c2ca9p-139,+0x1.5835c6895393bp-194},
/* 37! */ {0x1.9ec8d1c94e85bp-144,-0x1.670e9d4784ec6p-201},
/* 38! */ {0x1.5d4acb9c0c3abp-149,-0x1.6ec2c8f5b13b2p-205},
/* 39! */ {0x1.1e99449a4bacep-154,-0x1.fefbb89514b3cp-210},
/* 40! */ {0x1.ca8ed42a12ae3p-160,+0x1.a07244abad2abp-224},
/* 41! */ {0x1.65e61c39d0241p-165,-0x1.c0ed181727269p-220},
/* 42! */ {0x1.10af527530de8p-170,+0x1.b626c912ee5c8p-225},
/* 43! */ {0x1.95db45257e512p-176,+0x1.6e5d72b6f79b9p-231},
/* 44! */ {0x1.272b1b03fec6ap-181,+0x1.3f67cc9f9fdb8p-235},
/* 45! */ {0x1.a3cb872220648p-187,-0x1.c7f4e85b8e6cdp-241},
/* 46! */ {0x1.240804f659510p-192,+0x1.8b291b93c9718p-246},
/* 47! */ {0x1.8da8e0a127ebap-198,-0x1.21d2eac9d275cp-252},
/* 48! */ {0x1.091b406b6ff26p-203,+0x1.e973637973b18p-257},
/* 49! */ {0x1.5a42f0dfeb086p-209,-0x1.35ae015f78f6ep-264},
/* 50! */ {0x1.bb36f6e12cd78p-215,+0x1.02f85029a29b0p-270}
};
static constexpr size_t n_inv_fact = sizeof(inv_fact) / sizeof(inv_fact[0]);

static constexpr Float64x2 inv_fact_even[] = {
// /*  0! */ {0x1.0000000000000p+0  ,+0x0.0000000000000p+0  },
// /*  2! */ {0x1.0000000000000p-1  ,+0x0.0000000000000p+0  },
/*  4! */ {0x1.5555555555555p-5  ,+0x1.5555555555555p-59 },
/*  6! */ {0x1.6c16c16c16c17p-10 ,-0x1.f49f49f49f49fp-65 },
/*  8! */ {0x1.a01a01a01a01ap-16 ,+0x1.a01a01a01a01ap-76 },
/* 10! */ {0x1.27e4fb7789f5cp-22 ,+0x1.cbbc05b4fa99ap-76 },
/* 12! */ {0x1.1eed8eff8d898p-29 ,-0x1.2aec959e14c06p-83 },
/* 14! */ {0x1.93974a8c07c9dp-37 ,+0x1.05d6f8a2efd1fp-92 },
/* 16! */ {0x1.ae7f3e733b81fp-45 ,+0x1.1d8656b0ee8cbp-101},
/* 18! */ {0x1.6827863b97d97p-53 ,+0x1.eec01221a8b0bp-107},
/* 20! */ {0x1.e542ba4020225p-62 ,+0x1.ea72b4afe3c2fp-120},
/* 22! */ {0x1.0ce396db7f853p-70 ,-0x1.aebcdbd20331cp-124},
/* 24! */ {0x1.f2cf01972f578p-80 ,-0x1.9ada5fcc1ab14p-135},
/* 26! */ {0x1.88e85fc6a4e5ap-89 ,-0x1.71c37ebd16540p-143},
/* 28! */ {0x1.0a18a2635085dp-98 ,+0x1.b9e2e28e1aa54p-153},
/* 30! */ {0x1.3932c5047d60ep-108,+0x1.832b7b530a627p-162},
/* 32! */ {0x1.434d2e783f5bcp-118,+0x1.0b87b91be9affp-172},
/* 34! */ {0x1.2710231c0fd7ap-128,+0x1.3f8a2b4af9d6bp-184},
/* 36! */ {0x1.df983290c2ca9p-139,+0x1.5835c6895393bp-194},
/* 38! */ {0x1.5d4acb9c0c3abp-149,-0x1.6ec2c8f5b13b2p-205},
/* 40! */ {0x1.ca8ed42a12ae3p-160,+0x1.a07244abad2abp-224},
/* 42! */ {0x1.10af527530de8p-170,+0x1.b626c912ee5c8p-225},
/* 44! */ {0x1.272b1b03fec6ap-181,+0x1.3f67cc9f9fdb8p-235},
/* 46! */ {0x1.240804f659510p-192,+0x1.8b291b93c9718p-246},
/* 48! */ {0x1.091b406b6ff26p-203,+0x1.e973637973b18p-257},
/* 50! */ {0x1.bb36f6e12cd78p-215,+0x1.02f85029a29b0p-270}
};
static constexpr size_t n_inv_fact_even = sizeof(inv_fact_even) / sizeof(inv_fact_even[0]);

static constexpr Float64x2 inv_fact_odd[] = {
// /*  1! */ {0x1.0000000000000p+0  ,+0x0.0000000000000p+0  },
/*  3! */ {0x1.5555555555555p-3  ,+0x1.5555555555555p-57 },
/*  5! */ {0x1.1111111111111p-7  ,+0x1.1111111111111p-63 },
/*  7! */ {0x1.a01a01a01a01ap-13 ,+0x1.a01a01a01a01ap-73 },
/*  9! */ {0x1.71de3a556c734p-19 ,-0x1.c154f8ddc6c00p-73 },
/* 11! */ {0x1.ae64567f544e4p-26 ,-0x1.c062e06d1f209p-80 },
/* 13! */ {0x1.6124613a86d09p-33 ,+0x1.f28e0cc748ebep-87 },
/* 15! */ {0x1.ae7f3e733b81fp-41 ,+0x1.1d8656b0ee8cbp-97 },
/* 17! */ {0x1.952c77030ad4ap-49 ,+0x1.ac981465ddc6cp-103},
/* 19! */ {0x1.2f49b46814157p-57 ,+0x1.2650f61dbdcb4p-112},
/* 21! */ {0x1.71b8ef6dcf572p-66 ,-0x1.d043ae40c4647p-120},
/* 23! */ {0x1.761b41316381ap-75 ,-0x1.3423c7d91404fp-130},
/* 25! */ {0x1.3f3ccdd165fa9p-84 ,-0x1.58ddadf344487p-139},
/* 27! */ {0x1.d1ab1c2dccea3p-94 ,+0x1.054d0c78aea14p-149},
/* 29! */ {0x1.259f98b4358adp-103,+0x1.eaf8c39dd9bc5p-157},
/* 31! */ {0x1.434d2e783f5bcp-113,+0x1.0b87b91be9affp-167},
/* 33! */ {0x1.3981254dd0d52p-123,-0x1.2b1f4c8015a2fp-177},
/* 35! */ {0x1.0dc59c716d91fp-133,+0x1.419e3fad3f031p-188},
/* 37! */ {0x1.9ec8d1c94e85bp-144,-0x1.670e9d4784ec6p-201},
/* 39! */ {0x1.1e99449a4bacep-154,-0x1.fefbb89514b3cp-210},
/* 41! */ {0x1.65e61c39d0241p-165,-0x1.c0ed181727269p-220},
/* 43! */ {0x1.95db45257e512p-176,+0x1.6e5d72b6f79b9p-231},
/* 45! */ {0x1.a3cb872220648p-187,-0x1.c7f4e85b8e6cdp-241},
/* 47! */ {0x1.8da8e0a127ebap-198,-0x1.21d2eac9d275cp-252},
/* 49! */ {0x1.5a42f0dfeb086p-209,-0x1.35ae015f78f6ep-264}
};
static constexpr size_t n_inv_fact_odd = sizeof(inv_fact_odd) / sizeof(inv_fact_odd[0]);

//------------------------------------------------------------------------------
// Float64x2 Logarithms and Exponents
//------------------------------------------------------------------------------

/** 
 * @brief Exponential.  Computes exp(x) in double-double precision.
 *
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 *
 * @remarks This was originally the exp(x) function itself. I realized that
 * this function is able to properly calculate expm1(x) when x is between
 * -0.5ln(2) and +0.5ln(2). So the function has been modified so it can be used
 * by both the exp(x) and the expm1(x) function.
 */
static inline Float64x2 taylor_expm1(const Float64x2& x, fp64& m) {
	/* Strategy:  We first reduce the size of x by noting that
		
			exp(kr + m * log(2)) = 2^m * exp(r)^k

		where m and k are integers.  By choosing m appropriately
		we can make |kr| <= log(2) / 2 = 0.347.  Then exp(r) is 
		evaluated using the familiar Taylor series.  Reducing the 
		argument substantially speeds up the convergence.       */  

	constexpr fp64 k = 512.0;
	constexpr fp64 inv_k = 1.0 / k;

	m = floor(x.hi * LDF::const_log2e<fp64>() + static_cast<fp64>(0.5));
	Float64x2 r = mul_pwr2(x - LDF::const_ln2<Float64x2>() * m, inv_k);
	Float64x2 s, t, p;
	const fp64 thresh = inv_k * std::numeric_limits<Float64x2>::epsilon().hi;

	p = square(r);
	s = r + mul_pwr2(p, static_cast<fp64>(0.5));
	p *= r;
	t = p * inv_fact[0];
	size_t i = 0;
	// Originally set to `i < 5`, adding another term will sometimes improve precision.
	do {
		s += t;
		p *= r;
		++i;
		t = p * inv_fact[i];
	} while (fabs(t.hi) > thresh && (i < 6 && i < n_inv_fact));

	s += t;

	s = mul_pwr2(s, static_cast<fp64>(2.0)) + square(s);
	s = mul_pwr2(s, static_cast<fp64>(2.0)) + square(s);
	s = mul_pwr2(s, static_cast<fp64>(2.0)) + square(s);

	s = mul_pwr2(s, static_cast<fp64>(2.0)) + square(s);
	s = mul_pwr2(s, static_cast<fp64>(2.0)) + square(s);
	s = mul_pwr2(s, static_cast<fp64>(2.0)) + square(s);

	s = mul_pwr2(s, static_cast<fp64>(2.0)) + square(s);
	s = mul_pwr2(s, static_cast<fp64>(2.0)) + square(s);
	s = mul_pwr2(s, static_cast<fp64>(2.0)) + square(s);
	
	return s;
	// Original return code:
	// s += 1.0;
	// return ldexp(s, static_cast<int>(m));
}

Float64x2 exp(const Float64x2& x) {
	if (x.hi <= LDF::LDF_Input_Limits::exp_min<Float64x2, fp64>()) {
		// Gives a better approximation near extreme values
		return exp(x.hi);
	}
	/* ln(2^1023 * (1 + (1 - 2^-52)))) = ~709.782712893 */
	if (x.hi >= LDF::LDF_Input_Limits::exp_max<Float64x2, fp64>()) {
		return std::numeric_limits<Float64x2>::infinity();
	}
	if (isequal_zero(x)) {
		return static_cast<Float64x2>(1.0);
	}
	if (x == static_cast<fp64>(1.0)) {
		return LDF::const_e<Float64x2>();
	}
	if (x == static_cast<fp64>(-1.0)) {
		return LDF::const_inv_e<Float64x2>();
	}

	fp64 m;
	Float64x2 ret = taylor_expm1(x, m);
	ret += static_cast<fp64>(1.0);
	return ldexp(ret, static_cast<int>(m));
}

Float64x2 expm1(const Float64x2& x) {
	if (x.hi <= LDF::LDF_Input_Limits::expm1_min<Float64x2, fp64>()) {
		return static_cast<Float64x2>(-1.0);
	}
	if (x.hi >= LDF::LDF_Input_Limits::expm1_max<Float64x2, fp64>()) {
		return std::numeric_limits<Float64x2>::infinity();
	}
	if (isequal_zero(x)) {
		return static_cast<Float64x2>(0.0);
	}
	fp64 m;
	Float64x2 ret = taylor_expm1(x, m);
	/**
	 * @remarks LDF::const_ln2<fp64>() is less than LDF::const_ln2<Float64x2>(). If this were not the
	 * case, then one would have to compare against
	 * nextafter(LDF::const_ln2<fp64>(), 0.0) to ensure the function behaves
	 * correctly when `x` is very close to `ln2`.
	 *
	 * Although it still uses nextafter just to be safe and avoid pontential
	 * rounding errors.
	 */
	if (fabs(x) < mul_pwr2(static_cast<fp64>(0.5), LDF::const_ln2<Float64x2>())) {
		return ret; // expm1 to higher accuracy
	}
	ret += static_cast<fp64>(1.0);
	ret = ldexp(ret, static_cast<int>(m));
	return ret - static_cast<fp64>(1.0); // expm1 to standard accuracy
}

/** 
 * @brief Logarithm.  Computes log(x) in double-double precision.
 * @note This is a natural logarithm (i.e., base e).
 *
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
Float64x2 log(const Float64x2& x) {
	/* Strategy.  The Taylor series for log converges much more
		slowly than that of exp, due to the lack of the factorial
		term in the denominator.  Hence this routine instead tries
		to determine the root of the function

			f(x) = exp(x) - a

		using Newton iteration.  The iteration is given by

			x' = x - f(x)/f'(x) 
			= x - (1 - a * exp(-x))
			= x + a * exp(-x) - 1.
			
		Only one iteration is needed, since Newton's iteration
		approximately doubles the number of digits per iteration. */

	if (x == static_cast<fp64>(1.0)) {
		return static_cast<fp64>(0.0);
	}

	if (islessequal_zero(x)) {
		if (isequal_zero(x)) {
			std::feraiseexcept(FE_DIVBYZERO);
			return -std::numeric_limits<Float64x2>::infinity();
		}
		// Float64x2::error("(Float64x2::log): Non-positive argument.")
		std::feraiseexcept(FE_INVALID);
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	Float64x2 guess = log(x.hi);   /* Initial approximation */
	return guess.hi + x * exp(-guess) - static_cast<fp64>(1.0);
}

Float64x2 log1p(const Float64x2& x) {
	if (fabs(x) < static_cast<fp64>(0x1.0p-128)) {
		return x;
	}
	if (fabs(x) < static_cast<fp64>(0x1.0p-66)) {
		return x - mul_pwr2(square(x), static_cast<fp64>(0.5));
	}
	// Use 24 iterations and fabs(x) < 0.0626 for 100bits of precision
	// Use 8 iterations and fabs(x) < 0x1.0p-10 for 94bits of precision
	if (fabs(x) < static_cast<fp64>(0x1.0p-10)) {
		Float64x2 x_mult = square(x);
		fp64 x_div = 2.0;
		Float64x2 ret = x - mul_pwr2(square(x), static_cast<fp64>(0.5));
		
		for (int i = 0; i < 8; i += 2) {
			x_mult *= x;
			x_div++;
			ret += x_mult / x_div;
			x_mult *= x;
			x_div++;
			ret -= x_mult / x_div;
		}
		return ret;
	}
	if (x <= static_cast<fp64>(-1.0)) {
		if (x == static_cast<fp64>(-1.0)) {
			std::feraiseexcept(FE_DIVBYZERO);
			return -std::numeric_limits<Float64x2>::infinity();
		}
		// Float64x2::error("(Float64x2::log): Non-positive argument.");
		std::feraiseexcept(FE_INVALID);
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	return log(x + static_cast<fp64>(1.0));
}

//------------------------------------------------------------------------------
// Float64x2 Power functions
//------------------------------------------------------------------------------

/**
 * @remarks Based off of https://en.cppreference.com/w/cpp/numeric/math/pow
 * Retrived 2024, October 9th
 */
Float64x2 pow(const Float64x2& base, const Float64x2& expon) {
	const bool expon_is_integer = (isfinite(expon) && expon == trunc(expon));
	const bool expon_is_even = (
		expon_is_integer &&
		trunc(mul_pwr2(expon, static_cast<fp64>(0.5))) == mul_pwr2(expon, static_cast<fp64>(0.5))
	);

	/* basic tests */

	// pow(+1.0, expon) or pow(base, 0.0)
	if (base == static_cast<fp64>(1.0) || isequal_zero(expon)) {
		return static_cast<Float64x2>(1.0);
	}
	// NaN arguments
	if (isunordered(base, expon)) {
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}
	// pow(negative, non_integer)
	if (isfinite(base) && signbit(base) && !expon_is_integer) {
		std::feraiseexcept(FE_INVALID);
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	/* expon is infinity */

	// pow(base, inf)
	if (isinf(expon)) {
		// pow(-1.0, inf)
		if (base == static_cast<fp64>(-1.0)) {
			return static_cast<Float64x2>(1.0);
		}
		// pow(base, -inf)
		if (signbit(expon)) {
			if (isequal_zero(base)) {
				std::feraiseexcept(FE_DIVBYZERO);
				return std::numeric_limits<Float64x2>::infinity();
			}
			if (fabs(base) < static_cast<fp64>(1.0)) {
				return std::numeric_limits<Float64x2>::infinity();
			}
			return static_cast<Float64x2>(0.0);
		}
		// pow(base, +inf)
		if (fabs(base) < static_cast<fp64>(1.0)) {
			return static_cast<Float64x2>(0.0);
		}
		return std::numeric_limits<Float64x2>::infinity();
	}

	/* base is infinity */

	// pow(inf, expon)
	if (isinf(base)) {
		// pow(-inf, expon)
		if (signbit(base)) {
			// pow(-inf, integer)
			if (expon_is_integer) {
				// pow(-inf, negative_integer)
				if (signbit(expon)) {
					return expon_is_even ?
						static_cast<Float64x2>(0.0) :
						static_cast<Float64x2>(-0.0);
				}
				// pow(-inf, positive_integer)
				return expon_is_even ?
					std::numeric_limits<Float64x2>::infinity() :
					-std::numeric_limits<Float64x2>::infinity();
			}
			// pow(-inf, non_integer)
			return signbit(expon) ?
				static_cast<Float64x2>(0.0) :
				std::numeric_limits<Float64x2>::infinity();
		}
		// pow(+inf, positive)
		return signbit(expon) ?
			static_cast<Float64x2>(0.0) :
			std::numeric_limits<Float64x2>::infinity();
	}

	/* base is zero */

	// pow(0.0, expon)
	if (isequal_zero(base)) {
		if (signbit(expon)) {
			// pow(-0.0, negative_odd_integer)
			if (signbit(base) && !expon_is_even) {
				std::feraiseexcept(FE_DIVBYZERO);
				return -std::numeric_limits<Float64x2>::infinity();
			}
			// pow(0.0, negative)
			std::feraiseexcept(FE_DIVBYZERO);
			return std::numeric_limits<Float64x2>::infinity();
		}

		// pow(-0.0, positive_odd_integer)
		if (signbit(base) && !expon_is_even) {
			return static_cast<Float64x2>(-0.0);
		}
		// pow(0.0, positive)
		return static_cast<Float64x2>(0.0);
	}

	/* calculate powr(base, expon) */

	return powr(base, expon);
}


/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
Float64x2 pown(const Float64x2& x, int n) {
	if (n == 0) {
		return static_cast<fp64>(1.0);
	}
	if (isequal_zero(x)) {
		if (n > 0) {
			return static_cast<fp64>(0.0);
		}
		std::feraiseexcept(FE_INVALID);
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	Float64x2 r = x;
	Float64x2 s = static_cast<fp64>(1.0);
	// casts to unsigned int since abs(INT_MIN) < 0
	unsigned int N = static_cast<unsigned int>((n < 0) ? -n : n);

	if (N > 1) {
		/* Use binary exponentiation */
		while (N > 0) {
			if (N % 2 == 1) {
				s *= r;
			}
			N /= 2;
			if (N > 0) {
				r = square(r);
			}
		}
	} else {
		s = r;
	}

	/* Compute the reciprocal if n is negative. */
	if (n < 0) {
		return recip(s);
	}
	return s;
}

Float64x2 rootn(const Float64x2& x, int n) {
	switch (n) {
		case -3:
			return recip(cbrt(x));
		case -2:
			return recip(sqrt(x));
		case -1:
			return recip(x);
		case 1:
			return x;
		case 2:
			return sqrt(x);
		case 3:
			return cbrt(x);
		case 0: // reciprocal of +0.0 is +inf
			return pow(x, std::numeric_limits<Float64x2>::infinity());
		default:
			return pow(x, recip(static_cast<Float64x2>(n)));
	}
}

//------------------------------------------------------------------------------
// Float64x2 Trigonometry
//------------------------------------------------------------------------------

/* Table of sin(k * pi/16) and cos(k * pi/16). */
static constexpr Float64x2 sin_table [4] = {
	{1.950903220161282758e-01, -7.991079068461731263e-18},
	{3.826834323650897818e-01, -1.005077269646158761e-17},
	{5.555702330196021776e-01, +4.709410940561676821e-17},
	{7.071067811865475727e-01, -4.833646656726456726e-17}
};

static constexpr Float64x2 cos_table [4] = {
	{9.807852804032304306e-01, +1.854693999782500573e-17},
	{9.238795325112867385e-01, +1.764504708433667706e-17},
	{8.314696123025452357e-01, +1.407385698472802389e-18},
	{7.071067811865475727e-01, -4.833646656726456726e-17}
};

/** 
 * @brief Computes sin(a) using Taylor series.
 * @note Assumes |a| <= pi/32.
 *
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
static Float64x2 sin_taylor(const Float64x2& x) {
	const fp64 thresh = 0.5 * fabs(x.hi) * std::numeric_limits<Float64x2>::epsilon().hi;
	Float64x2 x_power, sum, term;

	if (isequal_zero(x)) {
		return 0.0;
	}

	size_t i = 0;
	const Float64x2 neg_x_squared = -square(x);
	x_power = x;

	// sum = x;
	sum = 0.0;
	do {
		x_power *= neg_x_squared;
		term = x_power * inv_fact_odd[i];
		sum += term;
		++i;
	} while (i < n_inv_fact_odd && fabs(term.hi) > thresh);

	// return sum;
	/**
	 * @remarks Adding x towards the end improves the ULP slightly
	 */
	return sum + x;
}

/** 
 * @brief Computes cos(a) using Taylor series.
 * @note Assumes |a| <= pi/32.
 *
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
static Float64x2 cos_taylor(const Float64x2 &x) {
	const fp64 thresh = 0.5 * std::numeric_limits<Float64x2>::epsilon().hi;
	Float64x2 x_power, sum, term;

	if (isequal_zero(x)) {
		return 1.0;
	}

	const Float64x2 neg_x_squared = -square(x);
	x_power = neg_x_squared;

	// sum = static_cast<fp64>(1.0) + mul_pwr2(x_power, 0.5);
	sum = 0.0;

	size_t i = 0;
	do {
		x_power *= neg_x_squared;
		term = x_power * inv_fact_even[i];
		sum += term;
		++i;
	} while (i < n_inv_fact_even && fabs(term.hi) > thresh);

	// return sum;
	/**
	 * @remarks Adding the first two terms towards the end improves the ULP slightly
	 */
	return (sum + mul_pwr2(neg_x_squared, 0.5)) + static_cast<fp64>(1.0);
}

/** 
 * @brief Computes sin(a) and cos(a) using Taylor series.
 * @note Assumes |a| <= pi/32.
 *
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
static void sincos_taylor(
	const Float64x2 &a, Float64x2 &p_sin, Float64x2 &p_cos
) {
	if (isequal_zero(a)) {
		p_sin = 0.0;
		p_cos = 1.0;
		return;
	}

	p_sin = sin_taylor(a);
	p_cos = sqrt(static_cast<fp64>(1.0) - square(p_sin));
}

static constexpr Float64x4 Float64x4_pi16 = {0x1.921fb54442d18p-3,+0x1.1a62633145c07p-57,-0x1.f1976b7ed8fbcp-113,+0x1.4cf98e804177dp-167};

static inline void trig_modulo(
	const Float64x2& x, Float64x2& ret, Float64x2& t,
	int& j, int& k
) {
	#if 0
		// approximately reduce modulo 2*pi
		Float64x2 z = round(x / Float64x2_2pi);
		ret = (x - Float64x2_2pi * z);

		// approximately reduce modulo pi/2 and then modulo pi/16.
		fp64 q = floor(ret.hi / LDF::const_pi2<fp64>() + 0.5);
		t = ret - LDF::const_pi2<Float64x2>() * q;
		j = static_cast<int>(q);
		q = floor(t.hi / Float64x2_pi16.hi + 0.5);
		t -= Float64x2_pi16 * q;
		k = static_cast<int>(q);
	#elif 0
		// approximately reduce modulo 2*pi
		Float64x4 z = round(x / Float64x4_2pi);
		ret = static_cast<Float64x2>(x - Float64x4_2pi * z);

		// approximately reduce modulo pi/2 and then modulo pi/16.
		fp64 q = floor(ret.hi / LDF::const_pi2<fp64>() + 0.5);
		t = ret - LDF::const_pi2<Float64x2>() * q;
		j = static_cast<int>(q);
		q = floor(t.hi / Float64x2_pi16.hi + 0.5);
		t -= Float64x2_pi16 * q;
		k = static_cast<int>(q);
	#elif 0
		// approximately reduce modulo 2*pi
		Float64x4 z = round(x / Float64x4_2pi);
		ret = static_cast<Float64x2>(x - Float64x4_2pi * z);

		// approximately reduce modulo pi/2 and then modulo pi/16.
		Float64x2 q = floor(ret / LDF::const_pi2<Float64x2>() + 0.5);
		t = ret - LDF::const_pi2<Float64x2>() * q;
		j = static_cast<int>(q);
		q = floor(t / Float64x2_pi16 + 0.5);
		t -= Float64x2_pi16 * q;
		k = static_cast<int>(q);
	#else
		// approximately reduce modulo 2*pi
		Float64x4 z = round(x / LDF::const_2pi<Float64x4>());
		Float64x4 ret_temp = x - LDF::const_2pi<Float64x4>() * z;
		
		// approximately reduce modulo pi/2 and then modulo pi/16.
		Float64x4 q = floor(ret_temp / LDF::const_pi2<Float64x4>() + 0.5);
		Float64x4 t_temp = ret_temp - LDF::const_pi2<Float64x4>() * q;
		j = static_cast<int>(q);
		q = floor(t / Float64x4_pi16 + 0.5);
		t_temp -= Float64x4_pi16 * q;
		k = static_cast<int>(q);
		ret = static_cast<Float64x2>(ret_temp);
		t = static_cast<Float64x2>(t_temp);
	#endif
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
Float64x2 sin(const Float64x2& x) {  

	/* Strategy.  To compute sin(x), we choose integers a, b so that

		x = s + a * (pi/2) + b * (pi/16)

		and |s| <= pi/32.  Using the fact that 

		sin(pi/16) = 0.5 * sqrt(2 - sqrt(2 + sqrt(2)))

		we can compute sin(x) from sin(s), cos(s).  This greatly 
		increases the convergence of the sine Taylor series. */

	if (isequal_zero(x)) {
		return 0.0;
	}

	Float64x2 ret, t;
	int j, k;
	trig_modulo(x, ret, t, j, k);
	unsigned int abs_k = static_cast<unsigned int>(abs(k));

	if (j < -2 || j > 2) {
		// Float64x2::error("(Float64x2::sin): Cannot reduce modulo pi/2.");
		// printf("ERROR: %+#.10Lg Cannot reduce modulo pi/2.\n", static_cast<long double>(x));
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	if (abs_k > 4) {
		// Float64x2::error("(Float64x2::sin): Cannot reduce modulo pi/16.");
		// printf("ERROR: %+#.10Lg Cannot reduce modulo pi/16.\n", static_cast<long double>(x));
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	if (k == 0) {
		switch (j) {
			case 0:
				return sin_taylor(t);
			case 1:
				return cos_taylor(t);
			case -1:
				return -cos_taylor(t);
			default:
				return -sin_taylor(t);
		}
	}

	Float64x2 u(cos_table[abs_k-1].hi, cos_table[abs_k-1].lo);
	Float64x2 v(sin_table[abs_k-1].hi, sin_table[abs_k-1].lo);
	Float64x2 t_sin, t_cos;
	sincos_taylor(t, t_sin, t_cos);

	if (j == 0) {
		if (k > 0) {
			ret = u * t_sin + v * t_cos;
		} else {
			ret = u * t_sin - v * t_cos;
		}
	} else if (j == 1) {
		if (k > 0) {
			ret = u * t_cos - v * t_sin;
		} else {
			ret = u * t_cos + v * t_sin;
		}
	} else if (j == -1) {
		if (k > 0) {
			ret = v * t_sin - u * t_cos;
		} else if (k < 0) {
			ret = -u * t_cos - v * t_sin;
		}
	} else {
		if (k > 0) {
			ret = -u * t_sin - v * t_cos;
		} else {
			ret = v * t_cos - u * t_sin;
		}
	}
	return ret;
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
Float64x2 cos(const Float64x2& x) {

	if (isequal_zero(x)) {
		return 1.0;
	}

	Float64x2 ret, t;
	int j, k;
	trig_modulo(x, ret, t, j, k);
	unsigned int abs_k = static_cast<unsigned int>(abs(k));

	if (j < -2 || j > 2) {
		// Float64x2::error("(Float64x2::cos): Cannot reduce modulo pi/2.");
		// printf("ERROR: %+#.10Lg Cannot reduce modulo pi/2.\n", static_cast<long double>(x));
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	if (abs_k > 4) {
		// Float64x2::error("(Float64x2::cos): Cannot reduce modulo pi/16.");
		// printf("ERROR: %+#.10Lg Cannot reduce modulo pi/16.\n", static_cast<long double>(x));
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	if (k == 0) {
		switch (j) {
			case 0:
				return cos_taylor(t);
			case 1:
				return -sin_taylor(t);
			case -1:
				return sin_taylor(t);
			default:
				return -cos_taylor(t);
		}
	}

	Float64x2 t_sin, t_cos;
	sincos_taylor(t, t_sin, t_cos);
	Float64x2 u(cos_table[abs_k-1].hi, cos_table[abs_k-1].lo);
	Float64x2 v(sin_table[abs_k-1].hi, sin_table[abs_k-1].lo);

	if (j == 0) {
		if (k > 0) {
			ret = u * t_cos - v * t_sin;
		} else {
			ret = u * t_cos + v * t_sin;
		}
	} else if (j == 1) {
		if (k > 0) {
			ret = - u * t_sin - v * t_cos;
		} else {
			ret = v * t_cos - u * t_sin;
		}
	} else if (j == -1) {
		if (k > 0) {
			ret = u * t_sin + v * t_cos;
		} else {
			ret = u * t_sin - v * t_cos;
		}
	} else {
		if (k > 0) {
			ret = v * t_sin - u * t_cos;
		} else {
			ret = - u * t_cos - v * t_sin;
		}
	}

	return ret;
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
void sincos(const Float64x2& x, Float64x2& p_sin, Float64x2& p_cos) {

	if (isequal_zero(x)) {
		p_sin = static_cast<fp64>(0.0);
		p_cos = static_cast<fp64>(1.0);
		return;
	}

	Float64x2 ret, t;
	int j, k;
	trig_modulo(x, ret, t, j, k);
	unsigned int abs_k = static_cast<unsigned int>(abs(k));
	unsigned int abs_j = static_cast<unsigned int>(abs(j));

	if (abs_j > 2) {
		// Float64x2::error("(Float64x2::sincos): Cannot reduce modulo pi/2.");
		p_cos = std::numeric_limits<Float64x2>::quiet_NaN();
		p_sin = std::numeric_limits<Float64x2>::quiet_NaN();
		return;
	}

	if (abs_k > 4) {
		// Float64x2::error("(Float64x2::sincos): Cannot reduce modulo pi/16.");
		p_cos = std::numeric_limits<Float64x2>::quiet_NaN();
		p_sin = std::numeric_limits<Float64x2>::quiet_NaN();
		return;
	}

	Float64x2 t_sin, t_cos;
	Float64x2 sin_val, cos_val;

	sincos_taylor(t, t_sin, t_cos);

	if (abs_k == 0) {
		sin_val = t_sin;
		cos_val = t_cos;
	} else {
		Float64x2 u(cos_table[abs_k-1].hi, cos_table[abs_k-1].lo);
		Float64x2 v(sin_table[abs_k-1].hi, sin_table[abs_k-1].lo);
		if (k > 0) {
			sin_val = u * t_sin + v * t_cos;
			cos_val = u * t_cos - v * t_sin;
		} else {
			sin_val = u * t_sin - v * t_cos;
			cos_val = u * t_cos + v * t_sin;
		}
	}

	if (abs_j == 0) {
		p_sin = sin_val;
		p_cos = cos_val;
	} else if (j == 1) {
		p_sin = cos_val;
		p_cos = -sin_val;
	} else if (j == -1) {
		p_sin = -cos_val;
		p_cos = sin_val;
	} else {
		p_sin = -sin_val;
		p_cos = -cos_val;
	}
}

//------------------------------------------------------------------------------
// Float64x2 Special Trigonometry
//------------------------------------------------------------------------------

	Float64x2 sinpi(const Float64x2& x) {
		if (!isfinite(x)) {
			if (isnan(x)) {
				return x;
			}
			return std::numeric_limits<Float64x2>::quiet_NaN();
		}

		if (isequal_zero(x)) {
			return x;
		}

		const fp64 max_safe_int = scalbn(x, std::numeric_limits<Float64x2>::digits);
		
		if (fabs(x) >= max_safe_int) {
			// behaviour at sin(pi * 0)
			return static_cast<Float64x2>(0.0);
		}
		
		Float64x2 x_mod = fmod(x, static_cast<Float64x2>(2.0));
		
		if (x_mod == -1.5) { return  1.0; }
		if (x_mod == -1.0) { return  0.0; }
		if (x_mod == -0.5) { return -1.0; }
		if (x_mod ==  0.0) { return  0.0; }
		if (x_mod ==  0.5) { return  1.0; }
		if (x_mod ==  1.0) { return  0.0; }
		if (x_mod ==  1.5) { return -1.0; }
		
		return sin(x * LDF::const_pi<Float64x2>());
	}

	Float64x2 cospi(const Float64x2& x) {
		if (!isfinite(x)) {
			if (isnan(x)) {
				return x;
			}
			return std::numeric_limits<Float64x2>::quiet_NaN();
		}

		if (isequal_zero(x)) {
			return 1.0;
		}

		const fp64 max_safe_int = scalbn(x, std::numeric_limits<Float64x2>::digits);
		
		if (fabs(x) >= 2.0 * max_safe_int) {
			// behaviour at cos(pi * 0)
			return static_cast<Float64x2>(1.0);
		}
		
		Float64x2 x_mod = fmod(x, static_cast<Float64x2>(2.0));
		
		if (x_mod == -1.5) { return  0.0; }
		if (x_mod == -1.0) { return -1.0; }
		if (x_mod == -0.5) { return  0.0; }
		if (x_mod ==  0.0) { return  1.0; }
		if (x_mod ==  0.5) { return  0.0; }
		if (x_mod ==  1.0) { return -1.0; }
		if (x_mod ==  1.5) { return  0.0; }
		
		return cos(x * LDF::const_pi<Float64x2>());
	}

	Float64x2 tanpi(const Float64x2& x) {
		if (!isfinite(x)) {
			if (isnan(x)) {
				return x;
			}
			return std::numeric_limits<Float64x2>::quiet_NaN();
		}
		
		if (isequal_zero(x)) {
			// preserves signed zero
			return x;
		}

		const fp64 max_safe_int = scalbn(x, std::numeric_limits<Float64x2>::digits);
		
		if (fabs(x) >= max_safe_int) {
			// behaviour at tan(pi * 0)
			return static_cast<Float64x2>(0.0);
		}
		
		Float64x2 x_mod = fmod(x, static_cast<Float64x2>(1.0));
		
		if (x_mod == -0.5 ) {
			std::feraiseexcept(FE_DIVBYZERO);
			return std::numeric_limits<Float64x2>::quiet_NaN();
		}
		if (x_mod ==  0.25) { return -1.0; }
		if (x_mod ==  0.0 ) { return  0.0; }
		if (x_mod ==  0.25) { return  1.0; }
		if (x_mod ==  0.5 ) {
			std::feraiseexcept(FE_DIVBYZERO);
			return std::numeric_limits<Float64x2>::quiet_NaN();
		}
		
		return tan(x * LDF::const_pi<Float64x2>());
	}

//------------------------------------------------------------------------------
// Float64x2 Inverse Trigonometry
//------------------------------------------------------------------------------

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
static Float64x2 internal_atan(const Float64x2& y, bool div_pi) {
	/* Strategy: Instead of using Taylor series to compute 
		arctan, we instead use Newton's iteration to solve
		the equation

		sin(z) = y/r    or    cos(z) = x/r

		where r = sqrt(x^2 + y^2).
		The iteration is given by

		z' = z + (y - sin(z)) / cos(z)          (for equation 1)
		z' = z - (x - cos(z)) / sin(z)          (for equation 2)

		Here, x and y are normalized so that x^2 + y^2 = 1.
		If |x| > |y|, then first iteration is used since the 
		denominator is larger.  Otherwise, the second is used.
	*/

	if (isequal_zero(y)) {
		return static_cast<fp64>(0.0);
	}

	if (y == static_cast<fp64>(1.0)) {
		return div_pi ? LDF::const_pi4<Float64x2>() : static_cast<Float64x2>(0.25);
	}

	if (y == static_cast<fp64>(-1.0)) {
		return div_pi ? -LDF::const_pi4<Float64x2>() : static_cast<Float64x2>(-0.25);
	}

	Float64x2 r = sqrt(static_cast<fp64>(1.0) + square(y));
	Float64x2 xx = recip(r);
	Float64x2 yy = y / r;

	/* Compute double precision approximation to atan. */
	Float64x2 z = atan(y.hi);
	Float64x2 sin_z, cos_z;

	if (fabs(xx.hi) > fabs(yy.hi)) {
		/* Use Newton iteration 1.  z' = z + (y - sin(z)) / cos(z)  */
		sincos(z, sin_z, cos_z);
		z += (yy - sin_z) / cos_z;
	} else {
		/* Use Newton iteration 2.  z' = z - (x - cos(z)) / sin(z)  */
		sincos(z, sin_z, cos_z);
		z -= (xx - cos_z) / sin_z;
	}

	if (div_pi) {
		return z * LDF::const_inv_pi<Float64x2>();
	}
	return z;
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
static Float64x2 internal_atan2(const Float64x2& y, const Float64x2& x, bool div_pi) {
	/* Strategy: Instead of using Taylor series to compute 
		arctan, we instead use Newton's iteration to solve
		the equation

		sin(z) = y/r    or    cos(z) = x/r

		where r = sqrt(x^2 + y^2).
		The iteration is given by

		z' = z + (y - sin(z)) / cos(z)          (for equation 1)
		z' = z - (x - cos(z)) / sin(z)          (for equation 2)

		Here, x and y are normalized so that x^2 + y^2 = 1.
		If |x| > |y|, then first iteration is used since the 
		denominator is larger.  Otherwise, the second is used.
	*/

	if (isequal_zero(x)) {
	
		if (isequal_zero(y)) {
			// /* Both x and y is zero. */
			// Float64x2::error("(Float64x2::atan2): Both arguments zero.");
			std::feraiseexcept(FE_INVALID);
			return std::numeric_limits<Float64x2>::quiet_NaN();
		}

		return isgreater_zero(y) ?
			(div_pi ? static_cast<Float64x2>( 0.5) :  LDF::const_pi2<Float64x2>()) :
			(div_pi ? static_cast<Float64x2>(-0.5) : -LDF::const_pi2<Float64x2>());
	} else if (isequal_zero(y)) {
		return isgreater_zero(x) ?
			Float64x2(0.0) :
			(div_pi ? Float64x2(1.0) : LDF::const_pi<Float64x2>());
	}

	if (x == y) {
		return isgreater_zero(y) ?
			(div_pi ? static_cast<Float64x2>( 0.25) :  LDF::const_pi4<Float64x2>()) :
			(div_pi ? static_cast<Float64x2>(-0.75) : -LDF::const_3pi4<Float64x2>());
	}

	if (x == -y) {
		return isgreater_zero(y) ?
			(div_pi ? static_cast<Float64x2>( 0.75) :  LDF::const_3pi4<Float64x2>()) :
			(div_pi ? static_cast<Float64x2>(-0.25) : -LDF::const_pi4<Float64x2>());
	}

	/* Compute double precision approximation to atan. */
	const Float64x2 radius = hypot(x, y);
	Float64x2 z = atan2(y.hi, x.hi);
	Float64x2 sin_z, cos_z;
	sincos(z, sin_z, cos_z);

	if (fabs(x.hi) > fabs(y.hi)) {
		/* Use Newton iteration 1.  z' = z + (y - sin(z)) / cos(z)  */
		z += ((y / radius) - sin_z) / cos_z;
	} else {
		/* Use Newton iteration 2.  z' = z - (x - cos(z)) / sin(z)  */
		z -= ((x / radius) - cos_z) / sin_z;
	}

	if (div_pi) {
		return z * LDF::const_inv_pi<Float64x2>();
	}
	return z;
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
static Float64x2 internal_asin(const Float64x2& x, bool div_pi) {
	if (isequal_zero(x)) {
		return static_cast<fp64>(0.0);
	}
	const Float64x2 abs_x = fabs(x);

	if (abs_x > static_cast<fp64>(1.0)) {
		// Float64x2::error("(Float64x2::asin): Argument out of domain.");
		std::feraiseexcept(FE_INVALID);
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	if (abs_x == static_cast<fp64>(1.0)) {
		
		return isgreater_zero(x) ?
			(div_pi ? static_cast<Float64x2>( 0.5) :  LDF::const_pi2<Float64x2>()) :
			(div_pi ? static_cast<Float64x2>(-0.5) : -LDF::const_pi2<Float64x2>());
	}

	return atan2(x, sqrt(static_cast<fp64>(1.0) - square(x)));
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
static Float64x2 internal_acos(const Float64x2& x, bool div_pi) {
	if (isequal_zero(x)) {
		return LDF::const_pi2<Float64x2>();
	}
	const Float64x2 abs_x = fabs(x);

	if (abs_x > static_cast<fp64>(1.0)) {
		// Float64x2::error("(Float64x2::acos): Argument out of domain.");
		std::feraiseexcept(FE_INVALID);
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	if (abs_x == static_cast<fp64>(1.0)) {
		return isgreater_zero(x) ?
			Float64x2(0.0) :
			(div_pi ? Float64x2(1.0) : LDF::const_pi<Float64x2>());
	}

	return atan2(sqrt(static_cast<fp64>(1.0) - square(x)), x);
}

Float64x2 asin(const Float64x2& x) {
	return internal_asin(x, false);
}

Float64x2 acos(const Float64x2& x) {
	return internal_acos(x, false);
}

Float64x2 atan(const Float64x2& x) {
	return internal_atan(x, false);
}

Float64x2 atan2(const Float64x2& y, const Float64x2& x) {
	return internal_atan2(y, x, false);
}

Float64x2 asinpi(const Float64x2& x) {
	return internal_asin(x, true);
}

Float64x2 acospi(const Float64x2& x) {
	return internal_acos(x, true);
}

Float64x2 atanpi(const Float64x2& x) {
	return internal_atan(x, true);
}

Float64x2 atan2pi(const Float64x2& y, const Float64x2& x) {
	return internal_atan2(y, x, true);
}

//------------------------------------------------------------------------------
// Float64x2 Hyperbolic Trigonometry
//------------------------------------------------------------------------------

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
Float64x2 sinh(const Float64x2& x) {
	if (isequal_zero(x)) {
		return static_cast<Float64x2>(0.0);
	}

	if (fabs(x.hi) > static_cast<fp64>(0.05)) {
		Float64x2 ea = exp(x);
		return mul_pwr2((ea - recip(ea)), 0.5);
	}

	/* since x is small, using the above formula gives
		a lot of cancellation.  So use Taylor series.   */
	

	Float64x2 t = x;
	Float64x2 r = square(t);
	const fp64 thresh = fabs(x.hi * std::numeric_limits<Float64x2>::epsilon().hi);

	fp64 m = 3.0;
	t *= r;
	t /= (m - 1.0) * m;

	// Float64x2 s = x + t;
	Float64x2 s = t;

	while (fabs(t.hi) > thresh) {
		m += 2.0;
		t *= r;
		t /= (m - 1.0) * m;

		s += t;
	}

	// return s;
	return s + x; // Adding x towards the end improves the ULP slightly
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
Float64x2 cosh(const Float64x2& x) {
	if (isequal_zero(x)) {
		return static_cast<Float64x2>(1.0);
	}

	Float64x2 ex = exp(x);
	return mul_pwr2(ex + recip(ex), 0.5);
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
Float64x2 tanh(const Float64x2& x) {
	if (isequal_zero(x)) {
		return static_cast<Float64x2>(0.0);
	}

	if (fabs(x.hi) > static_cast<fp64>(0.05)) {
		Float64x2 ex = exp(x);
		Float64x2 recip_ex = recip(ex);
		return (ex - recip_ex) / (ex + recip_ex);
	}
	Float64x2 sinh_val, cosh_val;
	sinh_val = sinh(x);
	cosh_val = sqrt(static_cast<fp64>(1.0) + square(sinh_val));
	return sinh_val / cosh_val;
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
void sinhcosh(const Float64x2& x, Float64x2& p_sinh, Float64x2& p_cosh) {
	if (fabs(x.hi) <= 0.05) {
		p_sinh = sinh(x);
		p_cosh = sqrt(static_cast<fp64>(1.0) + square(x));
	} else {
		Float64x2 ex = exp(x);
		Float64x2 recip_ex = recip(ex);
		p_sinh = mul_pwr2(ex - recip_ex, 0.5);
		p_cosh = mul_pwr2(ex + recip_ex, 0.5);
	}
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
Float64x2 asinh(const Float64x2& x) {
	if (isequal_zero(x)) {
		return static_cast<fp64>(0.0);
	}
	return log(x + sqrt(square(x) + static_cast<fp64>(1.0)));
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
Float64x2 acosh(const Float64x2& x) {
	if (x <= static_cast<fp64>(1.0)) {
		if (x == static_cast<fp64>(1.0)) {
			return static_cast<fp64>(0.0);
		}
		// Float64x2::error("(Float64x2::acosh): Argument out of domain.");
		std::feraiseexcept(FE_INVALID);
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}

	return log(x + sqrt(square(x) - static_cast<fp64>(1.0)));
}

/** 
 * @author Taken from libQD dd_real.cpp which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */
Float64x2 atanh(const Float64x2& x) {
	if (fabs(x) >= static_cast<fp64>(1.0)) {
		if (x == static_cast<fp64>(1.0)) {
			return std::numeric_limits<Float64x2>::infinity();
		}
		if (x == static_cast<fp64>(-1.0)) {
			return -std::numeric_limits<Float64x2>::infinity();
		}
		// Float64x2::error("(Float64x2::atanh): Argument out of domain.");
		return std::numeric_limits<Float64x2>::quiet_NaN();
	}
	#if 1
		/**
		 * @brief Value of ~0.472 yeilds the most precision. However, it should
		 * thoeritically be 0.5, where log and log1p would yeild equal
		 * precision for this function.
		 */
		if (fabs(x) >= static_cast<fp64>(0.5)) {
			// Accurate for larger numbers |x| >= 0.5
			return mul_pwr2(log((static_cast<fp64>(1.0) + x) / (static_cast<fp64>(1.0) - x)), 0.5);

		}
		// Accurate for smaller numbers |x| < 0.5
		return mul_pwr2(log1p(x) - log1p(-x), 0.5);
	#else
		#if 1
			// Original code
			return mul_pwr2(log((static_cast<fp64>(1.0) + x) / (static_cast<fp64>(1.0) - x)), 0.5);
		#elif 0
			// Removes division
			return mul_pwr2(
				log(x + 1.0) - log(static_cast<fp64>(1.0) - x), 0.5
			);
		#elif 1
			// Uses log1p
			return mul_pwr2(
				log1p(x) - log1p(-x), 0.5
			);
		#else
			// Can be used with log1p in this form: ln(2x / (1 - x) + 1)
			return mul_pwr2(log1p((mul_pwr2(x, static_cast<fp64>(2.0))) / (static_cast<fp64>(1.0) - x)), 0.5);
		#endif
	#endif
}

//------------------------------------------------------------------------------
// Float64x2 fma
//------------------------------------------------------------------------------

Float64x2 fma(const Float64x2& x, const Float64x2& y, const Float64x2& z) {
	Float64x4 ret = LDF::mul<Float64x4>(x, y);
	ret += z;
	return static_cast<Float64x2>(ret);
}

//------------------------------------------------------------------------------
// Float64x2 hypot
//------------------------------------------------------------------------------

Float64x2 hypot(const Float64x2& x, const Float64x2& y) {
	if (isunordered(x, y)) {
		return isnan(x) ? x : y;
	}
	if (isinf(x) || isinf(y)) {
		return std::numeric_limits<Float64x2>::infinity();
	}
	if (isequal_zero(x)) {
		return y;
	}
	if (isequal_zero(y)) {
		return x;
	}
	if (x == y) {
		return x * LDF::const_sqrt2<Float64x2>() ;
	}
	
	// 2 ^ +509
	constexpr fp64 sqrt_max = 1.6759759912428246374467531247757e+153;
	// 2 ^ -509
	constexpr fp64 sqrt_min = 5.9666725849601653946327722523703e-154;

	const Float64x2 max_value = fmax(fabs(x), fabs(y));
	// Prevents overflow
	if (max_value >= sqrt_max) {
		return ldexp(sqrt(
			square(ldexp(x, -512)) + square(ldexp(y, -512))
		), 256);
	}
	// Prevents underflow
	if (max_value <= sqrt_min) {
		return ldexp(sqrt(
			square(ldexp(x, 512)) + square(ldexp(y, 512))
		), -256);
	}
	// Trival case
	return sqrt(square(x) + square(y));
}

Float64x2 hypot(const Float64x2& x, const Float64x2& y, const Float64x2& z) {
	if (isunordered(x, y, z)) {
		return isnan(x) ? x : isnan(y) ? y : z;
	}
	if (isinf(x) || isinf(y) || isinf(z)) {
		return std::numeric_limits<Float64x2>::infinity();
	}
	if (isequal_zero(x)) {
		return hypot(y, z);
	}
	if (isequal_zero(y)) {
		return hypot(x, z);
	}
	if (isequal_zero(z)) {
		return hypot(x, y);
	}
	if (x == y && x == z) {
		return x * LDF::const_sqrt3<Float64x2>();
	}

	// 2 ^ +509
	constexpr fp64 sqrt_max = 1.6759759912428246374467531247757e+153;
	// 2 ^ -509
	constexpr fp64 sqrt_min = 5.9666725849601653946327722523703e-154;

	const Float64x2 max_value = fmax(fabs(x), fabs(y), fabs(z));
	// Prevents overflow
	if (max_value >= sqrt_max) {
		return ldexp(sqrt(
			square(ldexp(x, -512)) + square(ldexp(y, -512)) + square(ldexp(z, -512))
		), 256);
	}
	// Prevents underflow
	if (max_value <= sqrt_min) {
		return ldexp(sqrt(
			square(ldexp(x, 512)) + square(ldexp(y, 512)) + square(ldexp(z, 512))
		), -256);
	}
	// Trival case
	return sqrt(square(x) + square(y) + square(z));
}
 
 
//------------------------------------------------------------------------------
// Float64x2 erf and erfc
//------------------------------------------------------------------------------

#include "../FloatNxN/FloatNxN_erf.hpp"

/**
 * @remarks the lower bound of max_iter for erf and erfc seems to be 147.
 * 146 gives a few results that are accurate to 74bits instead of 108bits or
 * more. I am not sure why max_iter was set to 10000 initially instead of 1000.
 * A max_iter of 152 is choosen to cover any missed edge cases, and
 * because it is a multiple of 4 (Should that help the compiler at all).
 */

Float64x2 erf(const Float64x2& x) {
	return libDDFUN_erf<
		Float64x2, fp64,
		256
	>(x);
}

#if 0

Float64x2 erfc(const Float64x2& x) {
	// if (x > 26.7) {
	// 	return erfc((fp64)x);
	// }
	if (x > LDF::LDF_Input_Limits::erfc_max<Float64x2, fp64>()) {
		std::feraiseexcept(FE_UNDERFLOW);
		return static_cast<fp64>(0.0);
	}
	return libDDFUN_erfc<
		Float64x2, fp64,
		256
	>(x);
}

#else

static const Float64x2* erfc_lut_numer[] = {
	erfc_pade_0d5_numer ,
	erfc_pade_1d0_numer ,
	erfc_pade_2d0_numer ,
	erfc_pade_4d0_numer ,
	erfc_pade_8d0_numer ,
	erfc_pade_16d0_numer
};

static constexpr size_t erfc_len_numer[] = {
	sizeof(erfc_pade_0d5_numer ) / sizeof(Float64x2),
	sizeof(erfc_pade_1d0_numer ) / sizeof(Float64x2),
	sizeof(erfc_pade_2d0_numer ) / sizeof(Float64x2),
	sizeof(erfc_pade_4d0_numer ) / sizeof(Float64x2),
	sizeof(erfc_pade_8d0_numer ) / sizeof(Float64x2),
	sizeof(erfc_pade_16d0_numer) / sizeof(Float64x2)
};

static_assert(
	sizeof(erfc_lut_numer) / sizeof(erfc_lut_numer[0]) ==
	sizeof(erfc_len_numer) / sizeof(erfc_len_numer[0]),
	"erfc_lut_numer is missing data"
);

static const Float64x2* erfc_lut_denom[] = {
	erfc_pade_0d5_denom ,
	erfc_pade_1d0_denom ,
	erfc_pade_2d0_denom ,
	erfc_pade_4d0_denom ,
	erfc_pade_8d0_denom ,
	erfc_pade_16d0_denom
};

static constexpr size_t erfc_len_denom[] = {
	sizeof(erfc_pade_0d5_denom ) / sizeof(Float64x2),
	sizeof(erfc_pade_1d0_denom ) / sizeof(Float64x2),
	sizeof(erfc_pade_2d0_denom ) / sizeof(Float64x2),
	sizeof(erfc_pade_4d0_denom ) / sizeof(Float64x2),
	sizeof(erfc_pade_8d0_denom ) / sizeof(Float64x2),
	sizeof(erfc_pade_16d0_denom) / sizeof(Float64x2)
};

static_assert(
	sizeof(erfc_lut_denom) / sizeof(erfc_lut_denom[0]) ==
	sizeof(erfc_len_denom) / sizeof(erfc_len_denom[0]),
	"erfc_lut_denom is missing data"
);

static constexpr fp64 erfc_offset[] = {
	0.5,
	1.0,
	2.0,
	4.0,
	8.0,
	16.0
};

Float64x2 erfc(const Float64x2& x) {
	if (isnan(x)) {
		return x;
	}
	if (x < 0.5) {
		return 1.0 - erf(x);
	}

	size_t LUT_index = 0;

	if      (x < 0.75 ) { LUT_index = 0; }
	else if (x < 1.5  ) { LUT_index = 1; }
	else if (x < 3.0  ) { LUT_index = 2; }
	else if (x < 6.0  ) { LUT_index = 3; }
	else if (x < 12.0 ) { LUT_index = 4; }
	else if (x < 27.25) { LUT_index = 5; }
	else { 
		std::feraiseexcept(FE_UNDERFLOW);
		return 0.0;
	}

	const Float64x2* lut_numer = erfc_lut_numer[LUT_index];
	const Float64x2* lut_denom = erfc_lut_denom[LUT_index];
	const size_t len_numer = erfc_len_numer[LUT_index];
	const size_t len_denom = erfc_len_denom[LUT_index];

	Float64x2 w = x - erfc_offset[LUT_index];

	Float64x2 numer, denom;
	
	numer = lut_numer[0];
	for (size_t i = 1; i < len_numer; i++) {
		numer = numer * w + lut_numer[i];
	}

	denom = lut_denom[0];
	for (size_t i = 1; i < len_denom; i++) {
		denom = denom * w + lut_denom[i];
	}
	
	Float64x2 y = numer / (denom * exp(square(x)));
	return y;

	#if 0
	if (x <= 25.75) {
		Float64x2 y = sc / (sd * exp(square(x)));

		return y;
	}
	Float64x2 LbE = LDF::const_log2e<Float64x2>();
	Float64x2 y = exp2(log2(sc / sd) - square(x) * LbE);

	return y;
	#endif
}

#endif

//------------------------------------------------------------------------------
// Float64x2 tgamma
//------------------------------------------------------------------------------

#include "../FloatNxN/FloatNxN_tgamma.hpp"

Float64x2 tgamma(const Float64x2& t) {
	return libDQFUN_tgamma<
		Float64x2, fp64,
		100000
	>(t);
}

//------------------------------------------------------------------------------
// Float64x2 incgamma
//------------------------------------------------------------------------------

#include "../FloatNxN/FloatNxN_incgamma.hpp"

Float64x2 incgamma(const Float64x2& s, const Float64x2& z) {
	return libDDFUN_incgamma<
		Float64x2, fp64,
		1000000
	>(s, z);
}

//------------------------------------------------------------------------------
// Float64x2 expint
//------------------------------------------------------------------------------

Float64x2 expint(const Float64x2& x) {
	return libDDFUN_expint<
		Float64x2, fp64,
		1000000
	>(x);
}

//------------------------------------------------------------------------------
// Float64x2 riemann_zeta
//------------------------------------------------------------------------------

#include "../FloatNxN/FloatNxN_riemann_zeta.hpp"

Float64x2 riemann_zeta(const Float64x2& x) {
	return libDDFUN_riemann_zeta<
		Float64x2, fp64,
		1000000
	>(x);
}

//------------------------------------------------------------------------------
// Float64x2 Bessel Functions
//------------------------------------------------------------------------------

#include "../FloatNxN/FloatNxN_cyl_bessel.hpp"

/** @brief regular modified cylindrical Bessel function */
Float64x2 cyl_bessel_i(const Float64x2& nu, const Float64x2& x) {
	return libDDFUN_cyl_bessel_i<
		Float64x2, fp64,
		1000000
	>(nu, x);
}

/** @brief cylindrical Bessel functions (of the first kind) */
Float64x2 cyl_bessel_j(const Float64x2& nu, const Float64x2& x) {
	return libDDFUN_cyl_bessel_j<
		Float64x2, fp64,
		1000000
	>(nu, x);
}

/** @brief irregular modified cylindrical Bessel functions  */
Float64x2 cyl_bessel_k(const Float64x2& nu, const Float64x2& x) {
	return libDDFUN_cyl_bessel_k<
		Float64x2, fp64,
		1000000
	>(nu, x);
}

/** @brief Bessel function of the second kind. */
Float64x2 cyl_neumann(const Float64x2& nu, const Float64x2& x) {
	return libDDFUN_cyl_bessel_y<
		Float64x2, fp64,
		1000000
	>(nu, x);
}

/** @brief regular modified cylindrical Bessel function */
Float64x2 cyl_bessel_i(int nu, const Float64x2& x) {
	return libDDFUN_cyl_bessel_i_integer<
		Float64x2, fp64,
		1000000
	>(nu, x);
}

/** @brief cylindrical Bessel functions (of the first kind) */
Float64x2 cyl_bessel_j(int nu, const Float64x2& x) {
	return libDDFUN_cyl_bessel_j_integer<
		Float64x2, fp64,
		1000000
	>(nu, x);
}

/** @brief irregular modified cylindrical Bessel functions  */
Float64x2 cyl_bessel_k(int nu, const Float64x2& x) {
	return libDDFUN_cyl_bessel_k_integer<
		Float64x2, fp64,
		1000000
	>(nu, x);
}

/** @brief Bessel function of the second kind. */
Float64x2 cyl_neumann(int nu, const Float64x2& x) {
	return libDDFUN_cyl_bessel_y_integer<
		Float64x2, fp64,
		1000000
	>(nu, x);
}

//------------------------------------------------------------------------------
// Float64x2 math.h wrapper functions
//------------------------------------------------------------------------------

Float64x2 Float64x2_exp(Float64x2 x) {
	return exp(x);
}
Float64x2 Float64x2_expm1(Float64x2 x) {
	return exp(x);
}
Float64x2 Float64x2_log(Float64x2 x) {
	return log(x);
}
Float64x2 Float64x2_log1p(Float64x2 x) {
	return log1p(x);
}

Float64x2 Float64x2_sin(Float64x2 x) {
	return sin(x);
}
Float64x2 Float64x2_cos(Float64x2 x) {
	return cos(x);
}
void Float64x2_sincos(Float64x2 theta, Float64x2* LDF_restrict p_sin, Float64x2* LDF_restrict p_cos) {
	sincos(theta, *p_sin, *p_cos);
}
Float64x2 Float64x2_tan(Float64x2 x) {
	return tan(x);
}
Float64x2 Float64x2_asin(Float64x2 x) {
	return asin(x);
}
Float64x2 Float64x2_acos(Float64x2 x) {
	return acos(x);
}
Float64x2 Float64x2_atan(Float64x2 x) {
	return atan(x);
}
Float64x2 Float64x2_atan2(Float64x2 y, Float64x2 x) {
	return atan2(y, x);
}
Float64x2 Float64x2_sinh(Float64x2 x) {
	return sinh(x);
}
Float64x2 Float64x2_cosh(Float64x2 x) {
	return cosh(x);
}
Float64x2 Float64x2_tanh(Float64x2 x) {
	return tanh(x);
}
void Float64x2_sinhcosh(Float64x2 theta, Float64x2* LDF_restrict p_sinh, Float64x2* LDF_restrict p_cosh) {
	sinhcosh(theta, *p_sinh, *p_cosh);
}

Float64x2 Float64x2_erf(Float64x2 x) {
	return erf(x);
}
Float64x2 Float64x2_erfc(Float64x2 x) {
	return erfc(x);
}
Float64x2 Float64x2_tgamma(Float64x2 x) {
	return tgamma(x);
}
Float64x2 Float64x2_lgamma(Float64x2 x) {
	return lgamma(x);
}

//------------------------------------------------------------------------------
// Float64x2 from string
//------------------------------------------------------------------------------

#include "../FloatNxN/FloatNxN_stringTo.hpp"

Float64x2 stringTo_Float64x2(const char* nPtr, char** endPtr) {
	internal_FloatNxN_stringTo<Float64x2, fp64> stringTo_func;
	return stringTo_func.stringTo_FloatNxN(nPtr, endPtr);
}

std::istream& operator>>(std::istream& stream, Float64x2& value) {
	internal_FloatNxN_stringTo<Float64x2, fp64> func_cin;
	return func_cin.cin_FloatNxN(stream, value);
}

//------------------------------------------------------------------------------
// Float64x2 to string
//------------------------------------------------------------------------------

#include "../FloatNxN/FloatNxN_snprintf.hpp"

int Float64x2_snprintf(char* buf, size_t len, const char* format, ...) {
	va_list args;
	va_start(args, format);
	internal_FloatNxN_snprintf<Float64x2, fp64, 2> func_snprintf;
	int ret_val = func_snprintf.FloatNxN_snprintf(
		PRIFloat64x2, PRIFloat64,
		buf, len, format, args
	);
	va_end(args);
	return ret_val;
}

std::ostream& operator<<(std::ostream& stream, const Float64x2& value) {
	internal_FloatNxN_snprintf<Float64x2, fp64, 2> func_cout;
	return func_cout.FloatNxN_cout(PRIFloat64x2, PRIFloat64, stream, value);
}
