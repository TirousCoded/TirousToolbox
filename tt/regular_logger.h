

#pragma once


// The 'regular' logger implementation which logs to stdout.

// This implementation is not thread-safe.


#include "logger.h"


namespace tt {


	class regular_logger final : public logger {
	protected:

		inline void do_log(log_level level, tt_string&& message) override;
	};

	inline void regular_logger::do_log(log_level level, tt_string&& message) {


		switch (level) {

		case log_level::MESSAGE:	tt::fprintl(" > ", TT_FMOVE(tt_string, message));			break;
		case log_level::NOTICE:		tt::fprintl(" > (NOTICE) ", TT_FMOVE(tt_string, message));	break;
		case log_level::WARNING:	tt::fprintl(" > (WARNING) ", TT_FMOVE(tt_string, message)); break;
		case log_level::ERROR:		tt::fprintl(" > (ERROR) ", TT_FMOVE(tt_string, message));	break;

		default:					tt_assert_bad;												break;
		}
	}
}

