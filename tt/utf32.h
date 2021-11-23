

#pragma once


// A header file defining low-level UTF-32 encoding/decoding utilities.


#include "aliases.h"

#include "endian.h"

#include "unicode.h"

#include "utf_common.h"


namespace tt {
	

	// The number of bytes per UTF-32 multi-byte segment.
	// UTF-32 is not a multi-byte encoding, but this treats it as a trivial one to harmonize the API.
	constexpr tt_size utf32_segment_bytes = 4;

	// The maximum number of multi-byte segments a UTF-32 encoded character might require to describe a codepoint.
	constexpr tt_size utf32_max_segments = 1;

	// Returns the number of UTF-32 multi-byte encoding segments are required to encode x.
	// Since UTF-32 is not a multi-byte encoding, this will always return 1.
	// UTF-32 is not a multi-byte encoding, but this treats it as a trivial one to harmonize the API.
	constexpr tt_size count_utf32_segments(tt_unichar x) noexcept { return 1; }

	// Returns the number of UTF-32 encoded bytes are required to encode x.
	// Since UTF-32 is not a multi-byte encoding, this will always return 4.
	// UTF-32 is not a multi-byte encoding, but this treats it as a trivial one to harmonize the API.
	constexpr tt_size count_utf32_bytes(tt_unichar x) noexcept { return count_utf32_segments(x) * 4; }

	// Attempts to encode the given value using UTF-32, returning an encoded unit encapsulating the results.
	// The x value passed will be cleaned using tt::unicode_clean, overflowing x if it exceeds the Unicode codespace.
	inline tt::encoded_unit encode_utf32(tt_unichar x, tt::endian byte_order = tt::endian::NATIVE) noexcept {


		auto _x = tt::unicode_clean(x);

		auto bo = tt::get_endian(byte_order);

		tt::encoded_unit r{};

		r.encoding = tt::utf_encoding::UTF32;
		r.byte_order = bo;
		r.segments = 1;
		r.segment_bytes = utf32_segment_bytes;

		tt::write_to(_x, r.data, bo);

		return r;
	}

	// Attempts to decode a Unicode codepoint from the given memory using UTF-32, returning a decoded unit encapsulating the results.
	// The parameter n dictates the number of bytes following x inside which decoding may take place, with nothing beyond this limit being considered.
	template<typename T>
	inline tt::decoded_unit decode_utf32(const T* const x, tt_size n = 4, tt::endian byte_order = tt::endian::NATIVE) noexcept {


		tt::decoded_unit err{};

		err.encoding = tt::utf_encoding::UTF32;
		err.byte_order = byte_order;
		err.segment_bytes = utf32_segment_bytes;

		if (n < 4)
			return err;

		auto bo = tt::get_endian(byte_order);

		tt::decoded_unit r{};

		r.encoding = tt::utf_encoding::UTF32;
		r.byte_order = bo;
		r.success = true;
		r.segments = 1;
		r.segment_bytes = utf32_segment_bytes;

		tt::read_to(x, r.value, bo);

		return r;
	}
}

