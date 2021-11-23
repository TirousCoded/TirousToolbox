

#pragma once


// A header file of common components used in encoding/decoding UTF-8, 16, and 32.


#include "aliases.h"


namespace tt {


	// An enumeration of basic Unicode text encodings.
	enum class utf_encoding : tt_byte {

		// UTF-8 & ASCII
		UTF8,

		// UTF-16
		UTF16,

		// UTF-32
		UTF32
	};

	// A struct representing a Unicode codepoint encoded using UTF-8/16/32.
	struct encoded_unit final {

		// The Unicode encoding used to try and resolve this encoded unit.
		utf_encoding encoding = tt::utf_encoding::UTF8;

		// The byte order used to encode this encoded unit.
		// In UTF-8, this value is not meaningful.
		tt::endian byte_order = tt::endian::BIG;

		// The number of bytes per multi-byte segment of this encoded unit's encoding.
		tt_size segment_bytes = 1;

		// The binary representation of the encoded unit.
		// This buffer is sized to fit the largest size possible for the encoding, which for all UTF encodings is four bytes.
		tt_char data[4] = {};

		// The number of multi-byte segments taken up by the encoded unit in 'data'.
		tt_size segments = 0;

		// Returns the number of bytes taken up by the encoded unit in data.
		inline tt_size bytes() const noexcept { return segments * segment_bytes; }
	};

	// A struct representing a Unicode codepoint decoded using UTF-8/16/32.
	struct decoded_unit final {

		// The Unicode encoding used to try and resolve this decoded unit.
		utf_encoding encoding = tt::utf_encoding::UTF8;

		// The byte order used to decode this decoded unit.
		// In UTF-8, this value is not meaningful.
		tt::endian byte_order = tt::endian::BIG;

		// The number of bytes per multi-byte segment of this decoded unit's encoding.
		tt_size segment_bytes = 1;

		// If the decoded unit represents a successful decoding.
		// If this is false, the encoded unit represents either a decoding failure or is in its default initialized state.
		tt_bool success = false;

		// If the decoded unit failed to decode properly due to its value representing an *overlong* UTF-8 sequence.
		// Outside of UTF-8, this value is not meaningful.
		tt_bool overlong = false;

		// The Unicode codepoint value decoded.
		tt_unichar value = (tt_unichar)'\0';

		// The number of multi-byte segments used to decode 'value'.
		tt_size segments = 0;

		// Returns the number of bytes used to decode 'value'.
		inline tt_size bytes() const noexcept { return segments * segment_bytes; }
	};
}

