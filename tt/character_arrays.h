

#pragma once


// A header file defining common character arrays used in the Tirous Toolbox library.


#include "aliases.h"


namespace tt {


	// An array of base-2 binary digits.
	constexpr const tt_char* const binary_digits = "01";

	// An array of base-8 octal digits.
	constexpr const tt_char* const octal_digits = "01234567";

	// An array of base-10 digits.
	constexpr const tt_char* const base10_digits = "0123456789";

	// An array of base-16 hexadecimal digits, using uppercase letters.
	constexpr const tt_char* const hex_digits_uppercase = "0123456789ABCDEF";

	// An array of base-16 hexadecimal digits, using lowercase letters.
	constexpr const tt_char* const hex_digits_lowercase = "0123456789abcdef";

	// An array of uppercase latin alphabet characters.
	constexpr const tt_char* const alphabet_chars_uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	// An array of lowercase latin alphabet characters.
	constexpr const tt_char* const alphabet_chars_lowercase = "abcdefghijklmnopqrstuvwxyz";
}

