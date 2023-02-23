

#pragma once


// An abstract base class used to implement a diagnostic logger.


#include <array>

#include "aliases.h"
#include "exceptions.h"

#include "fprint.h"


namespace tt {


	// NOTE: the below log-levels can also be used when logging the resultant
	//		 of things like code compilation of one's custom compiler (as that's
	//		 certainly what I'll be using them for, lol)

	enum class log_level : tt_byte {

		MESSAGE,	// A log-level for non-notable diagnostic messages.
		NOTICE,		// A log-level for notable (but non-negative) diagnostic messages.
		WARNING,	// A log-level for diagnostic warning messages.
		ERROR,		// A log-level for diagnostic error messages.

		NUM,		// This is not a valid log-level.
	};

	constexpr auto log_levels = (tt_size)log_level::NUM;

	inline tt_string vis_log_level(log_level level) {


		tt_string r{};

		switch (level) {


		case log_level::MESSAGE:	r = "Message";	break;
		case log_level::NOTICE:		r = "Notice";	break;
		case log_level::WARNING:	r = "Warning";	break;
		case log_level::ERROR:		r = "Error";	break;

		default:					r = "???";		break;
		}

		static_assert(log_levels == 4);

		return r;
	}


	class logger {
	public:

		virtual ~logger() noexcept = default;


		// Behaviour is undefined if level == tt::log_level::NUM.
		// This method is thread-safe regardless of whether the logger as a whole is thread-safe.
		inline tt_bool get_enabled(log_level level) const noexcept { return _enabled[(tt_size)level]; }

		// Behaviour is undefined if level == tt::log_level::NUM.
		// This method is thread-safe regardless of whether the logger as a whole is thread-safe.
		inline void set_enabled(log_level level, tt_bool enable) noexcept { _enabled[(tt_size)level] = enable; }


		// No log occurs if the given log-level is disabled.
		// Interpolation of args is done via tt::fprint_s.
		// Behaviour is undefined if level == tt::log_level::NUM.
		template<typename... Args>
		inline void log(log_level level, Args&&... args);

		template<typename... Args>
		inline void log_msg(Args&&... args) { log(log_level::MESSAGE, TT_FMOVE_N(Args, args)); }
		template<typename... Args>
		inline void log_notice(Args&&... args) { log(log_level::NOTICE, TT_FMOVE_N(Args, args)); }
		template<typename... Args>
		inline void log_warning(Args&&... args) { log(log_level::WARNING, TT_FMOVE_N(Args, args)); }
		template<typename... Args>
		inline void log_error(Args&&... args) { log(log_level::ERROR, TT_FMOVE_N(Args, args)); }

		// Log-level enable/disable methods use atomic fields internally and are thus thread-safe by default.
		inline tt_bool is_thread_safe() const noexcept { return get_is_thread_safe(); }


	protected:

		virtual void do_log(log_level level, tt_string&& message) = 0;

		inline virtual tt_bool get_is_thread_safe() const noexcept { return false; }


	private:

		std::array<tt_atomic_bool, log_levels> _enabled = {
			true,
			true,
			true,
			true,
		};

		static_assert(log_levels == 4);
	};

	template<typename... Args>
	inline void logger::log(log_level level, Args&&... args) {


		tt_assert(level != log_level::NUM);

		if (get_enabled(level))
			do_log(level, tt::fprint_s(TT_FMOVE_N(Args, args)));
	}
}

