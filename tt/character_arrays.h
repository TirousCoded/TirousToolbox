

#pragma once


// A header file defining common character arrays used in the Tirous Toolbox library.


#include "aliases.h"


namespace tt {


	// An array of base-2 binary digits.
	constexpr const tt_char* const binary_digits = "01";

	// An array of base-2 binary digits.
	constexpr const tt_wchar* const binary_digits_wide = L"01";

	// An array of base-2 binary digits.
	constexpr const tt_char16* const binary_digits_u16 = u"01";

	// An array of base-2 binary digits.
	constexpr const tt_char32* const binary_digits_u32 = U"01";

	// An array of base-8 octal digits.
	constexpr const tt_char* const octal_digits = "01234567";

	// An array of base-8 octal digits.
	constexpr const tt_wchar* const octal_digits_wide = L"01234567";

	// An array of base-8 octal digits.
	constexpr const tt_char16* const octal_digits_u16 = u"01234567";

	// An array of base-8 octal digits.
	constexpr const tt_char32* const octal_digits_u32 = U"01234567";

	// An array of base-10 digits.
	constexpr const tt_char* const base10_digits = "0123456789";

	// An array of base-10 digits.
	constexpr const tt_wchar* const base10_digits_wide = L"0123456789";

	// An array of base-10 digits.
	constexpr const tt_char16* const base10_digits_u16 = u"0123456789";

	// An array of base-10 digits.
	constexpr const tt_char32* const base10_digits_u32 = U"0123456789";

	// An array of base-16 hexadecimal digits, using uppercase letters.
	constexpr const tt_char* const hex_digits_uppercase = "0123456789ABCDEF";

	// An array of base-16 hexadecimal digits, using uppercase letters.
	constexpr const tt_wchar* const hex_digits_uppercase_wide = L"0123456789ABCDEF";

	// An array of base-16 hexadecimal digits, using uppercase letters.
	constexpr const tt_char16* const hex_digits_uppercase_u16 = u"0123456789ABCDEF";

	// An array of base-16 hexadecimal digits, using uppercase letters.
	constexpr const tt_char32* const hex_digits_uppercase_u32 = U"0123456789ABCDEF";

	// An array of base-16 hexadecimal digits, using lowercase letters.
	constexpr const tt_char* const hex_digits_lowercase = "0123456789abcdef";

	// An array of base-16 hexadecimal digits, using lowercase letters.
	constexpr const tt_wchar* const hex_digits_lowercase_wide = L"0123456789abcdef";

	// An array of base-16 hexadecimal digits, using lowercase letters.
	constexpr const tt_char16* const hex_digits_lowercase_u16 = u"0123456789abcdef";

	// An array of base-16 hexadecimal digits, using lowercase letters.
	constexpr const tt_char32* const hex_digits_lowercase_u32 = U"0123456789abcdef";

	// An array of uppercase latin alphabet characters.
	constexpr const tt_char* const alphabet_chars_uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	// An array of uppercase latin alphabet characters.
	constexpr const tt_wchar* const alphabet_chars_uppercase_wide = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	// An array of uppercase latin alphabet characters.
	constexpr const tt_char16* const alphabet_chars_uppercase_u16 = u"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	// An array of uppercase latin alphabet characters.
	constexpr const tt_char32* const alphabet_chars_uppercase_u32 = U"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	// An array of lowercase latin alphabet characters.
	constexpr const tt_char* const alphabet_chars_lowercase = "abcdefghijklmnopqrstuvwxyz";

	// An array of lowercase latin alphabet characters.
	constexpr const tt_wchar* const alphabet_chars_lowercase_wide = L"abcdefghijklmnopqrstuvwxyz";

	// An array of lowercase latin alphabet characters.
	constexpr const tt_char16* const alphabet_chars_lowercase_u16 = u"abcdefghijklmnopqrstuvwxyz";

	// An array of lowercase latin alphabet characters.
	constexpr const tt_char32* const alphabet_chars_lowercase_u32 = U"abcdefghijklmnopqrstuvwxyz";
}

