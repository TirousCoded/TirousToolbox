

#pragma once


// A header file detailing the version information for the Tirous Toolbox library.


#include "aliases.h"


// NOTE: Remember to update these after every update, no matter it major or minor level.

namespace tt {


	// A C-string detailing the Tirous Toolbox library's version.
	constexpr const tt_char* const api_version_cstr = "version 1.2";

	// A value detailing the major level version number of the Tirous Toolbox library.
	constexpr tt_size api_version_major = 1;

	// A value detailing the major level version number of the Tirous Toolbox library.
	constexpr tt_size api_version_minor = 2;
}

