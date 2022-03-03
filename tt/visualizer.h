

#pragma once


// The 'visualizer' used to provide the static dispatch polymorphic mechanism of the Tirous Toolbox visualization system.


#include "aliases.h"


namespace tt {


	template<typename Value>
	struct visualizer final {

		using this_t = tt::visualizer<Value>;

		inline tt_string vis(const Value&) const {

			
			return "???";
		}
	};
}


#define TT_VISUALIZERS(__dummy__) namespace tt

// NOTE: We exclude the final ';' of the class so the macro plays nicely with C++'s syntax.

// Specializes tt::visualizer for CLASS such that it will invoke CLASS's vis function when used.
// This specialization enables CLASS to participate in the Tirous Toolbox visualization system.
// This MUST be used within the 'tt' namespace, which may be summarized using TT_VISUALIZERS.
// Use TT_REGISTER_VISUALIZE_T to create a partial specialization if CLASS has any outstanding template parameters.
#define TT_REGISTER_VISUALIZE(CLASS) \
template<> \
struct visualizer<CLASS> final { \
\
	using this_t = tt::visualizer<CLASS>; \
\
	inline tt_string vis(const CLASS& x) const { \
\
\
		return x.vis(); \
	} \
}

// A version of TT_REGISTER_VISUALIZE which defines the body of the visualizer's vis function explicitly.
// This also includes specifying the name of the parameter passed into this function via VALUE, of type 'const CLASS&'.
#define TT_REGISTER_VISUALIZE_EXT(CLASS, VALUE, BODY) \
template<> \
struct visualizer<CLASS> final { \
\
	using this_t = tt::visualizer<CLASS>; \
\
	inline tt_string vis(const CLASS& VALUE) const BODY \
}

// Specializes tt::visualizer for CLASS such that it will invoke CLASS's vis function when used.
// This specialization enables CLASS to participate in the Tirous Toolbox visualization system.
// This MUST be used within the 'tt' namespace, which may be summarized using TT_VISUALIZERS.
// This version exists to create partial specializations given a CLASS with outstanding template parameters.
// Given template parameters like 'T', 'PARAMS' should come in forms like '<template T>', and 'ARGS' should come in forms like '<T>'.
// If PARAMS/ARGS involves more than one parameter/argument (ie. '<T, U, V>') you will need to wrap the macro arguments passed to PARAMS and ARGS in preprocessor definitions in order for them to parse correctly.
#define TT_REGISTER_VISUALIZE_T(CLASS, PARAMS, ARGS) \
template PARAMS \
struct visualizer<CLASS ARGS> final { \
\
	using this_t = tt::visualizer<CLASS ARGS>; \
\
	inline tt_string vis(const CLASS ARGS& x) const { \
\
\
		return x.vis(); \
	} \
}

// A version of TT_REGISTER_VISUALIZE_T which defines the body of the visualizer's vis function explicitly.
// This also includes specifying the name of the parameter passed into this function via VALUE, of type 'const CLASS ARGS&'.
#define TT_REGISTER_VISUALIZE_T_EXT(CLASS, PARAMS, ARGS, VALUE, BODY) \
template PARAMS \
struct visualizer<CLASS ARGS> final { \
\
	using this_t = tt::visualizer<CLASS ARGS>; \
\
	inline tt_string vis(const CLASS ARGS& VALUE) const BODY \
}

