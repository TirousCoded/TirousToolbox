

#pragma once


// A header file detailing the configuration details of the Tirous Toolbox library.


#include "aliases.h"


// For right how we'll have two configuration options: TT_CONFIG_RELEASE, TT_CONFIG_MSVC, TT_CONFIG_64BIT

// TT_CONFIG_RELEASE asserts that the compilation is in *release* mode, as opposed to debug mode.
// The Tirous Toolbox library employs its own notion of a debug mode, which is enabled by default.

// TT_CONFIG_MSVC asserts that the compiler being used is MSVC, and thus the library should thusly
// specialize itself.

// TT_CONFIG_64BIT asserts 64-bit compilation, with the default being 32-bit.


// NOTE: When replacing any of the TT_CONFIG_* definitions below, remember that instances of them may appear
//		 in the code of the rest of the library AND that said instances being invalidated WILL NOT APPEAR
//		 AS AN ERROR, and will lead to nasty problems later if not managed!

// NOTE: As part of this, remember to include config.h in all config-dependent header files, just to be safe.

#if defined(TT_CONFIG_RELEASE)
namespace tt {


	// If the Tirous Toolbox library is configured for compilation of debug mode code, instead of release mode code.
	// The Tirous Toolbox library employs its own notion of a debug mode, which is enabled by default.
	constexpr tt_bool config_is_debug_mode = false;

	// If the Tirous Toolbox library is configured for compilation of release mode code, instead of debug mode code.
	// The Tirous Toolbox library employs its own notion of a debug mode, which is enabled by default.
	constexpr tt_bool config_is_release_mode = true;
}
#else
namespace tt {


	// If the Tirous Toolbox library is configured for compilation of debug mode code, instead of release mode code.
	// The Tirous Toolbox library employs its own notion of a debug mode, which is enabled by default.
	constexpr tt_bool config_is_debug_mode = true;

	// If the Tirous Toolbox library is configured for compilation of release mode code, instead of debug mode code.
	// The Tirous Toolbox library employs its own notion of a debug mode, which is enabled by default.
	constexpr tt_bool config_is_release_mode = false;
}
#endif

#if defined(TT_CONFIG_MSVC)
namespace tt {


	// If the Tirous Toolbox library is configured for use on the MSVC compiler.
	// By default, the Tirous Toolbox library makes no presumptions about the compiler involved.
	constexpr tt_bool config_is_msvc = true;
}
#else
namespace tt {


	// If the Tirous Toolbox library is configured for use on the MSVC compiler.
	// By default, the Tirous Toolbox library makes no presumptions about the compiler involved.
	constexpr tt_bool config_is_msvc = false;
}
#endif

#if defined(TT_CONFIG_64BIT)
namespace tt {


	// If the Tirous Toolbox library is configured for 32-bit compilation.
	// The Tirous Toolbox library presumes 32-bit compilation by default.
	constexpr tt_bool config_is_32bit = false;

	// If the Tirous Toolbox library is configured for 64-bit compilation.
	// The Tirous Toolbox library presumes 32-bit compilation by default.
	constexpr tt_bool config_is_64bit = true;

	static_assert(sizeof(tt_size) == 8, "Tirous Toolbox library configured for 64-bit compilation, but 32-bit detected. Consider undefining TT_CONFIG_64BIT.");
}
#else
namespace tt {


	// If the Tirous Toolbox library is configured for 32-bit compilation.
	// The Tirous Toolbox library presumes 32-bit compilation by default.
	constexpr tt_bool config_is_32bit = true;

	// If the Tirous Toolbox library is configured for 64-bit compilation.
	// The Tirous Toolbox library presumes 32-bit compilation by default.
	constexpr tt_bool config_is_64bit = false;

	static_assert(sizeof(tt_size) == 4, "Tirous Toolbox library configured for 32-bit compilation, but 64-bit detected. Consider defining TT_CONFIG_64BIT.");
}
#endif

