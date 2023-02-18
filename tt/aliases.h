

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

#include <atomic>


using tt_int8 = int8_t;
using tt_int16 = int16_t;
using tt_int32 = int32_t;
using tt_int64 = int64_t;

using tt_uint8 = uint8_t;
using tt_uint16 = uint16_t;
using tt_uint32 = uint32_t;
using tt_uint64 = uint64_t;

using tt_atomic_int8 = std::atomic<tt_int8>;
using tt_atomic_int16 = std::atomic<tt_int16>;
using tt_atomic_int32 = std::atomic<tt_int32>;
using tt_atomic_int64 = std::atomic<tt_int64>;

using tt_atomic_uint8 = std::atomic<tt_uint8>;
using tt_atomic_uint16 = std::atomic<tt_uint16>;
using tt_atomic_uint32 = std::atomic<tt_uint32>;
using tt_atomic_uint64 = std::atomic<tt_uint64>;


using tt_sbyte = tt_int8;
using tt_short = tt_int16;
using tt_int = tt_int32;
using tt_long = tt_int64;

using tt_byte = tt_uint8;
using tt_ushort = tt_uint16;
using tt_uint = tt_uint32;
using tt_ulong = tt_uint64;

using tt_atomic_sbyte = std::atomic<tt_sbyte>;
using tt_atomic_short = std::atomic<tt_short>;
using tt_atomic_int = std::atomic<tt_int>;
using tt_atomic_long = std::atomic<tt_long>;

using tt_atomic_byte = std::atomic<tt_byte>;
using tt_atomic_ushort = std::atomic<tt_ushort>;
using tt_atomic_uint = std::atomic<tt_uint>;
using tt_atomic_ulong = std::atomic<tt_ulong>;


using tt_float = float;
using tt_double = double;
using tt_ldouble = long double;

using tt_atomic_float = std::atomic<tt_float>;
using tt_atomic_double = std::atomic<tt_double>;
using tt_atomic_ldouble = std::atomic<tt_ldouble>;


using tt_bool = bool;

using tt_atomic_bool = std::atomic<tt_bool>;


using tt_char = char;
using tt_wchar = wchar_t;

using tt_atomic_char = std::atomic<tt_char>;
using tt_atomic_wchar = std::atomic<tt_wchar>;

// character types for UTF-8/16/32 usage

using tt_char8 = tt_char;
using tt_char16 = char16_t;
using tt_char32 = char32_t;

using tt_atomic_char8 = std::atomic<tt_char8>;
using tt_atomic_char16 = std::atomic<tt_char16>;
using tt_atomic_char32 = std::atomic<tt_char32>;

// a special character type used to carry the value
// of any Unicode codepoint

// this is used for encoding/decoding, and will thus
// not have an associated string type

using tt_unichar = tt_char32;

using tt_atomic_unichar = std::atomic<tt_unichar>;


using tt_size = std::size_t;
using tt_ssize = std::make_signed_t<tt_size>;

using tt_atomic_size = std::atomic<tt_size>;
using tt_atomic_ssize = std::atomic<tt_ssize>;


using tt_uintptr = std::uintptr_t;
using tt_intptr = std::intptr_t;

using tt_atomic_uintptr = std::atomic<tt_uintptr>;
using tt_atomic_intptr = std::atomic<tt_intptr>;


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

