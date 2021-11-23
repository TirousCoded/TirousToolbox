

#pragma once


// A header file defining low-level UTF-16 encoding/decoding utilities.


#include "aliases.h"
#include "debug.h"

#include "endian.h"

#include "unicode.h"

#include "utf_common.h"


namespace tt {


	// The number of bytes per UTF-16 multi-byte segment.
	constexpr tt_size utf16_segment_bytes = 2;

	// The maximum number of multi-byte segments a UTF-16 encoded character might require to describe a codepoint.
	constexpr tt_size utf16_max_segments = 2;

	// Returns the number of UTF-16 multi-byte encoding segments are required to encode x.
	// The x value passed will be cleaned using tt::unicode_clean, overflowing x if it exceeds the Unicode codespace.
	constexpr tt_size count_utf16_segments(tt_unichar x) noexcept {


		auto _x = tt::unicode_clean(x);

		if (_x < 0x10000)
			return 1;
		else
			return 2;
	}

	// Returns the number of UTF-16 encoded bytes are required to encode x.
	// The x value passed will be cleaned using tt::unicode_clean, overflowing x if it exceeds the Unicode codespace.
	constexpr tt_size count_utf16_bytes(tt_unichar x) noexcept { return count_utf16_segments(x) * 2; }

	// Attempts to encode the given value using UTF-16, returning an encoded unit encapsulating the results.
	// The x value passed will be cleaned using tt::unicode_clean, overflowing x if it exceeds the Unicode codespace.
	inline tt::encoded_unit encode_utf16(tt_unichar x, tt::endian byte_order = tt::endian::NATIVE) noexcept {


		auto _x = tt::unicode_clean(x);
		auto s = tt::count_utf16_segments(_x);

		auto bo = tt::get_endian(byte_order);

		tt::encoded_unit r{};

		r.encoding = tt::utf_encoding::UTF16;
		r.byte_order = bo;
		r.segments = s;
		r.segment_bytes = utf16_segment_bytes;

		tt_assert(s == 1 || s == 2);

		if (s == 1) {


			auto y = (tt_char16)_x;

			tt::write_to(y, r.data, bo);
		}

		else {


			auto _high = (tt_char16)((_x >> 10) & 0b00000000000000000000001111111111);
			auto _low = (tt_char16)(_x & 0b00000000000000000000001111111111);

			_high |= 0b1101100000000000;
			_low |= 0b1101110000000000;

			tt::write_to(_high, r.data, bo);
			tt::write_to(_low, r.data + 1, bo);
		}

		return r;
	}

	// Attempts to decode a Unicode codepoint from the given memory using UTF-16, returning a decoded unit encapsulating the results.
	// The parameter n dictates the number of bytes following x inside which decoding may take place, with nothing beyond this limit being considered.
	template<typename T>
	inline tt::decoded_unit decode_utf16(const T* const x, tt_size n = 4, tt::endian byte_order = tt::endian::NATIVE) noexcept {


		tt::decoded_unit err{};

		err.encoding = tt::utf_encoding::UTF16;
		err.byte_order = byte_order;
		err.segment_bytes = utf16_segment_bytes;

		if (n < 2)
			return err;

		auto bo = tt::get_endian(byte_order);

		// test if we can even try getting multi-byte values

		tt_bool _multi = n >= 4;

		// extract high/low surrogates, or just high surrogate half if we can't expect actual multi-byte values

		tt_char16 _high_half, _low_half = 0;

		_high_half = tt::read<tt_char16>(x, bo);

		if (_multi)
			_low_half = tt::read<tt_char16>(((tt_char16*)x) + 1, bo);

		// extract the 'head' of each of our surrogate halfs (even if low isn't gonna be used)

		tt_char16 _high_head = _high_half & ((tt_char16)0b1111110000000000U);
		tt_char16 _low_head = _low_half & ((tt_char16)0b1111110000000000U);

		// if we encounter a low surrogate for our high surrogate half, then this is a malformed UTF-16 character

		if (_high_head == (tt_char16)0b1101110000000000U)
			return err;

		// if our high surrogate half really is a high surrogate, then we can expect that this SHOULD be a mult-byte value
		// otherwise, we can thusly expect that this SHOULDN'T be a multi-byte value

		tt_bool _should_be_multi = _high_head == (tt_char16)0b1101100000000000U;

		// if we SHOULD expect a multi-byte value, then us NOT being able to decode a multi-byte value, or us being
		// able to, but finding no valid low surrogate half to match, means that this is a malformed UTF-16 character

		if (_should_be_multi) {


			if (!_multi)
				return err;

			if (_low_head != (tt_char16)0b1101110000000000U)
				return err;
		}

		// if we succeed up to this point, then with all the info we've collected, we can successfully decode a value

		tt::decoded_unit r{};

		r.encoding = tt::utf_encoding::UTF16;
		r.byte_order = bo;
		r.success = true;
		r.segments = _should_be_multi ? 2 : 1;
		r.segment_bytes = utf16_segment_bytes;

		if (_should_be_multi) {


			auto _high_val = tt::read<tt_char16>(x, bo);
			auto _low_val = tt::read<tt_char16>(((tt_char16*)x) + 1, bo);

			_high_val &= (tt_char16)0b0000001111111111U;
			_low_val &= (tt_char16)0b0000001111111111U;

			r.value = 0;

			r.value |= (tt_unichar)_high_val;

			r.value <<= 10;

			r.value |= (tt_unichar)_low_val;
		}

		else {


			r.value = tt::read<tt_char16>(x, bo);
		}

		return r;
	}
}

