

#pragma once


// A header file of utilities that allows for the encapsulation of the notion of deallocating allocated memory.

// Standardized deleters are important to big and complex systems which need to keep track of and safely dispose of 
// large amounts of dynamic memory, and which need to manually manage all of it.


#include "aliases.h"

#include "allocation.h"
#include "placement_construction.h"


namespace tt {


	// This function pointer represents the set of standardized deleter functions.
	
	// It is passed two parameters:
	//
	//		1. A void pointer to the allocated object which is to be deleted.
	//
	//		2. A tt::deleter_info containing a little bit of optional context information to aid in complex systems.
	
	// Our tt::deleter_info is a union types encapsulating a tt_size, tt_ssize, or another void pointer.
	
	// This should be enough to allow for complex systems to communicate additional contextual information needed 
	// for safe deletion in said more complex environments, be it a garbage collector, or simple placement destruction.

	// A trivial data union intended to pass additional, optional, context information to a deleter function.
	union deleter_info final {

		// The deleter info as a tt_size value.
		tt_size size_value;

		// The deleter info as a tt_ssize value.
		tt_ssize ssize_value;

		// The deleter info as a void pointer value.
		void* ptr_value;
	};

	typedef void(*deleter)(void*, tt::deleter_info);

	// Invokes the given deleter upon the given target object.
	// If the target or deleter is nullptr, the function fails quietly.
	template<typename Value> 
	inline void invoke_deleter(Value* x, deleter f, tt::deleter_info i = {}) {


		if ((f) && (x))
			f((void*)x, i);
	}


	// These are the standard deleters associated with various standard Tirous Toolbox and C++ standard 
	// library deallocation methods.

	// The deleter associated with the C++ new-operator (which invokes the delete-operator.)
	// The deleter info is unused.
	template<typename Value, tt_bool GlobalDelete = true> 
	inline void cpp_new_deleter(void* x, tt::deleter_info) {


		if constexpr (GlobalDelete)
			:: delete (Value*)x;
		else
			delete (Value*)x;
	}

	// The deleter associated with the C++ new[]-operator (which invokes the delete[]-operator.)
	// The deleter info is unused.
	template<typename Value, tt_bool GlobalDelete = true> 
	inline void cpp_new_array_deleter(void* x, tt::deleter_info) {


		if constexpr (GlobalDelete)
			:: delete[]((Value*)x);
		else
			delete[]((Value*)x);
	}

	// The deleter associated with std::malloc and related functions (which invokes std::free.)
	// The deleter info is unused.
	inline void cpp_malloc_deleter(void* x, tt::deleter_info) {


		std::free(x);
	}

	// The deleter associated with the C++ placement-new operator (which manually invokes the destructor.)
	// The deleter info is unused.
	template<typename Value> 
	inline void cpp_placement_new_deleter(void* x, tt::deleter_info) noexcept {


		((Value*)x)->~Value();
	}

	// The deleter associated with tt::alloc.
	// The deleter info is unused.
	template<typename Value> 
	inline void alloc_deleter(void* x, tt::deleter_info) {


		tt::dealloc((Value*)x);
	}

	// The deleter associated with tt::alloc_n.
	// The deleter info is unused.
	template<typename Value> 
	inline void alloc_n_deleter(void* x, tt::deleter_info) {


		tt::dealloc_n((Value*)x);
	}

	// The deleter associated with tt::alloc_uninit.
	// The deleter info is unused.
	template<typename Value> 
	inline void alloc_uninit_deleter(void* x, tt::deleter_info) {


		tt::dealloc_uninit((Value*)x);
	}

	// The deleter associated with tt::aligned_alloc_uninit.
	// The deleter info is unused.
	template<typename Value> 
	inline void aligned_alloc_uninit_deleter(void* x, tt::deleter_info) {


		tt::aligned_dealloc_uninit((Value*)x);
	}

	// The deleter associated with tt::construct_at.
	// The deleter info is unused.
	template<typename Value> 
	inline void construct_deleter(void* x, tt::deleter_info) {


		tt::destroy_at((Value*)x);
	}

	// The deleter associated with tt::construct_n_at.
	// The deleter info is interpreted as a tt_size and dictates the number of elements to placement-destroy.
	template<typename Value> 
	inline void construct_n_deleter(void* x, tt::deleter_info info) {


		tt::destroy_n_at((Value*)x, info.size_value);
	}


	// A pre-packaged deletion invocation which may be invoked when an object's lifetime is to be brought manually to an end.
	// The object, deleter, and deleter info information needed for the deletion, are all maintained.
	// If either the deleter function or target pointers are nullptr, the packaged deletion is still valid, but will be 'inert.'
	struct packaged_deletion final {

		using this_t = packaged_deletion;

		// The deleter of the packaged deletion.
		// If this is nullptr, the packaged deletion is inert.
		tt::deleter deleter;

		// The target object of the packaged deletion.
		// If this is nullptr, the packaged deletion is inert.
		void* target;

		// The deleter info to be passed to the packaged deletion's deleter.
		tt::deleter_info info;

		// Default initializes a packaged deletion.
		inline packaged_deletion() noexcept {


			deleter = nullptr;
			target = nullptr;
			info = {};
		}

		// Per-element initializes a packaged deletion.
		template<typename U> 
		inline packaged_deletion(tt::deleter d, U* t, tt::deleter_info i = {}) {


			deleter = d;
			target = (void*)t;
			info = i;
		}

		// Copy-initializes a packaged deletion.
		inline packaged_deletion(const this_t& x) noexcept {


			TT_COPY(deleter, x);
			TT_COPY(target, x);
			TT_COPY(info, x);
		}

		// Move-initializes a packaged deletion.
		inline packaged_deletion(this_t&& x) noexcept {


			TT_MOVEPTR(deleter, x);
			TT_MOVEPTR(target, x);
			TT_MOVE(info, x);
		}

		inline ~packaged_deletion() noexcept {}

		inline this_t& operator=(const this_t& rhs) noexcept {


			TT_COPY(deleter, rhs);
			TT_COPY(target, rhs);
			TT_COPY(info, rhs);

			TT_RETURN_THIS;
		}

		inline this_t& operator=(this_t&& rhs) noexcept {


			TT_SELF_MOVE_TEST(rhs);

			TT_MOVEPTR(deleter, rhs);
			TT_MOVEPTR(target, rhs);
			TT_MOVE(info, rhs);

			TT_RETURN_THIS;
		}

		// Returns if the packaged deletion is inert and thus does not represent a deleter invocation.
		// A packaged deletion is inert if either its deleter or its target is nullptr.
		constexpr tt_bool is_inert() const noexcept {


			return !((deleter) && (target));
		}

		// Returns the target of the deleter cast to a pointer to the given type.
		template<typename U> 
		constexpr U* target_as() const noexcept {


			return (U*)target;
		}

		// Invokes the packaged deletion, deleting (by whatever method) the target object via the packaged deleter and deleter info.
		// Whether the deletion executes properly or not is beyond the scope of the packaged deletion.
		// If inert, no attempted deletion will take place.
		// Once invoked, a given packaged deletion should not be used again until newly assigned.
		inline void invoke() const {


			// Our tt::invoke_deleter will fail quietly if the deleter is inert, thus no further checks are needed.

			tt::invoke_deleter(target, deleter, info);
		}

		// Returns if this and packaged deletion x are equal.
		constexpr tt_bool equal(const this_t& x) const noexcept {


			return target == x.target && deleter == x.deleter && info.size_value == x.info.size_value;
		}

		constexpr tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		constexpr tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// Retargets the packaged deletion.
		template<typename U> 
		inline void retarget(U* new_target) noexcept {


			target = (void*)new_target;
		}

		// Respecifies the deleter invoked by the packaged deletion, and optionally its deleter info.
		inline void respecify(tt::deleter new_deleter) noexcept {


			deleter = new_deleter;
		}

		// Respecifies the deleter invoked by the packaged deletion, and optionally its deleter info.
		inline void respecify(tt::deleter new_deleter, tt::deleter_info new_info) noexcept {


			deleter = new_deleter;
			info = new_info;
		}

		// Returns a null packaged deletion which is inert.
		static inline packaged_deletion null() noexcept { return tt::packaged_deletion(); }

		// Prepares a new packaged deletion.
		template<typename U> 
		static packaged_deletion make(tt::deleter d, U* t, tt::deleter_info i = {}) noexcept {


			return packaged_deletion(d, t, i);
		}
	};
}

