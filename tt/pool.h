

#pragma once


// A class implementing a generic 'pool' data structure used to provide easy memoization of resources
// so as to avoid costly resource building processes as much as possible, so long as the resources in
// question are immutable.


/*

	Pools are defined by three generic parameters:

		Key			- A type of light-weight object which is used to identify resources in the pool, and
					  which encapsulate enough information with which to instantiate a version of the
					  Resource object it corresponds to.

					  All Key types must be usable as keys within std::unordered_map.

					  The tt::pool class is designed to allow for Key types which describe memory-view 
					  objects viewing (but which do not own) internal memory owned by the Resource object
					  they correspond to, and as such said keys are thusly invalidated if they outlive their 
					  corresponding resources.

					  While it is ideal to not have this nuance, tt::pool is, again, designed to allow
					  for these.

					  As such, if a method like 'aquire' is passed a key, and instantiates a new resource
					  via it, the key used in the entry of said resource in the pool will NOT be a copy
					  of the argument, but will instead be a fresh key gotten from the new resource via
					  the pool's builder. This is done to ensure the key's validity is never called into
					  question, and thus the end-user of the tt::pool class need-not worry.

		Resource	- A type encapsulating an immutable resource.

					  All Resource types are expected to be move-constructable/assignable.

		Builder		- A type who's objects can be used as a factory to produce Key and Resource objects.

					  All Builder types are expected to be default constructable and to also be copy/move
					  constructable/assignable also.

					  Key and Resource objects have 1-to-1 associations where a key can be used to produce
					  a given resource it corresponds to, and said resource can likewise be used to produce 
					  a copy of said key.

					  In specific, a Builder type is expected to provide methods equivalent to these:

							Resource get_resource(const Key&)

							Key get_key(const Resource&) const noexcept

					  The get_resource method builds a resource from a key. These methods are epxected to
					  provide a basic guarantee of exception safety.

					  The get_key method gets a key associated with a resource. The key returned must
					  be valid for at least the lifetime of the resource which produced it. The key
					  returned must also equal the key used to produce the resource.

	Put simply, a pool is instructed to 'aquire' a resource, and is provided a Key object to identify it
	thusly in said pool.

	If a memoized immutable Resource object has been instantiated which is associated with the Key object,
	then aquire will return an lvalue-reference to it. Otherwise, the Builder object of the pool is instructed
	to instantiate a new Resource object for the Key object, and after this is done, a new entry is added
	for said new resource, and an lvalue-reference to it is returned.

	For example: a pool might be defined with a Resource type of std::string, a Key type of std::string_view,
	and a builder type which casts string views to strings. In this scenario, string views can be created
	from things like string literals, and longer-lived std::string instances can be gotten from this pool
	such that each unique string aquired will only ever be instantiated a single time and reused therafter.

*/

/*

	IMPORTANT:

		Internally, pools store their memory objects using shared pointers, meaning that aquiring one of these
		shared pointers can be used to extend the lifetime of said memory objects such that they live longer
		than the pool itself. This is intentional.

		As a consequence of this, notice that if a pool is copy-constructed/assigned, that the resulting
		copy will be initialized with references to these same memory objects, with said memory objects not
		being copied (and with this being pointless due to their immutability.)

		This is an important detail to note regarding pools and thusly their proper usage.

*/


#include <unordered_map>

#include "aliases.h"


namespace tt {


	// NOTE: this 'naive' builder attempts to build a Resource via a constructor
	//		 of Resource which takes a Key object as an argument

	template<typename Key, typename Resource>
	struct naive_builder final {

		inline Resource get_resource(const Key& key) {


			return Resource(key);
		}

		inline Key get_key(const Resource& resource) const noexcept {


			return Key(resource);
		}
	};


	template<typename Key, typename Resource, typename Builder = naive_builder<Key, Resource>>
	class pool {
	public:

		using key_t				= typename Key;
		using resource_t		= typename Resource;
		using builder_t			= typename Builder;

		using this_t			= pool<key_t, resource_t, builder_t>;

		using unordered_map_t	= std::unordered_map<key_t, std::shared_ptr<const resource_t>>;


		// Initializes a pool with a default-constructed builder.
		inline pool() 
			: _builder() {}

		// Initializes a pool with builder.
		inline pool(builder_t&& builder) 
			: _builder(TT_FMOVE(builder_t, builder)), 
			_resources() {}

		pool(const this_t&) = default; // <- clone pool

		inline pool(this_t&& x) noexcept 
			: _builder(TT_FMOVE(builder_t, x._builder)), 
			_resources(TT_FMOVE(unordered_map_t, x._resources)) {}

		~pool() noexcept = default;

		this_t& operator=(const this_t&) = default; // <- clone pool

		inline this_t& operator=(this_t&& rhs) noexcept;


		inline builder_t get_builder() const noexcept { return _builder; }


		// Returns the number of memoized resources in the pool.
		inline tt_size resources() const noexcept { return _resources.size(); }

		// Returns if the pool has no memoized resources.
		inline tt_bool empty() const noexcept { return resources() == 0; }


		// NOTE: while HIGHLY unlikely, 'find' is not noexcept, so no noexcept on fetch(_ptr) or contains

		// Returns a pointer to the memoized resource associated with key in the pool, if available, or returning nullptr if it's not yet available.
		inline const resource_t* fetch(const key_t& key) const;

		// Returns a pointer to the memoized resource associated with key in the pool, if available, or returning nullptr if it's not yet available.
		// This returns a shared pointer of the underlying memory object itself.
		inline std::shared_ptr<const resource_t> fetch_ptr(const key_t& key) const;

		// Returns if the pool contains a memoized resource associated with key yet.
		inline tt_bool contains(const key_t& key) const { return fetch(key); }

		
		// Instructs the memoized resource pool to aquire the resource associated with key, instantiated it if it's not yet available.
		// If the builder is instructed to build a new resource, it may throw exceptions, and is expected to provide a basic guarantee of exception safety.
		inline const resource_t& aquire(const key_t& key);

		inline const resource_t& operator[](const key_t& key) { return aquire(key); }

		// Instructs the memoized resource pool to aquire the resource associated with key, instantiated it if it's not yet available.
		// If the builder is instructed to build a new resource, it may throw exceptions, and is expected to provide a basic guarantee of exception safety.
		// This returns a shared pointer of the underlying memory object itself.
		inline std::shared_ptr<const resource_t> aquire_ptr(const key_t& key);


		// Instructs the memoized resource pool to insert resource into the pool, using the pool's builder to get a key for it, discarding any current entry in the pool.
		inline void insert(resource_t resource);
		
		// Instructs the memoized resource pool to insert resource into the pool, using the pool's builder to get a key for it, discarding any current entry in the pool.
		// This version inserts a resource already wrapped in a shared pointer.
		// Fails quietly if resource is nullptr.
		inline void insert(std::shared_ptr<resource_t> resource);


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
	inline std::shared_ptr<const typename pool<Key, Resource, Builder>::resource_t> pool<Key, Resource, Builder>::fetch_ptr(const key_t& key) const {


		const auto ff = _resources.find(key);

		if (ff != _resources.end())
			return ff->second;
		else
			return nullptr;
	}

	template<typename Key, typename Resource, typename Builder>
	inline const typename pool<Key, Resource, Builder>::resource_t& pool<Key, Resource, Builder>::aquire(const key_t& key) {


		const auto _aquired = aquire_ptr(key);

		tt_assert(_aquired);

		return *_aquired;
	}

	template<typename Key, typename Resource, typename Builder>
	inline std::shared_ptr<const typename pool<Key, Resource, Builder>::resource_t> pool<Key, Resource, Builder>::aquire_ptr(const key_t& key) {


		auto ff = fetch_ptr(key);

		if (!ff) {


			// NOTE: if this throws, the method's execution up to here means we'll provide a strong guarantee,
			//		 with the builder in turn being expected to provide a basic guarantee for its internal state

			auto res = _builder.get_resource(key);

			// NOTE: this may throw too, but res should be destroyed properly, so we're still providing
			//		 a strong guarantee on our end

			auto _shared = std::make_shared<const Resource>(std::move(res));

			tt_assert(_shared);

			// NOTE: this get_key usage here is to ensure that our _resource entry is made with a key object
			//		 who's lifetime will match or exceed *_shared's lifetime (such as when the key is a memory-view
			//		 object who's validity is tied to the memory of the object of its entry)

			_resources[_builder.get_key(*_shared)] = std::move(_shared);

			ff = fetch_ptr(key);
		}

		tt_assert(ff);

		return ff;
	}

	template<typename Key, typename Resource, typename Builder>
	inline void pool<Key, Resource, Builder>::insert(resource_t resource) {


		const auto _key = _builder.get_key(resource);

		discard(_key);

		_resources[_key] = std::make_shared<const Resource>(std::move(resource));
	}

	template<typename Key, typename Resource, typename Builder>
	inline void pool<Key, Resource, Builder>::insert(std::shared_ptr<resource_t> resource) {


		if (!resource)
			return;

		const auto _key = _builder.get_key(*resource);

		discard(_key);

		_resources[_key] = std::move(resource);
	}
}

