

#pragma once


// The standard formatted printing and string interpolation functions of the Tirous Toolbox library.


#include "aliases.h"

#include "visualize.h"
#include "console.h"


namespace tt {


	// Appends string visualizations of each term in args onto the end of string x, via tt::vis.
	// This process is recursive.
	inline void fprint_s_onto(tt_string& x) {}

	// Appends string visualizations of each term in args onto the end of string x, via tt::vis.
	// This process is recursive.
	template<typename Arg, typename... Args>
	inline void fprint_s_onto(tt_string& x, Arg&& arg, Args&&... args) {


		x.append(tt::vis(arg));

		tt::fprint_s_onto(x, TT_FMOVE_N(Args, args));
	}

	// TODO: Is there a way we can do this which could avoid the potential number of heap allocations involved?

	// Returns an interpolated string composed of the tt::vis string visualizations of args, concatenated, and in order.
	// This is performed internally via tt::fprint_s_onto, which is recursive.
	template<typename... Args>
	inline tt_string fprint_s(Args&&... args) {


		tt_string r{};

		tt::fprint_s_onto(r, TT_FMOVE_N(Args, args));

		return r;
	}

	// Prints an interpolated string composed of the tt::vis string visualizations of args, concatenated, and in order, to the console.
	// This is performed internally via tt::fprint_s, which uses tt::fprint_s_onto, which is recursive.
	template<typename... Args>
	inline void fprint(Args&&... args) {


		tt::print(tt::fprint_s(TT_FMOVE_N(Args, args)));
	}

	// Prints an interpolated string composed of the tt::vis string visualizations of args, concatenated, and in order, to the console, followed by a newline.
	// This is performed internally via tt::fprint_s, which uses tt::fprint_s_onto, which is recursive.
	template<typename... Args>
	inline void fprintl(Args&&... args) {


		tt::printl(tt::fprint_s(TT_FMOVE_N(Args, args)));
	}
}

