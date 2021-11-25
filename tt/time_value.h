

#pragma once


// A struct encapsulating a point in, or duration of, time measured in microseconds/nanoseconds.
// These values are unsigned, and this must be considered regarding time value arithmetic.


#include <chrono>

#include "aliases.h"

#include "numeric_limits.h"

#include "forward_declarations.h"

#include "visualize.h"
#include "hash.h"


namespace tt {


	// A struct encapsulating a point in, or duration of, time measured in microseconds.
	// These values are unsigned, and this must be considered regarding time value arithmetic.
	struct time_value final {

		using this_t = tt::time_value;


		// We're not gonna define constructors/destructors/assignment so we can use auto-generated ones.
		
		// That said, we will define static function initializers so the user needn't do it with 
		// something like aggregate-initializing a microsecond value, though they can if they want.
		

		// The count of microseconds defining the time value.
		tt_ulong microsec_count;


		// equality/inequality tests + lexicographical comparison function

		// Returns if this and the given time value are equal.
		constexpr tt_bool equal(this_t x) const noexcept { return microsec_count == x.microsec_count; }

		constexpr tt_bool operator==(this_t rhs) const noexcept { return equal(rhs); }
		constexpr tt_bool operator!=(this_t rhs) const noexcept { return !equal(rhs); }

		constexpr tt_bool operator<(this_t rhs) const noexcept { return microsec_count < rhs.microsec_count; }
		constexpr tt_bool operator>(this_t rhs) const noexcept { return microsec_count > rhs.microsec_count; }
		constexpr tt_bool operator<=(this_t rhs) const noexcept { return microsec_count <= rhs.microsec_count; }
		constexpr tt_bool operator>=(this_t rhs) const noexcept { return microsec_count >= rhs.microsec_count; }


		constexpr this_t operator+(this_t rhs) const noexcept { return { microsec_count + rhs.microsec_count }; }
		constexpr this_t operator-(this_t rhs) const noexcept { return { microsec_count - rhs.microsec_count }; }
		constexpr this_t operator*(this_t rhs) const noexcept { return { microsec_count * rhs.microsec_count }; }
		constexpr this_t operator/(this_t rhs) const noexcept { return { microsec_count / rhs.microsec_count }; }
		constexpr this_t operator%(this_t rhs) const noexcept { return { microsec_count % rhs.microsec_count }; }

		constexpr this_t operator*(tt_ulong rhs) const noexcept { return { microsec_count * rhs }; }
		constexpr this_t operator/(tt_ulong rhs) const noexcept { return { microsec_count / rhs }; }
		constexpr this_t operator%(tt_ulong rhs) const noexcept { return { microsec_count % rhs }; }


		inline this_t& operator+=(this_t rhs) noexcept { return *this = *this + rhs; }
		inline this_t& operator-=(this_t rhs) noexcept { return *this = *this - rhs; }
		inline this_t& operator*=(this_t rhs) noexcept { return *this = *this * rhs; }
		inline this_t& operator/=(this_t rhs) noexcept { return *this = *this / rhs; }
		inline this_t& operator%=(this_t rhs) noexcept { return *this = *this % rhs; }

		inline this_t& operator*=(tt_ulong rhs) noexcept { return *this = *this * rhs; }
		inline this_t& operator/=(tt_ulong rhs) noexcept { return *this = *this / rhs; }
		inline this_t& operator%=(tt_ulong rhs) noexcept { return *this = *this % rhs; }


		// Returns a time value of this time value's microsecond count, filtering out all larger and smaller units.
		constexpr this_t only_microsecs() const noexcept { return *this % one_millisec(); }

		// Returns a time value of this time value's millisecond count, filtering out all larger and smaller units.
		constexpr this_t only_millisecs() const noexcept { return (*this % one_second()) - (*this % one_millisec()); }

		// Returns a time value of this time value's second count, filtering out all larger and smaller units.
		constexpr this_t only_seconds() const noexcept { return (*this % one_minute()) - (*this % one_second()); }

		// Returns a time value of this time value's minute count, filtering out all larger and smaller units.
		constexpr this_t only_minutes() const noexcept { return (*this % one_hour()) - (*this % one_minute()); }

		// Returns a time value of this time value's minute count, filtering out all larger and smaller units.
		// Time values measure 24-hour days as a time unit larger than hours.
		constexpr this_t only_hours() const noexcept { return (*this % one_day()) - (*this % one_hour()); }

		// Returns a time value of this time value's minute count, filtering out all larger and smaller units.
		// These days are measured as 24-hour days.
		constexpr this_t only_days() const noexcept { return *this - (*this % one_day()); }


		// Returns the number of microseconds recorded by the time value, filtering out all larger and smaller units.
		constexpr tt_ulong count_microsecs() const noexcept { return only_microsecs().microsec_count; }

		// Returns the number of milliseconds recorded by the time value, filtering out all larger and smaller units.
		constexpr tt_ulong count_millisecs() const noexcept { return only_millisecs().microsec_count / 1'000ULL; }

		// Returns the number of seconds recorded by the time value, filtering out all larger and smaller units.
		constexpr tt_ulong count_seconds() const noexcept { return only_seconds().microsec_count / 1'000'000ULL; }

		// Returns the number of minutes recorded by the time value, filtering out all larger and smaller units.
		constexpr tt_ulong count_minutes() const noexcept { return only_minutes().microsec_count / (60ULL * 1'000'000ULL); }

		// Returns the number of hours recorded by the time value, filtering out all larger and smaller units.
		// Time values measure 24-hour days as a time unit larger than hours.
		constexpr tt_ulong count_hours() const noexcept { return only_hours().microsec_count / (60ULL * 60ULL * 1'000'000ULL); }

		// Returns the number of hours recorded by the time value, filtering out all larger and smaller units.
		// These days are measured as 24-hour days.
		constexpr tt_ulong count_days() const noexcept { return only_days().microsec_count / (24ULL * 60ULL * 60ULL * 1'000'000ULL); }


		// Returns a string visualization of the time value as a 24-hour clock, with optional components.
		// If any of the given include_* arguments are false, their respective time unit is omitted from the visualization.
		// If include_millisecs is false, include_microsecs will be set to false.
		inline tt_string visualize(tt_bool include_days = true, tt_bool include_millisecs = true, tt_bool include_microsecs = true) const noexcept {


			auto _days = count_days();
			auto _hours = count_hours();
			auto _minutes = count_minutes();
			auto _seconds = count_seconds();			
			auto _millisecs = count_millisecs();
			auto _microsecs = count_microsecs();

			tt_string r{};

			if (include_days)
				r += tt::visualize_uint(_days),
				r += ':';

			if (_hours < 10)
				r += '0';

			r += tt::visualize_uint(_hours);

			r += ':';

			if (_minutes < 10)
				r += '0';

			r += tt::visualize_uint(_minutes);

			r += ':';

			if (_seconds < 10)
				r += '0';

			r += tt::visualize_uint(_seconds);

			if (include_millisecs) {


				r += '.';
				
				if (_millisecs < 100)
					r += '0';

				if (_millisecs < 10)
					r += '0';

				r += tt::visualize_uint(_millisecs);

				if (include_millisecs) {


					if (_microsecs < 100)
						r += '0';

					if (_microsecs < 10)
						r += '0';

					r += tt::visualize_uint(_microsecs);
				}
			}

			return r;
		}

		// Returns a hash of the time value.
		constexpr tt_size hash() const noexcept {


			return (tt_size)microsec_count;
		}


		inline explicit operator tt::time_value_nano() const noexcept;


		// Returns a time value of zero.
		static constexpr this_t zero() noexcept { return { 0 }; }

		// Returns the maximum possible time value.
		static constexpr this_t maximum() noexcept { return { tt::max_ulong }; }


		// Returns a time value of the current system time.
		static inline this_t now() noexcept {


			return { static_cast<tt_ulong>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) };
		}


		// Returns a time value.
		static constexpr this_t one_microsec() noexcept { return { 1 }; }

		// Returns a time value.
		static constexpr this_t one_millisec() noexcept { return one_microsec() * 1'000ULL; }

		// Returns a time value.
		static constexpr this_t one_second() noexcept { return one_millisec() * 1'000ULL; }

		// Returns a time value.
		static constexpr this_t one_minute() noexcept { return one_second() * 60ULL; }

		// Returns a time value.
		// Time values measure 24-hour days as a time unit larger than hours.
		static constexpr this_t one_hour() noexcept { return one_minute() * 60ULL; }

		// Returns a time value.
		// These days are measured as 24-hour days.
		static constexpr this_t one_day() noexcept { return one_hour() * 24ULL; }


		// Returns a time value.
		static constexpr this_t microsecs(tt_ulong microsecs) noexcept {


			return one_microsec() * microsecs;
		}

		// Returns a time value.
		static constexpr this_t millisecs(tt_ulong millisecs, tt_ulong microsecs = 0) noexcept {


			return one_millisec() * millisecs + this_t::microsecs(microsecs);
		}

		// Returns a time value.
		static constexpr this_t seconds(tt_ulong seconds, tt_ulong millisecs = 0, tt_ulong microsecs = 0) noexcept {


			return one_second() * seconds + this_t::millisecs(millisecs, microsecs);
		}

		// Returns a time value.
		static constexpr this_t minutes(tt_ulong minutes, tt_ulong seconds = 0, tt_ulong millisecs = 0, tt_ulong microsecs = 0) noexcept {


			return one_minute() * minutes + this_t::seconds(seconds, millisecs, microsecs);
		}

		// Returns a time value.
		// Time values measure 24-hour days as a time unit larger than hours.
		static constexpr this_t hours(tt_ulong hours, tt_ulong minutes = 0, tt_ulong seconds = 0, tt_ulong millisecs = 0, tt_ulong microsecs = 0) noexcept {


			return one_hour() * hours + this_t::minutes(minutes, seconds, millisecs, microsecs);
		}

		// Returns a time value.
		// These days are measured as 24-hour days.
		static constexpr this_t days(tt_ulong days, tt_ulong hours = 0, tt_ulong minutes = 0, tt_ulong seconds = 0, tt_ulong millisecs = 0, tt_ulong microsecs = 0) noexcept {


			return one_day() * days + this_t::hours(hours, minutes, seconds, millisecs, microsecs);
		}
	};

	// A struct encapsulating a point in, or duration of, time measured in nanoseconds.
	// These values are unsigned, and this must be considered regarding time value arithmetic.
	struct time_value_nano final {

		using this_t = tt::time_value_nano;


		// The count of nanoseconds defining the time value.
		tt_ulong nanosec_count;


		// equality/inequality tests + lexicographical comparison function

		// Returns if this and the given time value are equal.
		constexpr tt_bool equal(this_t x) const noexcept { return nanosec_count == x.nanosec_count; }

		constexpr tt_bool operator==(this_t rhs) const noexcept { return equal(rhs); }
		constexpr tt_bool operator!=(this_t rhs) const noexcept { return !equal(rhs); }

		constexpr tt_bool operator<(this_t rhs) const noexcept { return nanosec_count < rhs.nanosec_count; }
		constexpr tt_bool operator>(this_t rhs) const noexcept { return nanosec_count > rhs.nanosec_count; }
		constexpr tt_bool operator<=(this_t rhs) const noexcept { return nanosec_count <= rhs.nanosec_count; }
		constexpr tt_bool operator>=(this_t rhs) const noexcept { return nanosec_count >= rhs.nanosec_count; }


		constexpr this_t operator+(this_t rhs) const noexcept { return { nanosec_count + rhs.nanosec_count }; }
		constexpr this_t operator-(this_t rhs) const noexcept { return { nanosec_count - rhs.nanosec_count }; }
		constexpr this_t operator*(this_t rhs) const noexcept { return { nanosec_count * rhs.nanosec_count }; }
		constexpr this_t operator/(this_t rhs) const noexcept { return { nanosec_count / rhs.nanosec_count }; }
		constexpr this_t operator%(this_t rhs) const noexcept { return { nanosec_count % rhs.nanosec_count }; }

		constexpr this_t operator*(tt_ulong rhs) const noexcept { return { nanosec_count * rhs }; }
		constexpr this_t operator/(tt_ulong rhs) const noexcept { return { nanosec_count / rhs }; }
		constexpr this_t operator%(tt_ulong rhs) const noexcept { return { nanosec_count % rhs }; }


		inline this_t& operator+=(this_t rhs) noexcept { return *this = *this + rhs; }
		inline this_t& operator-=(this_t rhs) noexcept { return *this = *this - rhs; }
		inline this_t& operator*=(this_t rhs) noexcept { return *this = *this * rhs; }
		inline this_t& operator/=(this_t rhs) noexcept { return *this = *this / rhs; }
		inline this_t& operator%=(this_t rhs) noexcept { return *this = *this % rhs; }

		inline this_t& operator*=(tt_ulong rhs) noexcept { return *this = *this * rhs; }
		inline this_t& operator/=(tt_ulong rhs) noexcept { return *this = *this / rhs; }
		inline this_t& operator%=(tt_ulong rhs) noexcept { return *this = *this % rhs; }


		// Returns a time value of this time value's nanosecond count, filtering out all larger and smaller units.
		constexpr this_t only_nanosecs() const noexcept { return *this % one_microsec(); }

		// Returns a time value of this time value's microsecond count, filtering out all larger and smaller units.
		constexpr this_t only_microsecs() const noexcept { return (*this % one_millisec()) - (*this % one_microsec()); }

		// Returns a time value of this time value's millisecond count, filtering out all larger and smaller units.
		constexpr this_t only_millisecs() const noexcept { return (*this % one_second()) - (*this % one_millisec()); }

		// Returns a time value of this time value's second count, filtering out all larger and smaller units.
		constexpr this_t only_seconds() const noexcept { return (*this % one_minute()) - (*this % one_second()); }

		// Returns a time value of this time value's minute count, filtering out all larger and smaller units.
		constexpr this_t only_minutes() const noexcept { return (*this % one_hour()) - (*this % one_minute()); }

		// Returns a time value of this time value's minute count, filtering out all larger and smaller units.
		// Time values measure 24-hour days as a time unit larger than hours.
		constexpr this_t only_hours() const noexcept { return (*this % one_day()) - (*this % one_hour()); }

		// Returns a time value of this time value's minute count, filtering out all larger and smaller units.
		// These days are measured as 24-hour days.
		constexpr this_t only_days() const noexcept { return *this - (*this % one_day()); }


		// Returns the number of nanoseconds recorded by the time value, filtering out all larger and smaller units.
		constexpr tt_ulong count_nanosecs() const noexcept { return only_nanosecs().nanosec_count; }

		// Returns the number of microseconds recorded by the time value, filtering out all larger and smaller units.
		constexpr tt_ulong count_microsecs() const noexcept { return only_microsecs().nanosec_count / 1'000; }

		// Returns the number of milliseconds recorded by the time value, filtering out all larger and smaller units.
		constexpr tt_ulong count_millisecs() const noexcept { return only_millisecs().nanosec_count / 1'000'000ULL; }

		// Returns the number of seconds recorded by the time value, filtering out all larger and smaller units.
		constexpr tt_ulong count_seconds() const noexcept { return only_seconds().nanosec_count / 1'000'000'000ULL; }

		// Returns the number of minutes recorded by the time value, filtering out all larger and smaller units.
		constexpr tt_ulong count_minutes() const noexcept { return only_minutes().nanosec_count / (60ULL * 1'000'000'000ULL); }

		// Returns the number of hours recorded by the time value, filtering out all larger and smaller units.
		// Time values measure 24-hour days as a time unit larger than hours.
		constexpr tt_ulong count_hours() const noexcept { return only_hours().nanosec_count / (60ULL * 60ULL * 1'000'000'000ULL); }

		// Returns the number of hours recorded by the time value, filtering out all larger and smaller units.
		// These days are measured as 24-hour days.
		constexpr tt_ulong count_days() const noexcept { return only_days().nanosec_count / (24ULL * 60ULL * 60ULL * 1'000'000'000ULL); }


		// Returns a string visualization of the time value as a 24-hour clock, with optional components.
		// If any of the given include_* arguments are false, their respective time unit is omitted from the visualization.
		// If include_millisecs is false, include_microsecs will be set to false.
		// If, following the above, include_microsecs is false, include_nanosecs will be set to false.
		inline tt_string visualize(tt_bool include_days = true, tt_bool include_millisecs = true, tt_bool include_microsecs = true, tt_bool include_nanosecs = true) const noexcept {


			auto _days = count_days();
			auto _hours = count_hours();
			auto _minutes = count_minutes();
			auto _seconds = count_seconds();
			auto _millisecs = count_millisecs();
			auto _microsecs = count_microsecs();
			auto _nanosecs = count_nanosecs();

			tt_string r{};

			if (include_days)
				r += tt::visualize_uint(_days),
				r += ':';

			if (_hours < 10)
				r += '0';

			r += tt::visualize_uint(_hours);

			r += ':';

			if (_minutes < 10)
				r += '0';

			r += tt::visualize_uint(_minutes);

			r += ':';

			if (_seconds < 10)
				r += '0';

			r += tt::visualize_uint(_seconds);

			if (include_millisecs) {


				r += '.';

				if (_millisecs < 100)
					r += '0';

				if (_millisecs < 10)
					r += '0';

				r += tt::visualize_uint(_millisecs);

				if (include_millisecs) {


					if (_microsecs < 100)
						r += '0';

					if (_microsecs < 10)
						r += '0';

					r += tt::visualize_uint(_microsecs);

					if (include_nanosecs) {


						if (_nanosecs < 100)
							r += '0';

						if (_nanosecs < 10)
							r += '0';

						r += tt::visualize_uint(_nanosecs);
					}
				}
			}

			return r;
		}

		// Returns a hash of the time value.
		constexpr tt_size hash() const noexcept {


			return (tt_size)nanosec_count;
		}


		inline explicit operator tt::time_value() const noexcept {


			return { nanosec_count / 1'000 };
		}


		// Returns a time value of zero.
		static constexpr this_t zero() noexcept { return { 0 }; }

		// Returns the maximum possible time value.
		static constexpr this_t maximum() noexcept { return { tt::max_ulong }; }


		// Returns a time value of the current system time.
		static inline this_t now() noexcept {


			return { static_cast<tt_ulong>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) };
		}


		// Returns a time value.
		static constexpr this_t one_nanosec() noexcept { return { 1 }; }

		// Returns a time value.
		static constexpr this_t one_microsec() noexcept { return one_nanosec() * 1'000ULL; }

		// Returns a time value.
		static constexpr this_t one_millisec() noexcept { return one_microsec() * 1'000ULL; }

		// Returns a time value.
		static constexpr this_t one_second() noexcept { return one_millisec() * 1'000ULL; }

		// Returns a time value.
		static constexpr this_t one_minute() noexcept { return one_second() * 60ULL; }

		// Returns a time value.
		// Time values measure 24-hour days as a time unit larger than hours.
		static constexpr this_t one_hour() noexcept { return one_minute() * 60ULL; }

		// Returns a time value.
		// These days are measured as 24-hour days.
		static constexpr this_t one_day() noexcept { return one_hour() * 24ULL; }


		// Returns a time value.
		static constexpr this_t nanosecs(tt_ulong nanosecs) noexcept {


			return one_nanosec() * nanosecs;
		}

		// Returns a time value.
		static constexpr this_t microsecs(tt_ulong microsecs, tt_ulong nanosecs = 0) noexcept {


			return one_microsec() * microsecs + this_t::nanosecs(nanosecs);
		}

		// Returns a time value.
		static constexpr this_t millisecs(tt_ulong millisecs, tt_ulong microsecs = 0, tt_ulong nanosecs = 0) noexcept {


			return one_millisec() * millisecs + this_t::microsecs(microsecs, nanosecs);
		}

		// Returns a time value.
		static constexpr this_t seconds(tt_ulong seconds, tt_ulong millisecs = 0, tt_ulong microsecs = 0, tt_ulong nanosecs = 0) noexcept {


			return one_second() * seconds + this_t::millisecs(millisecs, microsecs, nanosecs);
		}

		// Returns a time value.
		static constexpr this_t minutes(tt_ulong minutes, tt_ulong seconds = 0, tt_ulong millisecs = 0, tt_ulong microsecs = 0, tt_ulong nanosecs = 0) noexcept {


			return one_minute() * minutes + this_t::seconds(seconds, millisecs, microsecs, nanosecs);
		}

		// Returns a time value.
		// Time values measure 24-hour days as a time unit larger than hours.
		static constexpr this_t hours(tt_ulong hours, tt_ulong minutes = 0, tt_ulong seconds = 0, tt_ulong millisecs = 0, tt_ulong microsecs = 0, tt_ulong nanosecs = 0) noexcept {


			return one_hour() * hours + this_t::minutes(minutes, seconds, millisecs, microsecs, nanosecs);
		}

		// Returns a time value.
		// These days are measured as 24-hour days.
		static constexpr this_t days(tt_ulong days, tt_ulong hours = 0, tt_ulong minutes = 0, tt_ulong seconds = 0, tt_ulong millisecs = 0, tt_ulong microsecs = 0, tt_ulong nanosecs = 0) noexcept {


			return one_day() * days + this_t::hours(hours, minutes, seconds, millisecs, microsecs, nanosecs);
		}
	};
}

inline tt::time_value::operator tt::time_value_nano() const noexcept {


	return { microsec_count * 1'000ULL };
}

TT_VISUALIZERS(0) {


	TT_REGISTER_VISUALIZE(tt::time_value);
	TT_REGISTER_VISUALIZE(tt::time_value_nano);
}

TT_HASHERS(0) {


	TT_REGISTER_HASH(tt::time_value);
	TT_REGISTER_HASH(tt::time_value_nano);
}

