

#pragma once


// The Tirous Toolbox library iterator range implementation.


#include "aliases.h"
#include "macros.h"
#include "debug.h"

#include "forward_declarations.h"

#include "iterator_util.h"

#include "visualize.h"

#include "hash.h"
#include "hash_factory.h"


namespace tt {


	// The Tirous Toolbox library iterator range implementation.
	template<typename Iterator>
	class range final {
	public:

		using iterator_t = typename Iterator;

		using this_t = tt::range<iterator_t>;


	private:

		iterator_t _begin, _end;

		mutable tt_size _cached_distance;
		mutable tt_bool _has_cached_distance;

		inline void _mutate() const noexcept {


			_cached_distance = 0;
			_has_cached_distance = false;
		}

		inline void _cache_distance() const {


			if (_has_cached_distance)
				return;

			_cached_distance = std::distance(_begin, _end);
			_has_cached_distance = true;
		}

		inline tt_size _distance() const {


			_cache_distance();

			return _cached_distance;
		}


	public:

		// Default initializes a range.
		inline range() noexcept {


			_begin = {};
			_end = {};

			_mutate(); // <- initializes variables
		}

		// Initializes a range explicitly.
		inline range(iterator_t begin, iterator_t end) noexcept {


			_begin = begin;
			_end = end;

			_mutate(); // <- initializes variables
		}

		// Copy-initializes a range.
		inline range(const this_t& x) noexcept {


			TT_COPY(_begin, x);
			TT_COPY(_end, x);

			TT_COPY(_cached_distance, x);
			TT_COPY(_has_cached_distance, x);
		}

		// Move-initializes a range.
		inline range(this_t&& x) noexcept {


			TT_MOVE(_begin, x);
			TT_MOVE(_end, x);

			TT_MOVE(_cached_distance, x);
			TT_MOVE(_has_cached_distance, x);
		}

		inline ~range() noexcept = default;

		// Returns the beginning iterator of the range.
		constexpr const iterator_t& begin() const noexcept { return _begin; }

		// Returns the ending iterator of the range.
		constexpr const iterator_t& end() const noexcept { return _end; }

		// Returns the std::distance of the range.
		// Once gotten, the distance will be cached until the range is mutated.
		inline tt_size distance() const { return _distance(); }

		// Returns if the range is non-empty.
		// A non-empty range is defined by having its begin and end iterator not equal one another, as the end iterator is supposed to be past-the-end, and thus the begin iterator equalling it implies a distance of zero.
		inline tt_bool has_distance() const noexcept { return begin() != end(); }

		// Returns if the range is empty.
		// A non-empty range is defined by having its begin and end iterator not equal one another, as the end iterator is supposed to be past-the-end, and thus the begin iterator equalling it implies a distance of zero.
		inline tt_bool empty() const noexcept { return begin() == end(); }

		// Returns an iterator range who's begin iterator is this one's begin iterator incremented s times, and who's end iterator is this one's begin iterator incremented s + n times.
		// No bounds testing is performed as part of this process.
		inline this_t subrange(tt_size s, tt_size n) const {


			iterator_t ss = tt::advanced(begin(), s);

			return this_t(ss, tt::advanced(ss, n));
		}

		// Returns an iterator range who's begin iterator is this one's begin iterator incremented s times, and who's end iterator equals this one's end iterator.
		// No bounds testing is performed as part of this process.
		// It is the user's responsibility to ensure that the begin iterator of the range returned preceeds its end iterator.
		inline this_t subrange(tt_size s) const {


			return this_t(tt::advanced(begin(), s), end());
		}

		// Returns if this and the range x are equal.
		inline tt_bool equal(const this_t& x) const noexcept { return begin() == x.begin() && end() == x.end(); }

		inline tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		inline tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			if (begin() < rhs.begin())
				return true;

			else if (rhs.begin() < begin())
				return false;

			else if (end() < rhs.end())
				return true;

			else if (rhs.end() < end())
				return false;

			return false;
		}

		// Returns a string visualization of the range.
		inline tt_string visualize() const {


			return tt::visualize_iterable(*this);
		}

		// Returns a hash of the array.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(938362331, 1410087439, (tt_size)13764712087530567403, (tt_size)12981341254677683873);

			h.add(_begin);
			h.add(_end);

			return h.get();
		}

		// Assigns the begin and end iterator of the range.
		// This will cause the range to dump any cached distance information it might have.
		inline this_t& assign(iterator_t begin, iterator_t end) noexcept {


			_begin = begin;
			_end = end;

			_mutate();

			TT_RETURN_THIS;
		}

		// Copy-assigns the begin and end iterator of the range.
		inline this_t& assign(const this_t& x) noexcept {


			TT_COPY(_begin, x);
			TT_COPY(_end, x);

			TT_COPY(_cached_distance, x);
			TT_COPY(_has_cached_distance, x);

			TT_RETURN_THIS;
		}

		// Move-assigns the begin and end iterator of the range.
		inline this_t& assign(this_t&& x) noexcept {


			TT_MOVE(_begin, x);
			TT_MOVE(_end, x);

			TT_MOVE(_cached_distance, x);
			TT_MOVE(_has_cached_distance, x);

			x._mutate();

			TT_RETURN_THIS;
		}

		inline this_t& operator=(const this_t& rhs) noexcept { return assign(rhs); }
		inline this_t& operator=(this_t&& rhs) noexcept { return assign(TT_FMOVE(this_t, rhs)); }

		// Resets the state of the range's begin and end values to their default initialized states.
		// What these default initialized iterator states mean depends upon the iterator type.
		inline this_t& reset() noexcept {


			_begin = {};
			_end = {};

			_mutate();

			TT_RETURN_THIS;
		}
	};

	// Writing in the template parameters for iterator types is incredibly tedius, so we'll use this to imply them.

	// Returns a range as a function of the given begin and end iterators.
	// The type of the range's iterator may be implied by the iterators passed.
	template<typename Iterator>
	inline auto range_of(Iterator begin, Iterator end) noexcept {
		
		
		return tt::range<Iterator>(begin, end);
	}
}

TT_VISUALIZERS(0) {


	TT_REGISTER_VISUALIZE_T(tt::range, <typename Iterator>, <Iterator>);
}

TT_HASHERS(0) {


	TT_REGISTER_HASH_T(tt::range, <typename Iterator>, <Iterator>);
}

