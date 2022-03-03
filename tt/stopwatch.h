

#pragma once


// A class used to measure durations of time, likely for diagnostic purposes, though not exclusively.


#include "aliases.h"

#include "forward_declarations.h"

#include "time_value.h"


namespace tt {


	// A class used to measure durations of time, likely for diagnostic purposes, though not exclusively.
	template<typename TimeValue>
	class stopwatch_class final {
	public:

		using time_value_t = typename TimeValue;

		using this_t = tt::stopwatch_class<time_value_t>;


		// Default initializes a stopwatch.
		inline stopwatch_class() noexcept {
			
			
			reset();
		}

		// Copy-initializes a stopwatch.
		inline stopwatch_class(const this_t& x) noexcept {
			
			
			_copy(x);
		}

		// Move-initializes a stopwatch.
		inline stopwatch_class(this_t&& x) noexcept {
			
			
			_move(TT_FMOVE(this_t, x));
		}

		inline ~stopwatch_class() noexcept = default;

		inline this_t& operator=(const this_t& rhs) noexcept {


			_copy(rhs);

			TT_RETURN_THIS;
		}

		inline this_t& operator=(this_t&& rhs) noexcept {


			TT_SELF_MOVE_TEST(rhs);

			_move(TT_FMOVE(this_t, rhs));

			TT_RETURN_THIS;
		}

		// Returns the number of time intervals which the stopwatch has recorded since its last reset.
		// A time interval is only recorded once the stopwatch is stopped.
		constexpr tt_size intervals() const noexcept { return _intervals; }

		// Returns if the stopwatch has recorded any time intervals since its last reset.
		// A time interval is only recorded once the stopwatch is stopped.
		constexpr tt_bool has_intervals() const noexcept { return intervals() > 0; }

		// Returns if the stopwatch is running or not.
		// The running state of a stopwatch is carried across move-semantics, and is cloned via copy-semantics.
		constexpr tt_bool is_running() const noexcept { return _running; }

		// Returns the time the stopwatch was last started since its last reset.
		// If the stopwatch has not been started since its last reset, or since creation, this value is not meaningful.
		constexpr time_value_t start_time() const noexcept { return _last_start; }

		// Returns the time the stopwatch was last stopped since its last reset.
		// If the stopwatch has not been started since its last reset, or since creation, this value is not meaningful.
		constexpr time_value_t stop_time() const noexcept { return _last_stop; }

		// Returns the duration of the last interval of time recorded by the stopwatch since its last reset.
		constexpr time_value_t interval_time() const noexcept { return _last_interval; }

		// Returns the sum of all of the discrete time intervals recorded by the stopwatch since its last reset.
		constexpr time_value_t total_time() const noexcept { return _total; }

		// Returns the average duration of a time interval recorded by the stopwatch since its last reset.
		// If the stopwatch has not recorded any time intervals since its last reset, this value is not meaningful.
		// The value returned is a function of integer division, and so there will likely be some error.
		inline time_value_t average_time() const noexcept {


			if (has_intervals())
				return total_time() / intervals();
			else
				return time_value_t::zero();
		}

		// Starts the stopwatch, doing nothing if it's already started.
		// A time interval is only recorded once the stopwatch is stopped.
		inline this_t& start() noexcept {


			if (!is_running())
				_running = true,
				_last_start = time_value_t::now();

			TT_RETURN_THIS;
		}

		// Stops the stopwatch, doing nothing if it's already started.
		// A time interval is only recorded once the stopwatch is stopped.
		inline this_t& stop() noexcept {


			if (is_running())
				++_intervals,
				_running = false,
				_last_stop = time_value_t::now(),
				_last_interval = stop_time() - start_time(),
				_total += interval_time();

			TT_RETURN_THIS;
		}

		// Resets the state of the stopwatch.
		// If the stopwatch is running at the time of reset, it will be stopped prior to the reset.
		inline this_t& reset() noexcept {


			_intervals = 0;

			_running = false;

			_last_start = time_value_t::zero();
			_last_stop = time_value_t::zero();
			_last_interval = time_value_t::zero();

			_total = time_value_t::zero();

			TT_RETURN_THIS;
		}


	private:

		tt_size _intervals;

		tt_bool _running;

		time_value_t _last_start, _last_stop, _last_interval, _total;

		inline void _copy(const this_t& x) noexcept {


			TT_COPY(_intervals, x);

			TT_COPY(_running, x);

			TT_COPY(_last_start, x);
			TT_COPY(_last_stop, x);
			TT_COPY(_last_interval, x);

			TT_COPY(_total, x);
		}

		inline void _move(this_t&& x) noexcept {


			TT_MOVE(_intervals, x);

			TT_MOVE(_running, x);

			TT_MOVE(_last_start, x);
			TT_MOVE(_last_stop, x);
			TT_MOVE(_last_interval, x);

			TT_MOVE(_total, x);
		}
	};
}

