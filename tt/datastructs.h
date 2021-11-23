

#pragma once


// A header file of standard library datastructure aliases used by the Tirous Toolbox library.


#include "aliases.h"


// Forward declare the standard library containers, rather than directly including them.

// We do this as I want to avoid doing the whole 'just include everything' thing, which in the
// long run can prove damaging as we create a bunch of needlessly bloated translation units.


namespace std {


	template<typename T, std::size_t N>
	class array;

	template<typename T, typename Allocator>
	class vector;


	template<typename T, typename Allocator>
	class forward_list;

	template<typename T, typename Allocator>
	class list;


	template<typename Key, typename Compare, typename Allocator>
	class set;

	template<typename Key, typename T, typename Compare, typename Allocator>
	class map;


	template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
	class unordered_set;

	template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
	class unordered_map;
}


// And now we define our aliases for the above standard library types.

// We're not gonna bother aliasing ALL of them, just the ones I want to use alternative
// names for, like renaming 'unordered_map' to 'hash_map', for example.

// Alongside renaming, we also provide aliases to bring standard library containers like
// std::array<T, N> into our namespace as tt::array<Value, Size>, for aesthetics.

// All of this is vary petty. :D


namespace tt {


	template<typename Value, tt_size Size>
	using array = std::array<Value, Size>;

	template<typename Value, typename Allocator = std::allocator<Value>>
	using vector = std::vector<Value, Allocator>;


	template<typename Value, typename Allocator>
	using flist = std::forward_list<Value, Allocator>;

	template<typename Value, typename Allocator>
	using list = std::list<Value, Allocator>;


	// The 'key-value' type used by tt::btree_map and tt::hash_map internally.

	// I think this is worthy of a type alias.

	template<typename Key, typename Value>
	using keyvalue = std::pair<const Key, Value>;


	template<typename Key, 
			 typename Compare = std::less<Key>, 
			 typename Allocator = std::allocator<Key>>
	using btree_set = std::set<Key, Compare, Allocator>;

	template<typename Key, 
			 typename Value, 
			 typename Compare = std::less<Key>, 
			 typename Allocator = std::allocator<tt::keyvalue<Key, Value>>>
	using btree_map = std::map<Key, Value, Compare, Allocator>;


	template<typename Key, 
			 typename Hash = std::hash<Key>, 
			 typename KeyEqual = std::equal_to<Key>, 
			 typename Allocator = std::allocator<Key>>
	using hash_set = std::unordered_set<Key, Hash, KeyEqual, Allocator>;

	template<typename Key, 
			 typename Value, 
			 typename Hash = std::hash<Key>, 
			 typename KeyEqual = std::equal_to<Key>, 
			 typename Allocator = std::allocator<tt::keyvalue<Key, Value>>>
	using hash_map = std::unordered_map<Key, Value, Hash, KeyEqual, Allocator>;
}

