

#pragma once


// A class template which adapts a given logger type with additional semantics to ensure thread-safety.

// Basically, it wraps the adapted logger in a mutex when it logs.

// This means two layers of dynamic dispatch per log, but the overhead of the mutex I feel will
// more than muffle whatever comparatively marginal cost said virtual method invocations will incur.


#include <mutex>

#include "logger.h"


namespace tt {


	template<typename Client>
	class thread_safe_logger final : public logger {
	public:

		using client_t	= typename Client;

		using this_t	= thread_safe_logger<client_t>;


		template<typename... Args>
		inline thread_safe_logger(Args&&... args) 
			: _client(TT_FMOVE_N(Args, args)) {}


	protected:

		inline void do_log(log_level level, tt_string&& message) override;

		inline tt_bool get_is_thread_safe() const noexcept override { return true; }


	private:

		std::mutex _mtx;

		// NOTE: remember that we need all log-levels of _client to always be true,
		//		 so this logger can handle those instead

		client_t _client;
	};

	template<typename Client>
	inline void thread_safe_logger<Client>::do_log(log_level level, tt_string&& message) {


		std::scoped_lock lk(_mtx);

		_client.log(level, TT_FMOVE(tt_string, message));
	}
}

