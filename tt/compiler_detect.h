

#pragma once


// A header file defining preprocessor definitions and constants used to detect what compiler is being used.


#include "aliases.h"


// only one of these preprocessor definitions will be defined, based on the compiler

#if defined(_MSC_VER)
#define TT_COMPILER_IS_MSVC 1
#elif defined(__GNUC__)
#define TT_COMPILER_IS_GCC 1
#elif defined(__clang__)
#define TT_COMPILER_IS_CLANG 1
#else
#define TT_COMPILER_IS_UNKNOWN 1
#endif


namespace tt {


	// An enumeration of C++ compilers known to the Tirous Toolbox library.
	enum class cpp_compiler : tt_byte {

		MSVC,
		GCC,
		CLANG,

		UNKNOWN,
	};


#if defined(TT_COMPILER_IS_MSVC)
	// The C++ compiler being used (or tt::cpp_compiler::UNKNOWN if the compiler is not MSVC, GCC, or clang.)
	constexpr auto current_cpp_compiler = cpp_compiler::MSVC;
#elif defined(TT_COMPILER_IS_GCC)
	// The C++ compiler being used (or tt::cpp_compiler::UNKNOWN if the compiler is not MSVC, GCC, or clang.)
	constexpr auto current_cpp_compiler = cpp_compiler::GCC;
#elif defined(TT_COMPILER_IS_CLANG)
	// The C++ compiler being used (or tt::cpp_compiler::UNKNOWN if the compiler is not MSVC, GCC, or clang.)
	constexpr auto current_cpp_compiler = cpp_compiler::CLANG;
#else
	// The C++ compiler being used (or tt::cpp_compiler::UNKNOWN if the compiler is not MSVC, GCC, or clang.)
	constexpr auto current_cpp_compiler = cpp_compiler::UNKNOWN;
#endif


	// If the C++ compiler being used is MSVC.
	constexpr auto cpp_compiler_is_msvc = current_cpp_compiler == cpp_compiler::MSVC;

	// If the C++ compiler being used is GCC.
	constexpr auto cpp_compiler_is_gcc = current_cpp_compiler == cpp_compiler::GCC;

	// If the C++ compiler being used is clang.
	constexpr auto cpp_compiler_is_clang = current_cpp_compiler == cpp_compiler::CLANG;

	// If the C++ compiler being used is unknown to the Tirous Toolbox library.
	constexpr auto cpp_compiler_is_unknown = current_cpp_compiler == cpp_compiler::UNKNOWN;
}

