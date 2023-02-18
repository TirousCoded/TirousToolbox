

#pragma once


// A class implementing a generic 'pool' data structure used to provide easy memoization of resources
// so as to avoid costly resource building processes as much as possible, so long as the resources in
// question are immutable.


/*

	Pools are defined by three generic parameters:

		Key			- A type of light-weight object which is used to identify resources in the pool, and
					  which encapsulates enough information with which to instantiate a version of the
					  Resource object it corresponds to.

					  All Key types must be usable as keys within std::unordered_map.

		Resource	- A type encapsulating an immutable resource.

					  All Resource types are expected to be move-constructable and move-assignable.

		Builder		- A type who's objects can be used to take a Key object and construct its corresponding
					  immutable Resource object.

					  In specific, Builder types are expected to provide methods equivalent to this:

							Resource operator()(const Key&)

					  If the operator() method throws, Builder types are expected to provide a basic 
					  guarantee of exception safety.

					  All Builder types are expected to be default constructable, move-constructable, 
					  destructable, and move-assignable.

	Put simply, a pool is instructed to 'aquire' a resource, and is provided a Key object to identify it
	thusly in said pool.

	If a memoized immutable Resource object has been instantiated which is associated with the Key object,
	then aquire will return an lvalue-reference to it. Otherwise, the Builder object of the pool is instructed
	to instantiate a new Resource object for the Key object, and after this is done, a new entry is added
	for said new resource, and an lvalue-reference to it is returned.

	For example: a pool might be defined with a Resource type of std::string, a Key type of std::string_view,
	and a builder type which casts string views to strings. In this scenario, string views can be created
	from things like string literals, and longer-lived std::string instances can be gotten from this pool
	such that each unique string aquired will only ever be instantiated a single time and simply reused therafter.

*/


#include <unordered_map>

#include "aliases.h"


namespace tt {


	// NOTE: this 'default' builder attempts to build a Resource via a constructor
	//		 of Resource which takes a Key object as an argument

	template<typename Key, typename Resource>
	struct build_by_construction final {

		inline Resource operator()(const Key& key) {


			return Resource(key);
		}
	};


	template<typename Key, typename Resource, typename Builder = build_by_construction<Key, Resource>>
	class pool {
	public:

		using key_t				= typename Key;
		using resource_t		= typename Resource;
		using builder_t			= typename Builder;

		using this_t			= pool<key_t, resource_t, builder_t>;

		using unordered_map_t	= std::unordered_map<key_t, std::unique_ptr<const resource_t>>;


		// Initializes a pool with a default-constructed builder.
		inline pool() 
			: _builder() {}

		// Initializes a pool with builder.
		inline pool(builder_t&& builder) 
			: _builder(TT_FMOVE(builder_t, builder)), 
			_resources() {}

		pool(const this_t&) = delete;

		inline pool(this_t&& x) noexcept 
			: _builder(TT_FMOVE(builder_t, x._builder)), 
			_resources(TT_FMOVE(unordered_map_t, x._resources)) {}

		~pool() noexcept = default;

		this_t& operator=(const this_t&) = delete;

		inline this_t& operator=(this_t&& rhs) noexcept;


		constexpr const builder_t& get_builder() const noexcept { return _builder; }


		// Returns the number of memoized resources in the pool.
		inline tt_size resources() const noexcept { return _resources.size(); }

		// Returns if the pool has no memoized resources.
		inline tt_bool empty() const noexcept { return resources() == 0; }

		// NOTE: while HIGHLY unlikely, 'find' is not noexcept, so no noexcept on fetch or contains

		// Returns a pointer to the memoized resource associated with key in the pool, if available, or returning nullptr if it's not yet available.
		inline const resource_t* fetch(const key_t& key) const;

		// Returns if the pool contains a memoized resource associated with key yet.
		inline tt_bool contains(const key_t& key) const { return fetch(key); }

		
		// Instructs the memoized resource pool to aquire the resource associated with key, instantiated it if it's not yet available.
		// If the builder is instructed to build a new resource, it may throw exceptions, and is expected to provide a basic guarantee of exception safety.
		inline const resource_t& aquire(const key_t& key);

		inline const resource_t& operator[](const key_t& key) { return aquire(key); }


		// NOTE: while HIGHLY unlikely, 'erase' is not noexcept

		// Instructs the memoized resource pool to discard any memoized data associated with key.
		// Fails quietly if there is no data it discard.
		inline void discard(const key_t& key) { _resources.erase(key); }

		// Resets the internal state of the memoized resource pool.
		inline void reset() noexcept { _resources.clear(); }


	private:

		builder_t		_builder;
		unordered_map_t	_resources;
	};


	template<typename Key, typename Resource, typename Builder>
	inline typename pool<Key, Resource, Builder>::this_t& pool<Key, Resource, Builder>::operator=(this_t&& rhs) noexcept {


		TT_SELF_MOVE_TEST(rhs);

		TT_MOVE(_builder, rhs);
		TT_MOVE(_resources, rhs);

		TT_RETURN_THIS;
	}

	template<typename Key, typename Resource, typename Builder>
	inline const typename pool<Key, Resource, Builder>::resource_t* pool<Key, Resource, Builder>::fetch(const key_t& key) const {


		const auto ff = _resources.find(key);

		if (ff != _resources.end())
			return ff->second.get();
		else
			return nullptr;
	}

	template<typename Key, typename Resource, typename Builder>
	inline const typename pool<Key, Resource, Builder>::resource_t& pool<Key, Resource, Builder>::aquire(const key_t& key) {


		auto ff = fetch(key);

		if (!ff) {


			// NOTE: if this throws, the method's execution up to here means we'll provide a strong guarantee,
			//		 with the builder in turn being expected to provide a basic guarantee for its internal state

			auto res = _builder(key);

			// NOTE: this may throw too, but res should be destroyed properly, so we're still providing
			//		 a strong guarantee on our end

			auto _unique = std::make_unique<const Resource>(std::move(res));

			tt_assert(_unique);

			_resources[key] = std::move(_unique);

			ff = fetch(key);
		}

		tt_assert(ff);

		return *ff;
	}
}

