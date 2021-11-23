

#pragma once


// A header file defining the text encodings available in the Tirous Toolbox library.


#include "aliases.h"
#include "exceptions.h"

#include "endian.h"

#include "slice.h"

#include "utf_common.h"
#include "bom.h"


namespace tt {


	// A struct representing a fully defined text encoding.
	struct text_encoding final {

		// The base Unicode encoding of the text encoding.
		tt::utf_encoding base = tt::utf_encoding::UTF8;

		// The byte order of the text encoding.
		// If the encoding represented is UTF-8 based, or is a BOM encoding, this field is not meaningful.
		tt::endian byte_order = tt::endian::BIG;

		// If the encoding is a BOM encoding with unresolved BOM information.
		tt_bool bom_encoding = false;

		// Returns a text encoding of this with BOM information resolved.
		// Overwrites the byte order of this encoding if this encoding wasn't a BOM encoding.
		inline tt::text_encoding resolve(tt::endian byte_order) const noexcept {


			tt::text_encoding r{};

			r.base = base;
			r.byte_order = byte_order;
			r.bom_encoding = false;

			return r;
		}

		// Returns the number of bytes per multi-byte segment of the encoding.
		inline tt_size segment_bytes() const noexcept {


			tt_size r{};

			switch (base) {


			case tt::utf_encoding::UTF8: return tt::utf8_segment_bytes; break;
			case tt::utf_encoding::UTF16: return tt::utf16_segment_bytes; break;
			case tt::utf_encoding::UTF32: return tt::utf32_segment_bytes; break;

			default: tt_assert_bad; break;
			}

			return r;
		}

		// Returns the maximum number of multi-byte segments a codepoint might require to be encoded by the encoding.
		inline tt_size max_segments() const noexcept {


			tt_size r{};

			switch (base) {


			case tt::utf_encoding::UTF8: return tt::utf8_segment_bytes; break;
			case tt::utf_encoding::UTF16: return tt::utf16_segment_bytes; break;
			case tt::utf_encoding::UTF32: return tt::utf32_segment_bytes; break;

			default: tt_assert_bad; break;
			}

			return r;
		}

		// Returns the number of multi-byte segments are required to encode codepoint x using this encoding.
		// The x value passed will be cleaned using tt::unicode_clean, removing its leading 11 bits.
		inline tt_size count_segments(tt_unichar x) const noexcept {


			tt_size r{};

			switch (base) {


			case tt::utf_encoding::UTF8: return tt::count_utf8_segments(x); break;
			case tt::utf_encoding::UTF16: return tt::count_utf16_segments(x); break;
			case tt::utf_encoding::UTF32: return tt::count_utf32_segments(x); break;

			default: tt_assert_bad; break;
			}

			return r;
		}

		// Returns the number of bytes required to encode codepoint x using this encoding.
		// The x value passed will be cleaned using tt::unicode_clean, removing its leading 11 bits.
		inline tt_size count_bytes(tt_unichar x) const noexcept {


			return count_segments(x) * segment_bytes();
		}
		
		// Attempts to encode the given value using the encoding, returning an encoded unit encapsulating the results.
		// The x value passed will be cleaned using tt::unicode_clean, removing its leading 11 bits.
		// Throws tt::bom_encoding_error if this encoding contains unresolved BOM information, providing a strong guarantee.
		inline tt::encoded_unit encode_unit(tt_unichar x) const {


			if (bom_encoding)
				TT_THROW(tt::bom_encoding_error, "Cannot encode using encoding with unresolved BOM information!");

			tt::encoded_unit r{};

			switch (base) {


			case tt::utf_encoding::UTF8: r = tt::encode_utf8(x); break;
			case tt::utf_encoding::UTF16: r = tt::encode_utf16(x, byte_order); break;
			case tt::utf_encoding::UTF32: r = tt::encode_utf32(x, byte_order); break;

			default: tt_assert_bad; break;
			}

			return r;
		}

		// Attempts to decode a Unicode codepoint from the given memory using the encoding, returning a decoded unit encapsulating the results.
		// The parameter n dictates the number of bytes following x inside which decoding may take place, with nothing beyond this limit being considered.
		// Throws tt::bom_encoding_error if this encoding contains unresolved BOM information, providing a strong guarantee.
		template<typename T>
		inline tt::decoded_unit decode_unit(const T* const x, tt_size n = 4) const {


			if (bom_encoding)
				TT_THROW(tt::bom_encoding_error, "Cannot decode using encoding with unresolved BOM information!");

			tt::decoded_unit r{};

			switch (base) {


			case tt::utf_encoding::UTF8: r = tt::decode_utf8(x, n); break;
			case tt::utf_encoding::UTF16: r = tt::decode_utf16(x, n, byte_order); break;
			case tt::utf_encoding::UTF32: r = tt::decode_utf32(x, n, byte_order); break;

			default: tt_assert_bad; break;
			}

			return r;
		}

		// Tests if (n bytes long) buffer x starts with the BOM associated with the encoding.
		inline tt::bom_result test_bom(const tt_byte* const x, tt_size n) const noexcept {


			tt::bom_result r{};

			switch (base) {


			case tt::utf_encoding::UTF8: r = tt::test_utf8_bom(x, n); break;
			case tt::utf_encoding::UTF16: r = tt::test_utf16_bom(x, n); break;
			case tt::utf_encoding::UTF32: r = tt::test_utf32_bom(x, n); break;

			default: tt_assert_bad; break;
			}

			return r;
		}

		// Returns the number of bytes taken up by this encoding's associated BOM.
		inline tt_size bom_byte_count() const noexcept {


			tt_size r = 0;

			switch (base) {


			case tt::utf_encoding::UTF8: r = tt::utf8_bom_byte_count; break;
			case tt::utf_encoding::UTF16: r = tt::utf16_bom_byte_count; break;
			case tt::utf_encoding::UTF32: r = tt::utf32_bom_byte_count; break;

			default: tt_assert_bad; break;
			}

			return r;
		}

		// Returns a slice of the byte array of this encoding's associated BOM.
		inline tt::slice<const tt_byte> bom_byte_slice(tt::endian byte_order = tt::endian::NATIVE) const noexcept {


			tt::slice<const tt_byte> r = {};

			if (tt::is_big_endian(byte_order)) {


				switch (base) {


				case tt::utf_encoding::UTF8: r = tt::slice_of((const tt_byte*)tt::utf8_bom_bytes, tt::utf8_bom_byte_count); break;
				case tt::utf_encoding::UTF16: r = tt::slice_of((const tt_byte*)tt::utf16_bom_bytes, tt::utf16_bom_byte_count); break;
				case tt::utf_encoding::UTF32: r = tt::slice_of((const tt_byte*)tt::utf32_bom_bytes, tt::utf32_bom_byte_count); break;

				default: tt_assert_bad; break;
				}
			}

			else if (tt::is_little_endian(byte_order)) {


				switch (base) {


				case tt::utf_encoding::UTF8: r = tt::slice_of((const tt_byte*)tt::utf8_bom_bytes, tt::utf8_bom_byte_count); break;
				case tt::utf_encoding::UTF16: r = tt::slice_of((const tt_byte*)tt::utf16_antibom_bytes, tt::utf16_bom_byte_count); break;
				case tt::utf_encoding::UTF32: r = tt::slice_of((const tt_byte*)tt::utf32_antibom_bytes, tt::utf32_bom_byte_count); break;

				default: tt_assert_bad; break;
				}
			}

			else tt_assert_bad;

			return r;
		}

		// Returns a text encoding representing UTF-8.
		static inline tt::text_encoding utf8() noexcept {


			return { tt::utf_encoding::UTF8, tt::endian::NATIVE };
		}

		// Returns a text encoding representing UTF-8 BOM.
		static inline tt::text_encoding utf8_bom() noexcept {


			return { tt::utf_encoding::UTF8, {}, true };
		}

		// Returns a text encoding representing UTF-16 (native byte order.)
		static inline tt::text_encoding utf16() noexcept {


			return { tt::utf_encoding::UTF16, tt::endian::NATIVE };
		}

		// Returns a text encoding representing UTF-16 BE.
		static inline tt::text_encoding utf16_be() noexcept {


			return { tt::utf_encoding::UTF16, tt::endian::BIG };
		}

		// Returns a text encoding representing UTF-16 LE.
		static inline tt::text_encoding utf16_le() noexcept {


			return { tt::utf_encoding::UTF16, tt::endian::LITTLE };
		}

		// Returns a text encoding representing UTF-16 BOM.
		static inline tt::text_encoding utf16_bom() noexcept {


			return { tt::utf_encoding::UTF16, {}, true };
		}

		// Returns a text encoding representing UTF-32 (native byte order.)
		static inline tt::text_encoding utf32() noexcept {


			return { tt::utf_encoding::UTF32, tt::endian::NATIVE };
		}

		// Returns a text encoding representing UTF-32 BE.
		static inline tt::text_encoding utf32_be() noexcept {


			return { tt::utf_encoding::UTF32, tt::endian::BIG };
		}

		// Returns a text encoding representing UTF-32 LE.
		static inline tt::text_encoding utf32_le() noexcept {


			return { tt::utf_encoding::UTF32, tt::endian::LITTLE };
		}

		// Returns a text encoding representing UTF-32 BOM.
		static inline tt::text_encoding utf32_bom() noexcept {


			return { tt::utf_encoding::UTF32, {}, true };
		}
	};
}

