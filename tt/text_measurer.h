

#pragma once


// A generator class used to measure the byte length of a sequence of Unicode codepoint values encoded via a given encoding.


#include "aliases.h"

#include "text_encoding.h"

#include "chunk.h"


namespace tt {


	// A generator class used to measure the byte length of a sequence of Unicode codepoint values encoded via a given encoding.
	class text_measurer final {
	public:

		using this_t = tt::text_measurer;


		// Default initializes a text measurer.
		inline text_measurer() noexcept {


			_encoding = {};
			_bytes = 0;
		}

		// Initializes a text measurer using encoding e and block x.
		inline text_measurer(tt::text_encoding e) noexcept {


			_encoding = e;
			_bytes = 0;
		}

		// Copy-initializes a text measurer.
		inline text_measurer(const this_t& x) noexcept {


			TT_COPY(_encoding, x);
			TT_COPY(_bytes, x);
		}

		// Move-initializes a text measurer.
		inline text_measurer(this_t&& x) noexcept {


			TT_MOVE(_encoding, x);
			TT_MOVE(_bytes, x);
		}

		inline ~text_measurer() noexcept {}

		inline this_t& operator=(const this_t& rhs) noexcept {


			TT_COPY(_encoding, rhs);
			TT_COPY(_bytes, rhs);

			TT_RETURN_THIS;
		}

		inline this_t& operator=(this_t&& rhs) noexcept {


			TT_SELF_MOVE_TEST(rhs);

			TT_MOVE(_encoding, rhs);
			TT_MOVE(_bytes, rhs);

			TT_RETURN_THIS;
		}

		// Returns an lvalue to the current encoding used by the text measurer.
		constexpr tt::text_encoding& encoding() noexcept { return _encoding; }

		// Returns an lvalue to the current encoding used by the text measurer.
		constexpr const tt::text_encoding& encoding() const noexcept { return _encoding; }

		// Returns an lvalue to the number of bytes worth of encoded text measured so far.
		constexpr tt_size& bytes() noexcept { return _bytes; }

		// Returns an lvalue to the number of bytes worth of encoded text measured so far.
		constexpr const tt_size& bytes() const noexcept { return _bytes; }

		// Sets the number of bytes measured so far by the measurer to zero.
		inline this_t& zero_bytes() noexcept {


			bytes() = 0;

			TT_RETURN_THIS;
		}

		// throws indirectly

		// Adds the number of bytes required to encode x via the measurer's encoding to the measurer's byte count.
		// Returns the encoded unit resolved from x.
		// Throws tt::bom_encoding_error if this measurer's encoding contains unresolved BOM information, providing a strong guarantee.
		inline tt::encoded_unit encode(tt_unichar x) {


			auto r = encoding().encode_unit(x);

			bytes() += r.bytes();

			return r;
		}

		// Adds the number of bytes required to encode the BOM of the measurer's encoding.
		// Returns the number of bytes resolved.
		inline tt_size encode_bom() noexcept {


			auto r = encoding().bom_byte_count();

			bytes() += r;

			return r;
		}


	private:

		tt::text_encoding _encoding;

		tt_size _bytes;
	};
}

