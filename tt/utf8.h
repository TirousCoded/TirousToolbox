

#pragma once


// A header file defining low-level UTF-8 encoding/decoding utilities.


#include "aliases.h"
#include "debug.h"

#include "endian.h"

#include "unicode.h"

#include "utf_common.h"


namespace tt {


	// The number of bytes per UTF-8 multi-byte segment.
	constexpr tt_size utf8_segment_bytes = 1;

	// The maximum number of multi-byte segments a UTF-8 encoded character might require to describe a codepoint.
	constexpr tt_size utf8_max_segments = 4;

	// Returns the number of UTF-8 multi-byte encoding segments are required to encode x.
	// The x value passed will be cleaned using tt::unicode_clean, overflowing x if it exceeds the Unicode codespace.
	constexpr tt_size count_utf8_segments(tt_unichar x) noexcept {


		auto _x = tt::unicode_clean(x);

		if (tt::in_codepoint_range(_x, 0x0000, 0x007f))			return 1;
		else if (tt::in_codepoint_range(_x, 0x0080, 0x07ff))	return 2;
		else if (tt::in_codepoint_range(_x, 0x0800, 0xffff))	return 3;
		else if (tt::in_codepoint_range(_x, 0x10000, 0x10ffff))	return 4;

		else tt_assert_bad; // <- cleaning x should result in us NEVER reaching this far

		return 0;
	}

	// Returns the number of UTF-8 encoded bytes are required to encode x.
	// The x value passed will be cleaned using tt::unicode_clean, overflowing x if it exceeds the Unicode codespace.
	constexpr tt_size count_utf8_bytes(tt_unichar x) noexcept { return count_utf8_segments(x) * 1; }

	// Attempts to encode the given value using UTF-8, returning an encoded unit encapsulating the results.
	// The x value passed will be cleaned using tt::unicode_clean, overflowing x if it exceeds the Unicode codespace.
	inline tt::encoded_unit encode_utf8(tt_unichar x) noexcept {


		auto _x = tt::unicode_clean(x);
		auto s = tt::count_utf8_segments(_x);

		tt::encoded_unit r{};

		r.encoding = tt::utf_encoding::UTF8;
		r.segments = s;
		r.segment_bytes = utf8_segment_bytes;

		TT_FOR(i, 4)
			r.data[i] = 0;

		if (s == 1) {


			r.data[0] = (tt_char)(_x & 0b01111111UL);
		}

		else if (s == 2) {


			r.data[0] = (tt_char)(0b11000000UL | ((_x >> 6) & 0b00011111UL));
			r.data[1] = (tt_char)(0b10000000UL | (_x & 0b00111111UL));
		}

		else if (s == 3) {


			r.data[0] = (tt_char)(0b11100000UL | ((_x >> 12) & 0b00001111UL));
			r.data[1] = (tt_char)(0b10000000UL | ((_x >> 6) & 0b00111111UL));
			r.data[2] = (tt_char)(0b10000000UL | (_x & 0b00111111UL));
		}

		else if (s == 4) {


			r.data[0] = (tt_char)(0b11110000UL | ((_x >> 18) & 0b00000111UL));
			r.data[1] = (tt_char)(0b10000000UL | ((_x >> 12) & 0b00111111UL));
			r.data[2] = (tt_char)(0b10000000UL | ((_x >> 6) & 0b00111111UL));
			r.data[3] = (tt_char)(0b10000000UL | (_x & 0b00111111UL));
		}

		else tt_assert_bad;

		return r;
	}

	// Attempts to decode a Unicode codepoint from the given memory using UTF-8, returning a decoded unit encapsulating the results.
	// The parameter n dictates the number of bytes following x inside which decoding may take place, with nothing beyond this limit being considered.
	template<typename T>
	inline tt::decoded_unit decode_utf8(const T* const x, tt_size n = 4) noexcept {


		tt::decoded_unit err{};

		err.encoding = tt::utf_encoding::UTF8;
		err.segment_bytes = utf8_segment_bytes;

		if (n == 0)
			return err;

		// get the 'head' byte of the UTF-8 encoded sequence, from which we'll derive the length of the
		// sequence, and thus the bytes involved, afterwhich we'll begin decoding them

		tt_char _bytes[4];

		auto& _head = _bytes[0];

		_head = TT_REINTERPRET(tt_char, *x);

		// measure expected length of the UTF-8 encoded sequence
		// we count this as a matter of body bytes, since we already process the head to find it

		tt_size _expected_body_bytes{};

		// gotta be sure to do this in reverse order to avoid potential false positives

		if ((_head & 0b11111000UL) == 0b11110000UL)

			// mask out all but the bits we need going forward, to make decoding easier

			_head &= 0b00000111UL,
			_expected_body_bytes = 3;

		else if ((_head & 0b11110000UL) == 0b11100000UL)

			_head &= 0b00001111UL,
			_expected_body_bytes = 2;

		else if ((_head & 0b11100000UL) == 0b11000000UL)

			_head &= 0b00011111UL,
			_expected_body_bytes = 1;

		// if we find an invalid body byte of the encoded sequence, then the whole encoded unit is invalid

		else if ((_head & 0b11000000UL) == 0b10000000UL)
			return err;

		// account for pure ASCII

		else if ((_head & 0b10000000UL) == 0b00000000UL)

			_head &= 0b01111111UL,
			_expected_body_bytes = 0;

		// if it's not an out-of-place body byte, but also not otherwise valid, then the sequence is invalid

		else return err;

		// with the sequence head tested and processed, now we extract test and process the body bytes

		// test to ensure we can actually decode this value, whether the data is valid or not

		if (n < _expected_body_bytes + 1)
			return err;

		// with bounds testing done, we now extract our 1 to 3 new bytes, and test/process them

		TT_FOR(i, _expected_body_bytes) {


			_bytes[i + 1] = TT_REINTERPRET(tt_char, x[i + 1]);

			// test to ensure the byte has the proper UTF-8 body byte bit pattern elements required
			// with the whole sequence being deemed invalid if even one of them doesn't

			if ((_bytes[i + 1] & 0b11000000UL) != 0b10000000UL)
				return err;

			// mask out all but the bits we need going forward, to make decoding easier

			_bytes[i + 1] &= 0b00111111UL;
		}

		// having processed all our bytes, and having survived this long, we can now consider the
		// semantics of our resultant byte values, assembling our final value, and testing for whether
		// or not the sequence may be overlong

		tt_size _len = _expected_body_bytes + 1;

		// decode our final value, ignoring if the sequence is overlong for now

		tt_unichar _final = 0;

		TT_FOR(i, _len)
			_final <<= 6,
			_final |= (tt_unichar)_bytes[i];

		// measure length of what it should take to contain our final value, if this value is less
		// than the actual amount which encoded it, then the sequence is overlong, and thus invalid

		tt_size _expected_len = tt::count_utf8_bytes(_final);

		if (_len > _expected_len) {


			err.overlong = true;

			return err;
		}
		
		tt_assert(_len == _expected_len);

		// with our final value resolved and validated, we can now package it up and return

		tt::decoded_unit r{};
		
		r.encoding = tt::utf_encoding::UTF8;
		r.success = true;
		r.segments = _len;
		r.segment_bytes = utf8_segment_bytes;
		r.value = _final;

		return r;
	}
}

