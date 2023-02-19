

#pragma once


// A class encapsulating an immutable string.

// These strings are intended to implement the semantics of C#/Java/Go/etc.-style strings, with
// the thread-safety and cheap "copying" that such a model provides, but which C++-style strings 
// have historically lacked.

// Things like substrings will also NOT generate a new string. Instead, they'll just reference a 
// section of a larger string. This leads to the possibility of a vary small string keeping 
// alive the storage of a much larger one, which is often an acceptable trade-off for speed, as
// well as for ease-of-use.

// These strings will NOT be null terminated.

// These strings will provide hash precomputation.

// These strings can be instructed to NOT take ownership of memory when initialized from a C-string,
// which is to allow for the creation of strings from constant string literals without allocating.
// (See tt::basic_str<Char>::literal in this regard.)


// TODO: if we ever need it, maybe add a concatenating '+' operator overload


#include <memory>
#include <string>
#include <string_view>

#include "aliases.h"
#include "allocation.h"
#include "hash.h"
#include "visualize.h"


namespace tt {


	template<typename Char>
	class basic_str;


	using str	= basic_str<tt_char>;
	using wstr	= basic_str<tt_wchar>;

	using str8	= basic_str<tt_char8>;
	using str16 = basic_str<tt_char16>;
	using str32 = basic_str<tt_char32>;


	struct no_alloc_t final {};

	template<typename Char>
	class basic_str final {
	public:

		using char_t					= typename Char;

		static_assert(std::is_trivially_copyable_v<char_t>, "tt::basic_str<Char>::char_t must be trivially-copyable!");

		using string_t					= std::basic_string<char_t>;
		using string_view_t				= std::basic_string_view<char_t>;

		using this_t					= basic_str<char_t>;


		using iterator					= typename string_view_t::const_iterator;
		using const_iterator			= iterator;
		using reverse_iterator			= typename string_view_t::const_reverse_iterator;
		using const_reverse_iterator	= reverse_iterator;


		// Default initializes an empty string.
		basic_str() = default;

		// Initializes a string of a view v of memory block m.
		// Here m may be nullptr to initialize non-owning strings which simply view memory.
		inline basic_str(const std::shared_ptr<char_t[]>& m, string_view_t v) noexcept;

		// Initializes a string of count characters of chr.
		inline basic_str(tt_size count, char_t chr);

		// Initializes a string of the buffer of s and count.
		// Behaviour is undefined if s is nullptr.
		inline basic_str(const char_t* s, tt_size count);

		// NOTE: mark this as 'explicit' so we don't implicitly heap allocate via a string literal,
		//		 as the end-user might think the system would avoid one in that case, but be wrong

		// Initializes a string of the null-terminated C-string s.
		// This will allocate a copy of s.
		// Behaviour is undefined if s is nullptr.
		explicit inline basic_str(const char_t* s);

		// Initializes a string of the null-terminated C-string s.
		// This will not allocate a copy of s, and so will be invalidated if its lifetime exceeds the memory at s.
		// This can be useful if s is a constant string literal who's memory will always be valid.
		// Behaviour is undefined if s is nullptr.
		inline basic_str(const char_t* s, no_alloc_t) noexcept;

		basic_str(std::nullptr_t, tt_size) = delete;
		basic_str(std::nullptr_t) = delete;
		basic_str(std::nullptr_t, no_alloc_t) noexcept = delete;

		// Initializes a string of other.
		explicit inline basic_str(const string_t& other)
			: basic_str(string_view_t(other)) {}

		// Initializes a string of other.
		inline basic_str(string_view_t other) 
			: basic_str(other.data(), other.length()) {}

		basic_str(const this_t&) = default;
		inline basic_str(this_t&& x) noexcept;

		~basic_str() noexcept = default;

		this_t& operator=(const this_t&) = default;
		inline this_t& operator=(this_t&& rhs) noexcept;


		// Returns the shared memory block used to store the state of the string, if any.
		inline std::shared_ptr<const char_t[]> get_memory_block() const noexcept { return std::static_pointer_cast<const char_t[]>(_storage); }


		inline const char_t* data() const noexcept { return _view.data(); }

		// Throws std::out_of_range if ind is out-of-bounds, providing a strong guarantee.
		inline const char_t& at(tt_size ind) const { return _view.at(ind); }

		inline const char_t& operator[](tt_size ind) const noexcept { return _view[ind]; }

		// Throws std::out_of_range if ind is out-of-bounds, providing a strong guarantee.
		inline const char_t& front() const { return _view.front(); }
		
		// Throws std::out_of_range if ind is out-of-bounds, providing a strong guarantee.
		inline const char_t& back() const { return _view.back(); }


		inline tt_size length() const noexcept { return _view.length(); }
		inline tt_bool has_length() const noexcept { return length() > 0; }
		inline tt_bool empty() const noexcept { return _view.empty(); }


		inline iterator begin() const noexcept { return _view.begin(); }
		inline iterator cbegin() const noexcept { return _view.begin(); }
		inline reverse_iterator rbegin() const noexcept { return _view.rbegin(); }
		inline reverse_iterator crbegin() const noexcept { return _view.rbegin(); }
		
		inline iterator end() const noexcept { return _view.end(); }
		inline iterator cend() const noexcept { return _view.end(); }
		inline reverse_iterator rend() const noexcept { return _view.rend(); }
		inline reverse_iterator crend() const noexcept { return _view.rend(); }


		template<typename Other>
		inline tt_bool equal(const Other& other) const noexcept;


		explicit inline operator string_t() const { return string_t(_view); }
		inline operator string_view_t() const noexcept { return _view; }


		inline string_view_t view(tt_size ind = 0, tt_size len = tt::max_size) const noexcept { return _view.substr(ind, len); }
		inline this_t substr(tt_size ind, tt_size len = tt::max_size) const noexcept;


		constexpr const tt_size& hash() const noexcept { return _hash; }
		inline tt_string vis() const { return tt_string(_view); }


		inline void reset() noexcept { *this = this_t{}; }


		// Returns a string in a manner equivalent to tt::basic_str<Char>(s, tt::no_alloc_t{}).
		static inline this_t lit(const char_t* s) noexcept { return this_t(s, no_alloc_t{}); }


	private:

		struct _Deleter final {

			inline void operator()(char_t* x) const noexcept {


				_dealloc(x);
			}
		};


		// NOTE: remember that we need to use a shared pointer here for thread-safety reasons, as
		//		 std::shared_ptr does some fancy thread-safety stuff internally that we need

		std::shared_ptr<char_t[]>	_storage	= nullptr;
		string_view_t				_view		= {};
		tt_size						_hash		= 0;


		// NOTE: this is the *official* way to perform hash precomputation

		inline tt_size _calc_hash() const noexcept { return hash_of(_view); }


		// NOTE: these just wrap memory allocation/deallocation and building shared pointers

		static inline char_t* _alloc(tt_size count) { return alloc_uninit<char_t>(count); }
		static inline void _dealloc(char_t* x) noexcept { tt_assert(x); dealloc_uninit(x); }

		static inline std::shared_ptr<char_t[]> _alloc_ptr(tt_size count);
	};


	template<typename Char>
	inline tt_bool operator==(const basic_str<Char>& lhs, const basic_str<Char>& rhs) noexcept { return lhs.view() == rhs.view(); }
	template<typename Char>
	inline tt_bool operator==(const basic_str<Char>& lhs, std::basic_string_view<Char> rhs) noexcept { return lhs.view() == rhs; }
	template<typename Char>
	inline tt_bool operator==(const basic_str<Char>& lhs, const std::basic_string<Char>& rhs) noexcept { return lhs.view() == (std::basic_string_view<Char>)rhs; }
	template<typename Char>
	inline tt_bool operator==(const basic_str<Char>& lhs, const Char* rhs) noexcept { return lhs.view() == std::basic_string_view<Char>(rhs); }
	template<typename Char>
	inline tt_bool operator==(std::basic_string_view<Char> lhs, const basic_str<Char>& rhs) noexcept { return rhs == lhs; }
	template<typename Char>
	inline tt_bool operator==(const std::basic_string<Char>& lhs, const basic_str<Char>& rhs) noexcept { return rhs == lhs; }
	template<typename Char>
	inline tt_bool operator==(const Char* lhs, const basic_str<Char>& rhs) noexcept { return rhs == lhs; }

	template<typename Char>
	inline tt_bool operator!=(const basic_str<Char>& lhs, const basic_str<Char>& rhs) noexcept { return !(lhs == rhs); }
	template<typename Char>
	inline tt_bool operator!=(const basic_str<Char>& lhs, std::basic_string_view<Char> rhs) noexcept { return !(lhs == rhs); }
	template<typename Char>
	inline tt_bool operator!=(const basic_str<Char>& lhs, const std::basic_string<Char>& rhs) noexcept { return !(lhs == rhs); }
	template<typename Char>
	inline tt_bool operator!=(const basic_str<Char>& lhs, const Char* rhs) noexcept { return !(lhs == rhs); }
	template<typename Char>
	inline tt_bool operator!=(std::basic_string_view<Char> lhs, const basic_str<Char>& rhs) noexcept { return !(lhs == rhs); }
	template<typename Char>
	inline tt_bool operator!=(const std::basic_string<Char>& lhs, const basic_str<Char>& rhs) noexcept { return !(lhs == rhs); }
	template<typename Char>
	inline tt_bool operator!=(const Char* lhs, const basic_str<Char>& rhs) noexcept { return !(lhs == rhs); }


	namespace string_literals {


		inline str8 operator""_tt(const tt_char8 * s, tt_size len) { return str8(nullptr, tt_string8_view(s, len)); }
		inline str16 operator""_tt(const tt_char16 * s, tt_size len) { return str16(nullptr, tt_string16_view(s, len)); }
		inline str32 operator""_tt(const tt_char32 * s, tt_size len) { return str32(nullptr, tt_string32_view(s, len)); }
		inline wstr operator""_tt(const tt_wchar * s, tt_size len) { return wstr(nullptr, tt_wstring_view(s, len)); }
	}
	

	template<typename Char>
	inline tt::basic_str<Char>::basic_str(const std::shared_ptr<char_t[]>& m, string_view_t v) noexcept 
		: _storage(m), 
		_view(v), 
		_hash(_calc_hash()) {}
	
	template<typename Char>
	inline basic_str<Char>::basic_str(tt_size count, char_t chr) 
		: basic_str() {


		if (count > 0)
			_storage = _alloc_ptr(count),
			_view = string_view_t(_storage.get(), count),
			fill_array<char_t>(_storage.get(), count, chr),
			_hash = _calc_hash(); // <- NOTE: this must happen AFTER we fill our array!
	}
	
	template<typename Char>
	inline basic_str<Char>::basic_str(const char_t* s, tt_size count) 
		: basic_str() {


		tt_assert(s);

		if (count > 0)
			_storage = _alloc_ptr(count),
			_view = string_view_t(_storage.get(), count),
			copy_block(s, _storage.get(), count),
			_hash = _calc_hash(); // <- NOTE: this must happen AFTER we copy our array!
	}
	
	template<typename Char>
	inline basic_str<Char>::basic_str(const char_t* s) 
		: basic_str(s, measure_cstr(s)) {}
	
	template<typename Char>
	inline basic_str<Char>::basic_str(const char_t* s, no_alloc_t) noexcept 
		: _storage(nullptr), 
		_view(s, measure_cstr(s)), 
		_hash(_calc_hash()) {}
	
	template<typename Char>
	inline basic_str<Char>::basic_str(this_t&& x) noexcept {


		TT_MOVE(_storage, x);
		TT_MOVE(_view, x);
		TT_MOVESET(_hash, x, x._calc_hash());
	}
	
	template<typename Char>
	inline typename basic_str<Char>::this_t& basic_str<Char>::operator=(this_t&& rhs) noexcept {


		TT_SELF_MOVE_TEST(rhs);

		TT_MOVE(_storage, rhs);
		TT_MOVE(_view, rhs);
		TT_MOVESET(_hash, rhs, rhs._calc_hash());

		TT_RETURN_THIS;
	}

	template<typename Char>
	template<typename Other>
	inline tt_bool tt::basic_str<Char>::equal(const Other& other) const noexcept {


		return *this == other;
	}

	template<typename Char>
	inline typename basic_str<Char>::this_t basic_str<Char>::substr(tt_size ind, tt_size len) const noexcept {


		auto r = *this;

		r._view = r._view.substr(ind, len);
		r._hash = r._calc_hash();

		return r;
	}

	template<typename Char>
	inline std::shared_ptr<typename basic_str<Char>::char_t[]> basic_str<Char>::_alloc_ptr(tt_size count) {


		tt_assert(count > 0);

		return std::shared_ptr<char_t[]>(
			_alloc(count), 
			_Deleter{});
	}
}

TT_HASHERS(0) { TT_REGISTER_HASH_T(tt::basic_str, <typename Char>, <Char>); }
TT_VISUALIZERS(0) { TT_REGISTER_VISUALIZE_T(tt::basic_str, <typename Char>, <Char>); }

