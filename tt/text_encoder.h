

#pragma once


// A generator class used to encode Unicode codepoint values via a given encoding.


#include "aliases.h"

#include "text_encoding.h"

#include "chunk.h"


namespace tt {


	// A generator class used to encode Unicode codepoint values via a given encoding.
	class text_encoder final {
	public:

		using this_t = tt::text_encoder;


	private:

		tt::text_encoding _encoding;

		tt::chunk<1>* _block;


	public:

		// Default initializes a text encoder.
		inline text_encoder() noexcept {


			_encoding = {};
			_block = nullptr;
		}

		// Initializes a text encoder using encoding e and block x.
		inline text_encoder(tt::text_encoding e, tt::chunk<1>* x) noexcept {


			_encoding = e;
			_block = x;
		}

		// Copy-initializes a text encoder.
		inline text_encoder(const this_t& x) noexcept {


			TT_COPY(_encoding, x);
			TT_COPY(_block, x);
		}

		// Move-initializes a text encoder.
		inline text_encoder(this_t&& x) noexcept {


			TT_MOVE(_encoding, x);
			TT_MOVEPTR(_block, x);
		}

		inline ~text_encoder() noexcept {}

		inline this_t& operator=(const this_t& rhs) noexcept {


			TT_COPY(_encoding, rhs);
			TT_COPY(_block, rhs);

			TT_RETURN_THIS;
		}

		inline this_t& operator=(this_t&& rhs) noexcept {


			TT_SELF_MOVE_TEST(rhs);

			TT_MOVE(_encoding, rhs);
			TT_MOVEPTR(_block, rhs);

			TT_RETURN_THIS;
		}

		// Returns an lvalue to the current encoding used by the text encoder.
		constexpr tt::text_encoding& encoding() noexcept { return _encoding; }

		// Returns an lvalue to the current encoding used by the text encoder.
		constexpr const tt::text_encoding& encoding() const noexcept { return _encoding; }

		// Returns the memory block written by the encoder.
		constexpr tt::chunk<1>*& block() noexcept { return _block; }

		// Returns the memory block written by the encoder.
		constexpr tt::chunk<1>* const& block() const noexcept { return _block; }

		// throws indirectly

		// Pushes codepoint x to the end of the encoder's memory block, properly encoded.
		// Returns the encoded unit resolved from x.
		// Throws tt::bom_encoding_error if this encoder's encoding contains unresolved BOM information, providing a strong guarantee.
		inline tt::encoded_unit encode(tt_unichar x) {


			auto r = encoding().encode_unit(x);

			auto& b = *block();

			b.grow_by(r.bytes());

			tt::copy_block((const tt_byte*)r.data, b.get_byte_unchecked(b.size() - r.bytes()), r.bytes());

			return r;
		}

		// Pushes the encoder's encoding's BOM to the end of the encoder's memory block.
		// Returns the number of bytes pushed.
		inline tt_size encode_bom(tt::endian byte_order = tt::endian::NATIVE) {


			auto r = encoding().bom_byte_count();

			auto& b = *block();

			b.grow_by(r);

			auto s = encoding().bom_byte_slice(byte_order);

			tt::copy_block(s.data(), b.get_byte_unchecked(b.size() - s.size()), s.size());

			return r;
		}
	};
}

