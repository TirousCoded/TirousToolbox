

#pragma once


// A header file defining the type aliases of the Tirous Toolbox library.


#include <cstdint>
#include <cstdlib>

#include <type_traits>

// I tried forward declaring <string>'s stuff, but it didn't work out,
// so we'll just include this everywhere, as it's vary unlikely that the
// end user will ever NOT use <string>

#include <string>
#include <string_view>


using tt_int8 = int8_t;
using tt_int16 = int16_t;
using tt_int32 = int32_t;
using tt_int64 = int64_t;

using tt_uint8 = uint8_t;
using tt_uint16 = uint16_t;
using tt_uint32 = uint32_t;
using tt_uint64 = uint64_t;


using tt_sbyte = tt_int8;
using tt_short = tt_int16;
using tt_int = tt_int32;
using tt_long = tt_int64;

using tt_byte = tt_uint8;
using tt_ushort = tt_uint16;
using tt_uint = tt_uint32;
using tt_ulong = tt_uint64;


using tt_float = float;
using tt_double = double;
using tt_ldouble = long double;

using tt_bool = bool;


using tt_char = char;
using tt_wchar = wchar_t;

// character types for UTF-8/16/32 usage

using tt_char8 = tt_char;
using tt_char16 = char16_t;
using tt_char32 = char32_t;

// a special character type used to carry the value
// of any Unicode codepoint

// this is used for encoding/decoding, and will thus
// not have an associated string type

using tt_unichar = tt_char32;


using tt_size = std::size_t;
using tt_ssize = std::make_signed_t<tt_size>;

using tt_uintptr = std::uintptr_t;
using tt_intptr = std::intptr_t;


template<typename Value>
using tt_initlist = std::initializer_list<Value>;


// alongside these aliases we'll include aliases for strings, as I'd like these
// to use the C-style 'tt_' prefix, rather than be written with 'tt::'


using tt_string = std::basic_string<tt_char>;
using tt_wstring = std::basic_string<tt_wchar>;

// string types for UTF-8/16/32 usage

using tt_string8 = std::basic_string<tt_char8>;
using tt_string16 = std::basic_string<tt_char16>;
using tt_string32 = std::basic_string<tt_char32>;


using tt_string_view = std::basic_string_view<tt_char>;
using tt_wstring_view = std::basic_string_view<tt_wchar>;

// string view types for UTF-8/16/32 usage

using tt_string8_view = std::basic_string_view<tt_char8>;
using tt_string16_view = std::basic_string_view<tt_char16>;
using tt_string32_view = std::basic_string_view<tt_char32>;

