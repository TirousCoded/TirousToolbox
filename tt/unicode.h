

#pragma once


// A header file of Unicode codespace related utilities.


#include "aliases.h"


namespace tt {


	// Remember, 0x10ffff is the FINAL code point, not the total number of code points.

	// The number of code points available in the Unicode code space.
	constexpr tt_size unicode_code_space_size = (tt_size)0x110000;

	// The two byte UTF-16 BOM character value.
	// This is the BOM value used by UTF-8 BOM, UTF-16 BOM, and UTF-32 BOM, each in their own vary different way.
	constexpr tt_unichar unicode_utf16_bom = (tt_unichar)0xfeff;

	// Returns a codepoint value x *cleaned*, overflowing x's value if it exceeds tt::unicode_code_space_size, constraining x's value to always represent a valid Unicode codepoint.
	// Our implementation of Unicode and of its various encodings will use this to ensure that we needn't consider codepoints outside of Unicode's codespace.
	// It will be stated explicitly when this function is used to *clean* argument values.
	constexpr tt_unichar unicode_clean(tt_unichar x) noexcept {


		return x % (tt_unichar)tt::unicode_code_space_size;
	}

	// Returns if the given Unicode codepoint is at or within the given codepoint range.
	constexpr tt_bool in_codepoint_range(tt_unichar x, tt_unichar first_codepoint, tt_unichar last_codepoint) noexcept {


		return x >= first_codepoint && x <= last_codepoint;
	}

	// Returns if the given Unicode codepoint is within the Unicode codespace.
	constexpr tt_bool in_unicode(tt_unichar x) noexcept {


		return tt::in_codepoint_range(x, 0, tt::unicode_code_space_size - 1);
	}

	// Returns if the given Unicode codepoint is within the range of ASCII which is printable.
	constexpr tt_bool in_printable_ascii(tt_unichar x) noexcept {


		return tt::in_codepoint_range(x, 32, 126);
	}

	// Returns if the given Unicode codepoint is within the range of ASCII.
	constexpr tt_bool in_ascii(tt_unichar x) noexcept {


		return tt::in_codepoint_range(x, 0, 127);
	}

	// Returns if the given Unicode codepoint is within Unicode's UTF-16 surrogate pair blocks.
	constexpr tt_bool in_utf16_surrogate_pair_block(tt_unichar x) noexcept {


		return tt::in_codepoint_range(x, 0xd800, 0xdfff);
	}
}

