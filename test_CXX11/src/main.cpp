/*
**	Author: zerico2005 (2024)
**	Project: LIB-Dekker-Float
**	License: MIT License
**	A copy of the MIT License should be included with
**	this project. If not, see https://opensource.org/license/MIT
*/

#include <limits>
#include <math.h>
#include <algorithm>
#include <cstdio>
#include <float.h>
#include <cfloat>
#include <ios>
#include <numbers>

#include <stdint.h>

#include <iostream>

#include <ctime>
#include <chrono>
inline int64_t getNanoTime(void) { 
	using nanoseconds = std::chrono::duration<int64_t, std::nano>;
	auto now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<nanoseconds>(now.time_since_epoch()).count();
}

#include "../../LDF/LDF_float_types.h"

#ifdef LDF_ENABLE_FLOAT128
	#define MPFR_WANT_FLOAT128
#endif

#include "../../Float80x2/Float80x2_def.h"

#include "../../Float32/Float32.h"
#include "../../Float64/Float64.h"

#if 1
#include "../../Float32x2/Float32x2.hpp"
#endif

#include "../../Float64x2/Float64x2.hpp"

#if 1

#include "../../Float64x2/Float64x2.h"

#ifdef __AVX__
#include "../../Float64/Float64_AVX.h"
// #include "../../Float64/Float64_AVX.hpp"
#include "../../Float64x4/Float64x4_AVX.h"
#include "../../Float64x2/Float64x2_AVX.h"
#endif

#ifdef __SSE2__
#include "../../Float64/Float64_SSE2.h"
#include "../../Float64x2/Float64x2_SSE2.h"
#include "../../Float64x4/Float64x4_SSE2.h"
#endif


// #include "Float64x4/Float64x4.hpp"

#ifdef LDF_ENABLE_FLOAT80
	#include "../../Float80x2/Float80x2.hpp"
#endif
#ifdef LDF_ENABLE_FLOAT128
	#include "../../Float128x2/Float128x2.hpp"
#endif

#include "../../Float32x4/Float32x4.hpp"
#include "../../Float64x4/Float64x4.h"
#include "../../Float64x4/Float64x4.hpp"
#include "../../Float64x3/Float64x3.hpp"
#include "../../Float64x6/Float64x6.hpp"

// Deprecated since it runs slower than mpfr_t. It could be faster than the overhead from C++ wrappers for mpfr_t
// #include "Float80x4/Float80x4.h"

#include "../../util_mpfr/auto_include_FloatX_mpfr.h"

#include "FloatNx2_demo.hpp"

#include "FloatNx2_math_demo.hpp"

#include "precision_test.hpp"

#endif

#if 1
#include "generate_constants.hpp"
#endif

#if 1

void run_demo(void) {
	FloatNxN_demo<Float32x2>(
		"Float32x2_demo",
		"%+.50" PRIFloat32x2 "f",
		"% .*" PRIFloat32x2 "f 14-digits", 17,
		stringTo_Float32x2,
		Float32x2_snprintf
	);
	
	FloatNxN_demo<Float64x2>(
		"Float64x2_demo",
		"%+.50" PRIFloat64x2 "f",
		"% .*" PRIFloat64x2 "f 32-digits", 35,
		stringTo_Float64x2,
		Float64x2_snprintf
	);

	#ifdef Enable_Float80
		FloatNxN_demo<Float80x2>(
			"Float80x2_demo",
			"%+.50" PRIFloat80x2 "f",
			"% .*" PRIFloat80x2 "f 38-digits", 41,
			stringTo_Float80x2,
			Float80x2_snprintf
		);
	#endif
}

void run_math_demo(unsigned int seed) {
	FloatNxN_math_demo<double>(
		seed,
		"Float64_control",
		"%+.*" PRIFloat64 "f",
		snprintf
	);
	FloatNxN_math_demo<Float32x2>(
		seed,
		"Float32x2_math_demo",
		"%+.*" PRIFloat32x2 "f",
		Float32x2_snprintf
	);
	FloatNxN_math_demo<Float64x2>(
		seed,
		"Float64x2_math_demo",
		"%+.*" PRIFloat64x2 "f",
		Float64x2_snprintf
	);
	#ifdef Enable_Float80
		FloatNxN_math_demo<Float80x2>(
			seed,
			"Float80x2_math_demo",
			"%+.*" PRIFloat80x2 "f",
			Float80x2_snprintf
		);
	#endif
}

#endif



void run_generate_constants(void) {
	#if 1
	generate_constants<Float64x4, fp64>("Float64x4");
	#else
	generate_boost_constants<Float64x4, fp64>("Float64x4");
	#endif
}

#if 0

void get_fact(void) {
	#if 0
		#if 1
			for (unsigned long i = 1; i <= 99; i += 2) {
				FloatMPFR f;
				mpfr_fac_ui(f.value, i, MPFR_RNDN);
				mpfr_d_div(f.value, 1.0, f.value, MPFR_RNDN);
				Float80x2 num = mpfr_get_float80x2(f.value, MPFR_RNDN);
				printf(
					"/* %2lu! */ {%-24.15LaL,%+-25.15LaL},\n", i,
					num.hi, num.lo
				);
			}
		#else
			for (unsigned long i = 0; i <= 99; i += 1) {
				FloatMPFR f;
				mpfr_fac_ui(f.value, i, MPFR_RNDN);
				mpfr_d_div(f.value, 1.0, f.value, MPFR_RNDN);
				Float64x4 num = mpfr_get_float64x4(f.value, MPFR_RNDN);
				printf(
					"/* %2lu! */ {%-22.13a,%+-23.13a,%+-23.13a,%+-23.13a},\n", i,
					num.val[0], num.val[1], num.val[2], num.val[3]
				);
			}
		#endif
	#else
		#if 1
		for (unsigned long i = 0; i <= 256; i += 1) {
			FloatMPFR f;
			mpfr_set_ui(f.value, i, MPFR_RNDN);
			mpfr_div_ui(f.value, f.value, 1024, MPFR_RNDN);
			mpfr_cospi(f.value, f.value, MPFR_RNDN);
			Float80x2 num = mpfr_get_float80x2(f.value, MPFR_RNDN);
			printf(
				"/* %3lu */ {%-22.15LaL,%+-23.15LaL},\n", i,
				num.hi, num.lo
			);
		}
		#else
		for (unsigned long i = 0; i <= 256; i += 1) {
			FloatMPFR f;
			mpfr_set_ui(f.value, i, MPFR_RNDN);
			mpfr_div_ui(f.value, f.value, 1024, MPFR_RNDN);
			mpfr_sinpi(f.value, f.value, MPFR_RNDN);
			Float64x4 num = mpfr_get_float64x4(f.value, MPFR_RNDN);
			printf(
				"/* sin(pi * %3lu/1024) */ {%-20.13a,%+-22.13a,%+-23.13a,%+-23.13a},\n", i,
				num.val[0], num.val[1], num.val[2], num.val[3]
			);
		}
		#endif
	#endif
}

#endif

void generate_inverf(int Precision, int Maxiumum_Terms, int Digits = 0);
void test_inverf(void);

#if 0
#include <quadmath.h>
void aaa(void) {
	#if 1
	const int count = 65536;
	{
		int64_t start = getNanoTime();
		for (int i = 0; i < count; i++) {
			Float64x4 ret = ((fp64)i / 8192.0);
			ret *= erf(ret);
		}
		int64_t finish = getNanoTime();
		printf("Time: %.3Lfms\n", static_cast<long double>(finish - start) * 1.0e-6L);
	}
	{
		int64_t start = getNanoTime();
		for (int i = 0; i < count; i++) {
			FloatMPFR ret = ((fp64)i / 8192.0);
			ret *= erf(ret);
		}
		int64_t finish = getNanoTime();
		printf("Time: %.3Lfms\n", static_cast<long double>(finish - start) * 1.0e-6L);
	}
	#endif
	#if 0
	fp80 max_diff = -1000.0L;
	for (int i = 0; i < 16777216; i++) {
		Float80x2 x0 = (fp80)i;
		fp128 x1 = (fp128)i;
		x0 = sin(x0);
		x1 = sin(x1);
		fp80 diff = (fp80)log2q(fabsq((fp128)x0 - x1));
		if (diff > max_diff) {
			max_diff = diff;
			printf("%5d: %+24.18Le %+24.18Le | %+11.8Lf\n", i, (fp80)x0, (fp80)x1, diff);
		}
	}
	#endif
}
#endif

#include "../../Float64x6/Float64x6.hpp"

struct AFloat64x2 {
	int expon;
	uint64_t hi;
	uint64_t lo;
};

static constexpr AFloat64x2 c_Table[] = {
    {+1, 0xC90FDAA22168C234uL, 0xC4C6628B80DC1CD1uL},
    {-2, 0xA2F9836E4E441529uL, 0xFC2757D1F534DDC0uL},
    {+1, 0xADF85458A2BB4A9AuL, 0xAFDC5620273D3CF1uL},
    {-2, 0xBC5AB1B16779BE35uL, 0x75BD8F0520A9F21BuL},
    {0, 0xBB16C31AB5F1FB70uL, 0xD4E4432F05278358uL},
    {-1, 0x93C467E37DB0C7A4uL, 0xD1BE3F810152CB56uL},
    {0, 0x99DD0027803109C1uL, 0xB8B8AE2CF3483F8DuL},
    {0, 0x84BA0C7653E66DBDuL, 0x118271E8CCFFA309uL},
    {0, 0x811196D0A679C46DuL, 0x4DFACD442D6216FBuL},
    {0, 0x8041CF9EC0B5AB81uL, 0x7FD07DD8E6B4333CuL},
    {-1, 0xB17217F7D1CF79ABuL, 0xC9E3B39803F2F6AFuL},
    {0, 0xB8AA3B295C17F0BBuL, 0xBE87FED0691D3E88uL},
    {-2, 0x9A209A84FBCFF798uL, 0x8F8959AC0B7C9178uL},
    {+1, 0xD49A784BCD1B8AFEuL, 0x492BF6FF4DAFDB4CuL},
    {0, 0xB504F333F9DE6484uL, 0x597D89B3754ABE9FuL},
    {-1, 0x8000000000000000uL, 0x0000000000000000uL},
    {0, 0xCDA82FCF21F9121BuL, 0xF9B9629409231DE0uL},
    {2, 0x956A197E30E4BA3AuL, 0x3809DA8B5C4C87EDuL},
    {1, 0xA02FA4831C037B70uL, 0x65437EAC7F404AD3uL},
    {1, 0xA7CFCA7CFA858373uL, 0xCC3BB5D6046CF0BDuL},
    {0, 0xA42692797EC39898uL, 0x9F6AB71A1ED3BABCuL},
    {-1, 0xEA7CB89F409AE845uL, 0x215822E37D32D0C6uL},
    {1, 0xB3B281F50C729CB8uL, 0x7D0615AD23F4DC47uL},
    {1, 0xABDE7211E3686BBCuL, 0xBF7CF8E5199FE70CuL},
    {-2, 0x85E2F67259FF6A3EuL, 0x6F326DC1668A2F41uL},
    {-1, 0x91304D7C74B2BA5EuL, 0xAFDDAA6286DC28E1uL},
    {-1, 0xC3A4294EFECE5322uL, 0x1A3AAFEFD88366A3uL},
    {0, 0xA73763F7B46E5A60uL, 0x831E98883E852FFEuL},
    {0, 0xB9C677931F512642uL, 0xC676B1774EF3FFCBuL},
    {-1, 0xD2A4ACF1B1952D4CuL, 0xF9B6C08D9048AEA1uL},
    {1, 0xD7085DE3B412547AuL, 0x6E1D157303AB3415uL},
    {0, 0xDA445BCFC382C748uL, 0xBAA1D4C214E3C4ADuL},
    {-1, 0x9FD41712B4E41BD4uL, 0x26B9F2F254BA047AuL},
    {0, 0xCF1BBCDCBFA53E0AuL, 0xF9CE60302E76E41AuL}
};

static constexpr AFloat64x2 erfc_GtP5PadeTable[] = {
	{0, 0x8000000000000000uL, 0x0000000000000000uL}, {0, 0xCFE58F1CD6B15901uL, 0x377DD7FD748BB77AuL},
	{1, 0x8AB803C6F3475E75uL, 0x7A6EF4CDB8692F7FuL}, {2, 0x9BEEA85380B6BE97uL, 0x9C397B2EA3AF033DuL},
	{1, 0x935E4CC0A2902222uL, 0x2B2F682DF64FB89DuL}, {2, 0xDB303C5B1FAA7886uL, 0x1753779854F61828uL},
	{0, 0xC770F6A954B72B6AuL, 0xF122F45183C3A8BDuL}, {2, 0xBF3BEB65DDA6D4E9uL, 0x00C16F4EE1FA5682uL},
	{-1, 0xBDD87B34D74A2178uL, 0xD7F35FB303072A8AuL}, {1, 0xE733FECF0F8B6B14uL, 0x95C998AB7121DADCuL},
	{-2, 0x85355CA3FC03C74EuL, 0xEE485FC34E7CC25CuL}, {0, 0xCCA3E6B369C2AE12uL, 0x545250F410044BA2uL},
	{-4, 0x8CD5D367A161DB7EuL, 0x23C48D443CB15380uL}, {-1, 0x8882A1743989E0D0uL, 0x9A86DE54D9F534C9uL},
	{-7, 0xE196BB549DE9097AuL, 0x87E9F32102B74C2FuL}, {-3, 0x8B1253A7E2BB10DCuL, 0xE763A921A26451C8uL},
	{-9, 0x87A98D79DC7099C6uL, 0x2EC59FAF469DFB19uL}, {-6, 0xD889870F03E89848uL, 0x9D0AA3AE6B9D32FFuL},
	{-13, 0xEECD07B7B136AEDBuL, 0x45BC69DB53509F42uL}, {-9, 0xFEAAF61C01E8327BuL, 0x0CDB0DCC202642A9uL},
	{-16, 0x91F4283208E1E2F3uL, 0x0911C383182B533AuL}, {-12, 0xDC18CC2EBADCE3B5uL, 0xAF733E458118F742uL},
	{-21, 0xDE822A0E4D76651EuL, 0x7BE7F3A8306BE487uL}, {-15, 0x847F9F2825B1092AuL, 0x115C8DC5EC7DF2CBuL},
	{-26, 0xA00944D86EDB2C5DuL, 0x852483CCA1CF0B4EuL}, {-20, 0xC768B7E1BAE3CE4FuL, 0x04CAD8B7BF5B8472uL},
	{0, 0x0000000000000000uL, 0x0000000000000000uL}, {-25, 0x8DD4118E15F5D4F0uL, 0x7C266ABF2D30C293uL},
};

static constexpr AFloat64x2 erfc_Gt1PadeTable[] = {
	{0, 0x8000000000000000uL, 0x0000000000000000uL}, {1, 0x95ADA7B501D4B116uL, 0x1C293D62F156A804uL},
	{1, 0x999D65DF911850C4uL, 0x0754F1D897BE04D3uL}, {2, 0xE3737EE719F802D0uL, 0x96E01E4809795A3AuL},
	{1, 0xB20962272E0B53EDuL, 0x9F76AC013B9358FCuL}, {3, 0xA3407D8AB75C6C42uL, 0xFBFABBE98247AA87uL},
	{1, 0x832B2270E7895D1BuL, 0xAB9EA2EA9A2CE59CuL}, {3, 0x92E1165436E2E616uL, 0xE2C6595E76218743uL},
	{0, 0x88B41E6D38AFD6E6uL, 0x1AC916DB41B3E41DuL}, {2, 0xB951E2271799E9CFuL, 0xFA701C3CD61B2483uL},
	{-2, 0xD4A1F5CBFBE79704uL, 0xCF61376B257127DBuL}, {1, 0xADBB10CE59B3A920uL, 0x67117F429D591C67uL},
	{-4, 0xFE0FB9727C566433uL, 0xF68BCD054EF5DB19uL}, {-1, 0xFA1F1113C9745DEEuL, 0xE754305050089181uL},
	{-6, 0xEC9AF038F9AD2C54uL, 0x2A2182B431A74CC5uL}, {-2, 0x8CD7F0EA47D7BCCCuL, 0x21284C25F2EF1389uL},
	{-8, 0xAC72F1495F00BE7CuL, 0x76375039BF982E19uL}, {-5, 0xFA4F9BEE961EDE00uL, 0xCABAEA09876DE95BuL},
	{-11, 0xC3AF01FF05A2E065uL, 0x21EB2D1E7F95266BuL}, {-7, 0xAF96EC864E104B63uL, 0x7F4FBDF25BD1E2EFuL},
	{-14, 0xAA1C7290ADA6FD7EuL, 0xADD89675104B18C5uL}, {-10, 0xC0FB7481B8B5DED1uL, 0x17FDB72A22A258BBuL},
	{-18, 0xDBB4CEAF513438C9uL, 0x4F6EB6A63F538084uL}, {-13, 0xA34251F009A90DDEuL, 0x1232AD2132794003uL},
	{-22, 0xC749DCCB4F121F6FuL, 0x11B03ACF20A720BBuL}, {-17, 0xCDF10442A3EA6F8EuL, 0xE4025AC11EB86566uL},
	{-27, 0xE356B23CF260C070uL, 0xE5105357D098BCEAuL}, {-21, 0xB6F6D3D95B3401B7uL, 0xCF0CDCCCDC093007uL},
	{-33, 0xF62AE4E3AA6FD939uL, 0xF6C5531E9B9E54ECuL}, {-26, 0xCCE1E49542C502A5uL, 0xC10D740F109B7DA3uL},
	{0, 0x0000000000000000uL, 0x0000000000000000uL}, {-32, 0xDA290A6F232A69B6uL, 0xEF77BB3D4219B264uL},
};

static constexpr AFloat64x2 erfc_Gt2PadeTable[] = {
	{0, 0x8000000000000000uL, 0x0000000000000000uL}, {1, 0xFA97703BE40EAC7DuL, 0x247AB845F0C2A95BuL},
	{1, 0x973C745F0DBC0AA7uL, 0x51ACAFDCE807E19EuL}, {3, 0xAE3CE81B627507EBuL, 0xE67DED6F792E6F00uL},
	{1, 0xAA53CF44254182C2uL, 0x8A29C82264A1F35BuL}, {3, 0xE555938843FE2C16uL, 0x26D0426A03327E03uL},
	{0, 0xF218CB68C792577CuL, 0xCB29CF9D1E2E5378uL}, {3, 0xBDA9C4327E4A37F4uL, 0xAAF79D63DD72D679uL},
	{-1, 0xF2856765286C6899uL, 0x0F1AC21D77A8BB98uL}, {2, 0xDCAB8CCF4C7D873FuL, 0x64C9A8FB323EE315uL},
	{-2, 0xB534A7450FE24DF0uL, 0xD28447C9B9A45DDEuL}, {1, 0xBF8BF59838C13BC0uL, 0x04362D1AD0AD9DE7uL},
	{-4, 0xD08346DD56EB6CAEuL, 0x533D151EB4B4263BuL}, {0, 0x8058E9CDB4DC1FE9uL, 0xAFD7E7811140BF5AuL},
	{-6, 0xBC0DA933CF633876uL, 0x715BBBE9C93D0E31uL}, {-2, 0x87792AFABFCA3B3AuL, 0x7067AA23855C24B0uL},
	{-8, 0x85FBDE494F956FABuL, 0xBC2E5F57067A2320uL}, {-5, 0xE3B6335D1FADC27BuL, 0xF8CE93C913839D0CuL},
	{-11, 0x96CD297A89600034uL, 0x5E5AB21DEB28C152uL}, {-7, 0x98EE118EE05A832BuL, 0x396126E992EDAA71uL},
	{-14, 0x84F5D5EE6AEBD14AuL, 0x294096C69817426BuL}, {-10, 0xA3A73A45C0A19D50uL, 0x95EDD8BCE8C5269BuL},
	{-18, 0xB45F720F45AC130DuL, 0x7FF0A2BA4997AED7uL}, {-13, 0x8A1A5D787A595657uL, 0xA7CFC88CB52FA730uL},
	{-22, 0xB63CEF6D76F6A152uL, 0x489A1184819C92F3uL}, {-17, 0xB4404A88B5022291uL, 0x96D5AF0F752B0976uL},
	{-26, 0x817A2622B5743EC7uL, 0xDC2765CBA482FAC8uL}, {-21, 0xAFF1EBEFE32EC186uL, 0x93E397C948C89688uL},
	{-32, 0xE76D7620882C36E5uL, 0x82E003A299E3A01EuL}, {-26, 0xF25A7176C4F6E95BuL, 0x521ABA392849B9A7uL},
	{-38, 0xC42D963B574DA843uL, 0x5725E9CCE2443F58uL}, {-31, 0xD287CA1A289F3BF2uL, 0x7AC5C4F402BCD3DDuL},
	{-83, 0x8147BF99B84FB714uL, 0x5109333709F79E0BuL}, {-37, 0xADDBB8F5B662AC8BuL, 0x009A8067D645241BuL},
};

static constexpr AFloat64x2 erfc_Gt4PadeTable[] = {
	{0, 0x8000000000000000uL, 0x0000000000000000uL}, {2, 0xE993DB6F3F17B91BuL, 0x5585C0B72C77686AuL},
	{1, 0x87DFC2CD3CB1388BuL, 0xAB943F5FCB5B3B74uL}, {4, 0x89C6B9398EBA9A00uL, 0x8DB6B55BB99103ECuL},
	{1, 0x889C0B7DCE98A03FuL, 0x5286422AE046B990uL}, {4, 0x9A07814BA27C35D3uL, 0x9754372AF64DE021uL},
	{0, 0xACA54757EEA04849uL, 0xD9EE1100A06C8F51uL}, {3, 0xD8C27409E3EC4029uL, 0xF74481ADB3DE8CDEuL},
	{-1, 0x9975C5512B202808uL, 0x59E4991AD21CBBCFuL}, {2, 0xD70CEA7B0F2DDF98uL, 0xE90A2F0CD73F4327uL},
	{-3, 0xCB70B7F4E7E986B1uL, 0xD037144080FFA4A7uL}, {1, 0x9FA4CF12E1EAAC0DuL, 0x6314AD03D94D633EuL},
	{-5, 0xD007AD320745FEE2uL, 0x4DD22E4DDA8F0200uL}, {-1, 0xB7AD0195A484DB4DuL, 0x04FA9ED7A9B1CD86uL},
	{-7, 0xA75EFDB0C11B39CEuL, 0xCCAD0382F8F6BE15uL}, {-3, 0xA74B02F0125F743CuL, 0x750CC0C0BDDC8470uL},
	{-10, 0xD626C882E87DB7C6uL, 0x7BF28BD578B808F9uL}, {-6, 0xF4456FB1C0B25120uL, 0x9AB052E60E2FF1C1uL},
	{-13, 0xDA9CC71E16801D4CuL, 0x9CCF59744708C616uL}, {-8, 0x8FC90FB7AB75F0DEuL, 0x58CC0EB04AA45E03uL},
	{-16, 0xB177C273801B9F3FuL, 0xC98826F1B3F1E6EEuL}, {-11, 0x887FFD2F61E3F365uL, 0x9917D7246E020C0FuL},
	{-20, 0xE2BD55BF376A14C9uL, 0xA34C771B4E3663DAuL}, {-15, 0xCFD612418A6DC7CCuL, 0x69FFF76C7F018438uL},
	{-24, 0xDF8023F7FDC1AF79uL, 0xE0E7AD8E0BB8CC2AuL}, {-19, 0xFAB60DD3A35FC23DuL, 0x9432337C390F9C30uL},
	{-28, 0xA454CEB7B995A712uL, 0x670F080268BCDE5FuL}, {-23, 0xEA9DD3EEE330BC03uL, 0x39A056FF6F6F2A0AuL},
	{-33, 0xAA03F4BC6B274348uL, 0x8EFD47D78FDEFD50uL}, {-27, 0xA483CD0FA42350ABuL, 0xCCD639A70C8D492FuL},
	{-39, 0xDD2A61E2E49A7B13uL, 0x4431D0ADEF69EA12uL}, {-32, 0xA2EFEC077D7323DCuL, 0x228D66F6FD5AB114uL},
	{-45, 0x8848DC6A79D92521uL, 0xADEE925C8C6072D5uL}, {-38, 0xCB8D334F7996365EuL, 0x1386AB7C692A26F0uL},
	{0, 0x0000000000000000uL, 0x0000000000000000uL}, {-45, 0xF18EE56264BBE5F5uL, 0x0791F206FB7902ADuL},
};

static constexpr AFloat64x2 erfc_Gt8PadeTable[] = {
	{0, 0x8000000000000000uL, 0x0000000000000000uL}, {3, 0xE49EB02D10E55196uL, 0xF242A9AA48D3D599uL},
	{0, 0xAE7EFDF1B14EF53CuL, 0x926006BEE327A9DAuL}, {4, 0xA9E824BD7E45DE61uL, 0x9172EBC63EEF36ADuL},
	{-1, 0xDE084E83E86A3544uL, 0xDB966355F04C950EuL}, {3, 0xECAEC2002FD7D6CEuL, 0xCC73ED1D92E733BBuL},
	{-2, 0xAEC1E29F9CABD45BuL, 0x8B17F7152B33C636uL}, {2, 0xCCF474FAA4DBD2FBuL, 0x9D388CAB54C2E77BuL},
	{-4, 0xBE1BF616F76CA1E4uL, 0x0CCA5E2D607CABC7uL}, {0, 0xF6C804153A4DCCF2uL, 0xCFDBE86DFA7E68D4uL},
	{-6, 0x973073A3F3B2DD11uL, 0x59ECCAE5C4E11353uL}, {-2, 0xDAD664FB822EE99DuL, 0xCA6C34760B47981DuL},
	{-9, 0xB54C280F69835BCEuL, 0x6E40F45D98144021uL}, {-4, 0x93A65C1A643140DAuL, 0xF6A48E999F598EA4uL},
	{-12, 0xA682292C56934CA1uL, 0x8B8BC4CF51247C59uL}, {-7, 0x9A5F30788DA0CEE1uL, 0xFEF3BC8EBACE6742uL},
	{-16, 0xEB674C79AFAC5619uL, 0x0C7263F651F23518uL}, {-11, 0xFC2DECE344BBCE82uL, 0x5622DC20DEC6922EuL},
	{-20, 0xFEE9A70E70C50785uL, 0xB94A46BB641E6E47uL}, {-14, 0xA0EC91D8D1089501uL, 0x4EAD1299577E7B67uL},
	{-24, 0xD023158E364EDB3FuL, 0xCA2BDE6533AA521BuL}, {-18, 0x9F284B2422848652uL, 0x82EBA5B9FE1CDE8BuL},
	{-29, 0xF8869885097C7D90uL, 0xED69C1D563EA65D5uL}, {-23, 0xEF98E92C658DE104uL, 0x23389FF89A0F25C7uL},
	{-34, 0xCD20971186895AB1uL, 0x63CE69D44711DC0DuL}, {-27, 0x84DEBE911AC4C682uL, 0x687C41759457CDDBuL},
	{-40, 0xD185E4A1E2AF0BC7uL, 0xF1CE027D1B5F1CD5uL}, {-33, 0xCD02C1AABB3801CDuL, 0x4118E9BBD26F6709uL},
	{-47, 0xC7D4460652F81B09uL, 0x4F11A090F55D4616uL}, {-39, 0xC4C0D96A00D16E3AuL, 0xA7C3C7AC45F2E863uL},
	{0, 0x0000000000000000uL, 0x0000000000000000uL}, {-46, 0xB1181127AD047634uL, 0x3211269261DA78AEuL},
};

static constexpr AFloat64x2 erfc_Gt16PadeTable[] = {
	{0, 0x8000000000000000uL, 0x0000000000000000uL}, {4, 0xE350C4179D924C9BuL, 0x7094703468B7D8E1uL},
	{-1, 0xA49B29FC4BB0D6B3uL, 0x76147A830414A2BAuL}, {4, 0xA0507F06A2C831E0uL, 0x1E89E3A35287EAD7uL},
	{-3, 0xC0D1A735DA0DC9CAuL, 0x1B30DF05E25700F3uL}, {2, 0xCF9EA8B9712B16E9uL, 0x1BAA86B406258F5EuL},
	{-5, 0x87CA28F41F81B7CAuL, 0xB8CF372330E1B0B9uL}, {0, 0xA33A6B99FD17FCE2uL, 0x6F0478A882ADD22FuL},
	{-9, 0xFF83D68FE382A35AuL, 0xA1F38559D2A3A1D2uL}, {-3, 0xAD878D7B272E6134uL, 0xC428D8E826558A4FuL},
	{-12, 0xA89DCD8F800676F3uL, 0xFB8BADE41C44FF68uL}, {-6, 0x83678CB21E7C7ACEuL, 0xB11F9A57226BDE5CuL},
	{-16, 0x9F46F08AE80664A6uL, 0x49E3BEDC208A04F2uL}, {-10, 0x915AC549EDD02C0BuL, 0xCF4861D5629BB32CuL},
	{-21, 0xD75DFA5C372E77D7uL, 0x74AC9BB8A5DDB958uL}, {-15, 0xECA728D4300C4D74uL, 0x02E9B00C55883B1FuL},
	{-26, 0xCC41C0E81E6C2152uL, 0xE909FCDDC7E10C47uL}, {-19, 0x8CB5ACE5D5905205uL, 0x2D1B4EA0861E5BABuL},
	{-31, 0x816844888BE4DDB1uL, 0x1182D9FAA6509EF8uL}, {-25, 0xEE6127DB8CB1B675uL, 0x067A5A5521824DCBuL},
	{-38, 0xC52A6B8900B478FFuL, 0xBA756E232D5DAFFAuL}, {-30, 0x8887957C1268257AuL, 0xD585F49356764EBBuL},
	{-45, 0x88D31242713AC949uL, 0x359FF2079684A05EuL}, {-37, 0xBDE408254E997A57uL, 0x1FDCB5C043F596C3uL},
	{0, 0x0000000000000000uL, 0x0000000000000000uL}, {-45, 0xF283DE11BF2C2226uL, 0x294285FDBDCD1505uL},
};

static void print_table(const AFloat64x2 table[], const size_t len, const char* label) {
	printf("static constexpr Float64x2 %s[] = {\n", label);
	for (size_t i = 0; i < len; i++) {

		// fp128 f_hi = (fp128)(table[i].hi) + 18446744073709551616.0Q;
		// fp128 f_lo = (fp128)(table[i].lo) + 18446744073709551616.0Q;
		// f_hi = (table[i].hi == 0) ? 0.0Q : f_hi;
		// f_lo = (table[i].lo == 0) ? 0.0Q : f_lo;

		fp128 f_hi = (fp128)(table[i].hi);
		// fp128 f_lo = (fp128)(table[i].lo);

		f_hi = ldexp(f_hi, -63);
		// f_lo = ldexp(f_lo, -63 - 53);
		f_hi = ldexp(f_hi, table[i].expon);
		// f_lo = ldexp(f_lo, table[i].expon);
		printf("\t{%+.18le, %+.18le},\n", (fp64)f_hi, (fp64)0.0);
	}
	printf("};\n\n");
}

static void foo(void) {
	// print_table(c_Table, sizeof(c_Table) / sizeof(AFloat64x2), "c_Table");
	print_table(erfc_GtP5PadeTable, sizeof(erfc_GtP5PadeTable) / sizeof(AFloat64x2), "erfc_GtP5PadeTable");
	print_table(erfc_Gt1PadeTable , sizeof(erfc_Gt1PadeTable ) / sizeof(AFloat64x2), "erfc_Gt1PadeTable" );
	print_table(erfc_Gt2PadeTable , sizeof(erfc_Gt2PadeTable ) / sizeof(AFloat64x2), "erfc_Gt2PadeTable" );
	print_table(erfc_Gt4PadeTable , sizeof(erfc_Gt4PadeTable ) / sizeof(AFloat64x2), "erfc_Gt4PadeTable" );
	print_table(erfc_Gt8PadeTable , sizeof(erfc_Gt8PadeTable ) / sizeof(AFloat64x2), "erfc_Gt8PadeTable" );
	print_table(erfc_Gt16PadeTable, sizeof(erfc_Gt16PadeTable) / sizeof(AFloat64x2), "erfc_Gt16PadeTable");
}

int main(void) {
	const char* input_numer[] = {
		"0.4275835761558070044107503444905151808201595031642526637455397707405054218202",
		"0.7488042145685599356715179473582150725036718979438250795033813007021046091154",
		"0.6197074484260350875908760314600453763282096334056893312461784836283310225456",
		"0.3134808646006664943957924091860828376738978576910937310518032191868136774227",
		"0.1061079681379095445144129176917409834366709166926714157692774296541115736069",
		"0.0248259087666970023973378851119699562214165855838569896070667442983370420636",
		"0.004006189891414598916750786737352564394520790588608338355480119838596071823148",
		"0.0004294584846659999254894185085166902012518591255256462551641853260145769835204",
		"0.00002773889721131736086029574206879900463918119364263590525227435369348189629942",
		"0.0000008226003913503168596179655968983521098381175300989233775167726664483771518584",
		"0.00000000000002851828719619464755437579206767481670732199597217939248355971098404410846338",
	};
	const char* input_denom[] = {
		"1.0",
		"2.390213951950405188135669571133364183130522670558025706216606830579784994845",
		"2.615561305431335354014630759863639017610624512004510952131858352125450988805",
		"1.726749082488493159345670532030568646263680897254873692514558921144198060704",
		"0.7622027830360582545033193271471927538490713259664874351570344560676401931876",
		"0.2352451245117950239464623133157998297139260313172965462496809532048152677156",
		"0.05145962856497862187655267816714451612576161348722955148353078631907776576664",
		"0.007885830494878421223194505231729508494112901447776000695738585088810900604362",
		"0.0008110907772834349820453737038152748728058508866419738034483855952495902194269",
		"0.00005062389980477145703168694957105210360531303033237712269122376673921026889551",
		"0.00000145802314618724435387860098964851053306977031625118874716277055651782985061"
	};
	constants_from_text<Float64x2, fp64>(
		input_numer, sizeof(input_numer) / sizeof(input_numer[0]),
		"Float64x2", "erfc_pade_1_numer",
		256
	);
	constants_from_text<Float64x2, fp64>(
		input_denom, sizeof(input_denom) / sizeof(input_denom[0]),
		"Float64x2", "erfc_pade_1_denom",
		256
	);

	// printf("%016llX\n", sqrt(2.0));
	// printf("%016la %016lf\n", UINT64_C(0x3ffcfe58f1cd6b15), UINT64_C(0x3ffcfe58f1cd6b15));
	// printf("\n");
	// foo();
	// aaa();
	// run_demo();
	// run_math_demo(123456);
	// test_function();

	// generate_inverf(8192, 300);
	// test_inverf();

	// precision_test<Float64x2>();
	graph_precision<Float64x2>(10000, 1.0L, 1.0L);

	// run_generate_constants();

	// __attribute__((unused)) char buf[999];
	// Float32x2 x = {FLT_MAX, FLT_MIN};
	// Float32x2_snprintf(buf, sizeof(buf), "%+.50Df", x);
	// printf("FLT_MAX + FLT_MIN = [%s]\n", buf);

	#if 0
	{
		Float64x2 r = -0.75;
		for (int i = 0; i < 16; i++) {
			Float64x2 result = tgamma(r);
			std::cout << "tgamma(" << r << ") = " << result << std::endl;
			r += 0.75;
		}
	}
	#endif
	
	#if 0
	__attribute__((unused)) char buf[999];
	
	Float64x2 x = "2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382"_FP64X2;
	fp64 y = x;
	int exp_x, exp_y;
	x = frexp(x, exp_x);
	y = frexp(y, &exp_y);
	Float64x2_snprintf(buf, sizeof(buf), "%+.50" PRIFloat64x2 "f", x);
	printf(
		"Float64x2: %s * 2^%d\nFloat64  : %+.50f * 2^%d\n",
		buf, exp_x, y, exp_y
	);
	#endif
	#if 0
		// Testing the accuracy of modf
		printf("\n");
		for (int i = 0; i < 130; i++) {
			long double res0 = fmodl((long double)ldexpf(1.2345f, i), (long double)((float)M_PI));
			float       res1 = fmodf(             ldexpf(1.2345f, i),              ((float)M_PI));
			
			float res2 = trunc(ldexpf(1.2345f, i) / (float)M_PI);
			res2 = ldexpf(1.2345f, i) - (float)M_PI * res2;

			// long double res0 = sinl((long double)ldexpf(1.2345f, i));
			// float       res1 = sinf(             ldexpf(1.2345f, i));
			printf(
				"%4d [%+9.6Lf]: %+9.6f - %+9.6f = %+6.2Lf error\n", i,
				res0, res1, res2, log2l(fabsl((long double)res1 - (long double)res2))
			);
		}
	#endif

	// printf("%La\n", LDBL_MAX);

	// Float64x2 y = Float64x2_set_d(3.0);
	// Float64x4 result = Float64x4_recip_quick_dx2(y);
	// mpfr_t p;
	// mpfr_init2(p, 512);
	// mpfr_set_float64x4(p, result, MPFR_RNDN);
	// mpfr_printf("\nFloat64x4: \n1/3 = %.70Rf\n", p);
	
	// get_fact();
	printf("\n");
	fflush(stdout);
	return 0;
}
