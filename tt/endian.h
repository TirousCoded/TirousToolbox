

#pragma once


// A header file defining endianness in the Tirous Toolbox library.

// I've decided that for API *clean-ness* we'll include endianness based memory
// reading/writing utilities here as well.


#include "aliases.h"


namespace tt {


	// An enumeration of endiannesses.
	enum class endian : tt_byte {


		// Big Endian
		BIG,

		// Little Endian
		LITTLE,

		// Endianness of the user's machine.
		// This value is not an endianness, but a signal to the system to use the user's machine's endianness.
		// In practice, this may be used as BIG and LITTLE can be, care should be taken however that the user understand what a given usage is actually doing to avoid potentially subtle bugs.
		NATIVE
	};


	// Returns the native endianness of the user's machine.
	inline tt::endian get_native_endian() noexcept {


		// Not 100% sure if we should/need-to add volatile to our n variable here.

		// I think we should however, as otherwise the compiler might 'compile-out' the test and thus ruin it, 
		// though I don't entirely know.

		const volatile tt_int n = 1;

		return (*reinterpret_cast<const volatile tt_sbyte*>(&n) == 1) ? tt::endian::LITTLE : tt::endian::BIG;
	}

	// Returns the endianness passed, with NATIVE returning BIG or LITTLE.
	constexpr tt::endian get_endian(tt::endian x) noexcept { return (x == tt::endian::NATIVE) ? get_native_endian() : x; }

	// Returns if the endianness passed is BIG, resolving NATIVE accordingly.
	constexpr tt_bool is_big_endian(tt::endian x) noexcept { return get_endian(x) == tt::endian::BIG; }

	// Returns if the endianness passed is LITTLE, resolving NATIVE accordingly.
	constexpr tt_bool is_little_endian(tt::endian x) noexcept { return get_endian(x) == tt::endian::LITTLE; }

	// Returns if the endianness passed is NATIVE or the endianness of the user's machine.
	inline tt_bool is_native_endian(tt::endian x) noexcept { return get_endian(x) == get_native_endian(); }


	// Reads the bytes at 'from' into object 'to', interpreting them as being in the byte order provided.
	// If bytes_read is not nullptr, it will be set to the amount of bytes read by the operation.
	template<typename T, typename P> 
	inline void read_to(const P* const from, T& to, tt::endian byte_order = tt::endian::NATIVE, tt_size* bytes_read = nullptr) noexcept {


		bool t = is_native_endian(byte_order);

		TT_FOR(i, sizeof(T))
			((tt_sbyte*)&to)[i] = ((const tt_sbyte*)from)[t ? i : (sizeof(T) - i - 1)];

		if (bytes_read)
			*bytes_read = sizeof(T);
	}

	// Reads the bytes at 'from' into object 'to', interpreting them as being in big endian byte order.
	// If bytes_read is not nullptr, it will be set to the amount of bytes read by the operation.
	template<typename T, typename P> 
	inline void read_to_be(const P* const from, T& to, tt_size* bytes_read = nullptr) noexcept { read_to(from, to, tt::endian::BIG, bytes_read); }

	// Reads the bytes at 'from' into object 'to', interpreting them as being in little endian byte order.
	// If bytes_read is not nullptr, it will be set to the amount of bytes read by the operation.
	template<typename T, typename P> 
	inline void read_to_le(const P* const from, T& to, tt_size* bytes_read = nullptr) noexcept { read_to(from, to, tt::endian::LITTLE, bytes_read); }

	// Reads the bytes at 'from', interpreting them as being in the byte order provided, returning the value resolved.
	// If bytes_read is not nullptr, it will be set to the amount of bytes read by the operation.
	template<typename T, typename P> 
	inline T read(const P* const from, tt::endian byte_order = tt::endian::NATIVE, tt_size* bytes_read = nullptr) noexcept {


		T R;

		tt::read_to(from, R, byte_order, bytes_read);

		return R;
	}

	// Reads the bytes at 'from', interpreting them as being in big endian byte order, returning the value resolved.
	// If bytes_read is not nullptr, it will be set to the amount of bytes read by the operation.
	template<typename T, typename P> 
	inline T read_be(const P* const from, tt_size* bytes_read = nullptr) noexcept { return read(from, tt::endian::BIG, bytes_read); }

	// Reads the bytes at 'from', interpreting them as being in little endian byte order, returning the value resolved.
	// If bytes_read is not nullptr, it will be set to the amount of bytes read by the operation.
	template<typename T, typename P> 
	inline T read_le(const P* const from, tt_size* bytes_read = nullptr) noexcept { return read(from, tt::endian::LITTLE, bytes_read); }


	// Writes the bytes at object 'from' into 'to', interpreting them as being in the byte order provided.
	// If bytes_written is not nullptr, it will be set to the amount of bytes written by the operation.
	template<typename T, typename P> 
	inline void write_to(const T& from, P* const to, tt::endian byte_order = tt::endian::NATIVE, tt_size* bytes_written = nullptr) noexcept {


		bool t = is_native_endian(byte_order);

		TT_FOR(i, sizeof(T))
			reinterpret_cast<tt_sbyte*>(to)[i] = reinterpret_cast<const tt_sbyte*>(&from)[(t) ? i : (sizeof(T) - i - 1)];

		if (bytes_written)
			*bytes_written = sizeof(T);
	}

	// Writes bytes at object 'from' into 'to', interpreting them as being in big endian byte order.
	// If bytes_written is not nullptr, it will be set to the amount of bytes written by the operation.
	template<typename T, typename P> 
	inline void write_to_be(const T& from, P* const to, tt_size* bytes_written = nullptr) noexcept {


		write_to(from, to, tt::endian::BIG, bytes_written);
	}

	// Writes bytes at object 'from' into 'to', interpreting them as being in little endian byte order.
	// If bytes_written is not nullptr, it will be set to the amount of bytes written by the operation.
	template<typename T, typename P> 
	inline void write_to_le(const T& from, P* const to, tt_size* bytes_written = nullptr) noexcept {


		write_to(from, to, tt::endian::LITTLE, bytes_written);
	}
}

