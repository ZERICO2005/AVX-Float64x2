/*
**	Author: zerico2005 (2024)
**	Project: LIB-Dekker-Float
**	License: MIT License
**	A copy of the MIT License should be included with
**	this project. If not, see https://opensource.org/license/MIT
*/

/*
**	Portions of this file were part of the libQD library, licensed
**	under a modifed BSD license that can be found below:
**	https://www.davidhbailey.com/dhbsoftware/LBNL-BSD-License.docx
**	Or alternatively from this website:
**	https://www.davidhbailey.com/dhbsoftware/
**	A copy of the LBNL-BSD-License can also be found at:
**	LIB-Dekker-Float/libQD/LBNL-BSD-License.txt
*/

#ifndef FLOAT64X4_SSE2_H
#define FLOAT64X4_SSE2_H

/**
 * @note Requires AVX or later.
 * @warning -Ofast may break this library. -O3 compiles okay on gcc and clang.
 */

#include "../LDF/LDF_restrict.h"
#include "Float64x4_def.h"
#include "../Float64/Float64_SSE2.h"
#include "../Float64x2/Float64x2_SSE2.h"

#include <stdint.h>

#if (!defined(__SSE2__) && defined(__GNUC__))
	#error "__SSE2__ is not enabled in your compiler. Try -msse2"
#endif

#include <emmintrin.h>

#ifdef __SSE4_1__
	#include <smmintrin.h>
#endif
#ifdef __SSE4_2__
	#include <nmmintrin.h>
#endif
#ifdef __AVX__
	#include <immintrin.h>
#endif

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Holds four Quad-Float64 dekker floats
 */
typedef struct __m128dx4 {
	__m128d val[4];
} __m128dx4;

//------------------------------------------------------------------------------
// __m128dx4 set1 functions
//------------------------------------------------------------------------------

static inline __m128dx4 _mm128x4_setzero_pdx4(void) {
	__m128dx4 ret;
	ret.val[0] = _mm_setzero_pd();
	ret.val[1] = _mm_setzero_pd();
	ret.val[2] = _mm_setzero_pd();
	ret.val[3] = _mm_setzero_pd();
	return ret;
}

static inline __m128dx4 _mm128x4_set1_pdx4(const Float64x4 value) {
	__m128dx4 ret;
	ret.val[0] = _mm_set1_pd(value.val[0]);
	ret.val[1] = _mm_set1_pd(value.val[1]);
	ret.val[2] = _mm_set1_pd(value.val[2]);
	ret.val[3] = _mm_set1_pd(value.val[3]);
	return ret;
}

static inline __m128dx4 _mm128x4_set1_pdx2(const Float64x2 value) {
	__m128dx4 ret;
	ret.val[0] = _mm_set1_pd(value.hi);
	ret.val[1] = _mm_set1_pd(value.lo);
	ret.val[2] = _mm_setzero_pd();
	ret.val[3] = _mm_setzero_pd();
	return ret;
}


static inline __m128dx4 _mm128x4_set1_pd(double x) {
	__m128dx4 ret;
	ret.val[0] = _mm_set1_pd(x);
	ret.val[1] = _mm_setzero_pd();
	ret.val[2] = _mm_setzero_pd();
	ret.val[3] = _mm_setzero_pd();
	return ret;
}

static inline __m128dx4 _mm128x4_set1_epi64x(long long x) {
	__m128dx4 ret;
	ret.val[0] = _mm_castsi128_pd(_mm_set1_epi64x(x));
	ret.val[1] = _mm_castsi128_pd(_mm_set1_epi64x(x));
	ret.val[2] = _mm_castsi128_pd(_mm_set1_epi64x(x));
	ret.val[3] = _mm_castsi128_pd(_mm_set1_epi64x(x));
	return ret;
}

//------------------------------------------------------------------------------
// __m128dx4 set functions
//------------------------------------------------------------------------------

static inline __m128dx4 _mm128x4_set_pdx4(
	const Float64x4 e1, const Float64x4 e0
) {
	__m128dx4 ret;
	ret.val[0] = _mm_set_pd(e1.val[0], e0.val[0]);
	ret.val[1] = _mm_set_pd(e1.val[1], e0.val[1]);
	ret.val[2] = _mm_set_pd(e1.val[2], e0.val[2]);
	ret.val[3] = _mm_set_pd(e1.val[3], e0.val[3]);
	return ret;
}

static inline __m128dx4 _mm128x4_set_pdx2(
	const Float64x2 e1, const Float64x2 e0
) {
	__m128dx4 ret;
	ret.val[0] = _mm_set_pd(e1.hi, e0.hi);
	ret.val[1] = _mm_set_pd(e1.lo, e0.lo);
	ret.val[2] = _mm_setzero_pd();
	ret.val[3] = _mm_setzero_pd();
	return ret;
}

static inline __m128dx4 _mm128x4_set_pd(
	double e1, double e0
) {
	__m128dx4 ret;
	ret.val[0] = _mm_set_pd(e1, e0);
	ret.val[1] = _mm_setzero_pd();
	ret.val[2] = _mm_setzero_pd();
	ret.val[3] = _mm_setzero_pd();
	return ret;
}

static inline __m128dx4 _mm128x4_set_epi64x(
	const long long e1[4], const long long e0[4]
) {
	__m128dx4 ret;
	ret.val[0] = _mm_castsi128_pd(_mm_set_epi64x(e1[0], e0[0]));
	ret.val[1] = _mm_castsi128_pd(_mm_set_epi64x(e1[1], e0[1]));
	ret.val[2] = _mm_castsi128_pd(_mm_set_epi64x(e1[2], e0[2]));
	ret.val[3] = _mm_castsi128_pd(_mm_set_epi64x(e1[3], e0[3]));
	return ret;
}

//------------------------------------------------------------------------------
// __m128dx4 setr (set reverse) functions
//------------------------------------------------------------------------------

static inline __m128dx4 _mm128x4_setr_pdx4(
	const Float64x4 e1, const Float64x4 e0
) {
	__m128dx4 ret;
	ret.val[0] = _mm_setr_pd(e1.val[0], e0.val[0]);
	ret.val[1] = _mm_setr_pd(e1.val[1], e0.val[1]);
	ret.val[2] = _mm_setr_pd(e1.val[2], e0.val[2]);
	ret.val[3] = _mm_setr_pd(e1.val[3], e0.val[3]);
	return ret;
}

static inline __m128dx4 _mm128x4_setr_pdx2(
	const Float64x2 e1, const Float64x2 e0
) {
	__m128dx4 ret;
	ret.val[0] = _mm_setr_pd(e1.hi, e0.hi);
	ret.val[1] = _mm_setr_pd(e1.lo, e0.lo);
	ret.val[2] = _mm_setzero_pd();
	ret.val[3] = _mm_setzero_pd();
	return ret;
}

static inline __m128dx4 _mm128x4_setr_pd(
	double e1, double e0
) {
	__m128dx4 ret;
	ret.val[0] = _mm_setr_pd(e1, e0);
	ret.val[1] = _mm_setzero_pd();
	ret.val[2] = _mm_setzero_pd();
	ret.val[3] = _mm_setzero_pd();
	return ret;
}

static inline __m128dx4 _mm128x4_setr_epi64x(
	const long long e1[4], const long long e0[4]
) {
	__m128dx4 ret;
	ret.val[0] = _mm_castsi128_pd(_mm_set_epi64x(e0[0], e1[0]));
	ret.val[1] = _mm_castsi128_pd(_mm_set_epi64x(e0[1], e1[1]));
	ret.val[2] = _mm_castsi128_pd(_mm_set_epi64x(e0[2], e1[2]));
	ret.val[3] = _mm_castsi128_pd(_mm_set_epi64x(e0[3], e1[3]));
	return ret;
}

//------------------------------------------------------------------------------
// __m128dx4 load operations
//------------------------------------------------------------------------------

#if 0
/* Unimplemented */

/**
 * @note mem_addr must be aligned on a 32-byte boundary or a general-protection
 * exception may be generated.
 */
static inline __m128dx4 _mm128x4_load_pd(const double* mem_addr) {
	__m128dx4 ret;
	ret.val[0] = _mm_load_pd(mem_addr +  0);
	ret.val[1] = _mm_load_pd(mem_addr +  4);
	ret.val[2] = _mm_load_pd(mem_addr +  8);
	ret.val[3] = _mm_load_pd(mem_addr + 12);
	return _mm128x4_transpose_pdx4(ret);
}

static inline __m128dx4 _mm128x4_loadu_pd(const double* mem_addr) {
	__m128dx4 ret;
	ret.val[0] = _mm_loadu_pd(mem_addr +  0);
	ret.val[1] = _mm_loadu_pd(mem_addr +  4);
	ret.val[2] = _mm_loadu_pd(mem_addr +  8);
	ret.val[3] = _mm_loadu_pd(mem_addr + 12);
	return _mm128x4_transpose_pdx4(ret);
}

/**
 * @note mem_addr must be aligned on a 32-byte boundary or a general-protection
 * exception may be generated.
 */
static inline __m128dx4 _mm128x4_load_pdx4(const Float64x4* mem_addr) {
	return _mm128x4_load_pd((const double*)((const void*)mem_addr));
}

static inline __m128dx4 _mm128x4_loadu_pdx4(const Float64x4* mem_addr) {
	return _mm128x4_loadu_pd((const double*)((const void*)mem_addr));
}

#endif

/**
 * @note mem_addr must be aligned on a 32-byte boundary or a general-protection
 * exception may be generated.
 */
static inline __m128dx4 _mm128x4_load_raw_pd(const double* mem_addr) {
	__m128dx4 ret;
	ret.val[0] = _mm_load_pd(mem_addr +  0);
	ret.val[1] = _mm_load_pd(mem_addr +  4);
	ret.val[2] = _mm_load_pd(mem_addr +  8);
	ret.val[3] = _mm_load_pd(mem_addr + 12);
	return ret;
}

static inline __m128dx4 _mm128x4_loadu_raw_pd(const double* mem_addr) {
	__m128dx4 ret;
	ret.val[0] = _mm_loadu_pd(mem_addr +  0);
	ret.val[1] = _mm_loadu_pd(mem_addr +  4);
	ret.val[2] = _mm_loadu_pd(mem_addr +  8);
	ret.val[3] = _mm_loadu_pd(mem_addr + 12);
	return ret;
}

//------------------------------------------------------------------------------
// __m128dx4 store operations
//------------------------------------------------------------------------------

#if 0
/* Unimplemented */

/**
 * @note mem_addr must be aligned on a 32-byte boundary or a general-protection
 * exception may be generated.
 */
static inline void _mm128x4_store_pd(double* mem_addr, __m128dx4 src) {
	src = _mm128x4_transpose_pdx4(src);
	_mm_store_pd(mem_addr +  0, src.val[0]);
	_mm_store_pd(mem_addr +  4, src.val[1]);
	_mm_store_pd(mem_addr +  8, src.val[2]);
	_mm_store_pd(mem_addr + 12, src.val[3]);
}

static inline void _mm128x4_storeu_pd(double* mem_addr, __m128dx4 src) {
	src = _mm128x4_transpose_pdx4(src);
	_mm_storeu_pd(mem_addr +  0, src.val[0]);
	_mm_storeu_pd(mem_addr +  4, src.val[1]);
	_mm_storeu_pd(mem_addr +  8, src.val[2]);
	_mm_storeu_pd(mem_addr + 12, src.val[3]);
}

/**
 * @note mem_addr must be aligned on a 32-byte boundary or a general-protection
 * exception may be generated.
 */
static inline void _mm128x4_store_pdx4(Float64x4* mem_addr, __m128dx4 src) {
	_mm128x4_store_pd((double*)((void*)mem_addr), src);
}

static inline void _mm128x4_storeu_pdx4(Float64x4* mem_addr, __m128dx4 src) {
	_mm128x4_storeu_pd((double*)((void*)mem_addr), src);
}

#endif

/**
 * @note mem_addr must be aligned on a 32-byte boundary or a general-protection
 * exception may be generated.
 */
static inline void _mm_store_raw_pdx4(double* mem_addr, __m128dx4 src) {
	_mm_store_pd(mem_addr +  0, src.val[0]);
	_mm_store_pd(mem_addr +  4, src.val[1]);
	_mm_store_pd(mem_addr +  8, src.val[2]);
	_mm_store_pd(mem_addr + 12, src.val[3]);
}

static inline void _mm_storeu_raw_pdx4(double* mem_addr, __m128dx4 src) {
	_mm_storeu_pd(mem_addr +  0, src.val[0]);
	_mm_storeu_pd(mem_addr +  4, src.val[1]);
	_mm_storeu_pd(mem_addr +  8, src.val[2]);
	_mm_storeu_pd(mem_addr + 12, src.val[3]);
}

//------------------------------------------------------------------------------
// __m128dx4 bitwise operations
//------------------------------------------------------------------------------

/**
 * @brief bitwise not `~x`
 */
static inline __m128dx4 _mm128x4_not_pdx4(__m128dx4 x) {
	x.val[0] = _mm_not_pd(x.val[0]);
	x.val[1] = _mm_not_pd(x.val[1]);
	x.val[2] = _mm_not_pd(x.val[2]);
	x.val[3] = _mm_not_pd(x.val[3]);
	return x;
}

/**
 * @brief bitwise and `x & y`
 */
static inline __m128dx4 _mm128x4_and_pdx4(__m128dx4 x, const __m128dx4 y) {
	x.val[0] = _mm_and_pd(x.val[0], y.val[0]);
	x.val[1] = _mm_and_pd(x.val[1], y.val[1]);
	x.val[2] = _mm_and_pd(x.val[2], y.val[2]);
	x.val[3] = _mm_and_pd(x.val[3], y.val[3]);
	return x;
}

/**
 * @brief bitwise andnot `x & ~y`
 */
static inline __m128dx4 _mm128x4_andnot_pdx4(__m128dx4 x, const __m128dx4 y) {
	x.val[0] = _mm_andnot_pd(x.val[0], y.val[0]);
	x.val[1] = _mm_andnot_pd(x.val[1], y.val[1]);
	x.val[2] = _mm_andnot_pd(x.val[2], y.val[2]);
	x.val[3] = _mm_andnot_pd(x.val[3], y.val[3]);
	return x;
}

/**
 * @brief bitwise or `x | y`
 */
static inline __m128dx4 _mm128x4_or_pdx4(__m128dx4 x, const __m128dx4 y) {
	x.val[0] = _mm_or_pd(x.val[0], y.val[0]);
	x.val[1] = _mm_or_pd(x.val[1], y.val[1]);
	x.val[2] = _mm_or_pd(x.val[2], y.val[2]);
	x.val[3] = _mm_or_pd(x.val[3], y.val[3]);
	return x;
}

/**
 * @brief bitwise xor `x ^ y`
 */
static inline __m128dx4 _mm128x4_xor_pdx4(__m128dx4 x, const __m128dx4 y) {
	x.val[0] = _mm_xor_pd(x.val[0], y.val[0]);
	x.val[1] = _mm_xor_pd(x.val[1], y.val[1]);
	x.val[2] = _mm_xor_pd(x.val[2], y.val[2]);
	x.val[3] = _mm_xor_pd(x.val[3], y.val[3]);
	return x;
}

//------------------------------------------------------------------------------
// __m128dx4 comparison extend
//------------------------------------------------------------------------------

/**
 * @brief Copies cmp to val[0 - 3]. Used for extending comparison results.
 */
static inline __m128dx4 _mm128x4_cmp_extend_pd(__m128d cmp) {
	__m128dx4 ret = {{cmp, cmp, cmp, cmp}};
	return ret;
}

//------------------------------------------------------------------------------
// __m128dx4 comparison functions
//------------------------------------------------------------------------------

	/**
	 * @brief _CMP_ORD_Q
	 */
	static inline __m128d _mm_cmpord_pdx4(__m128dx4 x, __m128dx4 y) {
		return _mm_and_pd(
			_mm_and_pd(
				_mm_cmpord_pd(x.val[0], y.val[0]),
				_mm_cmpord_pd(x.val[1], y.val[1])
			),
			_mm_and_pd(
				_mm_cmpord_pd(x.val[2], y.val[2]),
				_mm_cmpord_pd(x.val[3], y.val[3])
			)
		);
	}

	/**
	 * @brief _CMP_UNORD_Q
	 */
	static inline __m128d _mm_cmpunord_pdx4(__m128dx4 x, __m128dx4 y) {
		return _mm_or_pd(
			_mm_or_pd(
				_mm_cmpunord_pd(x.val[0], y.val[0]),
				_mm_cmpunord_pd(x.val[1], y.val[1])
			),
			_mm_or_pd(
				_mm_cmpunord_pd(x.val[2], y.val[2]),
				_mm_cmpunord_pd(x.val[3], y.val[3])
			)
		);
	}

	/**
	 * @brief _CMP_EQ_OQ
	 */
	static inline __m128d _mm_cmpeq_pdx4(__m128dx4 x, __m128dx4 y) {
		return _mm_and_pd(
			_mm_and_pd(
				_mm_cmpeq_pd(x.val[0], y.val[0]),
				_mm_cmpeq_pd(x.val[1], y.val[1])
			),
			_mm_and_pd(
				_mm_cmpeq_pd(x.val[2], y.val[2]),
				_mm_cmpeq_pd(x.val[3], y.val[3])
			)
		);
	}

	/**
	 * @brief _CMP_NEQ_OQ
	 */
	static inline __m128d _mm_cmpneq_pdx4(__m128dx4 x, __m128dx4 y) {
		return _mm_or_pd(
			_mm_or_pd(
				_mm_cmpneq_pd(x.val[0], y.val[0]),
				_mm_cmpneq_pd(x.val[1], y.val[1])
			),
			_mm_or_pd(
				_mm_cmpneq_pd(x.val[2], y.val[2]),
				_mm_cmpneq_pd(x.val[3], y.val[3])
			)
		);
	}

	/**
	 * @brief _CMP_NEQ_UQ
	 */
	static inline __m128d _mm_cmplg_pdx4(__m128dx4 x, __m128dx4 y) {
		return _mm_or_pd(
			_mm_or_pd(
				_mm_cmplg_pd(x.val[0], y.val[0]),
				_mm_cmplg_pd(x.val[1], y.val[1])
			),
			_mm_or_pd(
				_mm_cmplg_pd(x.val[2], y.val[2]),
				_mm_cmplg_pd(x.val[3], y.val[3])
			)
		);
	}

	/**
	 * @brief _CMP_EQ_UQ
	 */
	static inline __m128d _mm_cmpnlg_pdx4(__m128dx4 x, __m128dx4 y) {
		return _mm_and_pd(
			_mm_and_pd(
				_mm_cmpnlg_pd(x.val[0], y.val[0]),
				_mm_cmpnlg_pd(x.val[1], y.val[1])
			),
			_mm_and_pd(
				_mm_cmpnlg_pd(x.val[2], y.val[2]),
				_mm_cmpnlg_pd(x.val[3], y.val[3])
			)
		);
	}

	/**
	 * @brief _CMP_LT_OQ
	 */
	static inline __m128d _mm_cmplt_pdx4(__m128dx4 x, __m128dx4 y) {
		__m128d cmp_lt_0 = _mm_cmplt_pd(x.val[0], y.val[0]);
		__m128d cmp_eq_0 = _mm_cmpeq_pd(x.val[0], y.val[0]);
		__m128d cmp_lt_1 = _mm_cmplt_pd(x.val[1], y.val[1]);
		__m128d cmp_eq_1 = _mm_cmpeq_pd(x.val[1], y.val[1]);
		__m128d cmp_lt_2 = _mm_cmplt_pd(x.val[2], y.val[2]);
		__m128d cmp_eq_2 = _mm_cmpeq_pd(x.val[2], y.val[2]);
		__m128d cmp_lt_3 = _mm_cmplt_pd(x.val[3], y.val[3]);
		return _mm_or_pd(cmp_lt_0, _mm_and_pd(cmp_eq_0,
			_mm_or_pd(cmp_lt_1, _mm_and_pd(cmp_eq_1,
				_mm_or_pd(cmp_lt_2, _mm_and_pd(cmp_eq_2, cmp_lt_3))
			))
		));
	}

	/**
	 * @brief _CMP_LE_OQ
	 */
	static inline __m128d _mm_cmple_pdx4(__m128dx4 x, __m128dx4 y) {
		__m128d cmp_lt_0 = _mm_cmplt_pd(x.val[0], y.val[0]);
		__m128d cmp_eq_0 = _mm_cmpeq_pd(x.val[0], y.val[0]);
		__m128d cmp_lt_1 = _mm_cmplt_pd(x.val[1], y.val[1]);
		__m128d cmp_eq_1 = _mm_cmpeq_pd(x.val[1], y.val[1]);
		__m128d cmp_lt_2 = _mm_cmplt_pd(x.val[2], y.val[2]);
		__m128d cmp_eq_2 = _mm_cmpeq_pd(x.val[2], y.val[2]);
		__m128d cmp_le_3 = _mm_cmple_pd(x.val[3], y.val[3]);
		return _mm_or_pd(cmp_lt_0, _mm_and_pd(cmp_eq_0,
			_mm_or_pd(cmp_lt_1, _mm_and_pd(cmp_eq_1,
				_mm_or_pd(cmp_lt_2, _mm_and_pd(cmp_eq_2, cmp_le_3))
			))
		));
	}

	/**
	 * @brief _CMP_GT_OQ
	 */
	static inline __m128d _mm_cmpgt_pdx4(__m128dx4 x, __m128dx4 y) {
		__m128d cmp_gt_0 = _mm_cmpgt_pd(x.val[0], y.val[0]);
		__m128d cmp_eq_0 = _mm_cmpeq_pd(x.val[0], y.val[0]);
		__m128d cmp_gt_1 = _mm_cmpgt_pd(x.val[1], y.val[1]);
		__m128d cmp_eq_1 = _mm_cmpeq_pd(x.val[1], y.val[1]);
		__m128d cmp_gt_2 = _mm_cmpgt_pd(x.val[2], y.val[2]);
		__m128d cmp_eq_2 = _mm_cmpeq_pd(x.val[2], y.val[2]);
		__m128d cmp_gt_3 = _mm_cmpgt_pd(x.val[3], y.val[3]);
		return _mm_or_pd(cmp_gt_0, _mm_and_pd(cmp_eq_0,
			_mm_or_pd(cmp_gt_1, _mm_and_pd(cmp_eq_1,
				_mm_or_pd(cmp_gt_2, _mm_and_pd(cmp_eq_2, cmp_gt_3))
			))
		));
	}

	/**
	 * @brief _CMP_GE_OQ
	 */
	static inline __m128d _mm_cmpge_pdx4(__m128dx4 x, __m128dx4 y) {
		__m128d cmp_gt_0 = _mm_cmpgt_pd(x.val[0], y.val[0]);
		__m128d cmp_eq_0 = _mm_cmpeq_pd(x.val[0], y.val[0]);
		__m128d cmp_gt_1 = _mm_cmpgt_pd(x.val[1], y.val[1]);
		__m128d cmp_eq_1 = _mm_cmpeq_pd(x.val[1], y.val[1]);
		__m128d cmp_gt_2 = _mm_cmpgt_pd(x.val[2], y.val[2]);
		__m128d cmp_eq_2 = _mm_cmpeq_pd(x.val[2], y.val[2]);
		__m128d cmp_ge_3 = _mm_cmpge_pd(x.val[3], y.val[3]);
		return _mm_or_pd(cmp_gt_0, _mm_and_pd(cmp_eq_0,
			_mm_or_pd(cmp_gt_1, _mm_and_pd(cmp_eq_1,
				_mm_or_pd(cmp_gt_2, _mm_and_pd(cmp_eq_2, cmp_ge_3))
			))
		));
	}

	/**
	 * @brief _CMP_NLT_UQ
	 */
	static inline __m128d _mm_cmpnlt_pdx4(__m128dx4 x, __m128dx4 y) {
		__m128d cmp_lt_0 = _mm_cmpnlt_pd(x.val[0], y.val[0]);
		__m128d cmp_eq_0 = _mm_cmpneq_pd(x.val[0], y.val[0]);
		__m128d cmp_lt_1 = _mm_cmpnlt_pd(x.val[1], y.val[1]);
		__m128d cmp_eq_1 = _mm_cmpneq_pd(x.val[1], y.val[1]);
		__m128d cmp_lt_2 = _mm_cmpnlt_pd(x.val[2], y.val[2]);
		__m128d cmp_eq_2 = _mm_cmpneq_pd(x.val[2], y.val[2]);
		__m128d cmp_lt_3 = _mm_cmpnlt_pd(x.val[3], y.val[3]);
		return _mm_and_pd(cmp_lt_0, _mm_or_pd(cmp_eq_0,
			_mm_and_pd(cmp_lt_1, _mm_or_pd(cmp_eq_1,
				_mm_and_pd(cmp_lt_2, _mm_or_pd(cmp_eq_2, cmp_lt_3))
			))
		));
	}

	/**
	 * @brief _CMP_NLE_UQ
	 */
	static inline __m128d _mm_cmpnle_pdx4(__m128dx4 x, __m128dx4 y) {
		__m128d cmp_lt_0 = _mm_cmpnlt_pd(x.val[0], y.val[0]);
		__m128d cmp_eq_0 = _mm_cmpneq_pd(x.val[0], y.val[0]);
		__m128d cmp_lt_1 = _mm_cmpnlt_pd(x.val[1], y.val[1]);
		__m128d cmp_eq_1 = _mm_cmpneq_pd(x.val[1], y.val[1]);
		__m128d cmp_lt_2 = _mm_cmpnlt_pd(x.val[2], y.val[2]);
		__m128d cmp_eq_2 = _mm_cmpneq_pd(x.val[2], y.val[2]);
		__m128d cmp_le_3 = _mm_cmpnle_pd(x.val[3], y.val[3]);
		return _mm_and_pd(cmp_lt_0, _mm_or_pd(cmp_eq_0,
			_mm_and_pd(cmp_lt_1, _mm_or_pd(cmp_eq_1,
				_mm_and_pd(cmp_lt_2, _mm_or_pd(cmp_eq_2, cmp_le_3))
			))
		));
	}

	/**
	 * @brief _CMP_NGT_UQ
	 */
	static inline __m128d _mm_cmpngt_pdx4(__m128dx4 x, __m128dx4 y) {
		__m128d cmp_gt_0 = _mm_cmpngt_pd(x.val[0], y.val[0]);
		__m128d cmp_eq_0 = _mm_cmpneq_pd(x.val[0], y.val[0]);
		__m128d cmp_gt_1 = _mm_cmpngt_pd(x.val[1], y.val[1]);
		__m128d cmp_eq_1 = _mm_cmpneq_pd(x.val[1], y.val[1]);
		__m128d cmp_gt_2 = _mm_cmpngt_pd(x.val[2], y.val[2]);
		__m128d cmp_eq_2 = _mm_cmpneq_pd(x.val[2], y.val[2]);
		__m128d cmp_gt_3 = _mm_cmpngt_pd(x.val[3], y.val[3]);
		return _mm_and_pd(cmp_gt_0, _mm_or_pd(cmp_eq_0,
			_mm_and_pd(cmp_gt_1, _mm_or_pd(cmp_eq_1,
				_mm_and_pd(cmp_gt_2, _mm_or_pd(cmp_eq_2, cmp_gt_3))
			))
		));
	}

	/**
	 * @brief _CMP_NGE_UQ
	 */
	static inline __m128d _mm_cmpnge_pdx4(__m128dx4 x, __m128dx4 y) {
		__m128d cmp_gt_0 = _mm_cmpngt_pd(x.val[0], y.val[0]);
		__m128d cmp_eq_0 = _mm_cmpneq_pd(x.val[0], y.val[0]);
		__m128d cmp_gt_1 = _mm_cmpngt_pd(x.val[1], y.val[1]);
		__m128d cmp_eq_1 = _mm_cmpneq_pd(x.val[1], y.val[1]);
		__m128d cmp_gt_2 = _mm_cmpngt_pd(x.val[2], y.val[2]);
		__m128d cmp_eq_2 = _mm_cmpneq_pd(x.val[2], y.val[2]);
		__m128d cmp_ge_3 = _mm_cmpnge_pd(x.val[3], y.val[3]);
		return _mm_and_pd(cmp_gt_0, _mm_or_pd(cmp_eq_0,
			_mm_and_pd(cmp_gt_1, _mm_or_pd(cmp_eq_1,
				_mm_and_pd(cmp_gt_2, _mm_or_pd(cmp_eq_2, cmp_ge_3))
			))
		));
	}

/* __m128dx4 compare __m128d */

	/**
	 * @brief _CMP_ORD_Q
	 */
	static inline __m128d _mm_cmpord_pdx4_pd(__m128dx4 x, __m128d y) {
		return _mm_cmpord_pd(x.val[0], y);
	}

	/**
	 * @brief _CMP_UNORD_Q
	 */
	static inline __m128d _mm_cmpunord_pdx4_pd(__m128dx4 x, __m128d y) {
		return _mm_cmpunord_pd(x.val[0], y);
	}

	/**
	 * @brief _CMP_EQ_OQ
	 */
	static inline __m128d _mm_cmpeq_pdx4_pd(__m128dx4 x, __m128d y) {
		__m128d cmp_hi = _mm_cmpeq_pd(x.val[0], y);
		__m128d cmp_lo = _mm_cmpeq_pd(x.val[1], _mm_setzero_pd());
		return _mm_and_pd(cmp_hi, cmp_lo);
	}

	/**
	 * @brief _CMP_NEQ_OQ
	 */
	static inline __m128d _mm_cmpneq_pdx4_pd(__m128dx4 x, __m128d y) {
		__m128d cmp_hi = _mm_cmpneq_pd(x.val[0], y);
		__m128d cmp_lo = _mm_cmpneq_pd(x.val[1], _mm_setzero_pd());
		return _mm_or_pd(cmp_hi, cmp_lo);
	}

	/**
	 * @brief _CMP_NEQ_UQ
	 */
	static inline __m128d _mm_cmplg_pdx4_pd(__m128dx4 x, __m128d y) {
		__m128d cmp_hi = _mm_cmplg_pd(x.val[0], y);
		__m128d cmp_lo = _mm_cmplg_pd(x.val[1], _mm_setzero_pd());
		return _mm_or_pd(cmp_hi, cmp_lo);
	}

	/**
	 * @brief _CMP_EQ_UQ
	 */
	static inline __m128d _mm_cmpnlg_pdx4_pd(__m128dx4 x, __m128d y) {
		__m128d cmp_hi = _mm_cmpnlg_pd(x.val[0], y);
		__m128d cmp_lo = _mm_cmpnlg_pd(x.val[1], _mm_setzero_pd());
		return _mm_and_pd(cmp_hi, cmp_lo);
	}

		/**
	 * @brief _CMP_LT_OQ
	 */
	static inline __m128d _mm_cmplt_pdx4_pd(__m128dx4 x, __m128d y) {
		return _mm_or_pd(
			_mm_cmplt_pd(x.val[0], y),
			_mm_and_pd(
				_mm_cmpeq_pd(x.val[0], y),
				_mm_cmplt_pd(x.val[1], _mm_setzero_pd())
			)
		);
	}

	/**
	 * @brief _CMP_LE_OQ
	 */
	static inline __m128d _mm_cmple_pdx4_pd(__m128dx4 x, __m128d y) {
		return _mm_or_pd(
			_mm_cmplt_pd(x.val[0], y),
			_mm_and_pd(
				_mm_cmpeq_pd(x.val[0], y),
				_mm_cmple_pd(x.val[1], _mm_setzero_pd())
			)
		);
	}

	/**
	 * @brief _CMP_GT_OQ
	 */
	static inline __m128d _mm_cmpgt_pdx4_pd(__m128dx4 x, __m128d y) {
		return _mm_or_pd(
			_mm_cmpgt_pd(x.val[0], y),
			_mm_and_pd(
				_mm_cmpeq_pd(x.val[0], y),
				_mm_cmpgt_pd(x.val[1], _mm_setzero_pd())
			)
		);
	}

	/**
	 * @brief _CMP_GE_OQ
	 */
	static inline __m128d _mm_cmpge_pdx4_pd(__m128dx4 x, __m128d y) {
		return _mm_or_pd(
			_mm_cmpgt_pd(x.val[0], y),
			_mm_and_pd(
				_mm_cmpeq_pd(x.val[0], y),
				_mm_cmpge_pd(x.val[1], _mm_setzero_pd())
			)
		);
	}

	/**
	 * @brief _CMP_NLT_UQ
	 */
	static inline __m128d _mm_cmpnlt_pdx4_pd(__m128dx4 x, __m128d y) {
		return _mm_and_pd(
			_mm_cmpnlt_pd(x.val[0], y),
			_mm_or_pd(
				_mm_cmpneq_pd(x.val[0], y),
				_mm_cmpnlt_pd(x.val[1], _mm_setzero_pd())
			)
		);
	}

	/**
	 * @brief _CMP_NLE_UQ
	 */
	static inline __m128d _mm_cmpnle_pdx4_pd(__m128dx4 x, __m128d y) {
		return _mm_and_pd(
			_mm_cmpnlt_pd(x.val[0], y),
			_mm_or_pd(
				_mm_cmpneq_pd(x.val[0], y),
				_mm_cmpnle_pd(x.val[1], _mm_setzero_pd())
			)
		);
	}

	/**
	 * @brief _CMP_NGT_UQ
	 */
	static inline __m128d _mm_cmpngt_pdx4_pd(__m128dx4 x, __m128d y) {
		return _mm_and_pd(
			_mm_cmpngt_pd(x.val[0], y),
			_mm_or_pd(
				_mm_cmpneq_pd(x.val[0], y),
				_mm_cmpngt_pd(x.val[1], _mm_setzero_pd())
			)
		);
	}

	/**
	 * @brief _CMP_NGE_UQ
	 */
	static inline __m128d _mm_cmpnge_pdx4_pd(__m128dx4 x, __m128d y) {
		return _mm_and_pd(
			_mm_cmpngt_pd(x.val[0], y),
			_mm_or_pd(
				_mm_cmpneq_pd(x.val[0], y),
				_mm_cmpnge_pd(x.val[1], _mm_setzero_pd())
			)
		);
	}

/* __m128d compare __m128dx4 */

	/**
	 * @brief _CMP_ORD_Q
	 */
	static inline __m128d _mm_cmpord_pd_pdx4(__m128d x, __m128dx4 y) {
		return _mm_cmpord_pd(x, y.val[0]);
	}

	/**
	 * @brief _CMP_UNORD_Q
	 */
	static inline __m128d _mm_cmpunord_pd_pdx4(__m128d x, __m128dx4 y) {
		return _mm_cmpunord_pd(x, y.val[0]);
	}

	/**
	 * @brief _CMP_EQ_OQ
	 */
	static inline __m128d _mm_cmpeq_pd_pdx4(__m128d x, __m128dx4 y) {
		__m128d cmp_hi = _mm_cmpeq_pd(x, y.val[0]);
		__m128d cmp_lo = _mm_cmpeq_pd(_mm_setzero_pd(), y.val[1]);
		return _mm_and_pd(cmp_hi, cmp_lo);
	}

	/**
	 * @brief _CMP_NEQ_OQ
	 */
	static inline __m128d _mm_cmpneq_pd_pdx4(__m128d x, __m128dx4 y) {
		__m128d cmp_hi = _mm_cmpneq_pd(x, y.val[0]);
		__m128d cmp_lo = _mm_cmpneq_pd(_mm_setzero_pd(), y.val[1]);
		return _mm_or_pd(cmp_hi, cmp_lo);
	}

	/**
	 * @brief _CMP_NEQ_UQ
	 */
	static inline __m128d _mm_cmplg_pd_pdx4(__m128d x, __m128dx4 y) {
		__m128d cmp_hi = _mm_cmplg_pd(x, y.val[0]);
		__m128d cmp_lo = _mm_cmplg_pd(_mm_setzero_pd(), y.val[1]);
		return _mm_or_pd(cmp_hi, cmp_lo);
	}

	/**
	 * @brief _CMP_EQ_UQ
	 */
	static inline __m128d _mm_cmpnlg_pd_pdx4(__m128d x, __m128dx4 y) {
		__m128d cmp_hi = _mm_cmpnlg_pd(x, y.val[0]);
		__m128d cmp_lo = _mm_cmpnlg_pd(_mm_setzero_pd(), y.val[1]);
		return _mm_and_pd(cmp_hi, cmp_lo);
	}

		/**
	 * @brief _CMP_LT_OQ
	 */
	static inline __m128d _mm_cmplt_pd_pdx4(__m128d x, __m128dx4 y) {
		return _mm_or_pd(
			_mm_cmplt_pd(x, y.val[0]),
			_mm_and_pd(
				_mm_cmpeq_pd(x, y.val[0]),
				_mm_cmplt_pd(_mm_setzero_pd(), y.val[1])
			)
		);
	}

	/**
	 * @brief _CMP_LE_OQ
	 */
	static inline __m128d _mm_cmple_pd_pdx4(__m128d x, __m128dx4 y) {
		return _mm_or_pd(
			_mm_cmplt_pd(x, y.val[0]),
			_mm_and_pd(
				_mm_cmpeq_pd(x, y.val[0]),
				_mm_cmple_pd(_mm_setzero_pd(), y.val[1])
			)
		);
	}

	/**
	 * @brief _CMP_GT_OQ
	 */
	static inline __m128d _mm_cmpgt_pd_pdx4(__m128d x, __m128dx4 y) {
		return _mm_or_pd(
			_mm_cmpgt_pd(x, y.val[0]),
			_mm_and_pd(
				_mm_cmpeq_pd(x, y.val[0]),
				_mm_cmpgt_pd(_mm_setzero_pd(), y.val[1])
			)
		);
	}

	/**
	 * @brief _CMP_GE_OQ
	 */
	static inline __m128d _mm_cmpge_pd_pdx4(__m128d x, __m128dx4 y) {
		return _mm_or_pd(
			_mm_cmpgt_pd(x, y.val[0]),
			_mm_and_pd(
				_mm_cmpeq_pd(x, y.val[0]),
				_mm_cmpge_pd(_mm_setzero_pd(), y.val[1])
			)
		);
	}

	/**
	 * @brief _CMP_NLT_UQ
	 */
	static inline __m128d _mm_cmpnlt_pd_pdx4(__m128d x, __m128dx4 y) {
		return _mm_and_pd(
			_mm_cmpnlt_pd(x, y.val[0]),
			_mm_or_pd(
				_mm_cmpneq_pd(x, y.val[0]),
				_mm_cmpnlt_pd(_mm_setzero_pd(), y.val[1])
			)
		);
	}

	/**
	 * @brief _CMP_NLE_UQ
	 */
	static inline __m128d _mm_cmpnle_pd_pdx4(__m128d x, __m128dx4 y) {
		return _mm_and_pd(
			_mm_cmpnlt_pd(x, y.val[0]),
			_mm_or_pd(
				_mm_cmpneq_pd(x, y.val[0]),
				_mm_cmpnle_pd(_mm_setzero_pd(), y.val[1])
			)
		);
	}

	/**
	 * @brief _CMP_NGT_UQ
	 */
	static inline __m128d _mm_cmpngt_pd_pdx4(__m128d x, __m128dx4 y) {
		return _mm_and_pd(
			_mm_cmpngt_pd(x, y.val[0]),
			_mm_or_pd(
				_mm_cmpneq_pd(x, y.val[0]),
				_mm_cmpngt_pd(_mm_setzero_pd(), y.val[1])
			)
		);
	}

	/**
	 * @brief _CMP_NGE_UQ
	 */
	static inline __m128d _mm_cmpnge_pd_pdx4(__m128d x, __m128dx4 y) {
		return _mm_and_pd(
			_mm_cmpngt_pd(x, y.val[0]),
			_mm_or_pd(
				_mm_cmpneq_pd(x, y.val[0]),
				_mm_cmpnge_pd(_mm_setzero_pd(), y.val[1])
			)
		);
	}

//------------------------------------------------------------------------------
// __m128dx4 compare to zero functions
//------------------------------------------------------------------------------

/**
 * @brief _CMP_EQ_OQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmpeq_zero_pdx4(__m128dx4 x) {
	return _mm_cmpeq_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_NEQ_OQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmpneq_zero_pdx4(__m128dx4 x) {
	return _mm_cmpneq_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_NEQ_UQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmplg_zero_pdx4(__m128dx4 x) {
	return _mm_cmplg_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_EQ_UQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmpnlg_zero_pdx4(__m128dx4 x) {
	return _mm_cmpnlg_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_LT_OQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmplt_zero_pdx4(__m128dx4 x) {
	return _mm_cmplt_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_LE_OQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmple_zero_pdx4(__m128dx4 x) {
	return _mm_cmple_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_GT_OQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmpgt_zero_pdx4(__m128dx4 x) {
	return _mm_cmpgt_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_GE_OQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmpge_zero_pdx4(__m128dx4 x) {
	return _mm_cmpge_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_NLT_UQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmpnlt_zero_pdx4(__m128dx4 x) {
	return _mm_cmpnlt_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_NLE_UQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmpnle_zero_pdx4(__m128dx4 x) {
	return _mm_cmpnle_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_NGT_UQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmpngt_zero_pdx4(__m128dx4 x) {
	return _mm_cmpngt_pd(x.val[0], _mm_setzero_pd());
}

/**
 * @brief _CMP_NGE_UQ
 * @note Assumes that if x.val[0] is zero then x.val[1 - 3] are also zero.
 */
static inline __m128d _mm_cmpnge_zero_pdx4(__m128dx4 x) {
	return _mm_cmpnge_pd(x.val[0], _mm_setzero_pd());
}

//------------------------------------------------------------------------------
// __m128dx4 qd_real functions
//------------------------------------------------------------------------------

/** 
 * @author Taken and/or modified from libQD which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */

/** @brief Computes fl(x + y) and err(x + y). Assumes |x| >= |y|. */
static inline __m128d _mm_quick_two_sum_pd(const __m128d x, const __m128d y, __m128d* LDF_restrict const err) {
	__m128d s = _mm_add_pd(x, y);
	*err = _mm_sub_pd(y, _mm_sub_pd(s, x));
	return s;
}

/** @brief Computes fl(x - y) and err(x - y). Assumes |x| >= |y|. */
static inline __m128d _mm_quick_two_diff_pd(const __m128d x, const __m128d y, __m128d* LDF_restrict const err) {
	__m128d s = _mm_sub_pd(x, y);
	*err = _mm_sub_pd(_mm_sub_pd(x, s), y);
	return s;
}

/** @brief Computes fl(x + y) and err(x + y). */
static inline __m128d _mm_two_sum_pd(const __m128d x, const __m128d y, __m128d* LDF_restrict const err) {
	__m128d s = _mm_add_pd(x, y);
	__m128d bb = _mm_sub_pd(s, x);
	*err = _mm_add_pd(
		_mm_sub_pd(x, _mm_sub_pd(s, bb)),
		_mm_sub_pd(y, bb)
	);
	return s;
}

/** @brief Computes fl(x - y) and err(x - y). */
static inline __m128d _mm_two_diff_pd(const __m128d x, const __m128d y, __m128d* LDF_restrict const err) {
	__m128d s = _mm_sub_pd(x, y);
	__m128d bb = _mm_sub_pd(s, x);
	*err = _mm_sub_pd(
		_mm_sub_pd(x, _mm_sub_pd(s, bb)),
		_mm_add_pd(y, bb)
	);
	return s;
}

/** @brief Computes fl(x * y) and err(x * y). */
static inline __m128d _mm_two_prod_pd(const __m128d x, const __m128d y, __m128d* LDF_restrict const err) {
	__m128d p = _mm_mul_pd(x, y);
	__m128dx2 a = _mm128x2_dekker_split_pd(x);
	__m128dx2 b = _mm128x2_dekker_split_pd(y);
	*err = _mm_add_pd(
		_mm_add_pd(
			_mm_add_pd(
				_mm_sub_pd(_mm_mul_pd(a.hi, b.hi), p),
				_mm_mul_pd(a.hi, b.lo)
			), _mm_mul_pd(a.lo, b.hi)
		), _mm_mul_pd(a.lo, b.lo)
	);
	return p;
}

/** @brief Computes fl(x * x) and err(x * x). Faster than _mm_two_prod_pd(x, x, err) */
static inline __m128d _mm_two_sqr_pd(const __m128d x, __m128d* LDF_restrict const err) {
	__m128d q = _mm_mul_pd(x, x);
	__m128dx2 a = _mm128x2_dekker_split_pd(x);
	*err = _mm_add_pd(
		_mm_add_pd(
			_mm_sub_pd(_mm_mul_pd(a.hi, a.hi), q),
			_mm_mul_pd(_mm_set1_pd(2.0), _mm_mul_pd(a.hi, a.lo))
		), _mm_mul_pd(a.lo, a.lo)
	);
	return q;
}

/** 
 * @brief Adds `c` to the dd-pair `(a, b)`. If the result does not fit in two
 * doubles, then the sum is output into `s` and `(a, b)` contains the
 * remainder. Otherwise `s` is zero and `(a, b)` contains the sum.
 */
static inline __m128d _mm_quick_three_accum_pd(
	__m128d* LDF_restrict const a,
	__m128d* LDF_restrict const b,
	const __m128d c
) {
	__m128d s;
	__m128d za, zb; // bool

	s = _mm_two_sum_pd(*b, c, b);
	s = _mm_two_sum_pd(*a, s, a);

	za = _mm_cmpneq_pd(*a, _mm_setzero_pd());
	zb = _mm_cmpneq_pd(*b, _mm_setzero_pd());

	*b = _internal_mm_blendv_pd(*a, *b, zb);
	__m128d cmp_ret = _mm_and_pd(za, zb);
	*a = _internal_mm_blendv_pd(s, *a, cmp_ret);
	return _internal_mm_blendv_pd(_mm_setzero_pd(), s, cmp_ret);
}

static inline void _mm_three_sum_pd(
	__m128d* LDF_restrict const a,
	__m128d* LDF_restrict const b,
	__m128d* LDF_restrict const c
) {
	__m128d t1, t2, t3;
	t1 = _mm_two_sum_pd(*a, *b, &t2);
	*a = _mm_two_sum_pd(*c, t1, &t3);
	*b = _mm_two_sum_pd(t2, t3, c  );
}

static inline void _mm_three_sum2_pd(
	__m128d* LDF_restrict const a,
	__m128d* LDF_restrict const b,
	const __m128d c
) {
	__m128d t1, t2, t3;
	t1 = _mm_two_sum_pd(*a, *b, &t2);
	*a = _mm_two_sum_pd( c, t1, &t3);
	*b = _mm_add_pd(t2, t3);
}

//------------------------------------------------------------------------------
// __m128dx4 qd_real renormalization
//------------------------------------------------------------------------------

#if 0
/**
 * @remarks This function has a lot of branching for SIMD
 */
static inline void _mm128x4_accurate_renorm_pdx4(__m128dx4* LDF_restrict const x) {
	__m128d s0, s1, s2 = 0.0, s3 = 0.0;

	// if (isinf(x->val[0])) {
	// 	return;
	// }

	s0 = _mm_quick_two_sum_pd(x->val[2], x->val[3], &x->val[3]);
	s0 = _mm_quick_two_sum_pd(x->val[1], s0, &x->val[2]);
	x->val[0] = _mm_quick_two_sum_pd(x->val[0], s0, &x->val[1]);

	s0 = x->val[0];
	s1 = x->val[1];



	if (s1 != 0.0) {
		s1 = _mm_quick_two_sum_pd(s1, x->val[2], &s2);
		if (s2 != 0.0) {
			s2 = _mm_quick_two_sum_pd(s2, x->val[3], &s3); // 4 terms
		} else {
			s1 = _mm_quick_two_sum_pd(s1, x->val[3], &s2); // 3 terms
		}
	} else {
		s0 = _mm_quick_two_sum_pd(s0, x->val[2], &s1);
		if (s1 != 0.0) {
			s1 = _mm_quick_two_sum_pd(s1, x->val[3], &s2); // 3 terms
		} else {
			s0 = _mm_quick_two_sum_pd(s0, x->val[3], &s1); // 2 terms
		}
	}

	x->val[0] = s0;
	x->val[1] = s1;
	x->val[2] = s2;
	x->val[3] = s3;
}
#else

static inline void _mm128x4_accurate_renorm_pdx4(__m128dx4* LDF_restrict const x) {
	__m128d s[4];

	// if (isinf(x->val[0])) {
	// 	return;
	// }

	s[0]      = _mm_quick_two_sum_pd(x->val[2], x->val[3], &x->val[3]);
	s[0]      = _mm_quick_two_sum_pd(x->val[1],      s[0], &x->val[2]);
	x->val[0] = _mm_quick_two_sum_pd(x->val[0],      s[0], &x->val[1]);

	s[0] = x->val[0];
	s[1] = x->val[1];

	__m128d temp[3];
	__m128d cmp_test[2];
	
	cmp_test[0] = _mm_cmpneq_pd(s[1], _mm_setzero_pd());
	temp[0] = _internal_mm_blendv_pd(s[0], s[1], cmp_test[0]);
	temp[0] = _mm_quick_two_sum_pd(temp[0], x->val[2], &temp[1]);

	cmp_test[1] = _mm_cmpneq_pd(temp[1], _mm_setzero_pd());
	temp[1] = _internal_mm_blendv_pd(temp[0], temp[1], cmp_test[0]);
	temp[1] = _mm_quick_two_sum_pd(temp[1], x->val[3], &temp[2]);

	x->val[0] = s[0];
	x->val[1] = temp[0];
	x->val[2] = temp[1];
	x->val[3] = temp[2];

	__m128d cmp_3_terms = _mm_or_pd(cmp_test[0], cmp_test[1]);
	__m128d cmp_4_terms = _mm_and_pd(cmp_test[0], cmp_test[1]);
	
	// shifts if terms >= 3
	x->val[0] = _internal_mm_blendv_pd(x->val[1]           , x->val[0], cmp_3_terms);
	x->val[1] = _internal_mm_blendv_pd(x->val[2]           , x->val[1], cmp_3_terms);
	x->val[2] = _internal_mm_blendv_pd(x->val[3]           , x->val[2], cmp_3_terms);
	x->val[3] = _internal_mm_blendv_pd(_mm_setzero_pd(), x->val[3], cmp_3_terms);
	// shifts if terms >= 4
	x->val[0] = _internal_mm_blendv_pd(x->val[1]           , x->val[0], cmp_4_terms);
	x->val[1] = _internal_mm_blendv_pd(x->val[2]           , x->val[1], cmp_4_terms);
	x->val[2] = _internal_mm_blendv_pd(_mm_setzero_pd(), x->val[2], cmp_4_terms);
	
	__m128d cmp_finite = _mm_isfinite_pd(x->val[0]);
	x->val[1] = _internal_mm_blendv_pd(_mm_setzero_pd(), x->val[1], cmp_finite);
	x->val[2] = _internal_mm_blendv_pd(_mm_setzero_pd(), x->val[2], cmp_finite);
	x->val[3] = _internal_mm_blendv_pd(_mm_setzero_pd(), x->val[3], cmp_finite);
}
#endif

static inline void _mm128x4_quick_renorm_pdx4(__m128dx4* LDF_restrict const x) {
	__m128d t0, t1, t2;
	__m128d s;
	s         = x->val[3];
	s         = _mm_quick_two_sum_pd(x->val[2], s, &t2);
	s         = _mm_quick_two_sum_pd(x->val[1], s, &t1);
	x->val[0] = _mm_quick_two_sum_pd(x->val[0], s, &t0);

	s         = _mm_quick_two_sum_pd(t1, t2, &t1);
	x->val[1] = _mm_quick_two_sum_pd(t0, s , &t0);
	x->val[2] = _mm_quick_two_sum_pd(t0, t1, &t0);
	x->val[3] = t0;
}

static inline void _mm128x4_renorm_pdx4(__m128dx4* LDF_restrict const x) {
	// #ifdef FLOAT64X4_IEEE_MODE
	// 	_mm128x4_accurate_renorm_pdx4(x, err);
	// #else
		_mm128x4_quick_renorm_pdx4(x);
	// #endif
}

#if 0
/**
 * @remarks This function has a lot of branching for SIMD
 */
static inline void _mm128x4_accurate_renorm_err_pdx4(
	__m128dx4* LDF_restrict const x,
	__m128d* LDF_restrict const err
) {
	__m128d s0, s1, s2 = 0.0, s3 = 0.0;

	if (isinf(x->val[0])) {
		return;
	}

	s0        = _mm_quick_two_sum_pd(x->val[3], *err, err       );
	s0        = _mm_quick_two_sum_pd(x->val[2], s0  , &x->val[3]);
	s0        = _mm_quick_two_sum_pd(x->val[1], s0  , &x->val[2]);
	x->val[0] = _mm_quick_two_sum_pd(x->val[0], s0  , &x->val[1]);

	s0 = x->val[0];
	s1 = x->val[1];

	if (s1 != 0.0) {
		s1 = _mm_quick_two_sum_pd(s1, x->val[2], &s2);
		if (s2 != 0.0) {
		s2 = _mm_quick_two_sum_pd(s2, x->val[3], &s3);
			if (s3 != 0.0) {
				s3 += *err;
			} else {
				s2 = _mm_quick_two_sum_pd(s2, *err, &s3);
			}
		} else {
		s1 = _mm_quick_two_sum_pd(s1, x->val[3], &s2);
			if (s2 != 0.0) {
				s2 = _mm_quick_two_sum_pd(s2, *err, &s3);
			} else {
				s1 = _mm_quick_two_sum_pd(s1, *err, &s2);
			}
		}
	} else {
		s0 = _mm_quick_two_sum_pd(s0, x->val[2], &s1);
		if (s1 != 0.0) {
		s1 = _mm_quick_two_sum_pd(s1, x->val[3], &s2);
			if (s2 != 0.0) {
				s2 = _mm_quick_two_sum_pd(s2, *err, &s3);
			} else {
				s1 = _mm_quick_two_sum_pd(s1, *err, &s2);
			}
		} else {
		s0 = _mm_quick_two_sum_pd(s0, x->val[3], &s1);
			if (s1 != 0.0) {
				s1 = _mm_quick_two_sum_pd(s1, *err, &s2);
			} else {
				s0 = _mm_quick_two_sum_pd(s0, *err, &s1);
			}
		}
	}

	x->val[0] = s0;
	x->val[1] = s1;
	x->val[2] = s2;
	x->val[3] = s3;
}
#endif

static inline void _mm128x4_quick_renorm_err_pdx4(
	__m128dx4* LDF_restrict const x, __m128d* LDF_restrict const err
) {
	__m128d t0, t1, t2, t3;
	__m128d s;
	s         = _mm_quick_two_sum_pd(x->val[3], *err, &t3);
	s         = _mm_quick_two_sum_pd(x->val[2], s   , &t2);
	s         = _mm_quick_two_sum_pd(x->val[1], s   , &t1);
	x->val[0] = _mm_quick_two_sum_pd(x->val[0], s   , &t0);

	s         = _mm_quick_two_sum_pd(t2, t3, &t2);
	s         = _mm_quick_two_sum_pd(t1, s , &t1);
	x->val[1] = _mm_quick_two_sum_pd(t0, s , &t0);

	s         = _mm_quick_two_sum_pd(t1, t2, &t1);
	x->val[2] = _mm_quick_two_sum_pd(t0, s , &t0);
	
	x->val[3] = _mm_add_pd(t0, t1);
}

static inline void _mm128x4_renorm_err_pdx4(
	__m128dx4* LDF_restrict const x, __m128d* LDF_restrict const err
) {
	// #ifdef FLOAT64X4_IEEE_MODE
	// 	_mm128x4_accurate_renorm_err_pdx4(x, err);
	// #else
		_mm128x4_quick_renorm_err_pdx4(x, err);
	// #endif
}

//------------------------------------------------------------------------------
// __m128dx4 Arithmetic
//------------------------------------------------------------------------------

/* Negation */

/**
 * @brief Negates a __m128dx4 value (multiplies by -1.0)
 */
static inline __m128dx4 _mm128x4_negate_pdx4(__m128dx4 x) {
	const __m128d sign_bit_mask = _mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x8000000000000000));
	x.val[0] = _mm_xor_pd(x.val[0], sign_bit_mask);
	x.val[1] = _mm_xor_pd(x.val[1], sign_bit_mask);
	x.val[2] = _mm_xor_pd(x.val[2], sign_bit_mask);
	x.val[3] = _mm_xor_pd(x.val[3], sign_bit_mask);
	return x;
}

//------------------------------------------------------------------------------
// __m128dx4 Addition
//------------------------------------------------------------------------------
/** 
 * @author Taken and/or modified from libQD which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */

/* __m128dx4 + __m128dx4 */

static inline __m128dx4 _mm128x4_add_quick_pdx4(__m128dx4 x, __m128dx4 y) {
	/*
	__m128d s0, s1, s2, s3;
	__m128d t0, t1, t2, t3;
	
	s0 = _mm_two_sum_pd(x.val[0], y.val[0], t0);
	s1 = _mm_two_sum_pd(x.val[1], y.val[1], t1);
	s2 = _mm_two_sum_pd(x.val[2], y.val[2], t2);
	s3 = _mm_two_sum_pd(x.val[3], y.val[3], t3);

	s1 = _mm_two_sum_pd(s1, t0, t0);
	_mm_three_sum_pd(s2, t0, t1);
	_mm_three_sum2_pd(s3, t0, t2);
	t0 = t0 + t1 + t3;

	_mm128x4_renorm_err_pdx4(s0, s1, s2, s3, t0);
	return __m128dx4(s0, s1, s2, s3, t0);
	*/

	/* Same as above, but addition re-organized to minimize
		data dependency ... unfortunately some compilers are
		not very smart to do this automatically */
	__m128dx4 s;
	__m128d t0, t1, t2, t3;

	__m128d v0, v1, v2, v3;
	__m128d u0, u1, u2, u3;
	__m128d w0, w1, w2, w3;

	s.val[0] = _mm_add_pd(x.val[0], y.val[0]);
	s.val[1] = _mm_add_pd(x.val[1], y.val[1]);
	s.val[2] = _mm_add_pd(x.val[2], y.val[2]);
	s.val[3] = _mm_add_pd(x.val[3], y.val[3]);

	v0 = _mm_sub_pd(s.val[0], x.val[0]);
	v1 = _mm_sub_pd(s.val[1], x.val[1]);
	v2 = _mm_sub_pd(s.val[2], x.val[2]);
	v3 = _mm_sub_pd(s.val[3], x.val[3]);

	u0 = _mm_sub_pd(s.val[0], v0);
	u1 = _mm_sub_pd(s.val[1], v1);
	u2 = _mm_sub_pd(s.val[2], v2);
	u3 = _mm_sub_pd(s.val[3], v3);

	w0 = _mm_sub_pd(x.val[0], u0);
	w1 = _mm_sub_pd(x.val[1], u1);
	w2 = _mm_sub_pd(x.val[2], u2);
	w3 = _mm_sub_pd(x.val[3], u3);

	u0 = _mm_sub_pd(y.val[0], v0);
	u1 = _mm_sub_pd(y.val[1], v1);
	u2 = _mm_sub_pd(y.val[2], v2);
	u3 = _mm_sub_pd(y.val[3], v3);

	t0 = _mm_add_pd(w0, u0);
	t1 = _mm_add_pd(w1, u1);
	t2 = _mm_add_pd(w2, u2);
	t3 = _mm_add_pd(w3, u3);

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);
	_mm_three_sum_pd (&s.val[2], &t0, &t1);
	_mm_three_sum2_pd(&s.val[3], &t0,  t2);
	t0 = _mm_add_pd(_mm_add_pd(t0, t1), t3);

	/* renormalize */
	_mm128x4_renorm_err_pdx4(&s, &t0);
	return s;
}

static inline __m128dx4 _mm128x4_add_pdx4(__m128dx4 x, __m128dx4 y) {
	// #ifdef __M256DX4_IEEE_MODE
	// 	return _mm128x4_add_accurate_pdx4(x, y);
	// #else
		return _mm128x4_add_quick_pdx4(x, y);
	// #endif
}

/* __m128dx4 + __m128dx2 */

static inline __m128dx4 _mm128x4_add_pdx4_pdx2(const __m128dx4 x, const __m128dx2 y) {
	__m128dx4 s;
	__m128d t0, t1;

	s.val[0] = _mm_two_sum_pd(x.val[0], y.hi, &t0);
	s.val[1] = _mm_two_sum_pd(x.val[1], y.lo, &t1);

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);

	s.val[2] = x.val[2];
	_mm_three_sum_pd(&s.val[2], &t0, &t1);

	s.val[3] = _mm_two_sum_pd(t0, x.val[3], &t0);
	t0 = _mm_add_pd(t0, t1);

	_mm128x4_renorm_err_pdx4(&s, &t0);
	return s;
}

/* __m128dx2 + __m128dx4 */

static inline __m128dx4 _mm128x4_add_pdx2_pdx4(const __m128dx2 x, const __m128dx4 y) {
	__m128dx4 s;
	__m128d t0, t1;

	s.val[0] = _mm_two_sum_pd(x.hi, y.val[0], &t0);
	s.val[1] = _mm_two_sum_pd(x.lo, y.val[1], &t1);

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);

	s.val[2] = y.val[2];
	_mm_three_sum_pd(&s.val[2], &t0, &t1);

	s.val[3] = _mm_two_sum_pd(t0, y.val[3], &t0);
	t0 = _mm_add_pd(t0, t1);

	_mm128x4_renorm_err_pdx4(&s, &t0);
	return s;
}

/* __m128dx4 + __m128d */

static inline __m128dx4 _mm128x4_add_pdx4_pd(const __m128dx4 x, const __m128d y) {
	__m128dx4 ret;
	__m128d e;

	ret.val[0] = _mm_two_sum_pd(x.val[0], y, &e);
	ret.val[1] = _mm_two_sum_pd(x.val[1], e, &e);
	ret.val[2] = _mm_two_sum_pd(x.val[2], e, &e);
	ret.val[3] = _mm_two_sum_pd(x.val[3], e, &e);

	_mm128x4_renorm_err_pdx4(&ret, &e);
	return ret;
}

/* __m128d + __m128dx4 */

static inline __m128dx4 _mm128x4_add_pd_pdx4(const __m128d x, const __m128dx4 y) {
	__m128dx4 ret;
	__m128d e;

	ret.val[0] = _mm_two_sum_pd(x, y.val[0], &e);
	ret.val[1] = _mm_two_sum_pd(e, y.val[1], &e);
	ret.val[2] = _mm_two_sum_pd(e, y.val[2], &e);
	ret.val[3] = _mm_two_sum_pd(e, y.val[3], &e);

	_mm128x4_renorm_err_pdx4(&ret, &e);
	return ret;
}

//------------------------------------------------------------------------------
// __m128dx4 Optimized Addition
//------------------------------------------------------------------------------

/* __m128dx2 + __m128dx2 */

static inline __m128dx4 _mm128x4_add_pdx2_pdx2(const __m128dx2 x, const __m128dx2 y) {
	__m128dx4 s;
	__m128d t0, t1;

	s.val[0] = _mm_two_sum_pd(x.hi, y.hi, &t0);
	s.val[1] = _mm_two_sum_pd(x.lo, y.lo, &t1);

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);
	s.val[2] = _mm_two_sum_pd(t0, t1, &t0);
	s.val[3] = t0;

	_mm128x4_renorm_err_pdx4(&s, &t1);
	return s;
}

/* __m128dx2 + __m128d */

static inline __m128dx4 _mm128x4_add_pdx2_pd(const __m128dx2 x, const __m128d y) {
	__m128dx4 s;
	__m128d t0;

	s.val[0] = _mm_two_sum_pd(x.hi, y, &t0);
	s.val[1] = x.lo;

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);
	s.val[2] = t0;
	s.val[3] = _mm_setzero_pd();
	return s; // renormalization not needed
}

/* __m128d + __m128dx2 */

static inline __m128dx4 _mm128x4_add_pd_pdx2(const __m128d x, const __m128dx2 y) {
	__m128dx4 s;
	__m128d t0;

	s.val[0] = _mm_two_sum_pd(x, y.hi, &t0);
	s.val[1] = y.lo;

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);
	s.val[2] = t0;
	s.val[3] = _mm_setzero_pd();
	return s; // renormalization not needed
}

/* __m128d + __m128d */

static inline __m128dx4 _mm128x4_add_pd_pd(const __m128d x, const __m128d y) {
	__m128dx4 s;
	__m128d t0;

	s.val[0] = _mm_two_sum_pd(x, y, &t0);
	s.val[1] = t0;
	s.val[2] = _mm_setzero_pd();
	s.val[3] = _mm_setzero_pd();
	return s; // renormalization not needed
}

//------------------------------------------------------------------------------
// __m128dx4 Subtraction
//------------------------------------------------------------------------------
/** 
 * @author Taken and/or modified from libQD which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */

/* __m128dx4 - __m128dx4 */

/** @brief Orginally qd_real::sloppy_add */
static inline __m128dx4 _mm128x4_sub_quick_pdx4(const __m128dx4 x, const __m128dx4 y) {
	/*
	__m128d s0, s1, s2, s3;
	__m128d t0, t1, t2, t3;
	
	s0 = _mm_two_sum_pd(x.val[0], y.val[0], t0);
	s1 = _mm_two_sum_pd(x.val[1], y.val[1], t1);
	s2 = _mm_two_sum_pd(x.val[2], y.val[2], t2);
	s3 = _mm_two_sum_pd(x.val[3], y.val[3], t3);

	s1 = _mm_two_sum_pd(s1, t0, t0);
	_mm_three_sum_pd(s2, t0, t1);
	_mm_three_sum2_pd(s3, t0, t2);
	t0 = t0 + t1 + t3;

	_mm128x4_renorm_err_pdx4(s0, s1, s2, s3, t0);
	return __m128dx4(s0, s1, s2, s3, t0);
	*/

	/* Same as above, but addition re-organized to minimize
		data dependency ... unfortunately some compilers are
		not very smart to do this automatically */
	__m128dx4 s;
	__m128d t0, t1, t2, t3;

	__m128d v0, v1, v2, v3;
	__m128d u0, u1, u2, u3;
	__m128d w0, w1, w2, w3;

	s.val[0] = _mm_sub_pd(x.val[0], y.val[0]);
	s.val[1] = _mm_sub_pd(x.val[1], y.val[1]);
	s.val[2] = _mm_sub_pd(x.val[2], y.val[2]);
	s.val[3] = _mm_sub_pd(x.val[3], y.val[3]);

	v0 = _mm_sub_pd(s.val[0], x.val[0]);
	v1 = _mm_sub_pd(s.val[1], x.val[1]);
	v2 = _mm_sub_pd(s.val[2], x.val[2]);
	v3 = _mm_sub_pd(s.val[3], x.val[3]);

	u0 = _mm_sub_pd(s.val[0], v0);
	u1 = _mm_sub_pd(s.val[1], v1);
	u2 = _mm_sub_pd(s.val[2], v2);
	u3 = _mm_sub_pd(s.val[3], v3);

	w0 = _mm_sub_pd(x.val[0], u0);
	w1 = _mm_sub_pd(x.val[1], u1);
	w2 = _mm_sub_pd(x.val[2], u2);
	w3 = _mm_sub_pd(x.val[3], u3);

	// u is negative
	u0 = _mm_add_pd(y.val[0], v0);
	u1 = _mm_add_pd(y.val[1], v1);
	u2 = _mm_add_pd(y.val[2], v2);
	u3 = _mm_add_pd(y.val[3], v3);

	t0 = _mm_sub_pd(w0, u0);
	t1 = _mm_sub_pd(w1, u1);
	t2 = _mm_sub_pd(w2, u2);
	t3 = _mm_sub_pd(w3, u3);

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);
	_mm_three_sum_pd (&s.val[2], &t0, &t1);
	_mm_three_sum2_pd(&s.val[3], &t0,  t2);
	t0 = _mm_add_pd(_mm_add_pd(t0, t1), t3);

	/* renormalize */
	_mm128x4_renorm_err_pdx4(&s, &t0);
	return s;
}

static inline __m128dx4 _mm128x4_sub_pdx4(const __m128dx4 x, const __m128dx4 y) {
	// #ifdef __M256DX4_IEEE_MODE
	// 	return _mm128x4_sub_accurate_pdx4(x, y);
	// #else
		return _mm128x4_sub_quick_pdx4(x, y);
	// #endif
}

/* __m128dx4 - __m128dx2 */

static inline __m128dx4 _mm128x4_sub_pdx4_pdx2(const __m128dx4 x, const __m128dx2 y) {
	__m128dx4 s;
	__m128d t0, t1;

	s.val[0] = _mm_two_diff_pd(x.val[0], y.hi, &t0);
	s.val[1] = _mm_two_diff_pd(x.val[1], y.lo, &t1);

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);

	s.val[2] = x.val[2];
	_mm_three_sum_pd(&s.val[2], &t0, &t1);

	s.val[3] = _mm_two_sum_pd(t0, x.val[3], &t0);
	t0 = _mm_add_pd(t0, t1);

	_mm128x4_renorm_err_pdx4(&s, &t0);
	return s;
}

/* __m128dx2 - __m128dx4 */

static inline __m128dx4 _mm128x4_sub_pdx2_pdx4(const __m128dx2 x, const __m128dx4 y) {
	__m128dx4 s;
	__m128d t0, t1;

	s.val[0] = _mm_two_diff_pd(x.hi, y.val[0], &t0);
	s.val[1] = _mm_two_diff_pd(x.lo, y.val[1], &t1);

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);

	s.val[2] = _mm_negate_pd(y.val[2]);
	_mm_three_sum_pd(&s.val[2], &t0, &t1);

	s.val[3] = _mm_two_diff_pd(t0, y.val[3], &t0);
	t0 = _mm_add_pd(t0, t1);

	_mm128x4_renorm_err_pdx4(&s, &t0);
	return s;
}

/* __m128dx4 - __m128d */

static inline __m128dx4 _mm128x4_sub_pdx4_pd(const __m128dx4 x, const __m128d y) {
	__m128dx4 ret;
	__m128d e;

	ret.val[0] = _mm_two_diff_pd(x.val[0], y, &e);
	ret.val[1] = _mm_two_sum_pd(x.val[1], e, &e);
	ret.val[2] = _mm_two_sum_pd(x.val[2], e, &e);
	ret.val[3] = _mm_two_sum_pd(x.val[3], e, &e);

	_mm128x4_renorm_err_pdx4(&ret, &e);
	return ret;
}

/* __m128d - __m128dx4 */

static inline __m128dx4 _mm128x4_sub_pd_pdx4(const __m128d x, const __m128dx4 y) {
	__m128dx4 ret;
	__m128d e;

	ret.val[0] = _mm_two_diff_pd(x, y.val[0], &e);
	ret.val[1] = _mm_two_diff_pd(e, y.val[1], &e);
	ret.val[2] = _mm_two_diff_pd(e, y.val[2], &e);
	ret.val[3] = _mm_two_diff_pd(e, y.val[3], &e);

	_mm128x4_renorm_err_pdx4(&ret, &e);
	return ret;
}

//------------------------------------------------------------------------------
// __m128dx4 Optimized Subtraction
//------------------------------------------------------------------------------

/* __m128dx2 - __m128dx2 */

static inline __m128dx4 _mm128x4_sub_pdx2_pdx2(const __m128dx2 x, const __m128dx2 y) {
	__m128dx4 s;
	__m128d t0, t1;

	s.val[0] = _mm_two_diff_pd(x.hi, y.hi, &t0);
	s.val[1] = _mm_two_diff_pd(x.lo, y.lo, &t1);

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);
	s.val[2] = _mm_two_sum_pd(t0, t1, &t0);
	s.val[3] = t0;

	_mm128x4_renorm_err_pdx4(&s, &t1);
	return s;
}

/* __m128dx2 - __m128d */

static inline __m128dx4 _mm128x4_sub_pdx2_pd(const __m128dx2 x, const __m128d y) {
	__m128dx4 s;
	__m128d t0;

	s.val[0] = _mm_two_diff_pd(x.hi, y, &t0);
	s.val[1] = x.lo;

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);
	s.val[2] = t0;
	s.val[3] = _mm_setzero_pd();
	return s; // renormalization not needed
}

/* __m128d - __m128dx2 */

static inline __m128dx4 _mm128x4_sub_pd_pdx2(const __m128d x, const __m128dx2 y) {
	__m128dx4 s;
	__m128d t0;

	s.val[0] = _mm_two_diff_pd(x, y.hi, &t0);
	s.val[1] = _mm_negate_pd(y.lo);

	s.val[1] = _mm_two_sum_pd(s.val[1], t0, &t0);
	s.val[2] = t0;
	s.val[3] = _mm_setzero_pd();
	return s; // renormalization not needed
}

/* __m128d - __m128d */

static inline __m128dx4 _mm128x4_sub_pd_pd(const __m128d x, const __m128d y) {
	__m128dx4 s;
	__m128d t0;

	s.val[0] = _mm_two_diff_pd(x, y, &t0);
	s.val[1] = t0;
	s.val[2] = _mm_setzero_pd();
	s.val[3] = _mm_setzero_pd();
	return s; // renormalization not needed
}

//------------------------------------------------------------------------------
// __m128dx4 Multiplication
//------------------------------------------------------------------------------
/** 
 * @author Taken and/or modified from libQD which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */

/* __m128dx4 * __m128dx4 */

/** @brief Orginally qd_real::accurate_mul */
static inline __m128dx4 _mm128x4_mul_accurate_pdx4(const __m128dx4 x, const __m128dx4 y) {
	__m128d p0, p1, p2, p3, p4, p5;
	__m128d q0, q1, q2, q3, q4, q5;
	__m128d p6, p7, p8, p9;
	__m128d q6, q7, q8, q9;
	__m128d r0, r1;
	__m128d t0, t1;
	__m128d s0, s1, s2;

	p0 = _mm_two_prod_pd(x.val[0], y.val[0], &q0);

	p1 = _mm_two_prod_pd(x.val[0], y.val[1], &q1);
	p2 = _mm_two_prod_pd(x.val[1], y.val[0], &q2);

	p3 = _mm_two_prod_pd(x.val[0], y.val[2], &q3);
	p4 = _mm_two_prod_pd(x.val[1], y.val[1], &q4);
	p5 = _mm_two_prod_pd(x.val[2], y.val[0], &q5);

	/* Start Accumulation */
	_mm_three_sum_pd(&p1, &p2, &q0);

	/* Six-Three Sum of p2, q1, q2, p3, p4, p5. */
	_mm_three_sum_pd(&p2, &q1, &q2);
	_mm_three_sum_pd(&p3, &p4, &p5);
	/* compute (s0, s1, s2) = (p2, q1, q2) + (p3, p4, p5). */
	s0 = _mm_two_sum_pd(p2, p3, &t0);
	s1 = _mm_two_sum_pd(q1, p4, &t1);
	s2 = _mm_add_pd(q2, p5);
	s1 = _mm_two_sum_pd(s1, t0, &t0);
	s2 = _mm_add_pd(s2, _mm_add_pd(t0, t1));

	/* O(eps^3) order terms */
	p6 = _mm_two_prod_pd(x.val[0], y.val[3], &q6);
	p7 = _mm_two_prod_pd(x.val[1], y.val[2], &q7);
	p8 = _mm_two_prod_pd(x.val[2], y.val[1], &q8);
	p9 = _mm_two_prod_pd(x.val[3], y.val[0], &q9);

	/* Nine-Two-Sum of q0, s1, q3, q4, q5, p6, p7, p8, p9. */
	q0 = _mm_two_sum_pd(q0, q3, &q3);
	q4 = _mm_two_sum_pd(q4, q5, &q5);
	p6 = _mm_two_sum_pd(p6, p7, &p7);
	p8 = _mm_two_sum_pd(p8, p9, &p9);
	/* Compute (t0, t1) = (q0, q3) + (q4, q5). */
	t0 = _mm_two_sum_pd(q0, q4, &t1);
	t1 = _mm_add_pd(t1, _mm_add_pd(q3, q5));
	/* Compute (r0, r1) = (p6, p7) + (p8, p9). */
	r0 = _mm_two_sum_pd(p6, p8, &r1);
	r1 = _mm_add_pd(r1, _mm_add_pd(p7, p9));
	/* Compute (q3, q4) = (t0, t1) + (r0, r1). */
	q3 = _mm_two_sum_pd(t0, r0, &q4);
	q4 = _mm_add_pd(q4, _mm_add_pd(t1, r1));
	/* Compute (t0, t1) = (q3, q4) + s1. */
	t0 = _mm_two_sum_pd(q3, s1, &t1);
	t1 = _mm_add_pd(t1, q4);

	/* O(eps^4) terms -- Nine-One-Sum */
	t1 = _mm_add_pd(t1, _mm_add_pd(_mm_add_pd(_mm_add_pd(_mm_add_pd(_mm_add_pd(
	_mm_add_pd(
		_mm_add_pd(
			_mm_mul_pd(x.val[1], y.val[3]),
			_mm_mul_pd(x.val[2], y.val[2])
		), _mm_mul_pd(x.val[3], y.val[1])
	), q6), q7), q8), q9), s2));
	
	__m128dx4 ret = {{p0, p1, s0, t0}};
	_mm128x4_renorm_err_pdx4(&ret, &t1);
	return ret;
}

/** @brief Orginally qd_real::sloppy_mul */
static inline __m128dx4 _mm128x4_mul_quick_pdx4(const __m128dx4 x, const __m128dx4 y) {
	__m128d p0, p1, p2, p3, p4, p5;
	__m128d q0, q1, q2, q3, q4, q5;
	__m128d t0, t1;
	__m128d s0, s1, s2;

	p0 = _mm_two_prod_pd(x.val[0], y.val[0], &q0);

	p1 = _mm_two_prod_pd(x.val[0], y.val[1], &q1);
	p2 = _mm_two_prod_pd(x.val[1], y.val[0], &q2);

	p3 = _mm_two_prod_pd(x.val[0], y.val[2], &q3);
	p4 = _mm_two_prod_pd(x.val[1], y.val[1], &q4);
	p5 = _mm_two_prod_pd(x.val[2], y.val[0], &q5);

	/* Start Accumulation */
	_mm_three_sum_pd(&p1, &p2, &q0);

	/* Six-Three Sum of p2, q1, q2, p3, p4, p5. */
	_mm_three_sum_pd(&p2, &q1, &q2);
	_mm_three_sum_pd(&p3, &p4, &p5);
	/* compute (s0, s1, s2) = (p2, q1, q2) + (p3, p4, p5). */
	s0 = _mm_two_sum_pd(p2, p3, &t0);
	s1 = _mm_two_sum_pd(q1, p4, &t1);
	s2 = _mm_add_pd(q2, p5);
	s1 = _mm_two_sum_pd(s1, t0, &t0);
	s2 = _mm_add_pd(s2, _mm_add_pd(t0, t1));

	/* O(eps^3) order terms */
	s1 = _mm_add_pd(s1, _mm_add_pd(_mm_add_pd(_mm_add_pd(_mm_add_pd(
	_mm_add_pd(
		_mm_add_pd(
			_mm_add_pd(
				_mm_mul_pd(x.val[0], y.val[3]),
				_mm_mul_pd(x.val[1], y.val[2])
			), _mm_mul_pd(x.val[2], y.val[1])
		), _mm_mul_pd(x.val[3], y.val[0])
	), q0), q3), q4), q5));
	__m128dx4 ret = {{p0, p1, s0, s1}};
	_mm128x4_renorm_err_pdx4(&ret, &s2);
	return ret;
}

static inline __m128dx4 _mm128x4_mul_pdx4(const __m128dx4 x, const __m128dx4 y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_mul_accurate_pdx4(x, y);
	#else
		return _mm128x4_mul_quick_pdx4(x, y);
	#endif
}

/* __m128dx4 * __m128dx2 */

static inline __m128dx4 _mm128x4_mul_pdx4_pdx2(const __m128dx4 x, const __m128dx2 y) {
	__m128dx4 p;
	__m128d p_err;
	__m128d q0, q1, q2, q3, q4;
	__m128d s0, s1, s2;
	__m128d t0, t1;

	p.val[0] = _mm_two_prod_pd(x.val[0], y.hi, &q0);
	p.val[1] = _mm_two_prod_pd(x.val[0], y.lo, &q1);
	p.val[2] = _mm_two_prod_pd(x.val[1], y.hi, &q2);
	p.val[3] = _mm_two_prod_pd(x.val[1], y.lo, &q3);
	p_err    = _mm_two_prod_pd(x.val[2], y.hi, &q4);
	
	_mm_three_sum_pd(&p.val[1], &p.val[2], &q0);
	
	/* Five-Three-Sum */
	_mm_three_sum_pd(&p.val[2], &p.val[3], &p_err);
	q1 = _mm_two_sum_pd(q1      , q2, &q2);
	s0 = _mm_two_sum_pd(p.val[2], q1, &t0);
	s1 = _mm_two_sum_pd(p.val[3], q2, &t1);
	s1 = _mm_two_sum_pd(s1      , t0, &t0);
	s2 = _mm_add_pd(_mm_add_pd(t0, t1), p_err);
	p.val[2] = s0;

	p.val[3] = _mm_add_pd(_mm_add_pd(_mm_add_pd(
		_mm_mul_pd(x.val[2], y.hi), _mm_mul_pd(x.val[3], y.lo)
	), q3), q4);
	_mm_three_sum2_pd(&p.val[3], &q0, s1);
	p_err = _mm_add_pd(q0, s2);

	_mm128x4_renorm_err_pdx4(&p, &p_err);
	return p;
}

/* __m128dx2 * __m128dx4 */

static inline __m128dx4 _mm128x4_mul_pdx2_pdx4(const __m128dx2 x, const __m128dx4 y) {
	__m128dx4 p;
	__m128d p_err;
	__m128d q0, q1, q2, q3, q4;
	__m128d s0, s1, s2;
	__m128d t0, t1;

	p.val[0] = _mm_two_prod_pd(x.hi, y.val[0], &q0);
	p.val[1] = _mm_two_prod_pd(x.lo, y.val[0], &q1);
	p.val[2] = _mm_two_prod_pd(x.hi, y.val[1], &q2);
	p.val[3] = _mm_two_prod_pd(x.lo, y.val[1], &q3);
	p_err    = _mm_two_prod_pd(x.hi, y.val[2], &q4);
	
	_mm_three_sum_pd(&p.val[1], &p.val[2], &q0);
	
	/* Five-Three-Sum */
	_mm_three_sum_pd(&p.val[2], &p.val[3], &p_err);
	q1 = _mm_two_sum_pd(q1      , q2, &q2);
	s0 = _mm_two_sum_pd(p.val[2], q1, &t0);
	s1 = _mm_two_sum_pd(p.val[3], q2, &t1);
	s1 = _mm_two_sum_pd(s1      , t0, &t0);
	s2 = _mm_add_pd(_mm_add_pd(t0, t1), p_err);
	p.val[2] = s0;

	p.val[3] = _mm_add_pd(_mm_add_pd(_mm_add_pd(
		_mm_mul_pd(y.val[2], x.hi), _mm_mul_pd(y.val[3], x.lo)
	), q3), q4);
	_mm_three_sum2_pd(&p.val[3], &q0, s1);
	p_err = _mm_add_pd(q0, s2);

	_mm128x4_renorm_err_pdx4(&p, &p_err);
	return p;
}

/* __m128dx4 * __m128d */

static inline __m128dx4 _mm128x4_mul_pdx4_pd(const __m128dx4 x, const __m128d y) {
	__m128d p0, p1, p2, p3;
	__m128d q0, q1, q2;
	__m128dx4 s;
	__m128d s_err;

	p0 = _mm_two_prod_pd(x.val[0], y, &q0);
	p1 = _mm_two_prod_pd(x.val[1], y, &q1);
	p2 = _mm_two_prod_pd(x.val[2], y, &q2);
	p3 = _mm_mul_pd(x.val[3], y);

	s.val[0] = p0;

	s.val[1] = _mm_two_sum_pd(q0, p1, &s.val[2]);

	_mm_three_sum_pd(&s.val[2], &q1, &p2);

	_mm_three_sum2_pd(&q1, &q2, p3);
	s.val[3] = q1;

	s_err = _mm_add_pd(q2, p2);

	_mm128x4_renorm_err_pdx4(&s, &s_err);
	return s;
}

/* __m128d * __m128dx4 */

static inline __m128dx4 _mm128x4_mul_pd_pdx4(const __m128d x, const __m128dx4 y) {
	__m128d p0, p1, p2, p3;
	__m128d q0, q1, q2;
	__m128dx4 s;
	__m128d s_err;

	p0 = _mm_two_prod_pd(x, y.val[0], &q0);
	p1 = _mm_two_prod_pd(x, y.val[1], &q1);
	p2 = _mm_two_prod_pd(x, y.val[2], &q2);
	p3 = _mm_mul_pd(x, y.val[3]);

	s.val[0] = p0;

	s.val[1] = _mm_two_sum_pd(q0, p1, &s.val[2]);

	_mm_three_sum_pd(&s.val[2], &q1, &p2);

	_mm_three_sum2_pd(&q1, &q2, p3);
	s.val[3] = q1;

	s_err = _mm_add_pd(q2, p2);

	_mm128x4_renorm_err_pdx4(&s, &s_err);
	return s;
}

//------------------------------------------------------------------------------
// __m128dx4 Optimized Mutliplication
//------------------------------------------------------------------------------

/* __m128dx2 * __m128dx2 */

static inline __m128dx4 _mm128x4_mul_pdx2_pdx2(const __m128dx2 x, const __m128dx2 y) {
	__m128dx4 p;
	__m128d p_err;
	__m128d q0, q1, q2, q3;
	__m128d s0, s1, s2;
	__m128d t0, t1;

	p.val[0] = _mm_two_prod_pd(x.hi, y.hi, &q0);
	p.val[1] = _mm_two_prod_pd(x.lo, y.hi, &q1);
	p.val[2] = _mm_two_prod_pd(x.hi, y.lo, &q2);
	p.val[3] = _mm_two_prod_pd(x.lo, y.lo, &q3);
	
	_mm_three_sum_pd(&p.val[1], &p.val[2], &q0);
	
	/* Five-Three-Sum */
	p.val[2] = _mm_two_sum_pd(p.val[2], p.val[3], &p.val[3]);
	q1 = _mm_two_sum_pd(q1      , q2, &q2);
	s0 = _mm_two_sum_pd(p.val[2], q1, &t0);
	s1 = _mm_two_sum_pd(p.val[3], q2, &t1);
	s1 = _mm_two_sum_pd(s1      , t0, &t0);
	s2 = _mm_add_pd(t0, t1);
	p.val[2] = s0;

	p.val[3] = q3;
	_mm_three_sum2_pd(&p.val[3], &q0, s1);
	p_err = _mm_add_pd(q0, s2);

	_mm128x4_renorm_err_pdx4(&p, &p_err);
	return p;
}

/* __m128dx2 * __m128d */

/**
 * @brief Multiplies a __m128dx2 value with a __m128d value. Storing the result as
 * a __m128dx4 value
 */
static inline __m128dx4 _mm128x4_mul_pdx2_pd(const __m128dx2 x, const __m128d y) {
	__m128d p0, p1;
	__m128d q0, q1;
	__m128dx4 s;

	p0 = _mm_two_prod_pd(x.hi, y, &q0);
	p1 = _mm_two_prod_pd(x.lo, y, &q1);

	s.val[0] = p0;
	s.val[1] = _mm_two_sum_pd(q0, p1, &s.val[2]);
	s.val[2] = _mm_two_sum_pd(s.val[2], q1, &q1);
	s.val[3] = q1;

	_mm128x4_renorm_pdx4(&s);
	return s;
}

/* __m128d * __m128dx2 */

/**
 * @brief Multiplies a __m128d value with a __m128dx2 value. Storing the result as
 * a __m128dx4 value
 */
static inline __m128dx4 _mm128x4_mul_pd_pdx2(const __m128d x, const __m128dx2 y) {
	__m128d p0, p1;
	__m128d q0, q1;
	__m128dx4 s;

	p0 = _mm_two_prod_pd(x, y.hi, &q0);
	p1 = _mm_two_prod_pd(x, y.lo, &q1);

	s.val[0] = p0;
	s.val[1] = _mm_two_sum_pd(q0, p1, &s.val[2]);
	s.val[2] = _mm_two_sum_pd(s.val[2], q1, &q1);
	s.val[3] = q1;

	_mm128x4_renorm_pdx4(&s);
	return s;
}

/* __m128d * __m128d */

/**
 * @brief Multiplies a __m128d value with another __m128d value. Storing the result
 * as a __m128dx4 value.
 * @note __m128dx2 can store the values from this function exactly.
 */
static inline __m128dx4 _mm128x4_mul_pd_pd(const __m128d x, const __m128d y) {
	__m128dx4 s;
	s.val[0] = _mm_two_prod_pd(x, y, &s.val[1]);
	s.val[2] = _mm_setzero_pd();
	s.val[3] = _mm_setzero_pd(); 
	return s; // renormalization not needed
}

//------------------------------------------------------------------------------
// __m128dx4 Square
//------------------------------------------------------------------------------
/** 
 * @author Taken/Modified from libQD which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */

static inline __m128dx4 _mm128x4_square_pdx4(const __m128dx4 x) {
	__m128dx4 p;
	__m128d p_err_hi, p_err_lo;
	__m128d q0, q1, q2, q3;
	__m128d s0, s1;
	__m128d t0, t1;
	
	p.val[0] = _mm_two_sqr_pd(x.val[0], &q0);
	// q2 is used as temp storage for 2.0 * x.val[0]
	q2 = _mm_mul_pd(_mm_set1_pd(2.0), x.val[0]);
	p.val[1] = _mm_two_prod_pd(q2, x.val[1], &q1);
	p.val[2] = _mm_two_prod_pd(q2, x.val[2], &q2);
	p.val[3] = _mm_two_sqr_pd(x.val[1], &q3);

	p.val[1] = _mm_two_sum_pd(q0, p.val[1], &q0);

	q0 = _mm_two_sum_pd(q0, q1, &q1);
	p.val[2] = _mm_two_sum_pd(p.val[2], p.val[3], &p.val[3]);

	s0 = _mm_two_sum_pd(q0, p.val[2], &t0);
	s1 = _mm_two_sum_pd(q1, p.val[3], &t1);

	s1 = _mm_two_sum_pd(s1, t0, &t0);
	t0 = _mm_add_pd(t0, t1);

	s1 = _mm_quick_two_sum_pd(s1, t0, &t0);
	p.val[2] = _mm_quick_two_sum_pd(s0, s1, &t1);
	p.val[3] = _mm_quick_two_sum_pd(t1, t0, &q0);

	p_err_hi = _mm_mul_pd(_mm_set1_pd(2.0), _mm_mul_pd(x.val[0], x.val[3]));
	p_err_lo = _mm_mul_pd(_mm_set1_pd(2.0), _mm_mul_pd(x.val[1], x.val[2]));

	p_err_hi = _mm_two_sum_pd(p_err_hi, p_err_lo, &p_err_lo);
	q2 = _mm_two_sum_pd(q2, q3, &q3);

	t0 = _mm_two_sum_pd(p_err_hi, q2, &t1);
	t1 = t1 + p_err_lo + q3;

	p.val[3] = _mm_two_sum_pd(p.val[3], t0, &p_err_hi);
	p_err_hi = _mm_add_pd(_mm_add_pd(p_err_hi, q0), t1);

	_mm128x4_renorm_err_pdx4(&p, &p_err_hi);
	return p;
}

/**
 * @brief Squares a __m128dx2 value with the result stored as a __m128dx4
 */
static inline __m128dx4 _mm128x4_square_pdx2(const __m128dx2 x) {
	__m128dx4 p;
	__m128d p_err;
	__m128d q0, q1, q2;
	__m128d s0, s1;
	__m128d t0, t1;
	
	p.val[0] = _mm_two_sqr_pd(x.hi, &q0);
	p.val[1] = _mm_two_prod_pd(2.0 * x.hi, x.lo, &q1);
	p.val[2] = _mm_two_sqr_pd(x.lo, &q2);

	p.val[1] = _mm_two_sum_pd(q0, p.val[1], &q0);

	q0 = _mm_two_sum_pd(q0, q1, &q1);
	s0 = _mm_two_sum_pd(q0, p.val[2], &t0);

	s1 = _mm_two_sum_pd(q1, t0, &t0);
	s1 = _mm_quick_two_sum_pd(s1, t0, &t0);
	p.val[2] = _mm_quick_two_sum_pd(s0, s1, &t1);
	p.val[3] = _mm_quick_two_sum_pd(t1, t0, &q0);

	p.val[3] = _mm_two_sum_pd(p.val[3], q2, &p_err);
	p_err = _mm_add_pd(p_err, q0);

	_mm128x4_renorm_err_pdx4(&p, &p_err);
	return p;
}

/**
 * @brief Squares a __m128d value with the result stored as a __m128dx4.
 * @note __m128dx2 can store the values from this function exactly.
 */
static inline __m128dx4 _mm128x4_square_pd(const __m128d x) {
	__m128dx4 p;
	p.val[0] = _mm_two_sqr_pd(x, &p.val[1]);
	p.val[2] = _mm_setzero_pd();
	p.val[3] = _mm_setzero_pd();
	return p; // renormalization not needed
}


//------------------------------------------------------------------------------
// __m128dx4 Division
//------------------------------------------------------------------------------
/** 
 * @author Taken and/or modified from libQD which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */

/* __m128dx4 / __m128dx4 */

/** @brief Orginally qd_real::accurate_div */
static inline __m128dx4 _mm128x4_div_accurate_pdx4(const __m128dx4 x, const __m128dx4 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(x, _mm128x4_mul_pdx4_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y.val[0]);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

/** @brief Orginally qd_real::sloppy_div */
static inline __m128dx4 _mm128x4_div_quick_pdx4(const __m128dx4 x, const __m128dx4 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(x, _mm128x4_mul_pdx4_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.val[0]);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_div_pdx4(const __m128dx4 x, const __m128dx4 y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_div_accurate_pdx4(x, y);
	#else
		return _mm128x4_div_quick_pdx4(x, y);
	#endif
}

/* __m128dx4 / __m128dx2 */

static inline __m128dx4 _mm128x4_div_accurate_pdx4_pdx2(const __m128dx4 x, const __m128dx2 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.val[0], y.hi);
	r = _mm128x4_sub_pdx4(x, _mm128x4_mul_pdx2_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y.hi);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_div_quick_pdx4_pdx2(const __m128dx4 x, const __m128dx2 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.val[0], y.hi);
	r = _mm128x4_sub_pdx4(x, _mm128x4_mul_pdx2_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.hi);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_div_pdx4_pdx2(const __m128dx4 x, const __m128dx2 y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_div_accurate_pdx4_pdx2(x, y);
	#else
		return _mm128x4_div_quick_pdx4_pdx2(x, y);
	#endif
}

/* __m128dx4 / __m128d */

static inline __m128dx4 _mm128x4_div_accurate_pdx4_pd(const __m128dx4 x, const __m128d y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(x, _mm128x2_mul_pd_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[3]));
	
	__m128d err = _mm_div_pd(r.val[0], y);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_div_quick_pdx4_pd(const __m128dx4 x, const __m128d y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(x, _mm128x2_mul_pd_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_div_pdx4_pd(const __m128dx4 x, const __m128d y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_div_accurate_pdx4_pd(x, y);
	#else
		return _mm128x4_div_quick_pdx4_pd(x, y);
	#endif
}

/* __m128dx2 / __m128dx4 */

static inline __m128dx4 _mm128x4_div_accurate_pdx2_pdx4(const __m128dx2 x, const __m128dx4 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.hi, y.val[0]);
	r = _mm128x4_sub_pdx2_pdx4(x, _mm128x4_mul_pdx4_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y.val[0]);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_div_quick_pdx2_pdx4(const __m128dx2 x, const __m128dx4 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.hi, y.val[0]);
	r = _mm128x4_sub_pdx2_pdx4(x, _mm128x4_mul_pdx4_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.val[0]);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_div_pdx2_pdx4(const __m128dx2 x, const __m128dx4 y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_div_accurate_pdx2_pdx4(x, y);
	#else
		return _mm128x4_div_quick_pdx2_pdx4(x, y);
	#endif
}

/* __m128d / __m128dx4 */

static inline __m128dx4 _mm128x4_div_accurate_pd_pdx4(const __m128d x, const __m128dx4 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x, y.val[0]);
	r = _mm128x4_sub_pd_pdx4(x, _mm128x4_mul_pdx4_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y.val[0]);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_div_quick_pd_pdx4(const __m128d x, const __m128dx4 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x, y.val[0]);
	r = _mm128x4_sub_pd_pdx4(x, _mm128x4_mul_pdx4_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.val[0]);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_div_pd_pdx4(const __m128d x, const __m128dx4 y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_div_accurate_pd_pdx4(x, y);
	#else
		return _mm128x4_div_quick_pd_pdx4(x, y);
	#endif
}

//------------------------------------------------------------------------------
// __m128dx4 Optimized Division
//------------------------------------------------------------------------------

/* __m128dx2 / __m128dx2 */

static inline __m128dx4 _mm128x4_div_accurate_pdx2_pdx2(const __m128dx2 x, const __m128dx2 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.hi, y.hi);
	r = _mm128x4_sub_pdx2_pdx4(x, _mm128x4_mul_pdx2_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y.hi);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_div_quick_pdx2_pdx2(const __m128dx2 x, const __m128dx2 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.hi, y.hi);
	r = _mm128x4_sub_pdx2_pdx4(x, _mm128x4_mul_pdx2_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.hi);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_div_pdx2_pdx2(const __m128dx2 x, const __m128dx2 y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_div_accurate_pdx2_pdx2(x, y);
	#else
		return _mm128x4_div_quick_pdx2_pdx2(x, y);
	#endif
}

/* __m128dx2 / __m128d */

static inline __m128dx4 _mm128x4_div_accurate_pdx2_pd(const __m128dx2 x, const __m128d y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.hi, y);
	r = _mm128x4_sub_pdx2_pdx2(x, _mm128x2_mul_pd_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_div_quick_pdx2_pd(const __m128dx2 x, const __m128d y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x.hi, y);
	r = _mm128x4_sub_pdx2_pdx2(x, _mm128x2_mul_pd_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_div_pdx2_pd(const __m128dx2 x, const __m128d y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_div_accurate_pdx2_pd(x, y);
	#else
		return _mm128x4_div_quick_pdx2_pd(x, y);
	#endif
}

/* __m128d / __m128dx2 */

static inline __m128dx4 _mm128x4_div_accurate_pd_pdx2(const __m128d x, const __m128dx2 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x, y.hi);
	r = _mm128x4_sub_pd_pdx4(x, _mm128x4_mul_pdx2_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y.hi);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_div_quick_pd_pdx2(const __m128d x, const __m128dx2 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x, y.hi);
	r = _mm128x4_sub_pd_pdx4(x, _mm128x4_mul_pdx2_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.hi);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_div_pd_pdx2(const __m128d x, const __m128dx2 y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_div_accurate_pd_pdx2(x, y);
	#else
		return _mm128x4_div_quick_pd_pdx2(x, y);
	#endif
}

/* __m128d / __m128d */

static inline __m128dx4 _mm128x4_div_accurate_pd_pd(const __m128d x, const __m128d y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x, y);
	r = _mm128x4_sub_pd_pdx2(x, _mm128x2_mul_pd_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_div_quick_pd_pd(const __m128d x, const __m128d y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(x, y);
	r = _mm128x4_sub_pd_pdx2(x, _mm128x2_mul_pd_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_div_pd_pd(const __m128d x, const __m128d y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_div_accurate_pd_pd(x, y);
	#else
		return _mm128x4_div_quick_pd_pd(x, y);
	#endif
}

//------------------------------------------------------------------------------
// __m128dx4 Reciprocal
//------------------------------------------------------------------------------
/** 
 * @author Taken and/or modified from libQD which can be found under a
 * LBNL-BSD license from https://www.davidhbailey.com/dhbsoftware/
 */

/* 1.0 / __m128dx4 */

static inline __m128dx4 _mm128x4_recip_accurate_pdx4(const __m128dx4 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(_mm_set1_pd(1.0), y.val[0]);
	r = _mm128x4_sub_pd_pdx4(_mm_set1_pd(1.0), _mm128x4_mul_pdx4_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y.val[0]);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_recip_quick_pdx4(const __m128dx4 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(_mm_set1_pd(1.0), y.val[0]);
	r = _mm128x4_sub_pd_pdx4(_mm_set1_pd(1.0), _mm128x4_mul_pdx4_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.val[0]);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx4_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.val[0]);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_recip_pdx4(const __m128dx4 y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_recip_accurate_pdx4(y);
	#else
		return _mm128x4_recip_quick_pdx4(y);
	#endif
}

/* 1.0 / __m128dx2 */

static inline __m128dx4 _mm128x4_recip_accurate_pdx2(const __m128dx2 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(_mm_set1_pd(1.0), y.hi);
	r = _mm128x4_sub_pd_pdx4(_mm_set1_pd(1.0), _mm128x4_mul_pdx2_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y.hi);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_recip_quick_pdx2(const __m128dx2 y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(_mm_set1_pd(1.0), y.hi);
	r = _mm128x4_sub_pd_pdx4(_mm_set1_pd(1.0), _mm128x4_mul_pdx2_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y.hi);
	r = _mm128x4_sub_pdx4(r, _mm128x4_mul_pdx2_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y.hi);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_recip_pdx2(const __m128dx2 y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_recip_accurate_pdx2(y);
	#else
		return _mm128x4_recip_quick_pdx2(y);
	#endif
}

/* 1.0 / __m128d */

static inline __m128dx4 _mm128x4_recip_accurate_pd(const __m128d y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(_mm_set1_pd(1.0), y);
	r = _mm128x4_sub_pd_pdx2(_mm_set1_pd(1.0), _mm128x2_mul_pd_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[3]));

	__m128d err = _mm_div_pd(r.val[0], y);
	_mm128x4_renorm_err_pdx4(&q, &err);
	return q;
}

static inline __m128dx4 _mm128x4_recip_quick_pd(const __m128d y) {
	__m128dx4 q, r;

	q.val[0] = _mm_div_pd(_mm_set1_pd(1.0), y);
	r = _mm128x4_sub_pd_pdx2(_mm_set1_pd(1.0), _mm128x2_mul_pd_pd(y, q.val[0]));

	q.val[1] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[1]));

	q.val[2] = _mm_div_pd(r.val[0], y);
	r = _mm128x4_sub_pdx4_pdx2(r, _mm128x2_mul_pd_pd(y, q.val[2]));

	q.val[3] = _mm_div_pd(r.val[0], y);
	_mm128x4_renorm_pdx4(&q);
	return q;
}

static inline __m128dx4 _mm128x4_recip_pd(const __m128d y) {
	#ifdef __M256DX4_IEEE_MODE
		return _mm128x4_recip_accurate_pd(y);
	#else
		return _mm128x4_recip_quick_pd(y);
	#endif
}

//------------------------------------------------------------------------------
// __m128dx4 specialized arithmetic
//------------------------------------------------------------------------------

/**
 * @brief Multiplies by a known power of two (such as 2.0, 0.5, etc.) or zero
 */
static inline __m128dx4 _mm128x4_mul_power2_pdx4_pd(__m128dx4 x, const __m128d y) {
	x.val[0] *= y;
	x.val[1] *= y;
	x.val[2] *= y;
	x.val[3] *= y;
	return x;
}

/**
 * @brief Multiplies by a known power of two (such as 2.0, 0.5, etc.) or zero
 */
static inline __m128dx4 _mm128x4_mul_power2_pd_pdx4(const __m128d x, __m128dx4 y) {
	y.val[0] *= x;
	y.val[1] *= x;
	y.val[2] *= x;
	y.val[3] *= x;
	return y;
}

/**
 * @brief Multiplies by a known power of two (such as 2.0, 0.5, etc.) or zero.
 * The result is stored as a __m128dx4
 */
static inline __m128dx4 _mm128x4_mul_power2_pd_pd(const __m128d x, const __m128d y) {
	__m128dx4 ret;
	ret.val[0] = x * y;
	ret.val[1] = _mm_setzero_pd();
	ret.val[2] = _mm_setzero_pd();
	ret.val[3] = _mm_setzero_pd();
	return ret;
}

//------------------------------------------------------------------------------
// __m128dx4 Mathematical Constants
//------------------------------------------------------------------------------

/**
 * @brief ~2.718281828 Returns the value of euler's number
 */
static inline __m128dx4 _mm128x4_const_e_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x4005BF0A8B145769)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3CA4D57EE2B1013A)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB92618713A31D3E2)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x35CC5A6D2B53C26D))
	}};
	return ret;
}

/**
 * @brief ~1.442695041 Returns the value of log2(e)
 */
static inline __m128dx4 _mm128x4_const_log2e_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3FF71547652B82FE)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3C7777D0FFDA0D24)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB9160BB8A5442AB9)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB594B52D3BA6D74D))
	}};
	return ret;
}

/**
 * @brief ~0.434294482 Returns the value of log10(e)
 */
static inline __m128dx4 _mm128x4_const_log10e_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3FDBCB7B1526E50E)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3C695355BAAAFAD3)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x38FEE191F71A3012)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3587268808E8FCB5))
	}};
	return ret;
}

/**
 * @brief ~3.141592654 Returns the value of pi
 */
static inline __m128dx4 _mm128x4_const_pi_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x400921FB54442D18)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3CA1A62633145C07)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB92F1976B7ED8FBC)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x35C4CF98E804177D))
	}};
	return ret;
}

/**
 * @brief ~0.318309886 Returns the value of 1 / pi
 */
static inline __m128dx4 _mm128x4_const_inv_pi_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3FD45F306DC9C883)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xBC76B01EC5417056)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB916447E493AD4CE)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x35BE21C820FF28B2))
	}};
	return ret;
}

/**
 * @brief ~0.564189584 Returns the value of 1 / sqrt(pi)
 */
static inline __m128dx4 _mm128x4_const_inv_sqrtpi_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3FE20DD750429B6D)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3C61AE3A914FED80)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB8F3CBBEBF65F145)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB58E0C574632F53E))
	}};
	return ret;
}

/**
 * @brief ~0.693147181 Returns the value of ln(2)
 */
static inline __m128dx4 _mm128x4_const_ln2_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3FE62E42FEFA39EF)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3C7ABC9E3B39803F)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3907B57A079A1934)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB5AACE93A4EBE5D1))
	}};
	return ret;
}

/**
 * @brief ~2.302585093 Returns the value of ln(10)
 */
static inline __m128dx4 _mm128x4_const_ln10_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x40026BB1BBB55516)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xBCAF48AD494EA3E9)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB949EBAE3AE0260C)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB5E2D10378BE1CF1))
	}};
	return ret;
}

/**
 * @brief ~1.414213562 Returns the value of sqrt(2)
 */
static inline __m128dx4 _mm128x4_const_sqrt2_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3FF6A09E667F3BCD)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xBC9BDD3413B26456)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x39357D3E3ADEC175)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x35B2775099DA2F59))
	}};
	return ret;
}

/**
 * @brief ~1.732050808 Returns the value of sqrt(3)
 */
static inline __m128dx4 _mm128x4_const_sqrt3_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3FFBB67AE8584CAA)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3C9CEC95D0B5C1E3)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB91F11DB689F2CCF)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x35B3DA4798C720A6))
	}};
	return ret;
}

/**
 * @brief ~0.577350269 Returns the value of 1 / sqrt(3)
 */
static inline __m128dx4 _mm128x4_const_inv_sqrt3_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3FE279A74590331C)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3C834863E0792BED)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB92A82F9E6C53222)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB5CCB0F41134253A))
	}};
	return ret;
}

/**
 * @brief ~0.577215665 Returns the value of gamma (The Euler–Mascheroni constant)
 */
static inline __m128dx4 _mm128x4_const_egamma_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3FE2788CFC6FB619)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xBC56CB90701FBFAB)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB8F34A95E3133C51)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3599730064300F7D))
	}};
	return ret;
}

/**
 * @brief ~1.618033989 Returns the value of phi (The golden ratio)
 */
static inline __m128dx4 _mm128x4_const_phi_pdx4(void) {
	const __m128dx4 ret = {{
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x3FF9E3779B97F4A8)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xBC8F506319FCFD19)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x392B906821044ED8)),
		_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0xB5A8BB1B5C0F272C))
	}};
	return ret;
}

//------------------------------------------------------------------------------
// __m128dx4 floating point classify
//------------------------------------------------------------------------------

/* values */

	/**
	 * @brief Returns a __m128dx4 value set to positive infinity
	 */
	static inline __m128dx4 _mm128x4_get_infinity_pdx4(void) {
		__m128dx4 ret = {{
			_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x7FF0000000000000)),
			_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x7FF0000000000000))
		}};
		return ret;
	}

	/**
	 * @brief Returns a __m128dx4 value set to signaling NaN
	 */
	static inline __m128dx4 _mm128x4_get_sNaN_pdx4(void) {
		__m128dx4 ret = {{
			_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x7FF0000000000001)),
			_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x7FF0000000000001)),
		}};
		return ret;
	}

	/**
	 * @brief Returns a __m128dx4 value set to quiet NaN
	 */
	static inline __m128dx4 _mm128x4_get_qNaN_pdx4(void) {
		__m128dx4 ret = {{
			_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x7FF8000000000001)),
			_mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x7FF8000000000001))
		}};
		return ret;
	}


/* Float Classify */

	/** @brief Returns true if x is negative */
	static inline __m128d _mm_signbit_pdx4(const __m128dx4 x) {
		return _mm_signbit_pd(x.val[0]);
	}

	/** @brief Returns true if x is finite */
	static inline __m128d _mm_isfinite_pdx4(const __m128dx4 x) {
		return _mm_isfinite_pd(x.val[0]);
	}

	/** @brief Returns true if x is +-infinity */
	static inline __m128d _mm_isinf_pdx4(const __m128dx4 x) {
		return _mm_isinf_pd(x.val[0]);
	}

	/** @brief Returns true if x is any kind of NaN */
	static inline __m128d _mm_isnan_pdx4(const __m128dx4 x) {
		return _mm_isnan_pd(x.val[0]);
	}

	/** @brief Returns true if x is normal */
	static inline __m128d _mm_isnormal_pdx4(const __m128dx4 x) {
		return _mm_and_pd(
			_mm_and_pd(
				_mm_isnormal_pd(x.val[0]),
				_mm_or_pd(
					_mm_isnormal_pd(x.val[1]),
					_mm_cmpeq_pd(x.val[1], _mm_setzero_pd())
				)
			),
			_mm_and_pd(
				_mm_or_pd(
					_mm_isnormal_pd(x.val[2]),
					_mm_cmpeq_pd(x.val[2], _mm_setzero_pd())
				),
				_mm_or_pd(
					_mm_isnormal_pd(x.val[3]),
					_mm_cmpeq_pd(x.val[3], _mm_setzero_pd())
				)
			)
		);
	}

	/** @brief Returns true if x is denormal and non-zero */
	static inline __m128d _mm_isdenormal_pdx4(const __m128dx4 x) {
		return _mm_or_pd(
			_mm_or_pd(
				_mm_isdenormal_pd(x.val[0]), _mm_isdenormal_pd(x.val[1])
			),
			_mm_or_pd(
				_mm_isdenormal_pd(x.val[2]), _mm_isdenormal_pd(x.val[3])
			)
		);
	}

	/** @brief Returns true if x and y are unordered */
	static inline __m128d _mm_isunordered_pdx4(const __m128dx4 x, const __m128dx4 y) {
		return _mm_isunordered_pd(x.val[0], y.val[1]);
	}

//------------------------------------------------------------------------------
// __m128dx4 max/min functions
//------------------------------------------------------------------------------

/**
 * @brief Returns the fmax of x and y. Correctly handling NaN and signed zeros.
 * You may use _mm128x4_max_pdx4 as a faster alternative.
 */
static inline __m128dx4 _mm128x4_fmax_pdx4(__m128dx4 x, __m128dx4 y) {
	__m128d fmax_cmp =
		_mm_or_pd(
			_mm_andnot_pd(
				_mm_or_pd(
					_mm_andnot_pd(
						_mm_signbit_pdx4(x),
						_mm_isnan_pdx4(y)
					),
					_mm_isnan_pdx4(x)
				),
				_mm_cmplt_pdx4(y, x)
			),
			_mm_cmplt_pdx4(x, y)
		);
	__m128dx4 ret;
	ret.val[0] = _internal_mm_blendv_pd(x.val[0], y.val[0], fmax_cmp);
	ret.val[1] = _internal_mm_blendv_pd(x.val[1], y.val[1], fmax_cmp);
	ret.val[2] = _internal_mm_blendv_pd(x.val[2], y.val[2], fmax_cmp);
	ret.val[3] = _internal_mm_blendv_pd(x.val[3], y.val[3], fmax_cmp);
	return ret;
}

/**
 * @brief Returns the fmin of x and y. Correctly handling NaN and signed zeros.
 * You may use _mm128x4_min_pdx4 as a faster alternative.
 */
static inline __m128dx4 _mm128x4_fmin_pdx4(__m128dx4 x, __m128dx4 y) {
	__m128d fmin_cmp =
		_mm_or_pd(
			_mm_andnot_pd(
				_mm_or_pd(
					_mm_andnot_pd(
						_mm_signbit_pdx4(x),
						_mm_isnan_pdx4(y)
					),
					_mm_isnan_pdx4(x)
				),
				_mm_cmpgt_pdx4(y, x)
			),
			_mm_cmpgt_pdx4(x, y)
		);
	__m128dx4 ret;
	ret.val[0] = _internal_mm_blendv_pd(x.val[0], y.val[0], fmin_cmp);
	ret.val[1] = _internal_mm_blendv_pd(x.val[1], y.val[1], fmin_cmp);
	ret.val[2] = _internal_mm_blendv_pd(x.val[2], y.val[2], fmin_cmp);
	ret.val[3] = _internal_mm_blendv_pd(x.val[3], y.val[3], fmin_cmp);
	return ret;
}

static inline __m128dx4 _mm128x4_max_pdx4(__m128dx4 x, __m128dx4 y) {
	const __m128d cmp_max = _mm_cmplt_pdx4(x, y);
	__m128dx4 ret;
	ret.val[0] = _internal_mm_blendv_pd(x.val[0], y.val[0], cmp_max);
	ret.val[1] = _internal_mm_blendv_pd(x.val[1], y.val[1], cmp_max);
	ret.val[2] = _internal_mm_blendv_pd(x.val[2], y.val[2], cmp_max);
	ret.val[3] = _internal_mm_blendv_pd(x.val[3], y.val[3], cmp_max);
	return ret;
}

static inline __m128dx4 _mm128x4_min_pdx4(__m128dx4 x, __m128dx4 y) {
	const __m128d cmp_min = _mm_cmpgt_pdx4(x, y);
	__m128dx4 ret;
	ret.val[0] = _internal_mm_blendv_pd(x.val[0], y.val[0], cmp_min);
	ret.val[1] = _internal_mm_blendv_pd(x.val[1], y.val[1], cmp_min);
	ret.val[2] = _internal_mm_blendv_pd(x.val[2], y.val[2], cmp_min);
	ret.val[3] = _internal_mm_blendv_pd(x.val[3], y.val[3], cmp_min);
	return ret;
}

//------------------------------------------------------------------------------
// __m128dx4 rounding functions
//------------------------------------------------------------------------------

#ifdef __SSE4_1__

static inline __m128dx4 _mm128x4_trunc_pdx4(__m128dx4 x) {
	__m128dx4 ret = {{
		_mm_trunc_pd(x.val[0]),
		_mm_trunc_pd(x.val[1]),
		_mm_trunc_pd(x.val[2]),
		_mm_trunc_pd(x.val[3]),
	}};
	_mm128x4_renorm_pdx4(&ret);
	return ret;
}

static inline __m128dx4 _mm128x4_floor_pdx4(__m128dx4 x) {
	__m128dx4 ret = {{
		_mm_floor_pd(x.val[0]),
		_mm_floor_pd(x.val[1]),
		_mm_floor_pd(x.val[2]),
		_mm_floor_pd(x.val[3]),
	}};
	_mm128x4_renorm_pdx4(&ret);
	return ret;
}

static inline __m128dx4 _mm128x4_ceil_pdx4(__m128dx4 x) {
	__m128dx4 ret = {{
		_mm_ceil_pd(x.val[0]),
		_mm_ceil_pd(x.val[1]),
		_mm_ceil_pd(x.val[2]),
		_mm_ceil_pd(x.val[3]),
	}};
	_mm128x4_renorm_pdx4(&ret);
	return ret;
}

#endif

//------------------------------------------------------------------------------
// __m128dx4 math.h functions
//------------------------------------------------------------------------------

static inline __m128dx4 _mm128x4_fabs_pdx4(__m128dx4 x) {
	const __m128d sign_mask = _mm_and_pd(
		x.val[0],
		_mm_get_sign_mask_pd()
	);
	x.val[0] = _mm_xor_pd(x.val[0], sign_mask);
	x.val[1] = _mm_xor_pd(x.val[1], sign_mask);
	x.val[2] = _mm_xor_pd(x.val[2], sign_mask);
	x.val[3] = _mm_xor_pd(x.val[3], sign_mask);
	return x;
}

static inline __m128dx4 _mm128x4_fdim_pdx4(__m128dx4 x, __m128dx4 y) {
	__m128dx4 ret;
	ret = _mm128x4_sub_pdx4(x, y);
	// returns true when ret > 0.0 or ret is NaN
	__m128d cmp_nle = _mm_cmpnle_zero_pdx4(ret);
	// NaN remains NaN, and -0.0 becomes +0.0
	ret.val[0] = _mm_and_pd(ret.val[0], cmp_nle);
	ret.val[1] = _mm_and_pd(ret.val[1], cmp_nle);
	ret.val[2] = _mm_and_pd(ret.val[2], cmp_nle);
	ret.val[3] = _mm_and_pd(ret.val[3], cmp_nle);
	return ret;
}

static inline __m128dx4 _mm128x4_copysign_pdx4(__m128dx4 x, __m128dx4 y) {
	const __m128d sign_mask = _mm_and_pd(
		_mm_xor_pd(x.val[0], y.val[0]),
		_mm_get_sign_mask_pd()
	);
	x.val[0] = _mm_xor_pd(x.val[0], sign_mask);
	x.val[1] = _mm_xor_pd(x.val[1], sign_mask);
	x.val[2] = _mm_xor_pd(x.val[2], sign_mask);
	x.val[3] = _mm_xor_pd(x.val[3], sign_mask);
	return x;
}

static inline __m128dx4 _mm128x4_copysign_pdx4_pd(__m128dx4 x, __m128d y) {
	const __m128d sign_mask = _mm_and_pd(
		_mm_xor_pd(x.val[0], y),
		_mm_get_sign_mask_pd()
	);
	x.val[0] = _mm_xor_pd(x.val[0], sign_mask);
	x.val[1] = _mm_xor_pd(x.val[1], sign_mask);
	x.val[2] = _mm_xor_pd(x.val[2], sign_mask);
	x.val[3] = _mm_xor_pd(x.val[3], sign_mask);
	return x;
}

static inline __m128dx4 _mm128x4_sqrt_pdx4(__m128dx4 x) {
	__m128dx2 guess = {x.val[0], x.val[1]};
	guess = _mm128x2_sqrt_pdx2(guess);
	return _mm128x4_and_pdx4(
		_mm128x4_mul_power2_pdx4_pd(
			_mm128x4_add_pdx2_pdx4(guess, _mm128x4_div_pdx4_pdx2(x, guess)),
			_mm_set1_pd(0.5)
		),
		// returns 0.0 on division by 0.0
		_mm128x4_cmp_extend_pd(_mm_cmpneq_zero_pdx2(guess))
	);
}

#if 0
/* Unimplemented */

static inline __m128dx4 _mm128x4_cbrt_pdx4(__m128dx4 x) {
	__m128dx2 guess = {x.val[0], x.val[1]};
	guess = _mm128x2_cbrt_pdx2(guess);
	return _mm128x4_and_pdx4(
		_mm128x4_div_pdx4_pd(
			_mm128x4_add_pdx2_pdx4(
				_mm128x2_mul_power2_pd_pdx2(
					_mm_set1_pd(2.0), guess
				),
				_mm128x4_div_pdx4(x, _mm128x4_square_pdx2(guess))
			),
			_mm_set1_pd(3.0)
		),
		// returns 0.0 on division by 0.0
		_mm128x4_cmp_extend_pd(_mm_cmpneq_zero_pdx2(guess))
	);
}

#endif

#ifdef __SSE4_1__

/**
* @brief returns the fraction part of a Float64x4 value. int_part may be NULL
*/
static inline __m128dx4 _mm128x4_modf_pdx4(const __m128dx4 x, __m128dx4* const int_part) {
	__m128dx4 trunc_part = _mm128x4_trunc_pdx4(x);
	if (int_part != NULL) {
		*int_part = trunc_part;
	}
	return _mm128x4_sub_pdx4(x, trunc_part);
}

static inline __m128dx4 _mm128x4_fmod_pdx4(const __m128dx4 x, const __m128dx4 y) {
	__m128dx4 trunc_part = _mm128x4_trunc_pdx4(_mm128x4_div_pdx4(x, y));
	return _mm128x4_sub_pdx4(x, _mm128x4_mul_pdx4(y, trunc_part));
}

#endif

//------------------------------------------------------------------------------
// __m128dx4 ilogb
//------------------------------------------------------------------------------

#if defined(__SSE4_1__) && defined(__SSE4_2__)

/**
 * @brief Computes ilogb(x) on a __m128dx4 value
 * @returns __m128i int64_t
 */
static inline __m128i _mm128x4_ilogb_pdx4_epi64(__m128dx4 x) {
	return _mm_ilogb_pd_epi64(x.val[0]);
}

#endif

//------------------------------------------------------------------------------
// __m128dx4 ldexp
//------------------------------------------------------------------------------

#if defined(__SSE4_1__) && defined(__SSE4_2__)

/**
 * @brief Computes ldexp(x, expon)
 */
static inline __m128dx4 _mm128x4_ldexp_pdx4_epi64(__m128dx4 x, __m128i expon) {
	x.val[0] = _mm_ldexp_pd_epi64(x.val[0], expon);
	x.val[1] = _mm_ldexp_pd_epi64(x.val[1], expon);
	x.val[2] = _mm_ldexp_pd_epi64(x.val[2], expon);
	x.val[3] = _mm_ldexp_pd_epi64(x.val[3], expon);
	return x;
}

#endif

//------------------------------------------------------------------------------
// __m128dx4 frexp
//------------------------------------------------------------------------------

#if defined(__SSE4_1__) && defined(__SSE4_2__)

/**
 * @brief Computes frexp(x, expon) on a __m128dx4 value
 * @returns __m128i int64_t
 */
static inline __m128dx4 _mm128x4_frexp_pdx4_epi64(__m128dx4 x, __m128i* const expon) {
	x.val[0] = _mm_frexp_pd_epi64(x.val[0], expon);
	const __m128i ldexp_value = _mm_sub_epi64(_mm_set1_epi64x((int64_t)1), *expon);
	x.val[1] = _mm_ldexp_pd_epi64(x.val[1], ldexp_value);
	x.val[2] = _mm_ldexp_pd_epi64(x.val[2], ldexp_value);
	x.val[3] = _mm_ldexp_pd_epi64(x.val[3], ldexp_value);
	return x;
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* FLOAT64X4_SSE2_H */
