

#pragma once


// A header file defining the central 'visualize' functions of the Tirous Toolbox visualization system.


#include "aliases.h"

#include "visualizer.h"
#include "visualize_functions.h"

#include "datastructs.h"


namespace tt {


	// forward declare tt::visualize

	// Returns a string visualization of x, according to type Value.
	// Unique visualization methods are employed if Value is a primitive type (sbyte, long, bool, etc.) or some standard library types (strings, pairs, etc.)
	// All other types will be visualized via tt::visualizer, a specialization of which must exist for Value in order to work.
	// If all of the above fails, the lack of tt::visualizer specialization will result in '???' being returned.
	// See TT_REGISTER_VISUALIZE and TT_REGISTER_VISUALIZE_T for details on how to specialize tt::visualizer properly.
	template<typename Value>
	inline tt_string visualize(const Value& x);

	// Add unique specializations for standard library container classes.

	// We'll only define these for ones we have aliases for, including tt::keyvalue to specialize for std::pair,
	// which is common enough that it would be annoying to not deal with.

#define _TPARAMS <typename Key, typename Value>
#define _TARGS <Key, Value>

	TT_REGISTER_VISUALIZE_T_EXT(tt::keyvalue, _TPARAMS, _TARGS, x, {
		return "{ " + tt::visualize(x.first) + ", " + tt::visualize(x.second) + " }";
		});

#undef _TPARAMS
#undef _TARGS

#define _TPARAMS <typename Value, tt_size Size>
#define _TARGS <Value, Size>

	TT_REGISTER_VISUALIZE_T_EXT(tt::array, _TPARAMS, _TARGS, x, {
		return tt::visualize_iterable(x);
		});

#undef _TPARAMS
#undef _TARGS

#define _TPARAMS <typename Value, typename Allocator>
#define _TARGS <Value, Allocator>

	TT_REGISTER_VISUALIZE_T_EXT(tt::vector, _TPARAMS, _TARGS, x, {
		return tt::visualize_iterable(x);
		});

	TT_REGISTER_VISUALIZE_T_EXT(tt::flist, _TPARAMS, _TARGS, x, {
		return tt::visualize_iterable(x);
		});

	TT_REGISTER_VISUALIZE_T_EXT(tt::list, _TPARAMS, _TARGS, x, {
		return tt::visualize_iterable(x);
		});

#undef _TPARAMS
#undef _TARGS

#define _TPARAMS <typename Key, typename Compare, typename Allocator>
#define _TARGS <Key, Compare, Allocator>

	TT_REGISTER_VISUALIZE_T_EXT(tt::btree_set, _TPARAMS, _TARGS, x, {
		return tt::visualize_iterable(x);
		});

#undef _TPARAMS
#undef _TARGS

#define _TPARAMS <typename Key, typename Value, typename Compare, typename Allocator>
#define _TARGS <Key, Value, Compare, Allocator>

	TT_REGISTER_VISUALIZE_T_EXT(tt::btree_map, _TPARAMS, _TARGS, x, {
		return tt::visualize_iterable(x);
		});

#undef _TPARAMS
#undef _TARGS

#define _TPARAMS <typename Key, typename Hash, typename KeyEqual, typename Allocator>
#define _TARGS <Key, Hash, KeyEqual, Allocator>

	TT_REGISTER_VISUALIZE_T_EXT(tt::hash_set, _TPARAMS, _TARGS, x, {
		return tt::visualize_iterable(x);
		});

#undef _TPARAMS
#undef _TARGS

#define _TPARAMS <typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator>
#define _TARGS <Key, Value, Hash, KeyEqual, Allocator>

	TT_REGISTER_VISUALIZE_T_EXT(tt::hash_map, _TPARAMS, _TARGS, x, {
		return tt::visualize_iterable(x);
		});

#undef _TPARAMS
#undef _TARGS

	// Returns a string visualization of x, according to type Value.
	// Unique visualization methods are employed if Value is a primitive type (sbyte, long, bool, etc.) or some standard library types (strings, pairs, etc.)
	// All other types will be visualized via tt::visualizer, a specialization of which must exist for Value in order to work.
	// If all of the above fails, the lack of tt::visualizer specialization will result in '???' being returned.
	// See TT_REGISTER_VISUALIZE and TT_REGISTER_VISUALIZE_T for details on how to specialize tt::visualizer properly.
	template<typename Value>
	inline tt_string visualize(const Value& x) {


		using _Value_ = std::decay_t<Value>;

		if constexpr (std::is_same_v<_Value_, tt_sbyte>)				return tt::visualize_int(x);
		else if constexpr (std::is_same_v<_Value_, tt_short>)			return tt::visualize_int(x);
		else if constexpr (std::is_same_v<_Value_, tt_int>)				return tt::visualize_int(x);
		else if constexpr (std::is_same_v<_Value_, tt_long>)			return tt::visualize_int(x);

		else if constexpr (std::is_same_v<_Value_, tt_byte>)			return tt::visualize_uint(x);
		else if constexpr (std::is_same_v<_Value_, tt_ushort>)			return tt::visualize_uint(x);
		else if constexpr (std::is_same_v<_Value_, tt_uint>)			return tt::visualize_uint(x);
		else if constexpr (std::is_same_v<_Value_, tt_ulong>)			return tt::visualize_uint(x);

		else if constexpr (std::is_same_v<_Value_, tt_float>)			return std::to_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_double>)			return std::to_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_ldouble>)			return std::to_string(x);

		else if constexpr (std::is_same_v<_Value_, tt_bool>)			return tt::visualize_bool(x);

		else if constexpr (std::is_same_v<_Value_, tt_char>)			return tt_string(1, (tt_char)x);
		else if constexpr (std::is_same_v<_Value_, tt_wchar>)			return tt_string(1, (tt_char)x);
		else if constexpr (std::is_same_v<_Value_, tt_char8>)			return tt_string(1, (tt_char)x);
		else if constexpr (std::is_same_v<_Value_, tt_char16>)			return tt_string(1, (tt_char)x);
		else if constexpr (std::is_same_v<_Value_, tt_char32>)			return tt_string(1, (tt_char)x);

		else if constexpr (std::is_same_v<_Value_, tt_char*>)			return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_wchar*>)			return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_char8*>)			return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_char16*>)			return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_char32*>)			return tt::visualize_string(x);

		// The const-lvalue reference above screws with the const-ness of C-strings, and thus I'm pretty
		// sure the below five should NEVER end up being used.

		else if constexpr (std::is_same_v<_Value_, const tt_char*>)		return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, const tt_wchar*>)	return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, const tt_char8*>)	return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, const tt_char16*>)	return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, const tt_char32*>)	return tt::visualize_string(x);

		else if constexpr (std::is_same_v<_Value_, tt_string>)			return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_wstring>)			return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string8>)			return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string16>)		return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string32>)		return tt::visualize_string(x);

		else if constexpr (std::is_same_v<_Value_, tt_string_view>)		return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_wstring_view>)	return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string8_view>)	return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string16_view>)	return tt::visualize_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string32_view>)	return tt::visualize_string(x);

		else return tt::visualizer<_Value_>().visualize(TT_REINTERPRET(_Value_, x));
	}
}

