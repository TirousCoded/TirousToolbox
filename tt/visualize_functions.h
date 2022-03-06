

#pragma once


// A header file of functions used to provide specialized string visualizations of certain objects.


#include "aliases.h"
#include "debug.h"

#include "visualizer.h"

#include "character_arrays.h"
#include "unicode.h"


namespace tt {

	
	// Returns a string visualization of x, according to type Value.
	// Unique visualization methods are employed if Value is a primitive type (sbyte, long, bool, etc.) or some standard library types (strings, pairs, etc.)
	// All other types will be visualized via tt::visualizer, a specialization of which must exist for Value in order to work.
	// If all of the above fails, the lack of tt::visualizer specialization will result in '???' being returned.
	// See TT_REGISTER_VISUALIZE and TT_REGISTER_VISUALIZE_T for details on how to specialize tt::visualizer properly.
	template<typename Value>
	inline tt_string vis(const Value& x);
	
	// Returns a string visualization of the given boolean value.
	inline tt_string vis_bool(tt_bool x) {


		if (x)
			return "true";
		else
			return "false";
	}

	// Returns a string visualization of the given unsigned integral value, in base-10.
	inline tt_string vis_uint(tt_ulong x) {


		tt_string r;

		if (x == 0)
			return "0";

		tt_ulong _digit, _curr = x;

		while (true) {


			_digit = _curr % 10;

			r.insert(r.begin(), tt::base10_digits[_digit]);

			if (_curr < 10)
				break;

			_curr /= 10;
		}

		return r;
	}

	// Returns a string visualization of the given signed integral value, in base-10.
	inline tt_string vis_int(tt_long x) {


		tt_string r;

		if (x > 0)
			r = tt::vis_uint(x);
		else
			r = tt::vis_uint(x * -1),
			r.insert(r.begin(), '-');

		return r;
	}

	// Returns a string visualization of the given unsigned integral value, in hexadecimal.
	inline tt_string vis_uint_hex(tt_ulong x, tt_bool uppercase = false) {


		tt_string r;

		if (x == 0)
			return uppercase ? "0X0" : "0x0";

		tt_ulong _digit, _curr = x;

		while (true) {


			_digit = _curr % 16;

			r.insert(r.begin(), (uppercase ? tt::hex_digits_uppercase : tt::hex_digits_lowercase)[_digit]);

			if (_curr < 16)
				break;

			_curr /= 16;
		}

		return tt_string(uppercase ? "0X" : "0x") + r;
	}

	// Returns a string visualization of the given signed integral value, in hexadecimal.
	inline tt_string vis_int_hex(tt_long x, tt_bool uppercase = false) {


		tt_string r;

		if (x > 0)
			r = tt::vis_uint_hex(x, uppercase);
		else
			r = tt::vis_uint_hex(x * -1, uppercase),
			r.insert(r.begin(), '-');

		return r;
	}

	// A specialized version of tt::vis_uint_hex used to visualize the byte value of a single byte, ensuring that it is represented by exactly two characters.
	// This version also omits the '0x' or '0X' at the start of the string, as this is intended for a pure hex-editor style binary visualization.
	inline tt_string vis_byte_hex(tt_byte x, tt_bool uppercase = false) {


		// all of this should be cheap due to SSO

		const tt_char* const _chars = uppercase ? tt::hex_digits_uppercase : tt::hex_digits_lowercase;

		tt_string r = "__";

		// gotta remember that the left bits go on to the FIRST value, not the second

		r[0] = _chars[(x >> 4) & 0b00001111U];
		r[1] = _chars[x & 0b00001111U];

		tt_assert(r != "__");

		return r;
	}

	// Returns a string visualization of the given unsigned integral value, in octal.
	inline tt_string vis_uint_octal(tt_ulong x) {


		tt_string r;

		if (x == 0)
			return "00";

		tt_ulong _digit, _curr = x;

		while (true) {


			_digit = _curr % 8;

			r.insert(r.begin(), tt::base10_digits[_digit]);

			if (_curr < 8)
				break;

			_curr /= 8;
		}

		return tt_string(1, '0') + r;
	}

	// Returns a string visualization of the given signed integral value, in octal.
	inline tt_string vis_int_octal(tt_long x) {


		tt_string r;

		if (x > 0)
			r = tt::vis_uint_octal(x);
		else
			r = tt::vis_uint_octal(x * -1),
			r.insert(r.begin(), '-');

		return r;
	}

	// Returns a string visualization of the given unsigned integral value, in binary.
	inline tt_string vis_uint_binary(tt_ulong x, tt_bool uppercase = false) {


		tt_string r;

		if (x == 0)
			return uppercase ? "0B0" : "0b0";

		tt_ulong _digit, _curr = x;

		while (true) {


			_digit = _curr % 2;

			r.insert(r.begin(), tt::binary_digits[_digit]);

			if (_curr < 2)
				break;

			_curr /= 2;
		}

		return tt_string(uppercase ? "0B" : "0b") + r;
	}

	// Returns a string visualization of the given signed integral value, in binary.
	inline tt_string vis_int_binary(tt_long x, tt_bool uppercase = false) {


		tt_string r;

		if (x > 0)
			r = tt::vis_uint_binary(x, uppercase);
		else
			r = tt::vis_uint_binary(x * -1, uppercase),
			r.insert(r.begin(), '-');

		return r;
	}

	// Returns a string visualization of the given Unicode codepoint of the given character.
	template<typename Char>
	inline tt_string vis_bin_char_codepoint(Char x) {


		tt_string r = "[U+";

		// gotta account for negative signed values of x if Char is signed

		using _UChar = std::make_unsigned_t<Char>;

		auto xx = *(_UChar*)&x;

		r += tt::vis_uint_hex((tt_ulong)xx);
		
		return r + "]";
	}

	// Returns a string visualization of the given character, interpreting it as a Unicode codepoint if it's not printable ASCII, or special strings for certain codepoints, like '[CR]' for carriage returns.
	template<typename Char>
	inline tt_string vis_bin_char(Char x) {


		switch ((tt_char32)x) {


		// the below two BOM ones are problematic as our native byte order causes them to evaluate incorrectly, and
		// furthermore, the character size makes manually building the correct values *problematic*

		// thusly, I've excluded them

		//case U'\xfeff': return "[BOM]"; break;
		//case U'\xfffe': return "[ANTI-BOM]"; break;

		case U'\0': return "[NUL]"; break;
		case U'\a': return "[BEL]"; break;
		case U'\b': return "[BS]"; break;
		case U'\t': return "[HT]"; break;
		case U'\n': return "[LF]"; break;
		case U'\v': return "[VT]"; break;
		case U'\f': return "[FF]"; break;
		case U'\r': return "[CR]"; break;

		default: break;
		}

		if (tt::in_printable_ascii((tt_size)x))
			return tt_string(1, (tt_char)x);
		else
			return tt::vis_bin_char_codepoint(x);
	}

	// Returns a string visualization of the given string, interpreting its characters via tt::vis_bin_char.
	// This is intended to provide a visualization of the binary of a string, including all hidden characters.
	// If multiline is true, a newline will be inserted following every line-feed (aka. '\n') encountered.
	template<typename Char>
	inline tt_string vis_bin_string(std::basic_string_view<Char> x, tt_bool multiline = true) {


		tt_string r;

		for (auto& I : x) {


			r += tt::vis_bin_char<Char>(I);

			if (multiline && I == (Char)'\n')
				r += (Char)'\n';
		}

		return r;
	}

	// Returns a string visualization of the given string, interpreting its characters via tt::vis_bin_char.
	// This is intended to provide a visualization of the binary of a string, including all hidden characters.
	// If multiline is true, a newline will be inserted following every line-feed (aka. '\n') encountered.
	template<typename Char>
	inline tt_string vis_bin_string(const std::basic_string<Char>& x, tt_bool multiline = true) {


		return tt::vis_bin_string<Char>((std::basic_string_view<Char>)x, multiline);
	}

	// Returns a string visualization of the given string, interpreting its characters via tt::vis_bin_char.
	// This is intended to provide a visualization of the binary of a string, including all hidden characters.
	// If multiline is true, a newline will be inserted following every line-feed (aka. '\n') encountered.
	template<typename Char>
	inline tt_string vis_bin_string(const Char* const x, tt_bool multiline = true) {


		return tt::vis_bin_string<Char>(std::basic_string_view<Char>(x), multiline);
	}

	// Returns a string visualization of the given string, using tt::vis_bin_string if binary is true.
	template<typename Char>
	inline tt_string vis_string(std::basic_string_view<Char> x, tt_bool binary = false, tt_bool multiline_if_binary = true) {


		tt_string r{};

		if (!binary) {


			r.reserve(x.length());

			for (auto& I : x)
				r += (tt_char)I;
		}
		else
			r = tt::vis_bin_string(x, multiline_if_binary);

		return r;
	}

	// Returns a string visualization of the given string, using tt::vis_bin_string if binary is true.
	template<typename Char>
	inline tt_string vis_string(const std::basic_string<Char>& x, tt_bool binary = false, tt_bool multiline_if_binary = true) {


		return tt::vis_string<Char>((std::basic_string_view<Char>)x, binary, multiline_if_binary);
	}

	// Returns a string visualization of the given string, using tt::vis_bin_string if binary is true.
	template<typename Char>
	inline tt_string vis_string(const Char* const x, tt_bool binary = false, tt_bool multiline_if_binary = true) {


		return tt::vis_string<Char>(std::basic_string_view<Char>(x), binary, multiline_if_binary);
	}

	// Returns a string visualization of the given iterable class, iterating it via a range-for loop.
	// If multiline is true, the string returned will be formatted to span multiple lines, using tab for indenting.
	template<typename Iterable>
	inline tt_string vis_iterable(const Iterable& x, tt_bool multiline = false, const tt_char* const tab = "    ") {


		tt_string r = multiline ? "{" : "{ ";

		if (multiline)
			r += '\n',
			r += tab;

		tt_bool _not_first = false;

		for (auto& I : x) {


			if (_not_first) {


				if (multiline)
					r += ",\n",
					r += tab;
				else
					r += ", ";
			}
			else
				_not_first = true;

			r += tt::vis(I);
		}

		r += multiline ? "\n}" : " }";

		return r;
	}
}

