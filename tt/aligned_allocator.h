

#pragma once


// The Tirous Toolbox library's custom aligned-address memory allocator implementation.


#include <type_traits>

#include "aliases.h"
#include "macros.h"
#include "debug.h"

#include "allocation.h"
#include "placement_construction.h"


namespace tt {


	// The Tirous Toolbox library's custom aligned-address memory allocator implementation.
	template<typename Value, tt_size Alignment = alignof(Value)> 
	struct aligned_allocator final {

		using value_t = typename Value;

		// The alignment of the allocator.
		static constexpr tt_size ALIGNMENT = Alignment;

		// defining this for rebind

		template<typename U>
		using aligned_allocator_t = tt::aligned_allocator<U, ALIGNMENT>;

		using this_t = aligned_allocator_t<value_t>;

		using pointer = value_t*;
		using const_pointer = const value_t*;
		using void_pointer = void*;
		using const_void_pointer = const void*;

		using value_type = value_t;

		using size_type = tt_size;
		using difference_type = std::ptrdiff_t;

		using is_always_equal = std::true_type;

		using propagate_on_container_copy_assignment = std::false_type;
		using propagate_on_container_move_assignment = std::true_type;
		using propagate_on_container_swap = std::false_type;

		template<typename U> 
		struct rebind final { using other = aligned_allocator_t<U>; };

		// If two instances of this allocator type will be definition equal to one another when compared.
		static constexpr tt_bool IS_ALWAYS_EQUAL = std::is_same_v<is_always_equal, std::true_type>;

		// If container copy-assignment should propagate the state of the allocator.
		static constexpr tt_bool PROPAGATE_ON_CONTAINER_COPY_ASSIGNMENT = std::is_same_v<propagate_on_container_copy_assignment, std::true_type>;

		// If container move-assignment should propagate the state of the allocator.
		static constexpr tt_bool PROPAGATE_ON_CONTAINER_MOVE_ASSIGNMENT = std::is_same_v<propagate_on_container_move_assignment, std::true_type>;

		// If container swapping should propagate the state of the allocator.
		static constexpr tt_bool PROPAGATE_ON_CONTAINER_SWAP = std::is_same_v<propagate_on_container_swap, std::true_type>;

		// The maximum array size which may be allocated by the allocator.
		static constexpr size_type MAX_SIZE = tt::max_of<size_type>;

		// Default initializes an aligned allocator.
		inline aligned_allocator() noexcept = default;

		// Initializes an allocator of a different underlying type.
		template<typename Other>
		inline aligned_allocator(const aligned_allocator_t<Other>&) noexcept {}

		// Copy-initializes an aligned allocator.
		inline aligned_allocator(const this_t&) noexcept {}

		// Move-initializes an aligned allocator.
		inline aligned_allocator(this_t&&) noexcept {}
		
		inline ~aligned_allocator() noexcept = default;

		inline this_t& operator=(const this_t&) noexcept { TT_RETURN_THIS; }
		inline this_t& operator=(this_t&&) noexcept { TT_RETURN_THIS; }

		// Returns the maximum array size allocatable by this allocator.
		constexpr size_type max_size() const noexcept { return MAX_SIZE; }

		// Returns a version of this allocator that should be used when copy-constructing the allocator's owner.
		constexpr this_t select_on_container_copy_construction() const noexcept { return this_t(*this); }

		// Returns if this and the given aligned allocator are equal.
		constexpr tt_bool equal(const this_t&) const noexcept { return true; }

		inline tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		inline tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// Invokes the allocator to allocator a block of n values, returning a pointer to this block.
		// This wraps tt::aligned_alloc_uninit and may throw accordingly.
		inline pointer allocate(size_type n) {


			return tt::aligned_alloc_uninit<value_type>(n, ALIGNMENT);
		}

		// Invokes the allocator to deallocate a block at address p of n values previous allocated by the allocator.
		// This wraps tt::aligned_dealloc_uninit and will not throw.
		inline void deallocate(pointer p, size_type n) noexcept {


			if (p)
				tt::aligned_dealloc_uninit<value_type>(p);
		}

		// Invokes the allocator to placement-construct an object of type U at address p, passing args to its constructor.
		// This wraps tt::construct_at and may throw accordingly.
		template<typename U, typename... Args> 
		inline void construct(U* p, Args&&... args) {


			tt::construct_at(p, TT_FMOVE_N(Args, args));
		}

		// Invokes the allocator to placement-destroy an object of type U at address p.
		// This wraps tt::destroy_at and will not throw.
		template<typename U> 
		inline void destroy(U* p) noexcept {


			tt::destroy_at(p);
		}
	};
}

