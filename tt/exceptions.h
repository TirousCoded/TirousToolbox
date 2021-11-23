

#pragma once


// A header file defining the exceptions thrown by the Tirous Toolbox library.


#include "debug.h"


namespace tt {


	TT_EXCEPTION_STRUCT(out_of_range_error);
	TT_EXCEPTION_STRUCT(max_size_error);
	
	TT_EXCEPTION_STRUCT(illegal_argument_error);
	TT_EXCEPTION_STRUCT(illegal_deref_error);

	TT_EXCEPTION_STRUCT(bom_encoding_error);
}

