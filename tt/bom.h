

#pragma once


// A header file of utilities used to detect the Byte Order Marker (BOM) of various text encodings.


#include "aliases.h"

#include "endian.h"
#include "memory_util.h"


namespace tt {


	// UTF-8's BOM is just UTF-16's BOM 0xfeff encoded in UTF-8, thus meaning that it will always come in the 
	// form [0xef, 0xbb, 0xbf], and never the reverse form of [0xbf, 0xbb, 0xef].

	// When parsing BOM-less UTF-8, if a BOM is encountered up front, it is to be ignored.

	// Scroll near the bottom of https://www.unicode.org/faq/utf_bom.html for a useful table of UTF encodings' BOMs.

	// The UTF-8 BOM value is just the UTF-16 BOM 0xfeff encoded in UTF-8, producing [0xef, 0xbb, 0xbf].

	// For UTF-16, expect a BOM of 0xfeff to indicate UTF-16 BE, and thus 0xfffe for UTF-16 LE, and neither to 
	// indicate non-UTF-16 BOM encoding.

	// For UTF-32, it's like with UTF-16 BOM but with 0x0000feff and 0xfffe0000 corresponding to UTF-32 BE and 
	// UTF-32 LE, respectively, and neither being non-UTF-32 BOM encoding.

	// The byte sequence of the encoded UTF-8 BOM.
	constexpr tt_byte utf8_bom_bytes[3] = { 0xef, 0xbb, 0xbf };

	// The byte sequence of the encoded UTF-16 BOM, implying UTF-16 BE if met.
	constexpr tt_byte utf16_bom_bytes[2] = { 0xfe, 0xff };

	// The byte sequence of the encoded UTF-16 anti-BOM, implying UTF-16 LE if met.
	constexpr tt_byte utf16_antibom_bytes[2] = { 0xff, 0xfe };

	// The byte sequence of the encoded UTF-32 BOM, implying UTF-32 BE if met.
	constexpr tt_byte utf32_bom_bytes[4] = { 0x00, 0x00, 0xfe, 0xff };

	// The byte sequence of the encoded UTF-32 anti-BOM, implying UTF-32 LE if met.
	constexpr tt_byte utf32_antibom_bytes[4] = { 0xff, 0xfe, 0x00, 0x00 };

	// Returns the number of bytes taken up by the UTF-8 BOM.
	constexpr tt_size utf8_bom_byte_count = 3;

	// Returns the number of bytes taken up by the UTF-16 BOM.
	constexpr tt_size utf16_bom_byte_count = 2;

	// Returns the number of bytes taken up by the UTF-32 BOM.
	constexpr tt_size utf32_bom_byte_count = 4;

	// A struct representing the result of a BOM test.
	struct bom_result final {

		// If the BOM test was successful.
		tt_bool success = false;

		// The endianness resolved by the BOM test.
		// If success is false, this value is not meaningful.
		tt::endian byte_order = tt::endian::BIG;

		// The number of bytes taken up by the detected BOM, if any.
		tt_size bytes = 0;
	};

	// Tests if (n bytes long) buffer x starts with the UTF-8 BOM or not.
	inline tt::bom_result test_utf8_bom(const tt_byte* const x, tt_size n) noexcept {


		tt::bom_result r{};

		if (n >= utf8_bom_byte_count)
			if (tt::equal_arrays(x, (const tt_byte*)utf8_bom_bytes, utf8_bom_byte_count))
				r.success = true,
				r.byte_order = tt::get_native_endian(),
				r.bytes = utf8_bom_byte_count;

		return r;
	}

	// Tests if (n bytes long) buffer x starts with the UTF-16 BOM or not.
	inline tt::bom_result test_utf16_bom(const tt_byte* const x, tt_size n) noexcept {


		tt::bom_result r{};

		if (n >= utf16_bom_byte_count) {


			if (tt::equal_arrays(x, (const tt_byte*)utf16_bom_bytes, utf16_bom_byte_count))
				r.success = true,
				r.byte_order = tt::endian::BIG,
				r.bytes = utf16_bom_byte_count;

			else if (tt::equal_arrays(x, (const tt_byte*)utf16_antibom_bytes, utf16_bom_byte_count))
				r.success = true,
				r.byte_order = tt::endian::LITTLE,
				r.bytes = utf16_bom_byte_count;
		}

		return r;
	}

	// Tests if (n bytes long) buffer x starts with the UTF-32 BOM or not.
	inline tt::bom_result test_utf32_bom(const tt_byte* const x, tt_size n) noexcept {


		tt::bom_result r{};

		if (n >= utf32_bom_byte_count) {


			if (tt::equal_arrays(x, (const tt_byte*)utf32_bom_bytes, utf32_bom_byte_count))
				r.success = true,
				r.byte_order = tt::endian::BIG,
				r.bytes = utf32_bom_byte_count;

			else if (tt::equal_arrays(x, (const tt_byte*)utf32_antibom_bytes, utf32_bom_byte_count))
				r.success = true,
				r.byte_order = tt::endian::LITTLE,
				r.bytes = utf32_bom_byte_count;
		}

		return r;
	}
}

