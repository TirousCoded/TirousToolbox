

#pragma once


// A header file of low-level pattern parsing utilities.


#include "aliases.h"

#include "numeric_limits.h"
#include "math_util.h"


namespace tt {


	// Matches character value x against (y_length long) character array y once, returning if it succeeded.
	// Returns false if y is nullptr.
	template<typename Char>
	inline tt_bool match_char_once(Char x, const Char* const y, tt_size y_length) noexcept {


		if ((y) && (y_length > 0))
			return x == y[0];
		else
			return false;
	}

	// Matches character value x against (y_length long) character array y, sequentially, up to maximum times, returning the number of matches found.
	// The upper limit imposed by maximum is disabled if maximum is 0.
	// Returns 0 if y is nullptr.
	template<typename Char>
	inline tt_size match_char(Char x, const Char* const y, tt_size y_length, tt_size maximum) noexcept {


		if (!(y))
			return 0;

		auto m = (maximum > 0) ? tt::max(y_length, maximum) : y_length;

		tt_size r = 0;

		TT_FOR(i, m)
			if (x == y[i])
				++r;
			else
				break;

		return r;
	}

	// Matches (x_length long) character array x against (y_length long) character array y once, returning if it succeeded.
	// Returns false if either x or y is nullptr.
	template<typename Char>
	inline tt_bool match_str_once(const Char* const x, tt_size x_length, const Char* const y, tt_size y_length) noexcept {


		if (!((x) && (y) && (x_length <= y_length)))
			return false;

		TT_FOR(i, x_length)
			if (x[i] != y[i])
				return false;

		return true;
	}

	// Matches (x_length long) character array x against (y_length long) character array y, sequentially, up to maximum times, returning the number of matches found.
	// The upper limit imposed by maximum is disabled if maximum is 0.
	// Returns 0 if either x or y is nullptr.
	template<typename Char>
	inline tt_size match_str(const Char* const x, tt_size x_length, const Char* const y, tt_size y_length, tt_size maximum) noexcept {


		if (!((x) && (y)))
			return 0;

		auto m = (maximum > 0) ? tt::max(y_length, maximum) : y_length;
		auto d = tt::division_instances(m, x_length);

		tt_size r = 0;

		while (r < d)
			if (tt::match_str_once(x, x_length, y + r * x_length, m - r * x_length))
				++r;
			else
				break;

		return r;
	}

	// Matches (x_length long) character set x against (y_length long) character array y once, returning if it succeeded.
	// Matching character set x against y means testing if a single character of y is located inside x's array.
	// Returns false if either x or y is nullptr.
	template<typename Char>
	inline tt_bool match_set_once(const Char* const x, tt_size x_length, const Char* const y, tt_size y_length) noexcept {


		if ((x) && (y) && (y_length > 0))
			TT_FOR(i, x_length)
				if (x[i] == y[0])
					return true;

		return false;
	}

	// Matches (x_length long) character set x against (y_length long) character array y, sequentially, up to maximum times, returning the number of matches found.
	// Matching character set x against y means testing if a single character of y is located inside x's array.
	// The upper limit imposed by maximum is disabled if maximum is 0.
	// Returns 0 if either x or y is nullptr.
	template<typename Char>
	inline tt_size match_set(const Char* const x, tt_size x_length, const Char* const y, tt_size y_length, tt_size maximum) noexcept {


		if (!((x) && (y)))
			return 0;

		auto m = (maximum > 0) ? tt::max(y_length, maximum) : y_length;

		tt_size r = 0;

		while (r < m)
			if (tt::match_set_once(x, x_length, y + r, m - r))
				++r;
			else
				break;

		return r;
	}

	// Matches the range of character values at and between x_low and x_high against character array y once, returning if it succeeded.
	// If x_low exceeds x_high, their values will be swapped.
	// Returns false if y is nullptr.
	template<typename Char>
	inline tt_bool match_range_once(Char x_low, Char x_high, const Char* const y, tt_size y_length) noexcept {


		if (!(y))
			return false;

		if (x_low > x_high)
			std::swap(x_low, x_high);

		return y_length >= 1 && y[0] >= x_low && y[0] <= x_high;
	}

	// Matches the range of character values at and between x_low and x_high against character array y, sequentially, up to maximum times, returning the number of matches found.
	// The upper limit imposed by maximum is disabled if maximum is 0.
	// Returns 0 if y is nullptr.
	template<typename Char>
	inline tt_size match_range(Char x_low, Char x_high, const Char* const y, tt_size y_length, tt_size maximum) noexcept {


		if (!(y))
			return 0;

		auto m = (maximum > 0) ? tt::max(y_length, maximum) : y_length;

		tt_size r = 0;

		while (r < m)
			if (tt::match_range_once(x_low, x_high, y + r, m - r))
				++r;
			else
				break;

		return r;
	}
}

