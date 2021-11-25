

#pragma once


// The Tirous Toolbox memory-view array slice implementation.


#include <algorithm>

#include "aliases.h"
#include "macros.h"
#include "debug.h"

#include "memory.h"

#include "exceptions.h"
#include "forward_declarations.h"

#include "contiguous_iterator.h"

#include "range.h"

#include "visualize.h"

#include "hash.h"
#include "hash_factory.h"
#include "hash_functions.h"


namespace tt {


	// TODO: If we ever encounter template issues, perhaps consider re-doing the const-slice semantics of the below.

	// The Tirous Toolbox memory-view array slice implementation.
	template<typename Value>
	class slice final {
	public:

		using value_t = typename Value;

		using this_t = tt::slice<value_t>;

		// If the slice is a const-slice.
		// A const-slice is a slice of an array of const typed values.
		static constexpr tt_bool IS_CONST = std::is_const_v<value_t>;

		using pointer = value_t*;
		using reference = value_t&;

		using const_pointer = const value_t*;
		using const_reference = const value_t&;

		using iterator = tt::contiguous_iterator<value_t>;
		using const_iterator = tt::contiguous_iterator<const value_t>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;


	private:

		pointer _data;

		tt_size _size;


	public:

		// Default initializes an empty slice.
		inline slice() noexcept {


			_data = nullptr;
			_size = 0;
		}

		// Initializes a slice explicitly.
		// If either data is nullptr, or size is zero, the slice initialized will be an empty slice.
		inline slice(pointer data, tt_size size) noexcept {


			if ((data) && (size > 0))
				_data = data,
				_size = size;
			else
				_data = nullptr,
				_size = 0;
		}

		// Copy-initializes a slice.
		inline slice(const this_t& x) noexcept {


			TT_COPY(_data, x);
			TT_COPY(_size, x);
		}

		// Move-initializes a slice.
		inline slice(this_t&& x) noexcept {


			TT_MOVEPTR(_data, x);
			TT_MOVE(_size, x);
		}

		inline ~slice() noexcept = default;

		// Returns if the slice is a const-slice or not.
		// A const-slice is a slice of an array of const typed values.
		constexpr tt_bool is_const() const noexcept { return IS_CONST; }

		// Returns the underlying data of the slice, or nullptr if the slice is empty.
		constexpr pointer const data() noexcept { return _data; }

		// Returns the underlying data of the slice, or nullptr if the slice is empty.
		constexpr const_pointer const data() const noexcept { return _data; }

		// Returns if the given element exists within the address space of the container's contiguous storage slice.
		constexpr tt_bool is_associated(const_reference x) const noexcept { return (&x >= data()) && (&x <= (data() + size())); }

		// Returns the size of the slice.
		constexpr tt_size size() const noexcept { return _size; }

		// Returns if the slice is non-empty.
		constexpr tt_bool has_size() const noexcept { return size() > 0; }

		// Returns if the slice is empty.
		constexpr tt_bool empty() const noexcept { return !has_size(); }

		// Returns if the given index is within the bounds of the slice.
		constexpr tt_bool in_bounds(tt_size x) const noexcept { return x < size(); }

		// Returns a const-slice version of this slice.
		// A const-slice is a slice of an array of const typed values.
		inline tt::slice<const value_t> to_const() const noexcept { return tt::slice<const value_t>(data(), size()); }

		// Returns an lvalue of the element at the given index.
		// This performs no bounds checking, and thus the lvalue returned may be invalid.
		constexpr reference at_unchecked(tt_size x) noexcept { return data()[x]; }

		// Returns an lvalue of the element at the given index.
		// This performs no bounds checking, and thus the lvalue returned may be invalid.
		constexpr const_reference at_unchecked(tt_size x) const noexcept { return data()[x]; }

		// Returns an lvalue of the element at the given index.
		// Throws tt::out_of_range_error if index is out-of-bounds.
		// Provides a strong guarantee of exception-safety.
		inline reference at(tt_size x) {


			if (!in_bounds(x))
				TT_THROW(tt::out_of_range_error, "slice index out-of-range!");

			return at_unchecked(x);
		}

		// Returns an lvalue of the element at the given index.
		// Throws tt::out_of_range_error if index is out-of-bounds.
		// Provides a strong guarantee of exception-safety.
		inline const_reference at(tt_size x) const {


			if (!in_bounds(x))
				TT_THROW(tt::out_of_range_error, "slice index out-of-range!");

			return at_unchecked(x);
		}

		inline reference operator[](tt_size x) { return at(x); }
		inline const_reference operator[](tt_size x) const { return at(x); }

		// Returns an lvalue of the last element of the slice.
		// This performs no bounds checking, and thus the lvalue returned may be invalid.
		constexpr reference back_unchecked() noexcept { return at_unchecked(size() - 1); }

		// Returns an lvalue of the last element of the slice.
		// This performs no bounds checking, and thus the lvalue returned may be invalid.
		constexpr const_reference back_unchecked() const noexcept { return at_unchecked(size() - 1); }

		// Returns an lvalue of the last element of the slice.
		// Throws tt::out_of_range_error if the slice has no last element.
		// Provides a strong guarantee of exception-safety.
		inline reference back() {


			if (empty())
				TT_THROW(tt::out_of_range_error, "slice has no back element!");

			return back_unchecked();
		}

		// Returns an lvalue of the last element of the slice.
		// Throws tt::out_of_range_error if the slice has no last element.
		// Provides a strong guarantee of exception-safety.
		inline const_reference back() const {


			if (empty())
				TT_THROW(tt::out_of_range_error, "slice has no back element!");

			return back_unchecked();
		}

		// Returns an lvalue of the first element of the slice.
		// This performs no bounds checking, and thus the lvalue returned may be invalid.
		constexpr reference front_unchecked() noexcept { return at_unchecked(0); }

		// Returns an lvalue of the first element of the slice.
		// This performs no bounds checking, and thus the lvalue returned may be invalid.
		constexpr const_reference front_unchecked() const noexcept { return at_unchecked(0); }

		// Returns an lvalue of the first element of the slice.
		// Throws tt::out_of_range_error if the slice has no first element.
		// Provides a strong guarantee of exception-safety.
		inline reference front() {


			if (empty())
				TT_THROW(tt::out_of_range_error, "slice has no front element!");

			return front_unchecked();
		}

		// Returns an lvalue of the first element of the slice.
		// Throws tt::out_of_range_error if the slice has no first element.
		// Provides a strong guarantee of exception-safety.
		inline const_reference front() const {


			if (empty())
				TT_THROW(tt::out_of_range_error, "slice has no front element!");

			return front_unchecked();
		}

		// Compares if this and slice x are equal.
		inline tt_bool equal(const this_t& x) const noexcept { return size() == x.size() && tt::equal_arrays(data(), x.data(), size()); }

		inline tt_bool operator==(const this_t& x) const noexcept { return equal(x); }
		inline tt_bool operator!=(const this_t& x) const noexcept { return !equal(x); }

		// lexicographical comparison function

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			TT_FOR(i, size()) {


				if (at_unchecked(i) < rhs.at_unchecked(i))
					return true;

				if (rhs.at_unchecked(i) < at_unchecked(i))
					return false;
			}

			return false;
		}

		// Returns a slice of the first n elements starting with the element at index ind.
		// If ind is out-of-range, and empty slice is returned.
		// If n, starting at ind, would exceed the container's size, n will be reduced to fit, possibly reducing to zero.
		inline this_t subslice(tt_size ind, tt_size n) const noexcept {


			if (!in_bounds(ind))
				return this_t();

			if (size() - ind < n)
				n = size() - ind;

			if (n == 0)
				return this_t();

			return this_t(data() + ind, n);
		}

		// Returns a slice of the elements of the container at and following the element at index ind.
		// If ind is out-of-range, and empty slice is returned.
		inline this_t subslice(tt_size ind) const noexcept { return subslice(ind, size()); }

		// Returns an iterator to the first element of the slice.
		constexpr iterator begin() noexcept { return iterator(_data); }

		// Returns an iterator to the past-the-end element of the slice.
		constexpr iterator end() noexcept { return iterator(_data + size()); }

		// Returns an iterator to the first element of the slice.
		constexpr const_iterator cbegin() const noexcept { return const_iterator(_data); }

		// Returns an iterator to the past-the-end element of the slice.
		constexpr const_iterator cend() const noexcept { return const_iterator(_data) + size(); }

		// Returns an iterator to the first element of the slice.
		constexpr const_iterator begin() const noexcept { return const_iterator(_data); }

		// Returns an iterator to the past-the-end element of the slice.
		constexpr const_iterator end() const noexcept { return const_iterator(_data) + size(); }

		// Returns a reverse iterator to the reversed first element of the slice.
		constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(_data + size()); }

		// Returns a reverse iterator to the reversed past-the-end element of the slice.
		constexpr reverse_iterator rend() noexcept { return reverse_iterator(_data + size()) + size(); }

		// Returns a reverse iterator to the reversed first element of the slice.
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(_data + size()); }

		// Returns a reverse iterator to the reversed past-the-end element of the slice.
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(_data + size()) + size(); }

		// Returns a reverse iterator to the reversed first element of the slice.
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(_data + size()); }

		// Returns a reverse iterator to the reversed past-the-end element of the slice.
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(_data + size()) + size(); }

		// Searches for an element with the given value, returning a past-the-end iterator if no match was found.
		// The starting iterator of the search may be specified.
		// To search for a match following a previously found one, pass that match's iterator, incremented.
		inline iterator find(const_reference x, iterator start) noexcept { return std::find(start, end(), x); }

		// Searches for an element with the given value, returning a past-the-end iterator if no match was found.
		// The starting iterator of the search may be specified.
		// To search for a match following a previously found one, pass that match's iterator, incremented.
		inline const_iterator find(const_reference x, iterator start) const noexcept { return std::find(start, end(), x); }

		// Searches for an element with the given value, returning a past-the-end iterator if no match was found.
		inline iterator find(const_reference x) noexcept { return find(x, begin()); }

		// Searches for an element with the given value, returning a past-the-end iterator if no match was found.
		inline const_iterator find(const_reference x) const noexcept { return find(x, begin()); }

		// A version of find which operates according to an index rather than an iterator.
		// If an element cannot be found, the size of the container is returned.
		inline tt_size find_index(const_reference x, tt_size start = 0) const noexcept {


			for (; start < size(); ++start)
				if (at_unchecked(start) == x)
					return start;

			return size();
		}

		// Returns a string visualization of the slice.
		inline tt_string visualize() const {


			return tt::visualize_iterable(*this);
		}

		// Returns a hash of the slice.
		inline tt_size hash() const noexcept {


			return tt::array_hash_of(data(), size());
		}

		// Assigns the contents of the slice.
		// If either data is nullptr, or size is zero, the slice initialized will be an empty slice.
		inline this_t& assign(pointer data, tt_size size) noexcept {


			if ((data) && (size > 0))
				_data = data,
				_size = size;
			else
				_data = nullptr,
				_size = 0;

			TT_RETURN_THIS;
		}

		// Assigns the contents of the slice.
		inline this_t& assign(const this_t& x) noexcept {


			TT_COPY(_data, x);
			TT_COPY(_size, x);

			TT_RETURN_THIS;
		}

		// Assigns the contents of the slice.
		inline this_t& assign(this_t&& x) noexcept {


			TT_MOVEPTR(_data, x);
			TT_MOVE(_size, x);

			TT_RETURN_THIS;
		}

		inline this_t& operator=(const this_t& rhs) noexcept { return assign(rhs); }
		inline this_t& operator=(this_t&& rhs) noexcept { return assign(TT_FMOVE(this_t, rhs)); }

		// Resets the slice, unbinding it, and making it into an empty slice.
		inline this_t& reset() noexcept { return *this = {}; }
	};

	// Returns a slice of the given array of values.
	// The type of the slice's values may be implied by the pointer passed.
	template<typename Value>
	inline auto slice_of(Value* const slice_data, tt_size slice_size) noexcept {


		return tt::slice<Value>(slice_data, slice_size);
	}
}

TT_VISUALIZERS(0) {


	TT_REGISTER_VISUALIZE_T(tt::slice, <typename Value>, <Value>);
}

TT_HASHERS(0) {


	TT_REGISTER_HASH_T(tt::slice, <typename Value>, <Value>);
}

