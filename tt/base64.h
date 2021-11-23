

#pragma once


// The Tirous Toolbox implementation of base64 encoding/decoding.


#include "aliases.h"

#include "memory_util.h"
#include "math_util.h"


namespace tt {


	// The character set used by the Tirous Toolbox base64 implementation.
	constexpr const tt_char* const base64_charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	static_assert(tt::measure_cstr(tt::base64_charset) == 64);

	// Returns the base64 character value associated with 6-bit base64 index value x, or non-base64 character '?' if x is beyond base64's 6-bit range.
	constexpr tt_char base64_index_to_char(tt_byte x) noexcept {


		if (x <= 63)
			return base64_charset[x];
		else
			return '?';
	}

	// Returns the 6-bit base64 index value associated with base64 character x, or the out-of-range index 64 if x is not a valid base64 encoded character, or is its padding character '='.
	constexpr tt_byte base64_char_to_index(tt_char x) noexcept {


		if (x >= 'A' && x <= 'Z')
			return x - (tt_byte)'A';

		else if (x >= 'a' && x <= 'z')
			return tt_byte(26) + (x - (tt_byte)'a');

		else if (x >= '0' && x <= '9')
			return tt_byte(52) + (x - (tt_byte)'0');

		else if (x == '+')
			return tt_byte(62);

		else if (x == '/')
			return tt_byte(63);

		else return 64;
	}

	// A data struct representing up to three 8-bit values to be encoded into a 24-bit base64 unit.
	struct base64_decoded_unit final {

		// The array of three 8-bit values defining the up to 24-bits of information of the decoded unit.
		tt_byte data[3] = {};

		// The number of 8-bit values actually representing this decoded unit.
		tt_size count = 0;
	};

	// A data struct representing up to four base64 character values to be encoded into a 24-bit base64 unit.
	struct base64_encoded_unit final {

		// The array of four base64 character values defining the up to 24-bits of information of the encoded unit.
		tt_char data[4] = {};

		// The number of base64 character values actually representing this encoded unit.
		tt_size count = 0;
	};

	// Returns a base64 encoded unit representing the data of decoded unit x.
	// If x is not a valid base64 decoded unit then the returned encoded unit will have a count of zero.
	inline tt::base64_encoded_unit base64_encode_unit(tt::base64_decoded_unit x) noexcept {


		tt::base64_encoded_unit r{};

		if (x.count > 0 && x.count <= 3) {


			// discard unneeded data in x so the below binary math can safely 
			// presume the excess terms are all zero bits

			if (x.count < 2)
				x.data[1] = 0;

			if (x.count < 3)
				x.data[2] = 0;

			// build the array of four 6-bit values describing the 24-bits of base64 data

			// during this process we also resolve r's count value

			tt_byte _data[4]{};

			_data[0] = x.data[0] >> 2;
			_data[1] = ((x.data[0] & (tt_byte)0b00000011U) << 4) | (x.data[1] >> 4);

			r.count = 2;

			if (x.count >= 2)
				_data[2] = ((x.data[1] & (tt_byte)0b00001111U) << 2) | (x.data[2] >> 6),
				++r.count;
			else
				_data[2] = 0;

			if (x.count >= 3)
				_data[3] = x.data[2] & (tt_byte)0b00111111U,
				++r.count;
			else
				_data[3] = 0;

			// finish up by building the final character array of r, translating _data's 6-bit index values

			// here we also add padding to the unused array values

			TT_FOR(i, 4)
				if (i < r.count)
					r.data[i] = tt::base64_index_to_char(_data[i]),
					tt_assert(r.data[i] != '?');
				else
					r.data[i] = '=';
		}

		return r;
	}

	// Returns a base64 decoded unit representing the data of the encoded unit x.
	// If x is not a valid base64 encoded unit then the returned decoded unit will have a count of zero.
	// If the leading two bits of any of the chars of x contain bit data prior to its trailing 6-bits, this data is ignored, and does not cause the process to fail.
	inline tt::base64_decoded_unit base64_decode_unit(tt::base64_encoded_unit x) noexcept {


		tt::base64_decoded_unit r{};

		if (x.count >= 2 && x.count <= 4) {


			// translate x from an array of four base64 characters to an array of 
			// four 6-bit base64 indices

			// along the way we resolve r's final count

			// we also resolve the unused terms to zero, so the binary math below
			// can safely presume that said terms will be composed solely of zero bits

			tt_byte _data[4]{};

			_data[0] = tt::base64_char_to_index(x.data[0]);
			_data[1] = tt::base64_char_to_index(x.data[1]);

			r.count = 1;

			if (x.count >= 3)
				_data[2] = tt::base64_char_to_index(x.data[2]),
				++r.count;
			else
				_data[2] = 0;

			if (x.count >= 4)
				_data[3] = tt::base64_char_to_index(x.data[3]),
				++r.count;
			else
				_data[3] = 0;

			// if any of the array entries of x were not valid base64 characters, exit prematurely

			TT_FOR(i, 4)
				if (_data[i] >= 64) {


					r.count = 0;

					return r;
				}

			// use the above resolved information to resolve our final r data entries

			// this takes advantage of the above presumption of unused terms being composed
			// solely of zero bits to avoid having to do *count tests* like we do above,
			// meaning that if the math below resolves r.data[1] or r.data[2] in a scenario
			// where they shouldn't be, it's fine since they'll just be doing binary math
			// on all-zero values, and thus come to an all-zero conclusion

			r.data[0] = (_data[0] << 2) | (_data[1] >> 4);
			r.data[1] = ((_data[1] & (tt_byte)0b00001111U) << 4) | (_data[2] >> 2);
			r.data[2] = ((_data[2] & (tt_byte)0b00000011U) << 6) | _data[3];
		}

		return r;
	}

	// A data struct recording the details of a base64 encoding process.
	struct base64_encoded_text final {

		// The number of bytes encoded.
		tt_size bytes = 0;

		// The number of characters used to encode, including padding.
		tt_size characters = 0;

		// The final chunk generated by the encoding process.
		tt::chunk<1> data = {};

		// Returns the data of the encoded text object reinterpreted as a string.
		// If the encoded text contains stray null terminators, the resultant string will also, which will likely cause problems.
		inline tt_string data_str() const {


			return tt_string(data.get_unchecked<tt_char>(0), data.size_bytes());
		}
	};

	// Converts byte sequence x into a base64 encoded chunk.
	// Encoding stops prematurely if an invalid byte sequence segment is encountered which fails to encode.
	inline tt::base64_encoded_text base64_encode_text(tt::chunk_view<1> x) {


		tt::base64_encoded_unit eu{};
		tt::base64_decoded_unit du{};

		tt::base64_encoded_text r{};

		while (r.bytes < x.size_bytes()) {


			du.count = tt::min<tt_size>(3, x.size_bytes() - r.bytes);

			tt::copy_block(x.get_byte_unchecked(r.bytes), (tt_byte*)du.data, du.count);

			eu = tt::base64_encode_unit(du);

			if (eu.count == 0)
				break;

			r.data.grow_by(eu.count);
			
			tt::copy_block((const tt_char*)eu.data, r.data.get_unchecked<tt_char>(r.characters), eu.count);
			
			r.bytes += du.count;
			r.characters += eu.count;
		}

		return r;
	}

	// Converts byte sequence x into a base64 encoded chunk.
	// Encoding stops prematurely if an invalid byte sequence segment is encountered which fails to encode.
	inline tt::base64_encoded_text base64_encode_text(const tt::chunk<1>& x) {


		return tt::base64_encode_text(x.view());
	}

	// Converts (n bytes long) byte sequence x into a base64 encoded chunk.
	// Encoding stops prematurely if an invalid byte sequence segment is encountered which fails to encode.
	inline tt::base64_encoded_text base64_encode_text(const tt_byte* const x, tt_size n) {


		return tt::base64_encode_text(tt::chunk_view<1>(x, n));
	}

	// Converts byte sequence x (here as a null terminated C-string) into a base64 encoded chunk.
	// Encoding stops prematurely if an invalid byte sequence segment is encountered which fails to encode.
	inline tt::base64_encoded_text base64_encode_text(const tt_char* const x) {


		return tt::base64_encode_text((const tt_byte*)x, tt::measure_cstr(x));
	}

	// Converts byte sequence x (here as the contents of a string) into a base64 encoded chunk.
	// Encoding stops prematurely if an invalid byte sequence segment is encountered which fails to encode.
	inline tt::base64_encoded_text base64_encode_text(tt_string_view x) {


		return tt::base64_encode_text((const tt_byte*)x.data(), x.length());
	}

	// Converts byte sequence x (here as the contents of a string) into a base64 encoded chunk.
	// Encoding stops prematurely if an invalid byte sequence segment is encountered which fails to encode.
	inline tt::base64_encoded_text base64_encode_text(const tt_string& x) {


		return tt::base64_encode_text((tt_string_view)x);
	}

	// A data struct recording the details of a base64 decoding process.
	struct base64_decoded_text final {

		// The number of bytes decoded.
		tt_size bytes = 0;

		// The number of characters processed during decoding, including padding.
		tt_size characters = 0;

		// The final chunk generated by the decoding process.
		tt::chunk<1> data = {};

		// Returns the data of the decoded text object reinterpreted as a string.
		// If the decoded text contains stray null terminators, the resultant string will also, which will likely cause problems.
		inline tt_string data_str() const {


			return tt_string(data.get_unchecked<tt_char>(0), data.size_bytes());
		}
	};

	// Converts base64 encoded character sequence x into a base64 decoded chunk.
	// Decoding stops prematurely if an invalid byte sequence segment is encountered which fails to decode.
	inline tt::base64_decoded_text base64_decode_text(tt::chunk_view<1> x) {


		tt::base64_encoded_unit eu{};
		tt::base64_decoded_unit du{};

		tt::base64_decoded_text r{};

		while (r.characters < x.size_bytes()) {


			eu.count = tt::min<tt_size>(4, x.size_bytes() - r.characters);

			tt::copy_block((const tt_char*)x.get_byte_unchecked(r.characters), (tt_char*)eu.data, eu.count);

			du = tt::base64_decode_unit(eu);

			if (du.count == 0)
				break;

			r.data.grow_by(du.count);

			tt::copy_block((const tt_byte*)du.data, r.data.get_unchecked<tt_byte>(r.bytes), du.count);

			r.bytes += du.count;
			r.characters += eu.count;
		}

		return r;
	}

	// Converts base64 encoded character sequence x into a base64 decoded chunk.
	// Decoding stops prematurely if an invalid byte sequence segment is encountered which fails to decode.
	inline tt::base64_decoded_text base64_decode_text(const tt::chunk<1>& x) {


		return tt::base64_decode_text(x.view());
	}

	// Converts (n characters long) base64 encoded character sequence x into a base64 decoded chunk.
	// Decoding stops prematurely if an invalid byte sequence segment is encountered which fails to decode.
	inline tt::base64_decoded_text base64_decode_text(const tt_char* const x, tt_size n) {


		return tt::base64_decode_text(tt::chunk_view<1>(x, n));
	}

	// Converts (null terminated) base64 encoded character sequence x into a base64 decoded chunk.
	// Decoding stops prematurely if an invalid byte sequence segment is encountered which fails to decode.
	inline tt::base64_decoded_text base64_decode_text(const tt_char* const x) {


		return tt::base64_decode_text(x, tt::measure_cstr(x));
	}

	// Converts (null terminated) base64 encoded character sequence x into a base64 decoded chunk.
	// Decoding stops prematurely if an invalid byte sequence segment is encountered which fails to decode.
	inline tt::base64_decoded_text base64_decode_text(tt_string_view x) {


		return tt::base64_decode_text(x.data(), x.length());
	}

	// Converts (null terminated) base64 encoded character sequence x into a base64 decoded chunk.
	// Decoding stops prematurely if an invalid byte sequence segment is encountered which fails to decode.
	inline tt::base64_decoded_text base64_decode_text(const tt_string& x) {


		return tt::base64_decode_text((tt_string_view)x);
	}
}

