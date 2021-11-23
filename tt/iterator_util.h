

#pragma once


// A header file of general purpose iterator utilities.


#include <iterator>

#include "aliases.h"


namespace tt {


	// Some fancy type aliases used to help us get the various different types of iterator types
	// a container type might define for itself.

	template<typename Object>
	using iter_of_t = typename Object::iterator;

	template<typename Object>
	using citer_of_t = typename Object::const_iterator;

	template<typename Object>
	using riter_of_t = typename Object::reverse_iterator;

	template<typename Object>
	using criter_of_t = typename Object::const_reverse_iterator;


	// Returns an iterator advanced via std::advance.
	template<typename Iterator, typename Distance>
	constexpr Iterator advanced(Iterator it, Distance n) {


		std::advance(it, n);

		return it;
	}
}

