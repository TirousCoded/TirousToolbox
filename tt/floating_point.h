

#pragma once


// A header file of general purpose floating-point Tirous Toolbox utilities.


#include <cmath>

#include "aliases.h"


namespace tt {


	// Remember that we can in fact use -tt::infinity to have negative infinity values.

	// A positive infinity float value.
	template<typename Value> 
	constexpr Value infinity = (Value)INFINITY;

	// A quiet NaN value.
	template<typename Value> 
	constexpr Value nan = (Value)NAN;

	// Returns if the floating point value is a finite value.
	template<typename Value> 
	inline tt_bool is_finite(Value x) { return std::isfinite(x); }

	// Returns if the floating point value is an infinity value.
	template<typename Value> 
	inline tt_bool is_infinity(Value x) { return std::isinf(x); }

	// Returns if the floating point value is an NaN (Not-a-Number) value.
	template<typename Value> 
	inline tt_bool is_nan(Value x) { return std::isnan(x); }
}

