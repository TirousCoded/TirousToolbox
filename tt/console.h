

#pragma once


// A header file defining object-orientated wrappers for standard console I/O.


#include <iostream>

#include "aliases.h"
#include "macros.h"


namespace tt {


	// Outputs the given values to the console via std::cout.
	// The sequence is processed via recursion.
	inline void print() {}

	// Outputs the given values to the console via std::cout.
	// The sequence is processed via recursion.
	template<typename Arg, typename... Args>
	inline void print(Arg&& arg, Args&&... args) {


		std::cout << arg;

		print(TT_FMOVE_N(Args, args));
	}

	// Outputs the given values to the console via std::wcout.
	// The sequence is processed via recursion.
	inline void wprint() {}

	// Outputs the given values to the console via std::wcout.
	// The sequence is processed via recursion.
	template<typename Arg, typename... Args>
	inline void wprint(Arg&& arg, Args&&... args) {


		std::wcout << arg;

		wprint(TT_FMOVE_N(Args, args));
	}

	// Outputs a newline to the console via std::cout.
	inline void endline() {


		std::cout << std::endl;
	}

	// Outputs a newline to the console via std::wcout.
	inline void wendline() {


		std::wcout << std::endl;
	}

	// Outputs the given values to the console via tt::print, followed by a newline.
	template<typename... Args>
	inline void printl(Args&&... args) {


		print(TT_FMOVE_N(Args, args));

		endline();
	}

	// Outputs the given values to the console via tt::wprint, followed by a newline.
	template<typename... Args>
	inline void wprintl(Args&&... args) {


		wprint(TT_FMOVE_N(Args, args));

		wendline();
	}

	// Returns the next character from the console via std::cin.
	inline tt_char input() {


		return std::cin.get();
	}

	// Returns the next character from the console via std::wcin.
	inline tt_wchar winput() {


		return std::wcin.get();
	}

	// Returns the next line from the console via std::cin.
	inline tt_string inputl() {


		tt_string r;

		std::getline(std::cin, r);

		return r;
	}

	// Returns the next line from the console via std::wcin.
	inline tt_wstring winputl() {


		tt_wstring r;

		std::getline(std::wcin, r);

		return r;
	}
}

