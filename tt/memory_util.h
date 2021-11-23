

#pragma once


// A header file of general purpose memory access/modification functions.


#include "aliases.h"
#include "macros.h"

#include "numeric_limits.h"
#include "math_util.h"


namespace tt {


	// Measures the length of null terminated C-string x, returning this length.
	// Measuring stops prematurely if max_length is reached, at which point max_length will be returned.
	// Returns zero if x is nullptr.
	// On 64-bit systems the default max_length is functionally infinite for all intents and purposes.
	template<typename Char>
	constexpr tt_size measure_cstr(const Char* const x, tt_size max_length = tt::max_size, Char null_terminator = (Char)0) noexcept {


		if (!(x))
			return 0;

		TT_FOR(i, max_length)
			if (x[i] == null_terminator)
				return i;

		return max_length;
	}


	// Returns a version of x which has had its bytes reversed.
	// This presumes that Value is trivially copyable.
	template<typename Value>
	inline Value flip_bytes(Value x) noexcept {


		static_assert(std::is_trivially_copyable_v<Value>);
		static_assert(sizeof(Value) > 0);
		static_assert((sizeof(Value) == 1) || tt::is_even(sizeof(Value)));

		if constexpr (sizeof(Value) > 1) {


			tt_byte* xx = (tt_byte*)&x;

			TT_FOR(i, sizeof(Value) / 2)
				std::swap(xx[i], xx[sizeof(Value) - 1 - i]);
		}

		return x;
	}


	// Compares the elements of arrays x and y of size n for equality, returning the number of matches found.
	// If x or y is nullptr, or n is zero, zero will be returned.
	template<typename Value>
	constexpr tt_size count_equal_arrays(const Value* const x, const Value* const y, tt_size n) noexcept {


		if (!((x) && (y) && (n > 0)))
			return 0;

		tt_size r = 0;

		TT_FOR(i, n)
			if (x[i] == y[i])
				++r;

		return r;
	}

	// Returns if the contents of arrays x and y are exactly equal.
	// This is performed via "count_equal_arrays(x, y, n) == n", keep this in mind regarding x and y being nullptr.
	template<typename Value>
	constexpr tt_bool equal_arrays(const Value* const x, const Value* const y, tt_size n) noexcept {


		return count_equal_arrays(x, y, n) == n;
	}


	// A version of tt::copy_block which does not statically assert that Value must be trivially-copyable.
	// If x or y is nullptr, or n is zero, the function fails quietly.
	template<typename Value>
	inline void copy_block_unchecked(const Value* const from, Value* const to, tt_size n) {


		if ((from) && (to) && (n > 0))
			std::memcpy((void*)to, (const void*)from, n * sizeof(Value));
	}

	// Copies the contents of array from to array to, both of length n, via std::memcpy.
	// If x or y is nullptr, or n is zero, the function fails quietly.
	// Type Value must be trivially-copyable for this to work.
	template<typename Value>
	inline void copy_block(const Value* const from, Value* const to, tt_size n) {


		static_assert(std::is_trivially_copyable_v<Value>, "copy_block Value must be trivially-copyable!");

		tt::copy_block_unchecked(from, to, n);
	}

	// A version of tt::copy_block_unchecked which does not statically assert that Value must be trivially-copyable.
	// If x or y is nullptr, or n is zero, the function fails quietly.
	template<typename Value>
	inline void copy_block_overlap_unchecked(const Value* const from, Value* const to, tt_size n) {


		if ((from) && (to) && (n > 0))
			std::memmove((void*)to, (const void*)from, n * sizeof(Value));
	}

	// A version of tt::copy_block which allows for overlap by using std::memmove rather than std::memcpy.
	// If x or y is nullptr, or n is zero, the function fails quietly.
	// Type Value must be trivially-copyable for this to work.
	template<typename Value>
	inline void copy_block_overlap(const Value* const from, Value* const to, tt_size n) {


		static_assert(std::is_trivially_copyable_v<Value>, "copy_block_overlap Value must be trivially-copyable!");

		tt::copy_block_overlap_unchecked(from, to, n);
	}


	// Sets the elements of array x of length n to v.
	// If x is nullptr, or n is zero, the function fails quietly.
	template<typename Value> 
	inline void fill_array(Value* const x, tt_size n, const Value& v) {


		if (x)
			TT_FOR(i, n)
				x[i] = v;
	}

	// Adding this little compile-time optimization to use std::memmove when possible,
	// to catch the every so often when we do something without realizing that it can
	// be optimized down to a simple std::memcpy, using std::memmove to ensure the user
	// can safely presume the two regions may overlap.

	// TODO: If this optimization causes problems, we can remove it in future versions.

	// Copies the elements of array from to array to, both of length n.
	// If from or to are nullptr, or n is zero, the function fails quietly.
	// If type Value is trivially-copyable, this function will be optimized to simply wrap tt::copy_block_overlap.
	template<typename Value>
	inline void copy_array(const Value* const from, Value* const to, tt_size n) {


		if constexpr (std::is_trivially_copyable_v<Value>)
			tt::copy_block_overlap(from, to, n);

		else if ((from) && (to) && (n > 0))
			TT_FOR(i, n)
				to[i] = from[i];
	}

	// A version of tt::copy_array which casts values of from into to's type.
	// If from or to are nullptr, or n is zero, the function fails quietly.
	template<typename From, typename To>
	inline void copy_array_cast(const From* const from, To* const to, tt_size n) {


		if ((from) && (to) && (n > 0))
			TT_FOR(i, n)
				to[i] = static_cast<To>(from[i]);
	}

	// A version of tt::copy_array which iterates in reverse order.
	// If from or to are nullptr, or n is zero, the function fails quietly.
	template<typename Value>
	inline void copy_array_reverse(const Value* const from, Value* const to, tt_size n) {


		if ((from) && (to) && (n > 0))
			TT_FOR(i, n)
				to[n - 1 - i] = from[n - 1 - i];
	}

	// A version of tt::copy_array which casts values of from into to's type, and iterates in reverse order.
	// If from or to are nullptr, or n is zero, the function fails quietly.
	template<typename From, typename To>
	inline void copy_array_cast_reverse(const From* const from, To* const to, tt_size n) {


		if ((from) && (to) && (n > 0))
			TT_FOR(i, n)
				to[n - 1 - i] = static_cast<To>(from[n - 1 - i]);
	}

	// Moves the elements of array from to array to, both of length n.
	// If from or to are nullptr, or n is zero, the function fails quietly.
	template<typename Value>
	inline void move_array(Value* const from, Value* const to, tt_size n) noexcept {


		if ((from) && (to) && (n > 0))
			TT_FOR(i, n)
				to[i] = std::move(from[i]);
	}

	// A version of tt::move_array which iterates in reverse order.
	// If from or to are nullptr, or n is zero, the function fails quietly.
	template<typename Value>
	inline void move_array_reverse(Value* const from, Value* const to, tt_size n) noexcept {


		if ((from) && (to) && (n > 0))
			TT_FOR(i, n)
				to[n - 1 - i] = std::move(from[n - 1 - i]);
	}

	// Swaps the contents of arrays x and y, both of length n.
	// If x or y is nullptr, or n is zero, the function fails quietly.
	template<typename Value>
	inline void swap_arrays(Value* const x, Value* const y, tt_size n) noexcept {


		if ((x) && (y) && (n > 0))
			TT_FOR(i, n)
				TT_SWAP(x[i], y[i]);
	}
}

