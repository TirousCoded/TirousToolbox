

#pragma once


// A class used to summarize the process of generating a hash code.


#include <functional>

#include "aliases.h"
#include "config.h"
#include "macros.h"


namespace tt {


	// A class used to summarize the process of generating a hash code.
	class hash_factory final {
	public:

		using this_t = hash_factory;


	private:

		// The 'product' being hashed. This value starts off as our first prime.

		tt_size _product;

		// The second prime needed to add sub-hashes. Unlike the first, this one needs to be stored.

		tt_size _prime;


	public:

		// Default initializes a hash factory.
		inline hash_factory() = delete;

		// Initializes a hash factory using the given four prime constants.
		// Only two will ever be used, either the 32-bit primes, or the 64-bit primes, based on the target platform.
		// The website I like getting these primes from: " https://asecuritysite.com/encryption/random3?val=32 ".
		inline hash_factory(tt_size prime_one_32bit, tt_size prime_two_32bit, tt_size prime_one_64bit, tt_size prime_two_64bit) noexcept {


			if constexpr (tt::config_is_32bit)
				_product = prime_one_32bit,
				_prime = prime_two_32bit;
			else
				_product = prime_one_64bit,
				_prime = prime_two_64bit;
		}

		// Copy-initializes a hash factory.
		inline hash_factory(const this_t&) = delete;

		// Move-initializes a hash factory.
		inline hash_factory(this_t&&) noexcept = delete;

		inline ~hash_factory() noexcept = default;

		inline this_t& operator=(const this_t&) = delete;
		inline this_t& operator=(this_t&&) noexcept = delete;

		// Returns the hash code produced by the hash factory.
		inline tt_size get() const noexcept { return _product; }

		// Adds an object to the hash as a function of its hash code.
		inline this_t& add_code(tt_size hash_code) noexcept {
			
			
			_product *= _prime;

			_product += hash_code;

			TT_RETURN_THIS;
		}

		// Adds an object to the hash as a function of its std::hash specialization.
		template<typename Value>
		inline this_t& add(const Value& x) noexcept {


			return add_code(std::hash<Value>()(x));
		}
	};
}

