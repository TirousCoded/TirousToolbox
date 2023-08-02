

#pragma once


// A header file defining assertions and exceptions in the Tirous Toolbox library.
// These shouldn't strictly be placed together, but we will, as putting them together should be harmless.

// This is, to an extent, an *extension* of macros.h.


#include <iostream>
#include <exception>

#include "aliases.h"
#include "compiler_detect.h"
#include "config.h"
#include "macros.h"


// A helper macro which declares an entire exception struct.
#define TT_EXCEPTION_STRUCT(struct_name) \
struct struct_name final : public std::exception { \
\
	using this_t = struct_name; \
\
	const tt_char* message; \
\
	inline struct_name() noexcept : std::exception() { message = ""; } \
	inline struct_name(const tt_char* msg) noexcept { message = msg; } \
	inline struct_name(const this_t& x) noexcept { message = x.message; } \
\
	inline virtual ~struct_name() noexcept {} \
\
	inline this_t& operator=(const this_t& rhs) noexcept { message = rhs.message; } \
\
	inline const tt_char* what() const noexcept override { \
\
		return message; \
	} \
}


#ifdef TT_CONFIG_LOG_EXCEPTS
#define ___TT_THROW(struct_name, message) (TT_PRINTERRL("TT EXCEPTION\nEXCEPTION: " << TT_XSTR(struct_name) << "\nMESSAGE: " << message), throw struct_name(message))
#else
#define ___TT_THROW(struct_name, message) (throw struct_name(message))
#endif

// A helper macro used to throw exceptions formed via TT_EXCEPTION_STRUCT.
#define TT_THROW(struct_name, message) TT_NO_RETURN(___TT_THROW(struct_name, message))

// A preprocessor definition used to rethrow an exception from a catch block.
#define TT_RETHROW throw


namespace tt {


	// A debug function encapsulating the behaviour of a program crashing due to a failed assertion.
	// On MSVC this breaks via __debugbreak, otherwise it calls std::abort.
	// When Tirous Toolbox debug features are disabled, this function becomes inert.
	inline void debugbreak(const tt_char* condition, const tt_char* file, const tt_char* line) {
		if constexpr (tt::config_is_debug_mode) { // <- disable if not debug mode
			TT_PRINTERRL("TT ASSERT FAILED\nCONDITION: " << condition << "\nFILE: " << file << "\nLINE: " << line);
#if defined(TT_COMPILER_IS_MSVC)
			__debugbreak();
#else
			std::abort();
#endif
		}
	}
}


// Practical macros written in non-screaming snake_case which we use all the time.

#if !defined(TT_CONFIG_RELEASE)
// The standard debug assert macro of the Tirous Toolbox library.
#define tt_assert(condition) TT_EVAL_IF_NOT(condition, tt::debugbreak(TT_XSTR(condition), TT_XSTR(__FILE__), TT_XSTR(__LINE__)))
#else
// The standard debug assert macro of the Tirous Toolbox library.
#define tt_assert(condition) TT_DO_NOTHING
#endif

// A summarized form of tt_assert for when we reach a section of code that shouldn't be reached.
#define tt_assert_bad tt_assert(false)

