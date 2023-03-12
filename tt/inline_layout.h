

#pragma once


// A static class used to describe the layout of blocks of memory.

// These layouts have two portions: a 'header', and a 'body'.

// The header is a simple struct used to provide information in the layout which needs only a single
// entry to encapsulate it fully.

// The body is an array of elements of a given type who's length is determined using runtime information.


#include "aliases.h"
#include "math_util.h"


namespace tt {


	template<typename Header, typename BodyItem>
	class inline_layout final {
	public:

		using header_t		= typename Header;
		using body_item_t	= typename BodyItem;


		inline_layout() = delete;
		~inline_layout() noexcept = delete;


		static_assert(alignof(header_t) <= alignof(body_item_t));


		static constexpr auto ALIGNMENT = alignof(body_item_t);

		struct unit final {

			alignas(ALIGNMENT) tt_byte _dummy[ALIGNMENT] = {};
		};


		// Returns the number of bytes per unit.
		static constexpr tt_size get_bytes_per_unit() noexcept {
			return sizeof(unit);
		}


		// Returns the minimum number of aligned units required to contain a header.
		static constexpr tt_size get_units_per_header() noexcept {
			return tt::aligned_count<tt_size>(sizeof(header_t), ALIGNMENT);
		}

		// Returns the minimum number of aligned units required to contain a body item.
		static constexpr tt_size get_units_per_body_item() noexcept {
			return tt::aligned_count<tt_size>(sizeof(body_item_t), ALIGNMENT);
		}

		// Returns the minimum number of aligned units required to contain a body of n items.
		static constexpr tt_size get_units_per_body(tt_size n) noexcept {
			return get_units_per_body_item() * n;
		}

		// Returns the minimum number of aligned units required to contain a header and body of n items.
		static constexpr tt_size get_units_total(tt_size n) noexcept {
			return get_units_per_header() + get_units_per_body(n);
		}

		// Returns the number of aligned units required to reach a body item at index ind in the array of a layout of n items.
		static constexpr tt_size get_units_per_body_item_offset(tt_size ind) noexcept {
			return get_units_per_header() + get_units_per_body_item() * ind;
		}


		// Returns the minimum number of bytes required to contain a header.
		static constexpr tt_size get_bytes_per_header() noexcept {
			return get_units_per_header() * get_bytes_per_unit();
		}

		// Returns the minimum number of bytes required to contain a body item.
		static constexpr tt_size get_bytes_per_body_item() noexcept {
			return get_units_per_body_item() * get_bytes_per_unit();
		}

		// Returns the minimum number of bytes required to contain a body of n items.
		static constexpr tt_size get_bytes_per_body(tt_size items) noexcept {
			return get_units_per_body(items) * get_bytes_per_unit();
		}

		// Returns the minimum number of bytes required to contain a header and body of n items.
		static constexpr tt_size get_bytes_total(tt_size items) noexcept {
			return get_units_total(items) * get_bytes_per_unit();
		}

		// Returns the number of bytes required to reach a body item at index ind in the array of a layout of n items.
		static constexpr tt_size get_bytes_per_body_item_offset(tt_size ind) noexcept {
			return get_bytes_per_header() + get_bytes_per_body_item() * ind;
		}


		// Returns an lvalue-reference of the header of ptr.
		static inline header_t& deref_header(unit* ptr) noexcept {
			return *(header_t*)ptr;
		}

		// Returns a const lvalue-reference of the header of ptr.
		static inline const header_t& deref_header(const unit* ptr) noexcept {
			return *(const header_t*)ptr;
		}

		// Returns an lvalue-reference of the body item of ptr at ind.
		// Behaviour is undefined if ind is out-of-bounds.
		static inline body_item_t& deref_body_item(unit* ptr, tt_size ind) noexcept {
			return *(body_item_t*)(ptr + get_units_per_body_item_offset(ind));
		}

		// Returns a const lvalue-reference of the body item of ptr at ind.
		// Behaviour is undefined if ind is out-of-bounds.
		static inline const body_item_t& deref_body_item(const unit* ptr, tt_size ind) noexcept {
			return *(const body_item_t*)(ptr + get_units_per_body_item_offset(ind));
		}
	};
}

