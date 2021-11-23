

#pragma once


// A header file of utilities for translating text buffers between encodings.


#include "aliases.h"

#include "endian.h"

#include "chunk.h"

#include "bom.h"

#include "text_encoding.h"

#include "text_encoder.h"
#include "text_decoder.h"


namespace tt {


	// A struct representing an attempted text encoding translation.
	struct translated_text final {

		// If decoding as UTF-8 the translation process detected, and promptly skipped, a UTF-8 BOM marker.
		tt_bool skipped_utf8_bom = false;

		// If the translation failed due to a BOM error where the encoding required a BOM up front, but none was detected.
		tt_bool fatal_bom_error = false;

		// The total number of invalid characters inserted into the final text due to decoding problems.
		tt_size invalid_characters = 0;

		// The resultant text of the translation, if any.
		tt::chunk<1> data = {};

		// Returns the data of the translated text object reinterpreted as a string.
		// If the translated text contains stray null terminators, the resultant string will also, which will likely cause problems.
		inline tt_string data_str() const {


			return tt_string(data.get_unchecked<tt_char>(0), data.size_bytes());
		}
	};

	// Translates the contents of x from in_encoding to out_encoding, returning the result of the translation.
	// Whenever an invalid character is detected in x, an out_encoding encoded value of err is output, and a single multi-byte segment worth of bytes in x is skipped.
	// If in_encoding is a BOM encoding, then x will be checked up front for appropriate BOM information, and translation will fail if no valid BOM, in any byte order, can be found.
	// If out_encoding is a BOM encoding, then BOM information will be written according to the endianness of out_bom_byte_order, followed by the translated contents of x, which is presumed to be in said byte order.
	inline tt::translated_text translate_text(tt::chunk_view<1> x, tt::text_encoding in_encoding, tt::text_encoding out_encoding, tt_unichar err = (tt_unichar)'?', tt::endian out_bom_byte_order = tt::endian::NATIVE) {


		tt::translated_text r{};

		tt::text_decoder td(in_encoding, x);
		tt::text_encoder te(out_encoding, &r.data);

		tt::decoded_unit du{};

		tt::bom_result br{};

		if (in_encoding.bom_encoding) {


			br = td.decode_bom();

			if (br.success)
				in_encoding = in_encoding.resolve(br.byte_order),
				td.encoding() = in_encoding;

			else {


				r.fatal_bom_error = true;

				return r;
			}
		}

		// if we're non-BOM UTF-8, then test for, and if found skip, any UTF-8 BOM found

		else if (in_encoding.base == tt::utf_encoding::UTF8 && in_encoding.test_bom(x.get_byte_unchecked(0), x.size_bytes()).success)
			r.skipped_utf8_bom = true,
			td.skip(in_encoding.bom_byte_count());

		if (out_encoding.bom_encoding)
			te.encode_bom(out_bom_byte_order),
			out_encoding = out_encoding.resolve(out_bom_byte_order),
			te.encoding() = out_encoding;

		while (!td.at_end()) {


			du = td.decode();

			if (du.success)
				te.encode(du.value);
			else
				++r.invalid_characters,
				te.encode(err),
				td.skip(in_encoding.segment_bytes());
		}

		return r;
	}

	// Translates the contents of x from in_encoding to out_encoding, returning the result of the translation.
	// Whenever an invalid character is detected in x, an out_encoding encoded value of err is output, and a single multi-byte segment worth of bytes in x is skipped.
	// If in_encoding is a BOM encoding, then x will be checked up front for appropriate BOM information, and translation will fail if no valid BOM, in any byte order, can be found.
	// If out_encoding is a BOM encoding, then BOM information will be written according to the endianness of out_bom_byte_order, followed by the translated contents of x, which is presumed to be in said byte order.
	inline tt::translated_text translate_text(const tt::chunk<1>& x, tt::text_encoding in_encoding, tt::text_encoding out_encoding, tt_unichar err = (tt_unichar)'?', tt::endian out_bom_byte_order = tt::endian::NATIVE) {


		return tt::translate_text(x.view(), in_encoding, out_encoding, err, out_bom_byte_order);
	}

	// Translates the contents of (n bytes long) buffer x from in_encoding to out_encoding, returning the result of the translation.
	// Whenever an invalid character is detected in x, an out_encoding encoded value of err is output, and a single multi-byte segment worth of bytes in x is skipped.
	// If in_encoding is a BOM encoding, then x will be checked up front for appropriate BOM information, and translation will fail if no valid BOM, in any byte order, can be found.
	// If out_encoding is a BOM encoding, then BOM information will be written according to the endianness of out_bom_byte_order, followed by the translated contents of x, which is presumed to be in said byte order.
	inline tt::translated_text translate_text(const tt_byte* const x, tt_size n, tt::text_encoding in_encoding, tt::text_encoding out_encoding, tt_unichar err = (tt_unichar)'?', tt::endian out_bom_byte_order = tt::endian::NATIVE) {


		return tt::translate_text(tt::chunk_view<1>(x, n), in_encoding, out_encoding, err, out_bom_byte_order);
	}
}

