

#pragma once


// A header file of class forward declarations, to avoid circular dependency issues.


#include "aliases.h"


namespace tt {


	template<typename Value>
	struct visualizer;

	class hash_factory;

	struct rtti;


	struct time_value;
	
	struct time_value_nano;

	template<typename TimeValue>
	class stopwatch_class;

	using stopwatch = tt::stopwatch_class<tt::time_value>;
	
	using stopwatch_nano = tt::stopwatch_class<tt::time_value_nano>;


	//template<typename Value>
	//class scoped;

	//class shared_state;

	//template<typename Value>
	//class shared;


	template<typename Value>
	class contiguous_iterator;

	template<typename Value, tt_size Alignment>
	struct aligned_allocator;
	

	template<typename Value>
	class slice;

	template<typename Iterator>
	class range;

	template<tt_size Alignment>
	struct chunk_unit;

	template<tt_size Alignment>
	class chunk_view;

	template<tt_size Alignment, typename Allocator = tt::aligned_allocator<tt::chunk_unit<Alignment>, Alignment>>
	class chunk;


	template<typename A, typename B = void, typename C = void, typename D = void, typename E = void, typename F = void, typename G = void, typename H = void>
	struct tuple_struct;
}

template<typename A, typename B = void, typename C = void, typename D = void, typename E = void, typename F = void, typename G = void, typename H = void>
using tt_tuple = tt::tuple_struct<A, B, C, D, E, F, G, H>;

template<typename A, typename B>
using tt_pair = tt_tuple<A, B>;

template<typename A, typename B, typename C>
using tt_triple = tt_tuple<A, B, C>;

namespace tt {


	struct text_encoding;
	struct bom_text_encoding;

	class text_encoder;
	class text_decoder;
	class text_measurer;
}

