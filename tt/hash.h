

#pragma once


// The macros used to allow users to easily generate C++ std::hash specializations.

// In specific, this allows the user to generate std::hash specializations which invoke a 'hash' member function,
// in a manner designed to mirror the way the Tirous Toolbox visualization system works.


#include <functional>

#include "aliases.h"


// NOTE the __dummy__ is here to make sure the macro doesn't confuse intellisense

#define TT_HASHERS(__dummy__) namespace std

// NOTE: we exclude the final ';' of the class so the macro plays nicely with C++'s syntax

// Specializes std::hash for CLASS such that it will invoke CLASS's hash function when used.
// This MUST be used within the 'std' namespace, which may be summarized using TT_HASHERS.
// Use TT_REGISTER_HASH_T to create a partial specialization if CLASS has any outstanding template parameters.
#define TT_REGISTER_HASH(CLASS) \
template<> \
struct hash<CLASS> final { \
\
	using this_t = std::hash<CLASS>; \
\
	inline tt_size operator()(const CLASS& x) const noexcept { \
\
\
		return x.hash(); \
	} \
}

// A version of TT_REGISTER_HASH which defines the body of the std::hash's operator() function explicitly.
// This also includes specifying the name of the parameter passed into this function via VALUE, of type 'const CLASS&'.
#define TT_REGISTER_HASH_EXT(CLASS, VALUE, BODY) \
template<> \
struct hash<CLASS> final { \
\
	using this_t = std::hash<CLASS>; \
\
	inline tt_size operator()(const CLASS& VALUE) const noexcept BODY \
}

// Specializes std::hash for CLASS such that it will invoke CLASS's hash function when used.
// This MUST be used within the 'std' namespace, which may be summarized using TT_HASHERS.
// This version exists to create partial specializations given a CLASS with outstanding template parameters.
// Given template parameters like 'T', 'PARAMS' should come in forms like '<template T>', and 'ARGS' should come in forms like '<T>'.
// If PARAMS/ARGS involves more than one parameter/argument (ie. '<T, U, V>') you will need to wrap the macro arguments passed to PARAMS and ARGS in preprocessor definitions in order for them to parse correctly.
#define TT_REGISTER_HASH_T(CLASS, PARAMS, ARGS) \
template PARAMS \
struct hash<CLASS ARGS> final { \
\
	using this_t = std::hash<CLASS ARGS>; \
\
	inline tt_size operator()(const CLASS ARGS& x) const noexcept { \
\
\
		return x.hash(); \
	} \
}

// A version of TT_REGISTER_HASH_T which defines the body of the std::hash's operator() function explicitly.
// This also includes specifying the name of the parameter passed into this function via VALUE, of type 'const CLASS ARGS&'.
#define TT_REGISTER_HASH_T_EXT(CLASS, PARAMS, ARGS, VALUE, BODY) \
template PARAMS \
struct hash<CLASS ARGS> final { \
\
	using this_t = std::hash<CLASS ARGS>; \
\
	inline tt_size operator()(const CLASS ARGS& VALUE) const noexcept BODY \
}

