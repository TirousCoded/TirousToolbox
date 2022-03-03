

#pragma once


// A header file of general purpose file and filesystem utilities.


#include "aliases.h"

#include "chunk.h"

#include <fstream>
#include <filesystem>


namespace tt {


	namespace fs = std::filesystem;
}

using tt_filepath = tt::fs::path;

namespace tt {


	// Returns the file path associated with the current working directory.
	inline tt_filepath working_directory() {


		return tt::fs::current_path();
	}

	// Returns if the file at x exists or not.
	inline tt_bool file_exists(const tt_filepath& x) {


		std::ifstream fs(x);

		// implicitly closes fs

		return fs.is_open();
	}

	// A struct representing file data loaded all at once from a file.
	struct loaded_file_info final {

		// If the file loaded successfully or not.
		tt_bool success = false;

		// The file path which was attempted to be loaded.
		tt_filepath path = {};

		// The chunk of binary data loaded from the file.
		tt::chunk<1> data = {};
	};

	// Loads the contents of the file at f all at once.
	inline tt::loaded_file_info load_file(tt_filepath f) {


		tt::loaded_file_info r{};

		r.path = std::move(f);

		std::ifstream s{};

		s.open(r.path, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);

		if (s.is_open()) {


			// set buffer size based on file size, measured by seek position starting at file end

			// TODO: If something ever happens, it may be due to s.tellg()'s 64-bit value being converted
			//		 to a 32-bit value and overflowing when compiling for 32-bit.

			r.data.resize((tt_size)s.tellg());

			s.seekg(0);

			s.read(r.data.get<tt_char>(0), r.data.size_bytes());

			// implicitly test of non-failbit and set success based on what s.read above set it to.

			r.success = (tt_bool)s;

			s.close();
		}

		return r;
	}

	// A struct encapsulating information about file data saved to a file.
	struct saved_file_info final {

		// If the file saved successfully or not.
		tt_bool success = false;

		// The file path which was attempted to be saved.
		tt_filepath path = {};
	};

	// Saves the contents of x to the file at f, creating a new file, or overwriting an existing one in the process.
	// If append is true, the contents of f will be appended instead of overwritten.
	inline tt::saved_file_info save_file(tt::chunk_view<1> x, tt_filepath f, tt_bool append = false) {


		tt::saved_file_info r{};

		r.path = std::move(f);

		std::ofstream s{};

		auto bf = std::ios_base::out | std::ios_base::binary;

		if (append)
			bf |= std::ios_base::app;
		else
			bf |= std::ios_base::trunc;

		s.open(r.path, bf);

		if (s.is_open()) {


			s.write(x.get<tt_char>(0), x.size_bytes());

			// implicitly test of non-failbit and set success based on what s.write above set it to.

			r.success = (tt_bool)s;

			s.close();
		}

		return r;
	}

	// Saves the contents of x to the file at f, creating a new file, or overwriting an existing one in the process.
	// If append is true, the contents of f will be appended instead of overwritten.
	inline tt::saved_file_info save_file(const tt::chunk<1>& x, tt_filepath f, tt_bool append = false) {


		return tt::save_file(x.view(), f, append);
	}

	// Saves the contents of (n bytes long) buffer x to the file at f, creating a new file, or overwriting an existing one in the process.
	// If append is true, the contents of f will be appended instead of overwritten.
	inline tt::saved_file_info save_file(const tt_byte* const x, tt_size n, tt_filepath f, tt_bool append = false) {


		return tt::save_file(tt::chunk_view<1>(x, n), f, append);
	}
}

