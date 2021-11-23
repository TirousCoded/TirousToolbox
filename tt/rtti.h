

#pragma once


// The Tirous Toolbox Runtime Type Information (RTTI) class implementation.


#include <typeinfo>

#include "aliases.h"

#include "visualize.h"

#include "hash.h"
#include "hash_functions.h"


namespace tt {


	// The Tirous Toolbox Runtime Type Information (RTTI) class implementation.
	struct rtti final {

		using this_t = rtti;


		// The MSVC std::type_index stores this as a pointer, and I assume that this is due to std::type_info being statically 
		// stored by the application in memory, thus making its pointer reliable. Regardless, we'll do the same.

		// Decided to be lazy and make this public so we can aggregate initialize, and thus imply rule-of-five.

		const std::type_info* _state = nullptr;


		// Returns if the RTTI object represents no object.
		constexpr bool is_null() const noexcept { return !(_state); }

		// Returns the C-string name associated with the RTTI object's type.
		// If the RTTI object is null, nullptr is returned.
		inline const tt_char* const name() const noexcept {


			if (is_null())
				return nullptr;

			return _state->name();
		}

		// Returns if this and the given RTTI object are equal.
		constexpr tt_bool equal(const this_t& other) const noexcept { return _state == other._state; }

		constexpr tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		constexpr tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// We'll consider all unbound RTTI objects to be 'less-than' all bound ones.

		// lexicographical comparison function

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			if (is_null())
				return !rhs.is_null();

			else if (rhs.is_null())
				return false;

			return _state->before(*(rhs._state));
		}

		// Returns a string visualization of the RTTI.
		inline tt_string visualize() const {


			return tt_string(name());
		}

		// Returns a hash of the RTTI.
		inline tt_size hash() const noexcept {


			// The actual pointer of the RTTI's underlying state is not consistent between compilations, thus we'll hash the
			// string name of the represented type instead since that should be reasonably consistent, even if it'll differ
			// between compilers.

			return tt::array_hash_of(name(), tt::measure_cstr(name()));
		}

		// Returns a null RTTI.
		static inline this_t null() noexcept { return {}; }
	};

	// Returns a RTTI object of type Value.
	template<typename Value>
	inline tt::rtti rtti_of() noexcept {


		return { &(typeid(Value)) };
	}

	// Returns a RTTI object of type Value.
	template<typename Value>
	inline tt::rtti rtti_of(const Value&) noexcept {


		return rtti_of<Value>();
	}
}

TT_VISUALIZERS(0) {


	TT_REGISTER_VISUALIZE(tt::rtti);
}

TT_HASHERS(0) {


	TT_REGISTER_HASH(tt::rtti);
}

