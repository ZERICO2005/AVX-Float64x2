/*
**	Author: zerico2005 (2024)
**	Project: LIB-Dekker-Float
**	License: MIT License
**	A copy of the MIT License should be included with
**	this project. If not, see https://opensource.org/license/MIT
*/

#include "../../util_mpfr/MPFR_Float.hpp"
#include "../../util_mpfr/mpfr_convert.hpp"
#include "../../LDF/LDF_type_info.hpp"

// template<typename FloatX>
// FloatX FloatMPFR_to_FloatX(FloatMPFR src) {
// 	FloatX dst;
// 	dst.hi = mpfr_get_d(src.value, MPFR_RNDN);
// 	FloatMPFR dst_temp;
// 	mpfr_set_d(dst_temp.value, dst.hi, MPFR_RNDN);
// 	mpfr_sub(src.value, src.value, dst_temp.value, MPFR_RNDN);
// 	dst.lo = mpfr_get_d(src.value, MPFR_RNDN);
// 	return dst;
// }

#include <cstdint>
#include <cinttypes>
#include <limits>
#include <stdio.h>
#include <cstdio>

template<typename FloatX>
void output_constant_SIMD(const char* label, FloatX dst) {
	#if 0
		uint32_t* dst_bin = reinterpret_cast<uint32_t*>(dst.val);
		#if 0
			// Float32 SSE2
			printf(
				"\n\n__m128x4 _mm128x4_const_%s_psx4(void) {\n"
				"    __m128x4 ret = {\n"\
				"        _mm_castsi128_ps(_mm_set1_epi32(0x%08" PRIX32 ")),\n"\
				"        _mm_castsi128_ps(_mm_set1_epi32(0x%08" PRIX32 ")),\n"\
				"        _mm_castsi128_ps(_mm_set1_epi32(0x%08" PRIX32 ")),\n"\
				"        _mm_castsi128_ps(_mm_set1_epi32(0x%08" PRIX32 "))\n"\
				"    };\n"\
				"    return ret;\n"\
				"}\n",
				label, dst_bin[0], dst_bin[1], dst_bin[2], dst_bin[3]
			);
		#elif 0
			// Float32 AVX
			printf(
				"\n\n__m256x4 _mm256x4_const_%s_psx4(void) {\n"
				"    __m256x4 ret = {\n"\
				"        _mm256_castsi256_ps(_mm256_set1_epi32(0x%08" PRIX32 ")),\n"\
				"        _mm256_castsi256_ps(_mm256_set1_epi32(0x%08" PRIX32 ")),\n"\
				"        _mm256_castsi256_ps(_mm256_set1_epi32(0x%08" PRIX32 ")),\n"\
				"        _mm256_castsi256_ps(_mm256_set1_epi32(0x%08" PRIX32 "))\n"\
				"    };\n"\
				"    return ret;\n"\
				"}\n",
				label, dst_bin[0], dst_bin[1], dst_bin[2], dst_bin[3]
			);
		#elif 0
			// Float32 AVX512F
			printf(
				"\n\n__m512x4 _mm512x4_const_%s_psx4(void) {\n"
				"    __m512x4 ret = {\n"\
				"        _mm512_castsi512_ps(_mm512_set1_epi32(0x%08" PRIX32 ")),\n"\
				"        _mm512_castsi512_ps(_mm512_set1_epi32(0x%08" PRIX32 ")),\n"\
				"        _mm512_castsi512_ps(_mm512_set1_epi32(0x%08" PRIX32 ")),\n"\
				"        _mm512_castsi512_ps(_mm512_set1_epi32(0x%08" PRIX32 "))\n"\
				"    };\n"\
				"    return ret;\n"\
				"}\n",
				label, dst_bin[0], dst_bin[1], dst_bin[2], dst_bin[3]
			);
		#endif
	#elif 1
		uint64_t* dst_bin = reinterpret_cast<uint64_t*>(dst.val);
		#if 0
			// Float64 SSE2
			printf(
				"\n\n__m128dx4 _mm128x4_const_%s_pdx4(void) {\n"
				"    __m128dx4 ret = {\n"\
				"        _mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x%016" PRIX64 ")),\n"\
				"        _mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x%016" PRIX64 ")),\n"\
				"        _mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x%016" PRIX64 ")),\n"\
				"        _mm_castsi128_pd(_mm_set1_epi64x((int64_t)0x%016" PRIX64 "))\n"\
				"    };\n"\
				"    return ret;\n"\
				"}\n",
				label, dst_bin[0], dst_bin[1], dst_bin[2], dst_bin[3]
			);
		#elif 0
			// Float64 AVX
			printf(
				"\n\n__m256dx4 _mm256x4_const_%s_pdx4(void) {\n"
				"    __m256dx4 ret = {\n"\
				"        _mm256_castsi256_pd(_mm256_set1_epi64x((int64_t)0x%016" PRIX64 ")),\n"\
				"        _mm256_castsi256_pd(_mm256_set1_epi64x((int64_t)0x%016" PRIX64 ")),\n"\
				"        _mm256_castsi256_pd(_mm256_set1_epi64x((int64_t)0x%016" PRIX64 ")),\n"\
				"        _mm256_castsi256_pd(_mm256_set1_epi64x((int64_t)0x%016" PRIX64 "))\n"\
				"    };\n"\
				"    return ret;\n"\
				"}\n",
				label, dst_bin[0], dst_bin[1], dst_bin[2], dst_bin[3]
			);
		#elif 1
			// Float64 AVX512F
			printf(
				"\n\n__m512dx4 _mm512x4_const_%s_pdx4(void) {\n"
				"    __m512dx4 ret = {\n"\
				"        _mm512_castsi512_pd(_mm512_set1_epi64((int64_t)0x%016" PRIX64 ")),\n"\
				"        _mm512_castsi512_pd(_mm512_set1_epi64((int64_t)0x%016" PRIX64 ")),\n"\
				"        _mm512_castsi512_pd(_mm512_set1_epi64((int64_t)0x%016" PRIX64 ")),\n"\
				"        _mm512_castsi512_pd(_mm512_set1_epi64((int64_t)0x%016" PRIX64 "))\n"\
				"    };\n"\
				"    return ret;\n"\
				"}\n",
				label, dst_bin[0], dst_bin[1], dst_bin[2], dst_bin[3]
			);
		#endif
	#endif
}

template<typename FloatX, typename FloatBase>
void output_constant(const char* label, __attribute__((unused)) const char* name, FloatMPFR src, const char* comment = nullptr) {
	constexpr bool enable_comment = true;
	const int label_width = 14;
	(void)label_width; // Gets rid of unused variable warnings
#if 0
	#if 0
		mpfr_printf(
			"static constexpr T C_%-*s {};",
			label_width, label
		);
		printf(" /**< ~%.9Lf", mpfr_get_ld(src.value, MPFR_RNDN));
		if (comment != nullptr && enable_comment) { printf(" %s", comment); }
		printf(" */\n");
	#else
		printf("/** ~%.9Lf", mpfr_get_ld(src.value, MPFR_RNDN));
		if (comment != nullptr && enable_comment) { printf(" %s", comment); }
		printf(" */\n");

		#if 0
		mpfr_printf(
			"template<> inline constexpr long double const_%-*s<long double>() { return %.40ReL; }\n",
			label_width, label, src.value
		);
		#elif 0
		mpfr_printf(
			"template<> inline constexpr double const_%-*s<double>() { return %.21Re; }\n",
			label_width, label, src.value
		);
		#elif 0
		mpfr_printf(
			"template<> inline constexpr float const_%-*s<float>() { return %.11Ref; }\n",
			label_width, label, src.value
		);
		#else
		mpfr_printf(
			"template<> inline constexpr __float128 const_%-*s<__float128>() { return %.36ReQ; }\n",
			label_width, label, src.value
		);
		#endif
	#endif
#else
	FloatX dst_value = mpfr_get_type<FloatX>(src.value, MPFR_RNDN);
	const FloatBase* dst = reinterpret_cast<FloatBase*>(&dst_value);
	const int precision = std::numeric_limits<FloatBase>::max_digits10 + 1;
	const int width = 7 + precision;
	
	printf("/** ~%.9Lf", mpfr_get_ld(src.value, MPFR_RNDN));
	if (comment != nullptr && enable_comment) { printf(" %s", comment); }
	printf(" */\n");
	
	printf(
		"template<> inline constexpr %s const_%-s<%s>() { return {\n",
		name, label, name
	);
	char dst_buf[999];
	for (int i = 0; i < LDF::LDF_Type_Info<FloatX>::FloatBase_Count; i++) {
		if (i != 0) { printf(",\n"); }
		snprintf(dst_buf, sizeof(dst_buf), "%+-*.*e", width, precision, dst[i]);
		printf("\t%s", dst_buf);
	};
	printf("\n}; }\n\n");
#endif
}

static const char lemniscate_const_text[] =
"2.622057554292119810464839589891119413682754951431623162816821703800790587070"\
"41425023029553296142909344613575267178321805560895690139393569470111943477523"\
"58404226414971649069519368999799321460723831213908102062218974296008565545398";

template<typename FloatX, typename FloatBase>
void generate_constants(const char* name) {
	FloatMPFR num;
	FloatMPFR temp;
	constexpr bool print_headings = true;
	const char prefix_ln[] = "";
	const char suffix_ln[] = "\n\n";

if (print_headings) { printf("%s/* Multiples of pi */%s", prefix_ln, suffix_ln); }

	mpfr_const_pi(num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("pi", name, num);

	mpfr_mul_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("2pi", name, num, "`2 * pi`");

	mpfr_div_d(num.value, num.value, 4.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("pi2", name, num, "`1/2 * pi`");

	mpfr_div_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("pi4", name, num, "`1/4 * pi`");

	mpfr_mul_d(num.value, num.value, 3.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("3pi4", name, num, "`3/4 * pi`");

	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_div_d(num.value, num.value, 3.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("pi3", name, num, "`1/3 * pi`");

if (print_headings) { printf("%s/* Inverse and sqrt pi */%s", prefix_ln, suffix_ln); }

	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_pi", name, num, "`1 / pi`");

	mpfr_div_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_2pi", name, num, "`1 / 2pi`");

/* sqrtpi constants */

	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("sqrtpi", name, num, "`sqrt(pi)`");

	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_mul_d(num.value, num.value, 2.0, MPFR_RNDN);
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("sqrt2pi", name, num, "`sqrt(2pi)`");

/* inv_sqrtpi constants */

	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_sqrtpi", name, num, "`1 / sqrt(pi)`");

	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_mul_d(num.value, num.value, 2.0, MPFR_RNDN);
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_sqrt2pi", name, num, "`1 / sqrt(2pi)`");

if (print_headings) { printf("%s/* Logarithms and Exponents */%s", prefix_ln, suffix_ln); }

	num = 1.0;
	mpfr_exp(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("e", name, num, "Eulers number");
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_e", name, num, "`1 / e`");

	mpfr_const_log2(num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("ln2", name, num, "`ln(2)`");

	num = 3.0;
	mpfr_log(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("ln3", name, num, "`ln(3)`");

	num = 10.0;
	mpfr_log(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("ln10", name, num, "`ln(10)`");

	mpfr_const_log2(num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("log2e", name, num, "`log2(e)`");

	num = 3.0;
	mpfr_log(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("log3e", name, num, "`log3(e)`");

	num = 10.0;
	mpfr_log(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("log10e", name, num, "`log10(e)`");

	// num = 10.0;
	// mpfr_log2(num.value, num.value, MPFR_RNDN);
	// output_constant<FloatX, FloatBase>("log2_10", name, num);

	// num = 2.0;
	// mpfr_log10(num.value, num.value, MPFR_RNDN);
	// output_constant<FloatX, FloatBase>("log10_2", name, num);

if (print_headings) { printf("%s/* Square Roots */%s", prefix_ln, suffix_ln); }

	num = 2.0;
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("sqrt2", name, num, "`sqrt(2)`");

	num = 3.0;
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("sqrt3", name, num, "`sqrt(3)`");

	num = 5.0;
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("sqrt5", name, num, "`sqrt(5)`");

	num = 6.0;
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("sqrt6", name, num, "`sqrt(6)`");

/* Inverse Square Roots */

	num = 2.0;
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_sqrt2", name, num, "`1 / sqrt(2)` or `1/2 * sqrt(2)`");

	num = 3.0;
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_sqrt3", name, num, "`1 / sqrt(3)`");

	num = 5.0;
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_sqrt5", name, num, "`1 / sqrt(5)`");

	num = 6.0;
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_sqrt6", name, num, "`1 / sqrt(6)`");

if (print_headings) { printf("%s/* Cube Roots */%s", prefix_ln, suffix_ln); }

	num = 2.0;
	mpfr_cbrt(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("cbrt2", name, num, "`cbrt(2)`");

	num = 3.0;
	mpfr_cbrt(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("cbrt3", name, num, "`cbrt(3)`");

	num = 5.0;
	mpfr_cbrt(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("cbrt5", name, num, "`cbrt(5)`");

	num = 6.0;
	mpfr_cbrt(num.value, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("cbrt6", name, num, "`cbrt(6)`");

/* Inverse Cube Roots */

	num = 2.0;
	mpfr_cbrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_cbrt2", name, num, "`1 / cbrt(2)`");

	num = 3.0;
	mpfr_cbrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_cbrt3", name, num, "`1 / cbrt(3)`");

	num = 5.0;
	mpfr_cbrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_cbrt5", name, num, "`1 / cbrt(5)`");

	num = 6.0;
	mpfr_cbrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_cbrt6", name, num, "`1 / cbrt(6)`");

if (print_headings) { printf("%s/* Additional Mathematical Constants */%s", prefix_ln, suffix_ln); }

	mpfr_const_euler(num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("egamma", name, num, "Euler-Mascheroni constant");

	num = 5.0;
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	mpfr_add_d(num.value, num.value, 1.0, MPFR_RNDN);
	mpfr_div_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("phi", name, num, "Golden Ratio");

	num = 33.0;
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	mpfr_mul_d(num.value, num.value, 3.0, MPFR_RNDN);
	mpfr_d_sub(temp.value, 19.0, num.value, MPFR_RNDN);
	mpfr_cbrt(temp.value, temp.value, MPFR_RNDN);
	mpfr_add_d(num.value, num.value, 19.0, MPFR_RNDN);
	mpfr_cbrt(num.value, num.value, MPFR_RNDN);
	mpfr_add(num.value, num.value, temp.value, MPFR_RNDN);
	mpfr_add_d(num.value, num.value, 1.0, MPFR_RNDN);
	mpfr_div_d(num.value, num.value, 3.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("tribonacci", name, num);

	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("inv_tribonacci", name, num, "`1 / tribonacci`");

	mpfr_const_catalan(num.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("catalan", name, num);

	mpfr_zeta_ui(num.value, 3.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("apery", name, num, "`zeta(3)`");

	num = -1.0;
	mpfr_eint(num.value, num.value, MPFR_RNDN);
	temp = 1.0;
	mpfr_exp(temp.value, temp.value, MPFR_RNDN);
	mpfr_mul_d(temp.value, temp.value, -1.0, MPFR_RNDN);
	mpfr_mul(num.value, num.value, temp.value, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("gompertz", name, num, "`-e * eint(-1)`");

	mpfr_set_str(num.value, lemniscate_const_text, 10, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("lemniscate", name, num);

	mpfr_mul_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("2lemniscate", name, num, "`2 * lemniscate`");

	mpfr_div_d(num.value, num.value, 4.0, MPFR_RNDN);
	output_constant<FloatX, FloatBase>("lemniscate2", name, num, "`1/2 * lemniscate`");

	// mpfr_const_pi(temp.value, MPFR_RNDN);
	// mpfr_set_str(num.value, lemniscate_const_text, 10, MPFR_RNDN);
	// mpfr_div(num.value, num.value, temp.value, MPFR_RNDN);
	// output_constant<FloatX, FloatBase>("gauss", name, num, "`lemniscate / pi`");

}

#include "boost_constants.hpp"

template<typename FloatX, typename FloatBase>
void output_boost_constant(
	size_t index, __attribute__((unused)) const char* name,
	FloatMPFR src, const char* comment = nullptr
) {
	output_constant<FloatX, FloatBase>(boost_constant_label[index].label, name, src, comment);
}

template<typename FloatX, typename FloatBase>
void generate_boost_constants(const char* name) {
	FloatMPFR num;
	FloatMPFR temp;
	size_t index = 0;

	// half
	mpfr_set_d(num.value, 0.5, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1 / 2`");

	// third
	mpfr_set_d(num.value, 1.0, MPFR_RNDN);
	mpfr_div_d(num.value, num.value, 3.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1 / 3`");

	// twothirds and two_thirds
	mpfr_mul_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`2 / 3`");
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`2 / 3`");

	// sixth
	mpfr_div_d(num.value, num.value, 4.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1 / 6`");

	// three_quarters
	mpfr_set_d(num.value, 0.75, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`3 / 4`");

	// root_two
	mpfr_set_d(num.value, 2.0, MPFR_RNDN);
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`sqrt(2)`");

	// root_three
	mpfr_set_d(num.value, 3.0, MPFR_RNDN);
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`sqrt(3)`");

	// half_root_two
	mpfr_set_d(num.value, 0.5, MPFR_RNDN);
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1 / sqrt(2)`");

	// ln_two
	mpfr_const_log2(num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`ln(2)`");

	// ln_ln_two
	mpfr_log(num.value, num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`ln(ln(2))`");

	// root_ln_four
	mpfr_set_d(num.value, 4.0, MPFR_RNDN);
	mpfr_log(num.value, num.value, MPFR_RNDN);
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`sqrt(ln(4))`");

	// one_div_root_two
	mpfr_set_d(num.value, 0.5, MPFR_RNDN);
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1 / sqrt(2)`");

	// pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num);

	// half_pi
	mpfr_div_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1/2 * pi`");

	// third_pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_div_d(num.value, num.value, 3.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1/3 * pi`");

	// sixth_pi
	mpfr_div_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1/6 * pi`");

	// two_pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_mul_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`2 * pi`");

	// two_thirds_pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_div_d(num.value, num.value, 3.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`2/3 * pi`");

	// three_quarters_pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_mul_d(num.value, num.value, 0.75, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`3/4 * pi`");

	// four_thirds_pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_div_d(num.value, num.value, 0.75, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`4/3 * pi`");

	// one_div_two_pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_mul_d(num.value, num.value, 2.0, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1 / 2pi`");

	// one_div_root_two_pi
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1 / sqrt(2pi)`");

	// root_pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`sqrt(pi)`");

	// root_half_pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_div_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`sqrt(1/2 * pi)`");

	// root_two_pi
	mpfr_mul_d(num.value, num.value, 2.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`sqrt(2pi)`");

	// log_root_two_pi
	mpfr_log(num.value, num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`ln(sqrt(2pi))`");

	// one_div_root_pi and root_one_div_pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_sqrt(num.value, num.value, MPFR_RNDN);
	mpfr_d_div(num.value, 1.0, num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1 / sqrt(pi)`");
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`1 / sqrt(pi)`");

	// pi_minus_three
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_sub_d(num.value, num.value, 3.0, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`pi - 3`");

	// four_minus_pi
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_d_sub(num.value, 4.0, num.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`4 - pi`");

	// pi_pow_e
	mpfr_const_pi(num.value, MPFR_RNDN);
	mpfr_set_d(temp.value, 1.0, MPFR_RNDN);
	mpfr_exp(temp.value, temp.value, MPFR_RNDN);
	mpfr_pow(num.value, num.value, temp.value, MPFR_RNDN);
	output_boost_constant<FloatX, FloatBase>(index++, name, num, "`pi ^ e`");

	// We will write in the rest later on
	for (; index < sizeof(boost_constant_label) / sizeof(boost_constant_label[0]); index++) {
		mpfr_set_str(num.value, boost_constant_label[index].value, 10, MPFR_RNDN);
		output_boost_constant<FloatX, FloatBase>(index++, name, num);
	}
}

template<typename FloatX, typename FloatBase>
static void output_text_constant(mpfr_t dst, const char* text) {
	mpfr_set_str(dst, text, 10, MPFR_RNDN);
	FloatX temp = mpfr_get_type<FloatX>(dst, MPFR_RNDN);
	const FloatBase* output = reinterpret_cast<FloatBase*>(&temp);
	const int precision = std::numeric_limits<FloatBase>::max_digits10 + 1;
	const int width = 7 + precision;

	printf("\t{");
	char dst_buf[999];
	for (int i = 0; i < LDF::LDF_Type_Info<FloatX>::FloatBase_Count; i++) {
		if (i != 0) { printf(", "); }
		snprintf(dst_buf, sizeof(dst_buf), "%+-*.*e", width, precision, output[i]);
		printf("%s", dst_buf);
	};
	printf("},\n");
}

template<typename FloatX, typename FloatBase>
void constants_from_text(
	const char* src[], size_t len,
	const char* name, const char* label,
	mpfr_prec_t precision
) {
	printf("static constexpr %s %s[] = {\n", name, label);
	mpfr_t dst;
	mpfr_init2(dst, precision);
	#if 0
		for (size_t i = 0; i < len; i++) {
			output_text_constant<FloatX, FloatBase>(dst, src[i]);
		}
	#else
		/* reverse */
		for (size_t i = len; --i < len;) {
			output_text_constant<FloatX, FloatBase>(dst, src[i]);
		}
	#endif
	mpfr_clear(dst);
	printf("};\n");
}