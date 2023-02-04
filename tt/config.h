

#pragma once


// A header file detailing the configuration details of the Tirous Toolbox library.


#include "aliases.h"
#include "compiler_detect.h"


// For right how we'll have the following configuration options:

// TT_CONFIG_RELEASE asserts that the compilation is in *release* mode, as opposed to debug mode.
// The Tirous Toolbox library employs its own notion of a debug mode, which is enabled by default.
// 
// (see https://stackoverflow.com/questions/19635630/what-predefined-macro-can-be-used-to-detect-debug-build-with-clang)
//
// Unless TT_CONFIG_NO_IMPLY is defined, the definition of TT_CONFIG_RELEASE will occur implicitly,
// if it would otherwise be missing, according to the following rules:
//
//		1. For MSVC, it is defined if _DEBUG is NOT defined.
//
//		2. For GCC, it is defined if NDEBUG is defined.
//
//		3. For clang, it is defined if __OPTIMIZE__ is defined.
//
// If the compiler is unknown, no implicit definition will occur.

// TT_CONFIG_64BIT asserts 64-bit compilation, with the default being 32-bit.

// TT_CONFIG_LOG_EXCEPTS asserts that TT_THROW should print a debug message to stderr.
//
// This logging is NOT thread-safe.

// TT_CONFIG_NO_IMPLY asserts that the above regarding implying the definition of things like
// the TT_CONFIG_RELEASE configuration option should not occur.


// NOTE: auto-detect appropriate time to define TT_CONFIG_RELEASE if possible/allowed

#if !defined(TT_CONFIG_RELEASE) && !defined(TT_CONFIG_NO_IMPLY)
#if defined(TT_COMPILER_IS_MSVC)
#if !defined(_DEBUG)
#define TT_CONFIG_RELEASE 1
#endif
#elif defined(TT_COMPILER_IS_GCC)
#if defined(NDEBUG)
#define TT_CONFIG_RELEASE 1
#endif
#elif defined(TT_COMPILER_IS_CLANG)
#if defined(__OPTIMIZE__)
#define TT_CONFIG_RELEASE 1
#endif
#endif
#endif


// NOTE: When replacing any of the TT_CONFIG_* definitions below, remember that instances of them may appear
//		 in the code of the rest of the library AND that said instances being invalidated WILL NOT APPEAR
//		 AS AN ERROR, and will lead to nasty problems later if not managed!

// NOTE: As part of this, remember to include config.h in all config-dependent header files, just to be safe.


namespace tt {


#if defined(TT_CONFIG_RELEASE)
	// If the Tirous Toolbox library is configured for compilation of debug mode code, instead of release mode code.
	// The Tirous Toolbox library employs its own notion of a debug mode, which is enabled by default.
	constexpr tt_bool config_is_debug_mode = false;

	// If the Tirous Toolbox library is configured for compilation of release mode code, instead of debug mode code.
	// The Tirous Toolbox library employs its own notion of a debug mode, which is enabled by default.
	constexpr tt_bool config_is_release_mode = true;
#else
	// If the Tirous Toolbox library is configured for compilation of debug mode code, instead of release mode code.
	// The Tirous Toolbox library employs its own notion of a debug mode, which is enabled by default.
	constexpr tt_bool config_is_debug_mode = true;

	// If the Tirous Toolbox library is configured for compilation of release mode code, instead of debug mode code.
	// The Tirous Toolbox library employs its own notion of a debug mode, which is enabled by default.
	constexpr tt_bool config_is_release_mode = false;
#endif


	// NOTE: the TT_CONFIG_MSVC configuration option has been removed due to it not being needed anymore
	//
	//		 that said, we'll still define tt::config_is_msvc for *compatibility* reasons, even though we're
	//		 the only ones using the library, lol

#if defined(TT_COMPILER_IS_MSVC)
// NOTE: also define TT_CONFIG_MSVC, also for *compatibility* reasons
#if !defined(TT_CONFIG_MSVC)
#define TT_CONFIG_MSVC 1
#endif
	// legacy
	constexpr tt_bool config_is_msvc = true;
#else
	// legacy
	constexpr tt_bool config_is_msvc = false;
#endif


#if defined(TT_CONFIG_64BIT)
	// If the Tirous Toolbox library is configured for 32-bit compilation.
	// The Tirous Toolbox library presumes 32-bit compilation by default.
	constexpr tt_bool config_is_32bit = false;

	// If the Tirous Toolbox library is configured for 64-bit compilation.
	// The Tirous Toolbox library presumes 32-bit compilation by default.
	constexpr tt_bool config_is_64bit = true;

	static_assert(sizeof(tt_size) == 8, "Tirous Toolbox library configured for 64-bit compilation, but 32-bit detected. Consider undefining TT_CONFIG_64BIT.");
#else
	// If the Tirous Toolbox library is configured for 32-bit compilation.
	// The Tirous Toolbox library presumes 32-bit compilation by default.
	constexpr tt_bool config_is_32bit = true;

	// If the Tirous Toolbox library is configured for 64-bit compilation.
	// The Tirous Toolbox library presumes 32-bit compilation by default.
	constexpr tt_bool config_is_64bit = false;

	static_assert(sizeof(tt_size) == 4, "Tirous Toolbox library configured for 32-bit compilation, but 64-bit detected. Consider defining TT_CONFIG_64BIT.");
#endif
}

