

#pragma once


// A header file defining the low-level memory allocation functions of the Tirous Toolbox library.


#include "aliases.h"
#include "compiler_detect.h"
//#include "config.h"
#include "macros.h"


namespace tt {


	// Allocates an instance of the given type with the given parameters (if any) passed to its constructor.
	// Allocations made using this should be released using tt::dealloc.
	template<typename Value, typename... Args> 
	inline Value* alloc(Args&&... args) {


		return new Value(TT_FMOVE_N(Args, args));
	}

	// Deallocates instances of the given type allocated using tt::alloc.
	// If nullptr is passed, the function fails quietly.
	template<typename Value> 
	inline void dealloc(Value* x) {


		if (x)
			delete x;
	}

	// Allocates an array of the given type with the given parameters (if any) passed to their constructors.
	// If the n passed is zero nullptr will be returned.
	// Allocations made using this should be released using tt::dealloc_n.
	template<typename Value, typename... Args> 
	inline Value* alloc_n(tt_size n, Args&&... args) {


		if (n == 0)
			return nullptr;

		return new Value[n](TT_FMOVE_N(Args, args));
	}

	// Deallocates arrays of the given type allocated using tt::alloc_n.
	// If nullptr is passed, the function fails quietly.
	template<typename Value> 
	inline void dealloc_n(Value* x) {


		if (x)
			delete[] x;
	}

	// Returns an uninitialized memory big enough to store n values of type Value.
	// If n is zero nullptr will be returned.
	// Allocations made using this should be released using tt::dealloc_uninit.
	template<typename Value> 
	inline Value* alloc_uninit(tt_size n = 1) {


		if (n == 0)
			return nullptr;

		return (Value*)std::malloc(n * sizeof(Value));
	}

	// Deallocates arrays of uninitialized memory allocated using tt::alloc_uninit.
	// If x is nullptr, the function fails quietly.
	// The pointer type passed need not be of the same type as the one originally allocated.
	template<typename Value> 
	inline void dealloc_uninit(Value* x) {


		if (x)
			std::free((void*)x);
	}

	// A version of tt::alloc_uninit which allocates an block of memory who's address adheres to the given alignment.
	// This function wraps std::aligned_alloc by default, or _aligned_malloc on MSVC.
	// To this end, the alignment and size should follow the rules regarding whichever it's wrapping.
	// If either n or alignment is zero, nullptr is returned. Beyond that, std::aligned_alloc/_aligned_malloc rules should be followed.
	// Allocations made using this should be released using tt::aligned_dealloc_uninit.
	template<typename Value>
	inline Value* aligned_alloc_uninit(tt_size n, tt_size alignment) {


		if (n == 0)
			return nullptr;

		else if (alignment == 0)
			return nullptr;

#if defined(TT_COMPILER_IS_MSVC)
		return (Value*)_aligned_malloc(n * sizeof(Value), alignment);
#else
		return (Value*)std::aligned_alloc(alignment, n * sizeof(Value));
#endif
	}

	// Deallocates arrays of uninitialized memory allocated using tt::aligned_alloc_uninit.
	// If nullptr is passed, the function fails quietly.
	// The pointer type passed need not be of the same type as the one originally allocated.
	template<typename Value>
	inline void aligned_dealloc_uninit(Value* x) {


		if (!(x))
			return;

#if defined(TT_COMPILER_IS_MSVC)
		_aligned_free((void*)x);
#else
		std::free((void*)x);
#endif
	}
}

