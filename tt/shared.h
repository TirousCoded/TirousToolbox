

#pragma once


// The Tirous Toolbox shared pointer implementation.

// TODO: Elaborate on how thread-safety does/doesn't work with our implementation.


#include <unordered_map>

#include "aliases.h"
#include "macros.h"
#include "debug.h"

#include "exceptions.h"

#include "forward_declarations.h"

#include "allocation.h"
#include "deletion.h"

#include "hash.h"

#include "datastructs.h"

#include "tuple.h"


namespace tt {


	// The state object used by tt::shared to manage and maintain the state of shared pointers.
	// A default state object is used when no explicit one is specified.
	// These shared state objects may be used to isolate shared pointers so as to make them thread-safe.
	// Also take note of the typelessness of these shared state objects, operating irrespective of the template arguments of tt::shared.
	class shared_state final {
	public:

		using this_t = tt::shared_state;

		template<typename Value>
		friend class tt::shared;


	private:

		// The map maintaining the reference counts and packaged deletions of each of the shared pointers.

		tt::hash_map<void*, tt_pair<tt_size, tt::packaged_deletion>> _refcounts;

		// Increment entry x, preparing a new entry if entry x doesn't exist.

		inline void _inc_maybe_init(void* x, tt::packaged_deletion pd) {


			auto it = _refcounts.find(x);

			if (it == _refcounts.end())
				_refcounts[x] = tt::tuple_of(1, pd);
			else {


				auto& it_c = it->second.a;

				tt_assert(it_c > 0);
				
				++it_c;
			}
		}

		// Increment entry x, expecting it to exist.

		inline void _inc(void* x) {


			auto it = _refcounts.find(x);

			tt_assert(it != _refcounts.end());

			auto& it_c = it->second.a;

			tt_assert(it_c > 0);

			++it_c;
		}

		// Decrement entry x, expecting it to exist.

		// If decrementing reduces entry x's reference count to zero, invoke its packaged deletion, and remove the entry.

		inline void _dec(void* x) {


			auto it = _refcounts.find(x);

			tt_assert(it != _refcounts.end());

			auto& it_c = it->second.a;

			tt_assert(it_c > 0);

			if (--it_c == 0)
				it->second.b.invoke(),
				_refcounts.erase(it);
		}
		

	public:

		// Default initializes a shared state object.
		inline shared_state() = default;

		// It doesn't make sense to allow for copying of shared pointer state objects, nor does
		// it make sense to allow move-assignment, which would involve discarding the state of,
		// and thus corrupting, the shared pointers of the moved-to object.

		// Thus, we'll just make it not copy or movable, forcing the user to wrap it in something
		// like a scoped pointer if they wish to moved it around.

		// Copy-initializes a shared state object.
		inline shared_state(const this_t&) = delete;

		// Move-initializes a shared state object.
		inline shared_state(this_t&&) noexcept = delete;

		inline ~shared_state() noexcept {


			// When destroying the shared state object, we need to ensure that any outstanding
			// objects under its management are disposed of alongside it, even if it means we
			// then have invalidated shared pointers floating around, that isn't our concern here.

			for (auto& I : _refcounts)
				I.second.b.invoke();
		}

		inline this_t& operator=(const this_t&) = delete;
		inline this_t& operator=(this_t&&) noexcept = delete;

		// Returns the number of resources under the management of this shared state object.
		inline tt_size size() const noexcept {


			return _refcounts.size();
		}

		// Returns the reference count of the given pointer under the management of this shared state object.
		// Returns zero if the pointer passed is not under the management of the shared state object.
		template<typename Value>
		inline tt_size refcount(Value* x) const noexcept {


			auto it = _refcounts.find((void*)x);

			if (it == _refcounts.end())
				return 0;
			else
				return it->second.a;
		}

		// Returns if the given pointer is under the management of this shared state object.
		template<typename Value>
		inline tt_bool contains(Value* x) const noexcept {


			return refcount(x) > 0;
		}
	};

	// We've no reason to make it so every argument combination of tt::shared defines a new default shared state object.
	// They use void pointers and packaged deletions internally, thus we can use this base class to use only one default state object.

	// internal, do not use
	struct base_shared_pointer {

		// The default shared state object used by shared pointers of this type when no state object is explicitly provided to use.
		// The use of this shared state renders the shared pointers involved not thread-safe.
		// Also take note of the typelessness of these shared state objects, operating irrespective of the template arguments of tt::shared.
		static tt::shared_state default_state_object;
	};

	// The Tirous Toolbox shared pointer implementation.
	template<typename Value>
	class shared final : public tt::base_shared_pointer {
	public:

		using value_t = typename Value;

		using this_t = tt::shared<value_t>;


	private:

		// The shared state object responsible for the lifetime of this shared pointer's resource.

		tt::shared_state* _manager;

		// A pointer to provide direct access to the shared pointer's resource without needing a hash map lookup.

		// This also serves as the key used when a hash map lookup is required.

		// This being nullptr dictates if the shared pointer as a whole is null.

		value_t* _direct;

		inline void _init(value_t* d, tt::shared_state* m) noexcept {


			_manager = m;
			_direct = d;
		}

		inline void _init_null() noexcept {


			_init(nullptr, nullptr);
		}

		// Remember that this doesn't nullify the shared pointer!

		inline void _drop() noexcept {


			if (*this)
				manager()->_dec(get());
		}


	public:

		// Default initializes a null shared pointer.
		inline shared() noexcept {


			_init_null();
		}

		// Initializes a shared pointer encapsulating shared ownership of pointer x.
		// If pointer x is not under the management of shared state object s, a new entry will be generated for it, encapsulating packaged deletion pd.
		// Otherwise, pd will be discarded, and the existing entry will be used instead.
		// It is the user's responsibility to ensure that pd actually encapsulates the deletion of allocated resource x.
		// Throws tt::illegal_argument_error if x is nullptr.
		// Throws tt::illegal_argument_error if s is nullptr.
		inline shared(value_t* const x, tt::packaged_deletion pd, tt::shared_state* const s = &default_state_object) {


			if (!(x))
				TT_THROW(tt::illegal_argument_error, "tt::shared constructor passed nullptr x argument!");

			else if (!(s))
				TT_THROW(tt::illegal_argument_error, "tt::shared constructor passed nullptr s argument!");

			_init(x, s);

			manager()->_inc_maybe_init(x, pd);
		}

		// Copy-initializes a shared pointer.
		inline shared(const this_t& x) {


			if (x)
				_init(x.get(), x.manager()),
				manager()->_inc(get());
			else
				_init_null();
		}

		// Move-initializes a shared pointer.
		inline shared(this_t&& x) noexcept {


			TT_MOVEPTR(_manager, x);
			TT_MOVEPTR(_direct, x);
		}

		inline ~shared() noexcept {


			_drop();
		}

		inline this_t& operator=(const this_t& rhs) {


			_drop();

			if (rhs)
				_init(rhs.get(), rhs.manager()),
				manager()->_inc(get());
			else
				_init_null();

			TT_RETURN_THIS;
		}

		inline this_t& operator=(this_t&& rhs) noexcept {


			TT_SELF_MOVE_TEST(rhs);

			_drop();

			TT_MOVEPTR(_manager, rhs);
			TT_MOVEPTR(_direct, rhs);

			TT_RETURN_THIS;
		}

		// Returns the shared state object managing this shared pointer, or nullptr if the shared pointer is null.
		constexpr tt::shared_state* manager() const noexcept { return _manager; }

		// Remember, const means the POINTER is const, not the underlying VALUE!

		// Returns the underlying pointer of the shared pointer.
		constexpr value_t* get() const noexcept { return _direct; }

		// Returns the underlying pointer of the shared pointer as NewValue*.
		template<typename NewValue>
		constexpr NewValue* get_as() const noexcept { return (NewValue*)get(); }

		constexpr operator value_t* () const noexcept { return get(); }

		// Returns if the shared pointer is null or not.
		constexpr tt_bool is_null() const noexcept { return !(get()); }

		constexpr operator tt_bool() const noexcept { return (tt_bool)get(); }
		constexpr tt_bool operator!() const noexcept { return is_null(); }

		// Returns a dereferenced lvalue to the shared pointer's underlying resource.
		// This does not perform null checking, and thus the returned lvalue may be invalid.
		constexpr value_t& deref_unchecked() const { return *get(); }

		// Returns a dereferenced lvalue to the shared pointer's underlying resource.
		// Throws tt::illegal_deref_error if the shared pointer is null.
		constexpr value_t& deref() const {


			if (is_null())
				TT_THROW(tt::illegal_deref_error, "illegal shared pointer dereference!");

			return deref_unchecked();
		}

		constexpr value_t& operator*() const { return deref(); }
		constexpr value_t* operator->() const noexcept { return get(); }

		// Returns a dereferenced element of the shared pointer's underlying pointer.
		// This does not perform bounds checking, nor null checking, and thus the returned lvalue may be invalid.
		constexpr value_t& at_unchecked(tt_size index) const { return get()[index]; }

		// Returns a dereferenced element of the shared pointer's underlying pointer.
		// This does not perform bounds checking, and thus the returned lvalue may be invalid (which is a vary strange feature of this 'checked' version.)
		// Throws tt::illegal_deref_error if the shared pointer is null.
		constexpr value_t& at(tt_size index) const {


			if (is_null())
				TT_THROW(tt::illegal_deref_error, "illegal shared pointer dereference!");

			return at_unchecked(index);
		}

		constexpr value_t& operator[](tt_size rhs) const { return at(rhs); }

		// Returns if this and the given shared pointer are equal.
		// This test is performed only on the addresses wrapped by the shared pointers.
		constexpr tt_bool equal(const this_t& other) const noexcept { return get() == other.get(); }

		constexpr tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		constexpr tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// Returns if this shared pointer's address and the given pointer are equal.
		constexpr tt_bool equal(const value_t* const x) const noexcept { return get() == x; }

		constexpr tt_bool operator==(const value_t* const rhs) const noexcept { return equal(rhs); }
		constexpr tt_bool operator!=(const value_t* const rhs) const noexcept { return !equal(rhs); }

		// lexicographical comparison function

		constexpr tt_bool operator<(const this_t& rhs) const noexcept { return get() < rhs.get(); }

		// TODO: Not sure how I'd like to add cast_moved, so we'll maybe add that later.

		// Returns an rvalue-reference to this shared pointer, to aid moving it around without needing to use std::move.
		inline this_t&& moved() noexcept { return std::move(*this); }

		// Returns a hash of the shared pointer.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(951679957, 209946241, (tt_size)4690907024294172739, (tt_size)3043771957074799031);

			h.add(manager());
			h.add(get());

			return h.get();
		}

		// Resets the state of shared pointer to null, possibly releasing its shared resource.
		inline this_t& reset() {


			_drop();

			TT_RETURN_THIS;
		}

		// Returns a null shared pointer.
		static inline this_t null() noexcept { return this_t(); }

		// Returns a newly bound shared pointer encapsulating shared ownership of pointer x.
		// If pointer x is not under the management of shared state object s, a new entry will be generated for it, encapsulating packaged deletion pd.
		// Otherwise, pd will be discarded, and the existing entry will be used instead.
		// It is the user's responsibility to ensure that pd actually encapsulates the deletion of allocated resource x.
		// Throws tt::illegal_argument_error if x is nullptr.
		// Throws tt::illegal_argument_error if s is nullptr.
		static inline this_t bind(value_t* const x, tt::packaged_deletion pd, tt::shared_state* const s = &default_state_object) {
			
			
			return this_t(x, pd, s);
		}

		// Returns a shared pointer bound to a newly allocated object, under the management of shared state object s.
		// The object is allocated via tt::alloc.
		// Throws tt::illegal_argument_error if s is nullptr.
		template<typename... Args>
		static inline this_t make_for(tt::shared_state* const s, Args&&... args) {


			if (!(s))
				TT_THROW(tt::illegal_argument_error, "tt::shared make_for passed nullptr s argument!");

			auto r = tt::alloc<value_t>(TT_FMOVE_N(Args, args));
			auto rpd = tt::packaged_deletion::make(tt::alloc_deleter<value_t>, r);

			return bind(r, rpd);
		}

		// Returns a shared pointer bound to a newly allocated object.
		// The object is allocated via tt::alloc.
		// The shared pointer returned will have its resource managed by default_state_object, see make_for to specify a different shared state object.
		template<typename... Args>
		static inline this_t make(Args&&... args) {


			return make_for(&default_state_object, TT_FMOVE_N(Args, args));
		}

		// Returns a shared pointer bound to a newly allocated array of n elements, under the management of shared state object s.
		// The array is allocated via tt::alloc_n.
		// If n is zero, a null shared pointer will be returned.
		// Throws tt::illegal_argument_error if s is nullptr.
		template<typename... Args>
		static inline this_t make_array_for(tt::shared_state* const s, tt_size n, Args&&... args) {


			if (!(s))
				TT_THROW(tt::illegal_argument_error, "tt::shared make_array_for passed nullptr s argument!");

			else if (n == 0)
				return null();

			auto r = tt::alloc_n<value_t>(n, TT_FMOVE_N(Args, args));
			auto rpd = tt::packaged_deletion::make(tt::alloc_n_deleter<value_t>, r);

			return bind(r, rpd);
		}

		// Returns a shared pointer bound to a newly allocated array of n elements.
		// The array is allocated via tt::alloc_n.
		// If n is zero, a null shared pointer will be returned.
		// The shared pointer returned will have its resource managed by default_state_object, see make_for to specify a different shared state object.
		template<typename... Args>
		static inline this_t make_array(tt_size n, Args&&... args) {


			return make_array_for(&default_state_object, n, TT_FMOVE_N(Args, args));
		}

		// Returns a shared pointer bound to a newly allocated array of n elements worth of uninitialized memory, under the management of shared state object s.
		// The array is allocated via tt::alloc_uninit.
		// If n is zero, a null shared pointer will be returned.
		// Throws tt::illegal_argument_error if s is nullptr.
		static inline this_t make_array_uninit_for(tt::shared_state* const s, tt_size n) {


			if (!(s))
				TT_THROW(tt::illegal_argument_error, "tt::shared make_array_uninit_for passed nullptr s argument!");

			else if (n == 0)
				return null();

			auto r = tt::alloc_uninit<value_t>(n);
			auto rpd = tt::packaged_deletion::make(tt::alloc_uninit_deleter<value_t>, r);

			return bind(r, rpd);
		}

		// Returns a shared pointer bound to a newly allocated array of n elements worth of uninitialized memory.
		// The array is allocated via tt::alloc_uninit.
		// If n is zero, a null shared pointer will be returned.
		// The shared pointer returned will have its resource managed by default_state_object, see make_for to specify a different shared state object.
		static inline this_t make_array_uninit(tt_size n) {


			return make_array_uninit_for(&default_state_object, n);
		}
	};
}

tt::shared_state tt::base_shared_pointer::default_state_object = {};

TT_HASHERS(0) {


	TT_REGISTER_HASH_T(tt::shared, <typename Value>, <Value>);
}

