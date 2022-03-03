

#pragma once


// A header file defining the central 'visualize' functions of the Tirous Toolbox visualization system.


#include "aliases.h"

#include "visualizer.h"
#include "visualize_functions.h"


namespace tt {


	// forward declare tt::vis

	// Returns a string visualization of x, according to type Value.
	// Unique visualization methods are employed if Value is a primitive type (sbyte, long, bool, etc.) or some standard library types (strings, pairs, etc.)
	// All other types will be visualized via tt::visualizer, a specialization of which must exist for Value in order to work.
	// If all of the above fails, the lack of tt::visualizer specialization will result in '???' being returned.
	// See TT_REGISTER_VISUALIZE and TT_REGISTER_VISUALIZE_T for details on how to specialize tt::visualizer properly.
	template<typename Value>
	inline tt_string vis(const Value& x);

	// Returns a string visualization of x, according to type Value.
	// Unique visualization methods are employed if Value is a primitive type (sbyte, long, bool, etc.) or some standard library types (strings, pairs, etc.)
	// All other types will be visualized via tt::visualizer, a specialization of which must exist for Value in order to work.
	// If all of the above fails, the lack of tt::visualizer specialization will result in '???' being returned.
	// See TT_REGISTER_VISUALIZE and TT_REGISTER_VISUALIZE_T for details on how to specialize tt::visualizer properly.
	template<typename Value>
	inline tt_string vis(const Value& x) {


		using _Value_ = std::decay_t<Value>;

		if constexpr (std::is_same_v<_Value_, tt_sbyte>)				return tt::vis_int(x);
		else if constexpr (std::is_same_v<_Value_, tt_short>)			return tt::vis_int(x);
		else if constexpr (std::is_same_v<_Value_, tt_int>)				return tt::vis_int(x);
		else if constexpr (std::is_same_v<_Value_, tt_long>)			return tt::vis_int(x);

		else if constexpr (std::is_same_v<_Value_, tt_byte>)			return tt::vis_uint(x);
		else if constexpr (std::is_same_v<_Value_, tt_ushort>)			return tt::vis_uint(x);
		else if constexpr (std::is_same_v<_Value_, tt_uint>)			return tt::vis_uint(x);
		else if constexpr (std::is_same_v<_Value_, tt_ulong>)			return tt::vis_uint(x);

		else if constexpr (std::is_same_v<_Value_, tt_float>)			return std::to_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_double>)			return std::to_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_ldouble>)			return std::to_string(x);

		else if constexpr (std::is_same_v<_Value_, tt_bool>)			return tt::vis_bool(x);

		else if constexpr (std::is_same_v<_Value_, tt_char>)			return tt_string(1, (tt_char)x);
		else if constexpr (std::is_same_v<_Value_, tt_wchar>)			return tt_string(1, (tt_char)x);
		else if constexpr (std::is_same_v<_Value_, tt_char8>)			return tt_string(1, (tt_char)x);
		else if constexpr (std::is_same_v<_Value_, tt_char16>)			return tt_string(1, (tt_char)x);
		else if constexpr (std::is_same_v<_Value_, tt_char32>)			return tt_string(1, (tt_char)x);

		else if constexpr (std::is_same_v<_Value_, tt_char*>)			return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_wchar*>)			return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_char8*>)			return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_char16*>)			return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_char32*>)			return tt::vis_string(x);

		// The const-lvalue reference above screws with the const-ness of C-strings, and thus I'm pretty
		// sure the below five should NEVER end up being used.

		else if constexpr (std::is_same_v<_Value_, const tt_char*>)		return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, const tt_wchar*>)	return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, const tt_char8*>)	return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, const tt_char16*>)	return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, const tt_char32*>)	return tt::vis_string(x);

		else if constexpr (std::is_same_v<_Value_, tt_string>)			return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_wstring>)			return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string8>)			return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string16>)		return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string32>)		return tt::vis_string(x);

		else if constexpr (std::is_same_v<_Value_, tt_string_view>)		return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_wstring_view>)	return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string8_view>)	return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string16_view>)	return tt::vis_string(x);
		else if constexpr (std::is_same_v<_Value_, tt_string32_view>)	return tt::vis_string(x);

		else return tt::visualizer<_Value_>().vis(TT_REINTERPRET(_Value_, x));
	}
}

