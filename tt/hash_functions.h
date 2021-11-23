

#pragma once


// A header file of general purpose hashing functions.


#include <functional>

#include "aliases.h"

#include "memory_util.h"

#include "hash_factory.h"


namespace tt {


	// Returns the hash code of x via std::hash.
	// The Tirous Toolbox library does not guarantee that hash codes will be (reasonably) unique unless two objects are of the same type.
	template<typename Value>
	inline tt_size hash_of(const Value& x) noexcept {


		return std::hash<Value>()(x);
	}

	// Returns the hash code of the given array.
	// The Tirous Toolbox library does not guarantee that hash codes will be (reasonably) unique unless two objects are of the same type.
	template<typename Value>
	inline tt_size array_hash_of(const Value* const x, tt_size n) noexcept {


		tt::hash_factory h(1880177309, 3725419109, (tt_size)3342954644411632897, (tt_size)2453334075520421939);

		h.add(n);

		TT_FOR(i, n)
			h.add(x[i]);

		return h.get();
	}
}

