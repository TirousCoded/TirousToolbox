

#pragma once


// A general purpose iterator used to iterate across contiguous arrays of elements.


#include <iterator>

#include "aliases.h"
#include "macros.h"

#include "hash.h"
#include "hash_factory.h"
#include "hash_functions.h"


namespace tt {


	// A general purpose iterator used to iterate across contiguous arrays of elements.
	template<typename Value>
	class contiguous_iterator final {
	public:

		using value_t = typename Value;

		// If the iterator is a 'const-iterator' or not.
		static constexpr tt_bool IS_CONST = std::is_const_v<value_t>;

		using this_t = tt::contiguous_iterator<value_t>;

		using difference_type = std::ptrdiff_t;
		using value_type = value_t;
		using pointer = value_t*;
		using reference = value_t&;

		using iterator_category = std::random_access_iterator_tag;


	private:

		pointer _value;


	public:

		// Default initializes a nullptr contiguous iterator.
		// Contiguous iterators which are nullptr are not treated as past-the-end iterators.
		inline contiguous_iterator() noexcept { _value = nullptr; }

		// Explicitly initializes a nullptr contiguous iterator.
		inline contiguous_iterator(pointer x) noexcept { _value = x; }

		// Copy-initializes a contiguous iterator.
		inline contiguous_iterator(const this_t& x) noexcept { TT_COPY(_value, x); }

		// Move-initializes a contiguous iterator.
		inline contiguous_iterator(this_t&& x) noexcept { TT_MOVEPTR(_value, x); }

		inline ~contiguous_iterator() noexcept {};

		inline this_t& operator=(const this_t& rhs) noexcept { TT_COPY(_value, rhs); TT_RETURN_THIS; }
		inline this_t& operator=(this_t&& rhs) noexcept { TT_MOVEPTR(_value, rhs); TT_RETURN_THIS; }

		// Returns the pointer associated with the iterator.
		constexpr pointer get() const noexcept { return _value; }

		// Returns if the iterator is a const-iterator or not.
		// A const-iterator is one which iterates over values of a const type.
		constexpr tt_bool is_const() const noexcept { return IS_CONST; }

		// Not gonna make a cast to a const-iterator since normally iterators can't make such an assumption, and thus we
		// won't potentially fall into the trap of going about doing so.

		// Returns the const-iterator equivalent of this iterator.
		inline tt::contiguous_iterator<value_t> to_const() const noexcept {


			return tt::contiguous_iterator<std::add_const_t<value_t>>(get());
		}

		// Returns if this and the given contiguous iterator are equal.
		constexpr tt_bool equal(const this_t& other) const noexcept { return get() == other.get(); }

		constexpr tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		constexpr tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		constexpr tt_bool operator<(const this_t& rhs) const noexcept { return get() < rhs.get(); }
		constexpr tt_bool operator<=(const this_t& rhs) const noexcept { return get() <= rhs.get(); }
		constexpr tt_bool operator>(const this_t& rhs) const noexcept { return get() > rhs.get(); }
		constexpr tt_bool operator>=(const this_t& rhs) const noexcept { return get() >= rhs.get(); }

		// Returns a hash of the contiguous iterator.
		inline tt_size hash() const noexcept {


			return tt::hash_of(_value);
		}
		
		constexpr pointer operator->() const { return get(); }
		constexpr reference operator*() const { return *get(); }

		constexpr difference_type operator-(const this_t& x) const { return get() - x.get(); }

		inline this_t operator+(difference_type x) const { return this_t(get() + x); }
		inline this_t operator-(difference_type x) const { return this_t(get() - x); }

		constexpr reference operator[](tt_size index) const { return *(*this + index); }

		inline this_t& operator++() { ++_value; TT_RETURN_THIS; }
		inline this_t operator++(int) { auto t = *this; ++* this; return t; }

		inline this_t& operator--() { --_value; TT_RETURN_THIS; }
		inline this_t operator--(int) { auto t = *this; --* this; return t; }

		inline this_t& operator+=(difference_type x) { _value += x; TT_RETURN_THIS; }
		inline this_t& operator-=(difference_type x) { _value -= x; TT_RETURN_THIS; }
	};

	template<typename Value>
	contiguous_iterator<Value> operator+(typename contiguous_iterator<Value>::difference_type x, const contiguous_iterator<Value>& it) {
		
		
		return it + x;
	}
}

TT_HASHERS(0) {


	TT_REGISTER_HASH_T(tt::contiguous_iterator, <typename Value>, <Value>);
}

