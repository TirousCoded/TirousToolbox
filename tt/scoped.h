

#pragma once


// A scoped pointer class used to provide simple yet powerful RAII based memory ownership semantics.


#include "aliases.h"
#include "macros.h"
#include "debug.h"

#include "exceptions.h"

#include "allocation.h"
#include "deletion.h"

#include "hash.h"
#include "hash_factory.h"


namespace tt {


	// A scoped pointer class which takes ownership of an allocated resource and releases it upon the destruction of the object, or when it is assigned a new value.
	template<typename Value>
	class scoped final {
	public:

		using value_t = typename Value;

		using this_t = tt::scoped<value_t>;


	private:

		value_t* _value;

		tt::packaged_deletion _pd;


	public:

		// Default initializes a null scoped pointer.
		inline scoped() noexcept {


			_value = nullptr;
			_pd = tt::packaged_deletion::null();
		}

		// Initializes a scoped pointer encapsulating RAII ownership of array x, to be deleted via pre-packaged packaged deletion pd.
		// It is the user's responsibility to ensure that pd actually encapsulates the deletion of allocated resource x.
		inline scoped(value_t* const x, tt::packaged_deletion pd) noexcept {


			_value = x;
			_pd = std::move(pd);
		}

		inline scoped(const this_t&) noexcept = delete;

		inline scoped(this_t&& x) noexcept {


			TT_MOVEPTR(_value, x);
			TT_MOVE(_pd, x);
		}

		inline ~scoped() noexcept {


			_pd.invoke();
		}

		inline this_t& operator=(const this_t&) = delete;

		inline this_t& operator=(this_t&& rhs) noexcept {


			TT_SELF_MOVE_TEST(rhs);

			pd().invoke();

			TT_MOVEPTR(_value, rhs);
			TT_MOVE(_pd, rhs);

			TT_RETURN_THIS;
		}

		// Returns the packaged deletion associated with the scoped pointer's underlying resource.
		constexpr const tt::packaged_deletion& pd() const noexcept { return _pd; }

		// Remember, const means the POINTER is const, not the underlying VALUE!

		// Returns the underlying pointer of the scoped pointer.
		constexpr value_t* get() const noexcept { return _value; }

		// Returns the underlying pointer of the scoped pointer as NewValue*.
		template<typename NewValue> 
		constexpr NewValue* get_as() const noexcept { return (NewValue*)get(); }

		constexpr operator value_t*() const noexcept { return get(); }

		// Returns if the scoped pointer is null or not.
		constexpr tt_bool is_null() const noexcept { return !(get()); }

		constexpr operator tt_bool() const noexcept { return (tt_bool)get(); }
		constexpr tt_bool operator!() const noexcept { return is_null(); }

		// Returns a dereferenced lvalue to the scoped pointer's underlying resource.
		// This does not perform null checking, and thus the returned lvalue may be invalid.
		constexpr value_t& deref_unchecked() const { return *get(); }

		// Returns a dereferenced lvalue to the scoped pointer's underlying resource.
		// Throws tt::illegal_deref_error if the scoped pointer is null.
		constexpr value_t& deref() const {


			if (is_null())
				TT_THROW(tt::illegal_deref_error, "illegal scoped pointer dereference!");

			return deref_unchecked();
		}

		constexpr value_t& operator*() const { return deref(); }
		constexpr value_t* operator->() const noexcept { return get(); }

		// Returns a dereferenced element of the scoped pointer's underlying pointer.
		// This does not perform bounds checking, nor null checking, and thus the returned lvalue may be invalid.
		constexpr value_t& at_unchecked(tt_size index) const { return get()[index]; }

		// Returns a dereferenced element of the scoped pointer's underlying pointer.
		// This does not perform bounds checking, and thus the returned lvalue may be invalid (which is a vary strange feature of this 'checked' version.)
		// Throws tt::illegal_deref_error if the scoped pointer is null.
		constexpr value_t& at(tt_size index) const {
			

			if (is_null())
				TT_THROW(tt::illegal_deref_error, "illegal scoped pointer dereference!");

			return at_unchecked(index);
		}

		constexpr value_t& operator[](tt_size rhs) const { return at(rhs); }

		// Returns if this and the given scoped pointer are equal.
		// This test is performed only on the addresses wrapped by the scoped pointers.
		constexpr tt_bool equal(const this_t& other) const noexcept { return get() == other.get(); }

		constexpr tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		constexpr tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// Returns if this scoped pointer's address and the given pointer are equal.
		constexpr tt_bool equal(const value_t* const x) const noexcept { return get() == x; }

		constexpr tt_bool operator==(const value_t* const rhs) const noexcept { return equal(rhs); }
		constexpr tt_bool operator!=(const value_t* const rhs) const noexcept { return !equal(rhs); }

		// lexicographical comparison function

		constexpr tt_bool operator<(const this_t& rhs) const noexcept { return get() < rhs.get(); }

		// Returns an rvalue-reference to this scoped pointer, to aid moving it around without needing to use std::move.
		inline this_t&& moved() noexcept { return std::move(*this); }

		// Returns a scoped pointer this one's contents has been moved into, with its underlying pointer having been cast to NewValue* to allow for polymorphic usage.
		// This process involves 'release' being invoked.
		template<typename NewValue>
		inline tt::scoped<NewValue> cast_moved() noexcept {


			auto R = tt::scoped<NewValue>((NewValue*)get(), pd());

			release();

			return R;
		}

		// Returns a hash of the scoped pointer.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(2097210091, 3451084697, (tt_size)12271123889775921953, (tt_size)4085181983912195003);

			h.add(get());

			return h.get();
		}

		// Rebinds the scoped pointer, disassociating it from its current pointer, and associating it with a new one.
		// This function does not deallocate the scoped pointer's managed resource. It is left to the user to do this manually.
		// It is the user's responsibility to ensure that pd actually encapsulates the deletion of allocated resource x.
		inline this_t& rebind(value_t* const x, tt::packaged_deletion pd) noexcept {


			_value = x;
			_pd = pd;

			TT_RETURN_THIS;
		}

		// A version of rebind which disassociates the current tt::packaged_deletion but not the pointer of the scoped pointer.
		// It is the user's responsibility to ensure that pd actually encapsulates the deletion of allocated resource x.
		inline this_t& rebind(tt::packaged_deletion pd) noexcept {


			return rebind(get(), pd);
		}

		// Releases the scoped pointer's claim to its current pointer, nullifying the scoped pointer.
		// Notice that this DOES NOT release the resources associated with the scoped pointer's wrapped pointer, merely the scoped pointer's OWNERSHIP CLAIM.
		inline this_t& release() noexcept {


			return rebind(nullptr, tt::packaged_deletion::null());
		}

		// Invokes the packaged deletion of the scoped pointer's managed resource, then nullifying it.
		inline this_t& reset() {


			this->pd().invoke();

			return release();
		}

		// Invokes the packaged deletion of the scoped pointer's managed resource, then rebinds it to a new target.
		// It is the user's responsibility to ensure that pd actually encapsulates the deletion of allocated resource x.
		/*inline this_t& reset(value_t* const x = nullptr, tt::packaged_deletion pd = tt::packaged_deletion::null()) {


			this->pd().invoke();

			return rebind(x, pd);
		}*/

		// Returns a null scoped pointer.
		static inline this_t null() noexcept { return this_t(); }

		// Returns a newly bound scoped pointer.
		// It is the user's responsibility to ensure that pd actually encapsulates the deletion of allocated resource x.
		static inline this_t bind(value_t* const x, tt::packaged_deletion pd) noexcept { return this_t(x, pd); }

		// Returns a scoped pointer bound to a newly allocated object.
		// The object is allocated via tt::alloc.
		template<typename... Args>
		static inline this_t make(Args&&... args) {


			auto r = tt::alloc<value_t>(TT_FMOVE_N(Args, args));
			auto rpd = tt::packaged_deletion::make(tt::alloc_deleter<value_t>, r);

			return bind(r, rpd);
		}

		// Returns a scoped pointer bound to a newly allocated array of n elements.
		// The array is allocated via tt::alloc_n.
		// If n is zero, a null scoped pointer will be returned.
		template<typename... Args>
		static inline this_t make_array(tt_size n, Args&&... args) {
			

			if (n == 0)
				return null();

			auto r = tt::alloc_n<value_t>(n, TT_FMOVE_N(Args, args));
			auto rpd = tt::packaged_deletion::make(tt::alloc_n_deleter<value_t>, r);

			return bind(r, rpd);
		}

		// Returns a scoped pointer bound to a newly allocated array of n elements worth of uninitialized memory.
		// The array is allocated via tt::alloc_uninit.
		// If n is zero, a null scoped pointer will be returned.
		static inline this_t make_array_uninit(tt_size n) {


			if (n == 0)
				return null();

			auto r = tt::alloc_uninit<value_t>(n);
			auto rpd = tt::packaged_deletion::make(tt::alloc_uninit_deleter<value_t>, r);

			return bind(r, rpd);
		}
	};
}

TT_HASHERS(0) {


	TT_REGISTER_HASH_T(tt::scoped, <typename Value>, <Value>);
}

