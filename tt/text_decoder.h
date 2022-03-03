

#pragma once


// A generator class used to decode Unicode codepoint values via a given encoding.


#include "aliases.h"

#include "text_encoding.h"

#include "chunk.h"


namespace tt {


	// A generator class used to decode Unicode codepoint values via a given encoding.
	class text_decoder final {
	public:

		using this_t = tt::text_decoder;


		// Default initializes a text encoder.
		inline text_decoder() noexcept {


			_encoding = {};
			_block = {};
			_pos = 0;
		}

		// Initializes a text encoder using encoding e and block view x.
		inline text_decoder(tt::text_encoding e, tt::chunk_view<1> x) noexcept {


			_encoding = e;
			_block = x;
			_pos = 0;
		}

		// Copy-initializes a text encoder.
		inline text_decoder(const this_t& x) noexcept {


			TT_COPY(_encoding, x);
			TT_COPY(_block, x);
			TT_COPY(_pos, x);
		}

		// Move-initializes a text encoder.
		inline text_decoder(this_t&& x) noexcept {


			TT_MOVE(_encoding, x);
			TT_MOVE(_block, x);
			TT_MOVE(_pos, x);
		}

		inline ~text_decoder() noexcept {}

		inline this_t& operator=(const this_t& rhs) noexcept {


			TT_COPY(_encoding, rhs);
			TT_COPY(_block, rhs);
			TT_COPY(_pos, rhs);

			TT_RETURN_THIS;
		}

		inline this_t& operator=(this_t&& rhs) noexcept {


			TT_SELF_MOVE_TEST(rhs);

			TT_MOVE(_encoding, rhs);
			TT_MOVE(_block, rhs);
			TT_MOVE(_pos, rhs);

			TT_RETURN_THIS;
		}

		// Returns an lvalue to the current encoding used by the text decoder.
		constexpr tt::text_encoding& encoding() noexcept { return _encoding; }

		// Returns an lvalue to the current encoding used by the text decoder.
		constexpr const tt::text_encoding& encoding() const noexcept { return _encoding; }

		// Returns the memory block read by the decoder.
		constexpr tt::chunk_view<1>& block() noexcept { return _block; }

		// Returns the memory block read by the decoder.
		constexpr const tt::chunk_view<1>& block() const noexcept { return _block; }

		// Returns the current read position of the decoder.
		constexpr tt_size pos() const noexcept { return _pos; }

		// Sets the current read position of the decoder to x, not bounding x.
		inline this_t& set_pos(tt_size x) noexcept {


			_pos = x;

			TT_RETURN_THIS;
		}

		// Returns if the decoder's current read position is at the beginning of its memory block.
		inline tt_bool at_begin() const noexcept { return pos() == 0; }

		// Returns if the decoder's current read position is at or beyond the end of its memory block.
		inline tt_bool at_end() const noexcept { return pos() >= block().size_bytes(); }

		// Returns the number of bytes in the decoder's block from the current read position onwards.
		// If the current read position exceeds the bounds of the decoder's block, zero is returned.
		inline tt_size excess_bytes() const noexcept {


			if (at_end())
				return 0;
			else
				return block().size() - pos();
		}

		// Advances the read position of the decoder by n bytes, not bounding x.
		inline this_t& skip(tt_size n) noexcept {


			return set_pos(pos() + n);
		}

		// throws indirectly

		// Decodes the next codepoint at the current read position of the decoder's memory block, advancing the read position if successful.
		// Returns the decoded unit resolved from x.
		// Throws tt::bom_encoding_error if this decoder's encoding contains unresolved BOM information, providing a strong guarantee.
		inline tt::decoded_unit decode() {

			
			auto r = encoding().decode_unit(block().get_byte_unchecked(pos()), excess_bytes());

			if (r.success)
				skip(r.bytes());

			return r;
		}

		// Decodes the encoding's BOM at the current read position of the decoder's memory block, advancing the read position if successful.
		// Returns an object detailing the BOM found, if any.
		inline tt::bom_result decode_bom() {


			auto r = encoding().test_bom(block().get_byte_unchecked(pos()), excess_bytes());

			if (r.success)
				skip(r.bytes);

			return r;
		}


	private:

		tt::text_encoding _encoding;

		tt::chunk_view<1> _block;

		tt_size _pos;
	};
}

