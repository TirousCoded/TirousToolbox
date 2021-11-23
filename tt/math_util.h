

#pragma once


// A header file of general purpose Tirous Toolbox mathematics utilities.


#include "aliases.h"


namespace tt {


	// Returns the factorial of x.
	// Sub-zero values will be treated as zero when computing.
	template<typename Value>
	constexpr Value factorial(const Value& x) {


		if (x <= Value(0))
			return Value(1);
		else
			return x * tt::factorial<Value>(x - Value(1));
	}

	// Returns the maximum of the two given values.
	// The type Value must have a valid less-than operator.
	template<typename Value>
	constexpr Value max(const Value& x, const Value& y) { return (x < y) ? y : x; }

	// Returns the minimum of the two given values.
	// The type Value must have a valid less-than operator.
	template<typename Value>
	constexpr Value min(const Value& x, const Value& y) { return (x < y) ? x : y; }

	// Returns the value passed clamped to stay between the given minimum and maximum.
	template<typename Value>
	constexpr Value clamp(const Value& x, const Value& max_x, const Value& min_x) { return tt::min(tt::max(x, min_x), max_x); }

	// Returns the number of times d can be subtracted from x such that x remains non-negative.
	template<typename Value>
	constexpr Value division_instances(const Value& x, const Value& d) { return (x - (x % d)) / d; }

	// Returns if the given x is even, with this function being intended for positive values, or zero which is treated as even.
	template<typename Value>
	constexpr bool is_even(const Value& x) { return x % (Value)2 == (Value)0; }

	// Returns if the given x is odd, with this function being intended for positive values, or zero which is treated as even.
	template<typename Value>
	constexpr bool is_odd(const Value& x) { return !is_even(x); }

	// Given a value x, and an alignment s, this returns x snapped to the aligned grid created by s.
	template<typename Value>
	constexpr Value aligned_index(const Value& x, const Value& s) { return tt::division_instances<Value>(x, s) * s; }

	// Given a value x, and an alignment s, this returns the minimum number of aligned increments of s which can contain x.
	template<typename Value>
	constexpr Value aligned_count(const Value& x, const Value& s) {


		Value R = tt::division_instances<Value>(x, s);

		if (x % s > 0)
			++R;

		return R;
	}

	// Given a value x, and an alignment s, this returns the minimum aligned size which can contain x.
	template<typename Value>
	constexpr Value aligned_size(const Value& x, const Value& s) { return aligned_count<Value>(x, s) * s; }

	// Given a type U and an alignment s, this returns the minimum aligned size which can contain sizeof(U).
	template<typename Value, typename U>
	constexpr Value aligned_size_of(const Value& s) { return aligned_size<Value>((Value)sizeof(U), s); }

	// Given a type U (of u) and an alignment s, this returns the minimum aligned size which can contain sizeof(U).
	template<typename Value, typename U>
	constexpr Value aligned_size_of(const U& u, const Value& s) { return aligned_size_of<Value, U>(s); }
}

