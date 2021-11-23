

#pragma once


// A header file detailing the macros of the Tirous Toolbox library.


// A macro which expands to a string of x.
// The contents of x are NOT expanded first, use TT_XSTR for that.
#define TT_STR(x) #x

// A macro which expands to a string of x.
// This version expands the contents of x, unlike TT_STR
#define TT_XSTR(x) TT_STR(x)

// A macro which expands to an expression which discards return value, if any, of the passed expression.
#define TT_NO_RETURN(expression) ((void)(expression))

// A preprocessor definition which expands to an expression which is inert and does nothing.
#define TT_DO_NOTHING TT_NO_RETURN(0)


// A macro which expands to an expression which only evaluates expression if condition is false.
// Any return value from expression is discarded, and thus the generated expression does not return.
#define TT_EVAL_IF_NOT(condition, expression) TT_NO_RETURN((condition) || (TT_NO_RETURN(expression), 0))

// A macro which expands to an expression which only evaluates expression if condition is true.
// Any return value from expression is discarded, and thus the generated expression does not return.
#define TT_EVAL_IF(condition, expression) TT_EVAL_IF_NOT((!(condition)), expression)


// A preprocessor definition which returns '*this'.
#define TT_RETURN_THIS return *this


// A helper macro used to define a simple for-loop.
#define TT_FOR(i, n) for (tt_size i = 0; i < n; ++i)

// A helper macro used to define a manual iterator for-loop.
#define TT_FOR_ITER(it, f, e) for (auto it = f; it != e; it = std::next(it))

// A helper macro used to define a range-based for-loop.
#define TT_FOR_RANGE(I, C) for (auto& I : C)


// A helper macro used to automate the copying of a class variable, without template information.
#define TT_COPY(x, v) x = v.x

// A helper macro used to automate the moving of a class variable, without template information.
#define TT_MOVE(x, v) x = std::move(v.x)

// A version of TT_MOVE suitable for when we need to set our moved-from variable following moving.
#define TT_MOVESET(x, v, x_set) TT_MOVE(x, v), v.x = x_set

// A version of TT_MOVE suitable for use with pointers which must be nullified after moving.
#define TT_MOVEPTR(x, v) TT_MOVESET(x, v, nullptr)


// A helper macro for use in move-assignment operators to detect if we should exit when on self-assignment.
#define TT_SELF_MOVE(v) (&v == this)

// An elaboration on TT_SELF_MOVE's usage in move-assignment operators.
#define TT_SELF_MOVE_TEST(v) if (TT_SELF_MOVE(v)) TT_RETURN_THIS

// A version of TT_SELF_MOVE_TEST for the occasions when self-copy-assignment can destructively interfere.
#define TT_SELF_COPY_TEST(v) TT_SELF_MOVE_TEST(v)


// A helper macro to invoke std::forward for a single rvalue-reference.
#define TT_FMOVE(t, a) std::forward<t&&>(a)

// A helper macro to invoke std::forward for a parameter pack of rvalue-references.
#define TT_FMOVE_N(t, a) TT_FMOVE(t, a)...


// A macro wrapping the common "std::cout << x" pattern.
#define TT_PRINT(x) TT_NO_RETURN(std::cout << x)

// A macro wrapping the common "std::cout << x << std::endl" pattern.
#define TT_PRINTL(x) TT_NO_RETURN(std::cout << x << std::endl)

// A macro wrapping the common "std::cerr << x" pattern.
#define TT_PRINTERR(x) TT_NO_RETURN(std::cerr << x)

// A macro wrapping the common "std::cerr << x << std::endl" pattern.
#define TT_PRINTERRL(x) TT_NO_RETURN(std::cerr << x << std::endl)

// A macro wrapping the common "std::wcout << x" pattern.
#define TT_WPRINT(x) TT_NO_RETURN(std::wcout << x)

// A macro wrapping the common "std::wcout << x << std::endl" pattern.
#define TT_WPRINTL(x) TT_NO_RETURN(std::wcout << x << std::endl)

// A macro wrapping the common "std::wcerr << x" pattern.
#define TT_WPRINTERR(x) TT_NO_RETURN(std::wcerr << x)

// A macro wrapping the common "std::wcerr << x << std::endl" pattern.
#define TT_WPRINTERRL(x) TT_NO_RETURN(std::wcerr << x << std::endl)

// A preprocessor definition wrapping the common "std::cout << std::endl" pattern.
#define TT_ENDLINE TT_NO_RETURN(std::cout << std::endl)

// A preprocessor definition wrapping the common "std::wcout << std::endl" pattern.
#define TT_WENDLINE TT_NO_RETURN(std::wcout << std::endl)


// A macro wrapping std::swap.
#define TT_SWAP(x, y) std::swap(x, y)

// Performs a C-style pointer cast to reinterpret the given lvalue as an lvalue of the given type.
#define TT_REINTERPRET(new_type, x) (*(new_type*)&(x))

