

#pragma once


// A header file of numeric limits for Tirous Toolbox types, as well as general numeric limit utilities.


#include <limits>

#include "aliases.h"


namespace tt {


	// The minimum value of the given numeric type.
	template<typename Value> 
	constexpr Value min_of = std::numeric_limits<Value>::min();

	// The maximum value of the given numeric type.
	template<typename Value>
	constexpr Value max_of = std::numeric_limits<Value>::max();


	// The minimum value of tt_int8.
	constexpr tt_int8 min_int8 = tt::min_of<tt_int8>;

	// The maximum value of tt_int8.
	constexpr tt_int8 max_int8 = tt::max_of<tt_int8>;

	// The minimum value of tt_int16.
	constexpr tt_int16 min_int16 = tt::min_of<tt_int16>;

	// The maximum value of tt_int16.
	constexpr tt_int16 max_int16 = tt::max_of<tt_int16>;

	// The minimum value of tt_int32.
	constexpr tt_int32 min_int32 = tt::min_of<tt_int32>;

	// The maximum value of tt_int32.
	constexpr tt_int32 max_int32 = tt::max_of<tt_int32>;

	// The minimum value of tt_int64.
	constexpr tt_int64 min_int64 = tt::min_of<tt_int64>;

	// The maximum value of tt_int64.
	constexpr tt_int64 max_int64 = tt::max_of<tt_int64>;

	// The minimum value of tt_uint8.
	constexpr tt_uint8 min_uint8 = tt::min_of<tt_uint8>;

	// The maximum value of tt_uint8.
	constexpr tt_uint8 max_uint8 = tt::max_of<tt_uint8>;

	// The minimum value of tt_uint16.
	constexpr tt_uint16 min_uint16 = tt::min_of<tt_uint16>;

	// The maximum value of tt_uint16.
	constexpr tt_uint16 max_uint16 = tt::max_of<tt_uint16>;

	// The minimum value of tt_uint32.
	constexpr tt_uint32 min_uint32 = tt::min_of<tt_uint32>;

	// The maximum value of tt_uint32.
	constexpr tt_uint32 max_uint32 = tt::max_of<tt_uint32>;

	// The minimum value of tt_uint64.
	constexpr tt_uint64 min_uint64 = tt::min_of<tt_uint64>;

	// The maximum value of tt_uint64.
	constexpr tt_uint64 max_uint64 = tt::max_of<tt_uint64>;


	// The minimum value of tt_sbyte.
	constexpr tt_sbyte min_sbyte = tt::min_of<tt_sbyte>;

	// The maximum value of tt_sbyte.
	constexpr tt_sbyte max_sbyte = tt::max_of<tt_sbyte>;

	// The minimum value of tt_short.
	constexpr tt_short min_short = tt::min_of<tt_short>;

	// The maximum value of tt_short.
	constexpr tt_short max_short = tt::max_of<tt_short>;

	// The minimum value of tt_int.
	constexpr tt_int min_int = tt::min_of<tt_int>;

	// The maximum value of tt_int.
	constexpr tt_int max_int = tt::max_of<tt_int>;

	// The minimum value of tt_long.
	constexpr tt_long min_long = tt::min_of<tt_long>;

	// The maximum value of tt_long.
	constexpr tt_long max_long = tt::max_of<tt_long>;

	// The minimum value of tt_byte.
	constexpr tt_byte min_byte = tt::min_of<tt_byte>;

	// The maximum value of tt_byte.
	constexpr tt_byte max_byte = tt::max_of<tt_byte>;

	// The minimum value of tt_ushort.
	constexpr tt_ushort min_ushort = tt::min_of<tt_ushort>;

	// The maximum value of tt_ushort.
	constexpr tt_ushort max_ushort = tt::max_of<tt_ushort>;

	// The minimum value of tt_uint.
	constexpr tt_uint min_uint = tt::min_of<tt_uint>;

	// The maximum value of tt_uint.
	constexpr tt_uint max_uint = tt::max_of<tt_uint>;

	// The minimum value of tt_ulong.
	constexpr tt_ulong min_ulong = tt::min_of<tt_ulong>;

	// The maximum value of tt_ulong.
	constexpr tt_ulong max_ulong = tt::max_of<tt_ulong>;


	// Character limits will be included here as C++ treats characters as numeric types.

	// The minimum value of tt_char.
	constexpr tt_char min_char = tt::min_of<tt_char>;

	// The maximum value of tt_char.
	constexpr tt_char max_char = tt::max_of<tt_char>;

	// The minimum value of tt_wchar.
	constexpr tt_wchar min_wchar = tt::min_of<tt_wchar>;

	// The maximum value of tt_wchar.
	constexpr tt_wchar max_wchar = tt::max_of<tt_wchar>;

	// The minimum value of tt_char8.
	constexpr tt_char8 min_char8 = tt::min_of<tt_char8>;

	// The maximum value of tt_char8.
	constexpr tt_char8 max_char8 = tt::max_of<tt_char8>;

	// The minimum value of tt_char16.
	constexpr tt_char16 min_char16 = tt::min_of<tt_char16>;

	// The maximum value of tt_char16.
	constexpr tt_char16 max_char16 = tt::max_of<tt_char16>;

	// The minimum value of tt_char32.
	constexpr tt_char32 min_char32 = tt::min_of<tt_char32>;

	// The maximum value of tt_char32.
	constexpr tt_char32 max_char32 = tt::max_of<tt_char32>;


	// The minimum value of tt_size.
	constexpr tt_size min_size = tt::min_of<tt_size>;

	// The maximum value of tt_size.
	constexpr tt_size max_size = tt::max_of<tt_size>;

	// The minimum value of tt_ssize.
	constexpr tt_ssize min_ssize = tt::min_of<tt_ssize>;

	// The maximum value of tt_ssize.
	constexpr tt_ssize max_ssize = tt::max_of<tt_ssize>;


	// The minimum value of tt_uintptr.
	constexpr tt_ssize min_uintptr = tt::min_of<tt_uintptr>;

	// The maximum value of tt_uintptr.
	constexpr tt_ssize max_uintptr = tt::max_of<tt_uintptr>;

	// The minimum value of tt_intptr.
	constexpr tt_size min_intptr = tt::min_of<tt_intptr>;

	// The maximum value of tt_intptr.
	constexpr tt_size max_intptr = tt::max_of<tt_intptr>;
}

