/*
**	Author: zerico2005 (2024)
**	Project: LIB-Dekker-Float
**	License: MIT License
**	A copy of the MIT License should be included with
**	this project. If not, see https://opensource.org/license/MIT
*/

#ifndef FLOATNXN_PRECISION_TEST_HPP
#define FLOATNXN_PRECISION_TEST_HPP

#include <cstdio>
#include <math.h>
#include <cmath>
#include <limits>
#include "../../util_mpfr/MPFR_Float.hpp"
#include "../../util_mpfr/mpfr_convert.hpp"

#define linearInterpolation(x, x0, x1, y0, y1) \
	((y0) + ( (((y1) - (y0)) * ((x) - (x0))) / ((x1) - (x0)) ))

/**
 * @brief Compares against MPFR
 * @returns log2(fabs(y1 - y0))
 */
template<typename fpX>
long double calc_precision(fpX x, fpX& ground_truth, fpX& func_result) {
	fpX y0; // ground truth
	fpX y1; // func result

	FloatMPFR y0_mpfr;
	FloatMPFR y1_mpfr;

	mpfr_set_type<fpX>(y0_mpfr.value, x, MPFR_RNDN);

	{ // Calculate ground truth
		mpfr_erfc(y0_mpfr.value, y0_mpfr.value, MPFR_RNDN);
	}
	{ // Calculate func result
		y1 = erfc(x);
	}

	y0 = mpfr_get_type<fpX>(y0_mpfr.value, MPFR_RNDN);
	ground_truth = y0;
	func_result = y1;

	// Convert back to mpfr for accurate ULP calculation
	mpfr_set_type<fpX>(y0_mpfr.value, y0, MPFR_RNDN);
	mpfr_set_type<fpX>(y1_mpfr.value, y1, MPFR_RNDN);

	mpfr_sub(y1_mpfr.value, y1_mpfr.value, y0_mpfr.value, MPFR_RNDN);
	mpfr_abs(y1_mpfr.value, y1_mpfr.value, MPFR_RNDN);
	mpfr_log2(y1_mpfr.value, y1_mpfr.value, MPFR_RNDN);
	long double diff = mpfr_get_ld(y1_mpfr.value, MPFR_RNDN);
	return diff;
}

template <typename fpX>
void precision_test(void) {
	constexpr size_t points = 65536;

	long double max_diff = -9999999.0L;
	size_t values_printed = 0;

	long double range =  (long double)13.5L;
	long double offset = (long double)14.0L;

	for (size_t i = 0; i < points; i++) {

		fpX x = (fpX)linearInterpolation(
			(long double)i, 0.0L, (long double)points,
			offset - range,  offset + range
		);
		#if 0
			x = exp2(x);
		#endif

		fpX y0;
		fpX y1;
		long double diff =
			calc_precision(static_cast<fpX>(x), y0, y1) -
			static_cast<long double>(ilogb(fabs(static_cast<long double>(y0))));

		if (y0 == static_cast<fpX>(0.0) && y1 == static_cast<fpX>(0.0)) {
			diff = -std::numeric_limits<long double>::infinity();
		}
		// char comp_sign = (y0 == y1) ? '=' : ((y0 > y1) ? '>' : '<');
		if (diff > max_diff + 0.99999L || values_printed == 0 || (isnan(y0) != isnan(y1))) {
			
			if (!std::isnan(diff) && diff > max_diff) {
				max_diff = diff;
			}
			printf(
				"%6zu: %+20.15Lf | x = %+15.9Le | %+15.9Le - %+15.9Le = %+15.9Le\n",
				i, diff,
				static_cast<long double>(x),
				static_cast<long double>(y1),
				static_cast<long double>(y0),
				static_cast<long double>(y1 - y0)
			);

			values_printed++;
		}
	}
}

template <typename fpX>
void graph_precision(const size_t points, const long double range, const long double offset) {
	struct graph_point {
		long double x;
		long double diff;
		long double y0;
		long double y1;
	};
	graph_point* data = (graph_point*)calloc(points, sizeof(graph_point));
	if (data == nullptr) {
		printf("calloc failure\n");
		return;
	}

	int64_t start_time = getNanoTime();
	for (size_t i = 0; i < points; i++) {
		
		fpX x = linearInterpolation(
			(long double)i, 0.0L, (long double)points,
			offset - range,  offset + range
		);
		#if 0
		x = exp2(x);
		#endif

		fpX y0;
		fpX y1;
		long double diff =
			calc_precision(static_cast<fpX>(x), y0, y1) -
			static_cast<long double>(ilogb(fabs(static_cast<long double>(y0))));

		if (y0 == static_cast<fpX>(0.0) && y1 == static_cast<fpX>(0.0)) {
			diff = -std::numeric_limits<long double>::infinity();
		}

		data[i].x    = static_cast<long double>(x );
		data[i].diff = static_cast<long double>(diff);
		data[i].y0   = static_cast<long double>(y0);
		data[i].y1   = static_cast<long double>(y1);

		if (i % 100 == 0) {
			printf(
				"%6zu: %+20.15Lf | x = %+15.9Le | %+15.9Le - %+15.9Le = %+15.9Le\n",
				i, diff,
				static_cast<long double>(x),
				static_cast<long double>(y1),
				static_cast<long double>(y0),
				static_cast<long double>(y1 - y0)
			);
		}
	}
	int64_t finish_time = getNanoTime();

	
	FILE* data_file;
	#if 0
		// Use double
		const char path_name[] = "./graph.bin";
		data_file = fopen(path_name, "wb");
		fwrite(data, points, sizeof(graph_point), data_file);
	#else
		// Use long double
		const char path_name[] = "./graph.csv";
		data_file = fopen(path_name, "wb");
		for (size_t i = 0; i < points; i++) {
			fprintf(data_file,
				"% #16.12Lg, % #16.12Lg, % #16.12Lg, % #16.12Lg,\n",
				data[i].x, data[i].diff, data[i].y0, data[i].y1
			);
		}
	#endif
	fclose(data_file);
	printf(
		"Time Taken: %.3lfs. Wrote %zu points to %s\n",
		static_cast<fp64>(finish_time - start_time) * 1.0e-9, points, path_name
	);

	free(data);
	data = nullptr;
}

#endif /* FLOATNXN_PRECISION_TEST_HPP */
