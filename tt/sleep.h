

#pragma once


// A header file of utilities used to cause the current thread to sleep.


#include <thread>

#include "aliases.h"

#include "time_value.h"


namespace tt {


	// Signals to the OS that the current thread should be put to sleep for the given duration.
	// If the given duration a length of zero time, no signal is sent.
	inline void sleep(tt::time_value duration) {


		std::this_thread::sleep_for(std::chrono::microseconds::duration(duration.microsec_count));
	}

	// Signals to the OS that the current thread should be put to sleep for the given duration.
	// If the given duration a length of zero time, no signal is sent.
	inline void sleep(tt::time_value_nano duration) {


		std::this_thread::sleep_for(std::chrono::nanoseconds::duration(duration.nanosec_count));
	}

	// Signals to the OS that the current thread should be put to sleep until the given point in time.
	// If the given point in time is or preceeds the current system time, no signal is sent.
	inline void sleep_until(tt::time_value point_in_time) {


		auto _now = tt::time_value::now();

		if (point_in_time > _now)
			tt::sleep(point_in_time - _now);
	}

	// Signals to the OS that the current thread should be put to sleep until the given point in time.
	// If the given point in time is or preceeds the current system time, no signal is sent.
	inline void sleep_until(tt::time_value_nano point_in_time) {


		auto _now = tt::time_value_nano::now();

		if (point_in_time > _now)
			tt::sleep(point_in_time - _now);
	}
}

