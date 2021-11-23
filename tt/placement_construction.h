

#pragma once


// A header file defining the placement construction functions of the Tirous Toolbox library.


#include <type_traits>

#include "aliases.h"
#include "macros.h"
#include "debug.h"


namespace tt {


	// Should be able to make these noexcept since the destructor is supposed to be noexcept.

	// Placement-destroys the object of the given type at the given address, using the given parameters.
	// The function quietly fails if nullptr is passed.
	template<typename Value>
	inline void destroy_at(Value* const x) noexcept {


		if constexpr (!std::is_trivially_destructible_v<Value>)
			if (x)
				x->~Value();
	}

	// A version of tt::destroy_at which destructs a contiguous array of objects sequentially.
	// The function quietly fails if nullptr is passed.
	// The elements are destroyed sequentially, rather than in reverse order.
	template<typename Value>
	inline void destroy_n_at(Value* const x, tt_size n) noexcept {


		// Again, avoid unneeded branching.

		if constexpr (!std::is_trivially_destructible_v<Value>)
			if (x)
				TT_FOR(i, n)
				(x + i)->~Value();
	}

	// Placement-constructs the given type at the given address, using the given parameters.
	// The function quietly fails if nullptr is passed.
	// The pointer passed is returned.
	template<typename Value, typename... Args> 
	inline Value* construct_at(Value* const x, Args&&... args) {


		if (x)
			:: new (x) Value(TT_FMOVE_N(Args, args));

		return x;
	}

	// A version of tt::construct_at which constructs a contiguous array of objects sequentially in x.
	// The first object will be constructed via args, any additional objects will be copy constructed using this first object.
	// The function quietly fails if nullptr is passed, or n is zero.
	// The pointer passed is returned.
	// If any placement construction initializations throw, any initialized successfully will be placement-destroyed via tt::destroy_n_at prior to rethrowing the exception, thus providing proper exception-safety.
	template<typename Value, typename... Args>
	inline Value* construct_n_at_args(Value* const x, tt_size n, Args&&... args) {


		// Avoid the needless branch-test in tt::construct_at by doing things manually.

		if (!(x))
			return x;

		tt_size _number_initialized = 0;

		try {


			if (n > 0)
				:: new (x) Value(TT_FMOVE_N(Args, args)),
				++_number_initialized;

			if (n > 1)
				for (tt_size i = 1; i < n; ++i)
					:: new (x + i) Value(*x),
					++_number_initialized;
		}
		catch (...) {


			tt::destroy_n_at(x, _number_initialized);

			TT_RETHROW;
		}

		return x;
	}

	// A version of tt::construct_at which constructs a contiguous array of objects sequentially in x.
	// The objects are default constructed.
	// The function quietly fails if nullptr is passed, or n is zero.
	// The pointer passed is returned.
	// Use tt::construct_n_at_args to pass arguments to these object constructor calls.
	// If any placement construction initializations throw, any initialized successfully will be placement-destroyed via tt::destroy_n_at prior to rethrowing the exception, thus providing proper exception-safety.
	template<typename Value>
	inline Value* construct_n_at(Value* const x, tt_size n) {


		// Avoid the needless branch-test in tt::construct_at by doing things manually.

		tt_size _number_initialized = 0;

		try {


			if (x)
				TT_FOR(i, n)
					:: new (x + i) Value(),
					++_number_initialized;
		}
		catch (...) {


			tt::destroy_n_at(x, _number_initialized);

			TT_RETHROW;
		}

		return x;
	}

	// A version of tt::construct_at which constructs a contiguous array of objects sequentially in x.
	// The objects are copy-constructed using v.
	// The function quietly fails if nullptr is passed, or n is zero.
	// The pointer passed is returned.
	// Use tt::construct_n_at_args to pass arguments to these object constructor calls.
	// If any placement construction initializations throw, any initialized successfully will be placement-destroyed via tt::destroy_n_at prior to rethrowing the exception, thus providing proper exception-safety.
	template<typename Value>
	inline Value* construct_n_at(Value* const x, tt_size n, const Value& v) {


		// Avoid the needless branch-test in tt::construct_at by doing things manually.

		tt_size _number_initialized = 0;

		try {


			if (x)
				TT_FOR(i, n)
					:: new (x + i) Value(v),
					++_number_initialized;
		}
		catch (...) {


			tt::destroy_n_at(x, _number_initialized);

			TT_RETHROW;
		}

		return x;
	}

	// A version of tt::construct_n_at which constructs a contiguous array of objects sequentially in x, such that each element is copy-constructed from elements in y, with x and y both being of size n.
	// The function quietly fails if x or y are nullptr, or n is zero.
	// The pointer x is returned.
	// If type Value is trivially-copyable, an std::memmove will be used to perform the *initialization*, optimizing the function.
	// If any placement construction initializations throw, any initialized successfully will be placement-destroyed via tt::destroy_n_at prior to rethrowing the exception, thus providing proper exception-safety.
	template<typename Value>
	inline Value* copy_construct_n_at(Value* const x, const Value* const y, tt_size n) {


		if (!((x) && (y) && (n > 0)))
			return x;

		else if constexpr (std::is_trivially_copyable_v<Value>)
			std::memmove((void*)x, (const void*)y, n * sizeof(Value));

		else {


			tt_size _number_initialized = 0;

			try {


				TT_FOR(i, n)
					:: new (x + i) Value(*(y + i)),
					++_number_initialized;
			}
			catch (...) {


				tt::destroy_n_at(x, _number_initialized);

				TT_RETHROW;
			}
		}

		return x;
	}

	// A version of tt::construct_n_at which constructs a contiguous array of objects sequentially in x, such that each element is move-constructed from elements in y, with x and y both being of size n.
	// The function quietly fails if x or y are nullptr, or n is zero.
	// The pointer x is returned.
	// If type Value is trivially-copyable, an std::memmove will be used to perform the *initialization*, optimizing the function.
	template<typename Value>
	inline Value* move_construct_n_at(Value* const x, Value* const y, tt_size n) noexcept {


		if (!((x) && (y) && (n > 0)))
			return x;
		
		else if constexpr (std::is_trivially_copyable_v<Value>)
			std::memmove((void*)x, (const void*)y, n * sizeof(Value));

		else {


			// Unlike the above, we can presume that move-construction *shouldn't ever* throw.

			TT_FOR(i, n)
				:: new (x + i) Value(std::move(*(y + i)));
		}

		return x;
	}
}

