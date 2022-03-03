

#pragma once


// A class representing a string-like block of general purpose uninitialized memory.


#include "aliases.h"
#include "macros.h"
#include "debug.h"

#include "memory_util.h"
#include "math_util.h"

#include "exceptions.h"

#include "forward_declarations.h"

#include "contiguous_iterator.h"
#include "aligned_allocator.h"

#include "visualize.h"

#include "hash.h"
#include "hash_functions.h"

#include "fprint.h"

#include "endian.h"


namespace tt {


	// The internal struct used to define the aligned units of tt::chunk and tt::chunk_view.
	template<tt_size Alignment>
	struct chunk_unit final {

		alignas(Alignment)
		tt_byte data[Alignment] = {};
	};

	// The memory-view class associated with tt::chunk.
	template<tt_size Alignment>
	class chunk_view final {
	public:

		// The alignment of the chunk view.
		static constexpr tt_size ALIGNMENT = Alignment;

		using this_t = tt::chunk_view<ALIGNMENT>;

		using chunk_unit_t = tt::chunk_unit<ALIGNMENT>;


	private:

		chunk_unit_t* _data;

		// size is a measure of chunk units

		tt_size _size;


	public:

		// Default initializes a chunk view.
		inline chunk_view() noexcept {


			_data = nullptr;
			_size = 0;
		}

		// Initializes a chunk view of the given block of memory.
		// Unless otherwise specified, indices and sizes in chunk views are measured in alignment-sized 'units' of bytes.
		template<typename T>
		inline chunk_view(T* const x, tt_size n) noexcept {


			_data = (chunk_unit_t*)x;
			_size = n;
		}

		// Copy-initializes a chunk view.
		inline chunk_view(const this_t& x) noexcept {


			TT_COPY(_data, x);
			TT_COPY(_size, x);
		}

		// Move-initializes a chunk view.
		inline chunk_view(this_t&& x) noexcept {


			TT_MOVEPTR(_data, x);
			TT_MOVE(_size, x);
		}

		inline ~chunk_view() noexcept = default;

		// Assigns the chunk view.
		inline this_t& assign(const this_t& rhs) noexcept {


			TT_COPY(_data, rhs);
			TT_COPY(_size, rhs);

			TT_RETURN_THIS;
		}

		// Assigns the chunk view.
		inline this_t& assign(this_t&& rhs) noexcept {


			TT_SELF_MOVE_TEST(rhs);

			TT_MOVEPTR(_data, rhs);
			TT_MOVE(_size, rhs);

			TT_RETURN_THIS;
		}

		inline this_t& operator=(const this_t& rhs) noexcept { return assign(rhs); }
		inline this_t& operator=(this_t&& rhs) noexcept { return assign(TT_FMOVE(this_t, rhs)); }

		// Returns the data of the chunk view.
		constexpr chunk_unit_t* const data() noexcept { return _data; }

		// Returns the data of the chunk view.
		constexpr const chunk_unit_t* const data() const noexcept { return _data; }

		// Returns the size of the chunk view.
		constexpr tt_size size() const noexcept { return _size; }

		// Returns the number of bytes in a chunk view unit.
		// This value is equal to the chunk view's Alignment.
		constexpr tt_size unit_bytes() const noexcept { return ALIGNMENT; }

		// Returns the given unit index/size value converted to its byte index/size equivalent, in regards to this chunk's type.
		constexpr tt_size units_to_bytes(tt_size units) const noexcept { return units * unit_bytes(); }

		// Returns the size of the chunk view, in bytes.
		constexpr tt_size size_bytes() const noexcept { return units_to_bytes(size()); }

		// Returns if the chunk view is non-empty.
		constexpr tt_bool has_size() const noexcept { return size() > 0; }

		// Returns if the chunk view is empty.
		constexpr tt_bool empty() const noexcept { return size() == 0; }

		// Returns if the given unit index ind is in-bounds of the chunk view.
		// Unless otherwise specified, indices and sizes in chunk views are measured in alignment-sized 'units' of bytes.
		constexpr tt_bool in_bounds(tt_size ind) const noexcept { return ind < size(); }

		// Returns if the given byte (not unit) index byte_ind is in-bounds of the chunk view.
		constexpr tt_bool in_bounds_byte(tt_size byte_ind) const noexcept { return byte_ind < size_bytes(); }

		// Returns if an object of type T, at the given unit index ind, is in-bounds, and fits completely within, the chunk view.
		// Unless otherwise specified, indices and sizes in chunk views are measured in alignment-sized 'units' of bytes.
		template<typename T>
		constexpr tt_bool in_bounds_and_fits(tt_size ind) const noexcept {


			return in_bounds(ind) && (size_bytes() - units_to_bytes(ind)) >= tt::aligned_size_of<tt_size, T>(unit_bytes());
		}

		// Returns a pointer to an individual byte of the chunk view at index byte_ind.
		// This index byte_ind is an index into the underlying byte array of the chunk view.
		// This does not perform bounds checking.
		inline tt_byte* const get_byte_unchecked(tt_size byte_ind) noexcept {


			return ((tt_byte*)data()) + byte_ind;
		}

		// Returns a pointer to an individual byte of the chunk view at index byte_ind.
		// This index byte_ind is an index into the underlying byte array of the chunk view.
		// This does not perform bounds checking.
		inline const tt_byte* const get_byte_unchecked(tt_size byte_ind) const noexcept {


			return ((const tt_byte*)data()) + byte_ind;
		}

		// Returns a pointer to an individual byte of the chunk view at index byte_ind.
		// This index byte_ind is an index into the underlying byte array of the chunk view.
		// Throws tt::out_of_range_error if ind is out-of-range.
		inline tt_byte* const get_byte(tt_size byte_ind) {


			if (!in_bounds_byte(byte_ind))
				TT_THROW(tt::out_of_range_error, "tt::chunk_view get_byte index byte_ind is out-of-range!");

			return get_byte_unchecked(byte_ind);
		}

		// Returns a pointer to an individual byte of the chunk view at index byte_ind.
		// This index byte_ind is an index into the underlying byte array of the chunk view.
		// Throws tt::out_of_range_error if ind is out-of-range.
		inline const tt_byte* const get_byte(tt_size byte_ind) const {


			if (!in_bounds_byte(byte_ind))
				TT_THROW(tt::out_of_range_error, "tt::chunk_view get_byte index byte_ind is out-of-range!");

			return get_byte_unchecked(byte_ind);
		}

		// Returns a pointer to the memory at unit index ind as type T.
		// This does not perform bounds checking.
		// Unless otherwise specified, indices and sizes in chunk views are measured in alignment-sized 'units' of bytes.
		template<typename T>
		inline T* const get_unchecked(tt_size ind) noexcept {


			return (T* const)(data() + ind);
		}

		// Returns a pointer to the memory at unit index ind as type T.
		// This does not perform bounds checking.
		// Unless otherwise specified, indices and sizes in chunk views are measured in alignment-sized 'units' of bytes.
		template<typename T>
		inline const T* const get_unchecked(tt_size ind) const noexcept {


			return (const T* const)(data() + ind);
		}

		// Returns a pointer to the memory at unit index ind as type T.
		// Throws tt::out_of_range_error if ind is out-of-range, or if an object of type T at ind would exceed the chunk view's bounds.
		// Unless otherwise specified, indices and sizes in chunk views are measured in alignment-sized 'units' of bytes.
		template<typename T>
		inline T* const get(tt_size ind) {


			if (!in_bounds_and_fits<T>(ind))
				TT_THROW(tt::out_of_range_error, "tt::chunk_view get index ind is out-of-range!");

			return get_unchecked<T>(ind);
		}

		// Returns a pointer to the memory at unit index ind as type T.
		// Throws tt::out_of_range_error if ind is out-of-range, or if an object of type T at ind would exceed the chunk view's bounds.
		// Unless otherwise specified, indices and sizes in chunk views are measured in alignment-sized 'units' of bytes.
		template<typename T>
		inline const T* const get(tt_size ind) const {


			if (!in_bounds_and_fits<T>(ind))
				TT_THROW(tt::out_of_range_error, "tt::chunk_view get index ind is out-of-range!");

			return get_unchecked<T>(ind);
		}

		// Returns a chunk view of the given subsection of the chunk view.
		// The portion viewed starts at unit index ind in the chunk view and continues for the first n units.
		// If n is zero, the viewed portion will continue to the end of the chunk view.
		// If ind is out-of-range, or n makes it extend out-of-range, the subsection viewed will be reduced to fit, or reduced to zero.
		// Unless otherwise specified, indices and sizes in chunk views are measured in alignment-sized 'units' of bytes.
		inline this_t view(tt_size ind = 0, tt_size n = 0) const noexcept {


			if (n == 0)
				n = size(); // <- this'll be reduced to fit below

			if (!in_bounds(ind))
				n = 0;

			else if (size() - ind < n)
				n = size() - ind;

			if (n == 0) // <- post 'n == 0' rule above
				return {};

			return this_t(get_byte_unchecked(0) + units_to_bytes(ind), n);
		}

		// Returns a string of the contents of the chunk view.
		// If byte_order doesn't match up with the user's machine's native endianness, then the bytes of each character of the returned string will have their byte order flipped.
		// The string's null terminator is not included in the section of memory used to build the string.
		// The length of this string is equal to the number of sizeof(Char) blocks which can be fully filled starting from the beginning of the chunk view.
		// This function operates upon the entire chunk view, and thus view should be used to extract the specific portion desired.
		template<typename Char>
		inline std::basic_string<Char> extract_str(tt::endian byte_order = tt::endian::NATIVE) const {


			auto r = std::basic_string<Char>(get<Char>(0), tt::division_instances(size_bytes(), sizeof(Char)));

			if (!tt::is_native_endian(byte_order))
				for (auto& I : r)
					I = tt::flip_bytes(I);

			return r;
		}

		// Returns a string visualization of the chunk view.
		// The portion visualized starts at unit index ind in the chunk view and continues for the first n units.
		// If n is zero, the visualization will continue to the end of the chunk view.
		// If ind is out-of-range, or n makes it extend out-of-range, the block visualized will be reduced to fit, or reduced to zero.
		// Unless otherwise specified, indices and sizes in chunk views are measured in alignment-sized 'units' of bytes.
		inline tt_string vis(tt_size ind = 0, tt_size n = 0) const {


			if (n == 0)
				n = size(); // <- this'll be reduced to fit below

			if (!in_bounds(ind))
				n = 0;

			else if (size() - ind < n)
				n = size() - ind;

			tt_size nn = units_to_bytes(n);

			tt_string r{};

			r.reserve(nn * 2U);

			TT_FOR(i, nn)
				r += tt::vis_byte_hex(*get_byte_unchecked(i), true);

			return tt::fprint_s("{ index=", ind, ", bytes=", size_bytes(), ", unit bytes=", unit_bytes(), ", ", r, " }");
		}

		// Returns a hash of the chunk view.
		inline tt_size hash() const noexcept {


			return tt::array_hash_of(get_byte_unchecked(0), size_bytes());
		}

		// Fills the given subsection of the chunk view's memory with the given byte value.
		// The portion filled starts at unit index ind in the chunk view and continues for the first n units.
		// If n is zero, the filled portion will continue to the end of the chunk view.
		// If ind is out-of-range, or n makes it extend out-of-range, the subsection filled will be reduced to fit, or reduced to zero.
		// Unless otherwise specified, indices and sizes in chunk views are measured in alignment-sized 'units' of bytes.
		inline this_t& fill(tt_byte x, tt_size ind = 0, tt_size n = 0) noexcept {


			if (n == 0)
				n = size(); // <- this'll be reduced to fit below

			if (!in_bounds(ind))
				n = 0;

			else if (size() - ind < n)
				n = size() - ind;

			if (n == 0) // <- post 'n == 0' rule above
				TT_RETURN_THIS;

			std::memset((void*)(data() + ind), (int)x, units_to_bytes(n));

			TT_RETURN_THIS;
		}

		// Resets the chunk view.
		inline this_t& reset() noexcept {


			_data = nullptr;
			_size = 0;
		}
	};

	// A class representing a string-like block of general purpose uninitialized memory.
	template<tt_size Alignment, typename Allocator>
	class chunk final {
	public:

		// The alignment of the chunk.
		static constexpr tt_size ALIGNMENT = Alignment;

		using allocator_t = typename Allocator;

		using this_t = tt::chunk<ALIGNMENT, allocator_t>;

		using chunk_unit_t = tt::chunk_unit<ALIGNMENT>;
		using chunk_view_t = tt::chunk_view<ALIGNMENT>;

		// The number of units of data stored in the chunk's SBO (Small Buffer Optimization) data buffer.
		// The SBO of a chunk type is the smallest size required to store 16 bytes of data, such that this size adheres to the chunk's alignment.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		static constexpr tt_size SBO_SIZE = tt::aligned_count<tt_size>(16, ALIGNMENT);

		using allocator_traits_t = std::allocator_traits<allocator_t>;


	private:

		allocator_t _al;

		chunk_unit_t* _data;

		chunk_unit_t _sbo_data[SBO_SIZE];

		// size/capacity is a measure of chunk units

		tt_size _size, _capacity;

		// helpers for chunk allocator

		inline tt_size _al_max_size() const noexcept {


			return allocator_traits_t::max_size(_al);
		}

		static constexpr tt_bool _al_is_always_equal = std::is_base_of_v<std::true_type, typename allocator_traits_t::is_always_equal>;

		static constexpr tt_bool _al_propagate_on_container_copy_assignment = std::is_base_of_v<std::true_type, typename allocator_traits_t::propagate_on_container_copy_assignment>;
		static constexpr tt_bool _al_propagate_on_container_move_assignment = std::is_base_of_v<std::true_type, typename allocator_traits_t::propagate_on_container_move_assignment>;
		static constexpr tt_bool _al_propagate_on_container_swap = std::is_base_of_v<std::true_type, typename allocator_traits_t::propagate_on_container_swap>;

		inline allocator_t _al_select_on_container_copy_construction() {


			return allocator_traits_t::select_on_container_copy_construction(_al);
		}

		inline chunk_unit_t* const _al_alloc(tt_size units) {


			return allocator_traits_t::allocate(_al, units_to_bytes(units));
		}

		inline void _al_free(chunk_unit_t* const x, tt_size units) {


			allocator_traits_t::deallocate(_al, x, units_to_bytes(units));
		}

		// general helpers

		inline void _init() noexcept {


			_data = _sbo_data;

			_size = 0;
			_capacity = SBO_SIZE;
		}

		inline void _throw_if_capacity_change_exceeds_max_size(tt_size n) const {


			if (n > _al_max_size())
				TT_THROW(tt::max_size_error, "tt::chunk capacity change exceeds allocator max size!");
		}

		// provides strong guarantee

		inline void _change_capacity(tt_size n, tt_bool preserve_data) {


			_throw_if_capacity_change_exceeds_max_size(n);

			auto _old_capacity = capacity();

			_capacity = n;

			if (_old_capacity == n)
				return;

			else if (_old_capacity <= SBO_SIZE) {


				// if we're using SBO and we're changing to SBO contained size

				// in this case, our capacity may not be lowered below that
				// of the SBO, thus we return immediately

				// notice that due to the lack of ANY change, this scenario
				// can also savely forgo the bottom-most bit of code relating
				// to correcting the chunk size if the capacity goes below
				// it, it won't here

				if (n <= SBO_SIZE)
					return;

				// if we're using SBO and we're changing to EXCEED SBO size

				else {


					auto _new_data = _al_alloc(n); // <- nothing to worry about if this throws, thus strong guarantee

					// nothing beyond here can throw, thus strong guarantee

					if (preserve_data)
						tt::copy_block(_sbo_data, _new_data, _old_capacity);

					_data = _new_data;
				}
			}

			else {


				// if we're not SBO but we're changing to SBO size

				if (n <= SBO_SIZE) {


					if (preserve_data)
						tt::copy_block(data(), _sbo_data, n);

					_al_free(_data, _old_capacity);

					_data = _sbo_data;
				}

				// if we're not SBO and we're changing to still not be in SBO size range

				else {


					auto _new_data = _al_alloc(n); // <- nothing to worry about if this throws, thus strong guarantee

					// nothing beyond here can throw, thus strong guarantee

					if (preserve_data)
						tt::copy_block(_data, _new_data, _old_capacity);

					_al_free(_data, _old_capacity);

					_data = _new_data;
				}
			}

			// correct size if capacity change made it exceed new capacity

			if (size() > capacity())
				_size = capacity();
		}

		inline tt_size _calc_geometric_growth(tt_size minimum_units) const noexcept {


			tt_assert(minimum_units > 0);

			auto ms = this->max_size();

			tt_assert(minimum_units <= ms);

			tt_size r = capacity() + capacity() / 2;

			if (r < minimum_units)
				r = minimum_units;

			if (r > ms)
				r = ms;

			return r;
		}

		// provides strong guarantee

		inline void _grow_to_contain(tt_size minimum_capacity, tt_bool preserve_data) {


			if (capacity() < minimum_capacity)
				_throw_if_capacity_change_exceeds_max_size(minimum_capacity),
				_change_capacity(_calc_geometric_growth(minimum_capacity), preserve_data);
		}

		// provides strong guarantee

		inline void _reserve(tt_size minimum_capacity, tt_bool preserve_data) {


			if (capacity() < minimum_capacity)
				_change_capacity(minimum_capacity, preserve_data);
		}

		// provides strong guarantee

		inline void _change_size(tt_size new_size, tt_bool preserve_data) {


			_grow_to_contain(new_size, preserve_data);

			_size = new_size;
		}

		inline void _dispose() noexcept {


			// discard any allocated memory, and reset back to SBO

			if (_data != _sbo_data)
				_al_free(_data, capacity());

			_init();
		}


	public:

		// Default initializes a chunk.
		inline chunk() noexcept {


			_al = {};

			_init();
		}

		// Default initializes a chunk, using an explicit allocator.
		inline explicit chunk(allocator_t al) noexcept {


			_al = std::move(al);

			_init();
		}

		// Initializes a chunk of a block of n units worth of memory, with the values of this chunk of memory being arbitrary.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline chunk(tt_size n, allocator_t al = {}) {


			_al = std::move(al);

			_init();

			_change_size(n, false);
		}

		// Initializes a chunk of a block of n units worth of memory, copying n units worth of bytes from x to populate this memory.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		template<typename T>
		inline chunk(const T* const x, tt_size n, allocator_t al = {}) {


			_al = std::move(al);

			_init();

			_change_size(n, false);

			tt::copy_block((const chunk_unit_t* const)x, data(), n);
		}

		// Initializes a chunk of a block cloned from the contents of chunk view x.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline chunk(chunk_view_t x, allocator_t al = {}) {


			_al = std::move(al);

			_init();

			_change_size(x.size(), false);

			tt::copy_block(x.data(), data(), x.size());
		}

		// Copy-initializes a chunk.
		// Provides a strong guarantee if an exception arises.
		inline chunk(const this_t& x) {


			_al = _al_select_on_container_copy_construction();

			_init();

			_change_size(x.size(), false);

			tt::copy_block(x.data(), data(), size());
		}

		// Move-initializes a chunk.
		inline chunk(this_t&& x) noexcept {


			_al = std::move(x._al);

			_init();

			_size = x.size();

			if (x.data() == x._sbo_data) {


				// setup our SBO buffer for our data and copy x's data

				tt::copy_block(x.data(), data(), x.size());
			}

			else {


				_capacity = x.capacity();

				TT_MOVEPTR(_data, x);
			}

			x._init(); // <- reset x's state, having stolen its buffer, if it had one
		}

		inline ~chunk() noexcept {


			_dispose();
		}

		// Assigns the chunk to be a chunk of size n units of uninitialized memory.
		// The values of any uninitialized memory added to the chunk will be arbitrary.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline this_t& assign(tt_size n) {


			resize(n);

			TT_RETURN_THIS;
		}

		// Assigns the chunk to be a chunk of size n units of uninitialized memory, copying n units worth of bytes from x to populate this memory.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		template<typename T>
		inline this_t& assign(const T* const x, tt_size n) {


			resize(n);

			tt::copy_block((const chunk_unit_t* const)x, data(), n);

			TT_RETURN_THIS;
		}

		// Assigns the chunk to be a clone of the contents of chunk view x.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline this_t& assign(chunk_view_t x) {


			return assign(x.data(), x.size());
		}

		// Assigns the chunk.
		// Provides a strong guarantee if an exception arises.
		inline this_t& assign(const this_t& rhs) {


			TT_SELF_COPY_TEST(rhs);

			clear();

			if constexpr (_al_propagate_on_container_copy_assignment) {


				// dump our capacity if we propagate allocators and our new one can't deallocate our current capacity block

				if (get_allocator() != rhs.get_allocator())
					reset();

				TT_COPY(_al, rhs);
			}

			resize(rhs.size());

			tt::copy_block(rhs.data(), data(), size());

			TT_RETURN_THIS;
		}

		// Assigns the chunk.
		// Due to the rules of allocator propagation move-assign may (though vary rarely) require an interim allocation, which can throw, violating noexcept.
		// Provides a strong guarantee if an exception arises.
		inline this_t& assign(this_t&& rhs) noexcept {


			TT_SELF_MOVE_TEST(rhs);

			reset();

			if constexpr (_al_propagate_on_container_move_assignment) {


				TT_MOVE(_al, rhs);

				if (rhs.data() == rhs._sbo_data)
					tt_assert(rhs.size() <= SBO_SIZE),
					_size = rhs.size(),
					tt::copy_block(rhs.data(), data(), size());
				else
					TT_MOVEPTR(_data, rhs),
					TT_MOVE(_size, rhs),
					TT_MOVE(_capacity, rhs);
				
				rhs._init();
			}

			else {


				if (get_allocator() == rhs.get_allocator()) {


					// if the allocators are equal, then our allocator can deallocate rhs's buffer, so we can steal it

					if (rhs.data() == rhs._sbo_data)
						tt_assert(rhs.size() <= SBO_SIZE),
						_size = rhs.size(),
						tt::copy_block(rhs.data(), data(), size());
					else
						TT_MOVEPTR(_data, rhs),
						TT_MOVE(_size, rhs),
						TT_MOVE(_capacity, rhs);

					rhs._init();
				}

				else {


					_change_size(rhs.size(), false); // <- the dreaded heap allocation, allowing our noexcept function to throw

					tt::copy_block(rhs.data(), data(), rhs.size());

					rhs.clear(); // <- simulate stealing rhs's block, but don't bother actually deallocating
				}
			}

			TT_RETURN_THIS;
		}

		inline this_t& operator=(const this_t& rhs) noexcept { return assign(rhs); }
		inline this_t& operator=(this_t&& rhs) noexcept { return assign(TT_FMOVE(this_t, rhs)); }

		// Returns the allocator of the chunk.
		constexpr allocator_t& get_allocator() noexcept { return _al; }

		// Returns the allocator of the chunk.
		constexpr const allocator_t& get_allocator() const noexcept { return _al; }

		// Returns the data of the chunk.
		constexpr chunk_unit_t* const data() noexcept { return _data; }

		// Returns the data of the chunk.
		constexpr const chunk_unit_t* const data() const noexcept { return _data; }

		// Returns the number of bytes in a chunk unit.
		// This value is equal to the chunk's Alignment.
		constexpr tt_size unit_bytes() const noexcept { return ALIGNMENT; }

		// Returns the given unit index/size value converted to its byte index/size equivalent, in regards to this chunk's type.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		constexpr tt_size units_to_bytes(tt_size units) const noexcept { return units * unit_bytes(); }

		// Returns the size of the chunk.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		constexpr tt_size size() const noexcept { return _size; }

		// Returns the size of the chunk, in bytes.
		constexpr tt_size size_bytes() const noexcept { return units_to_bytes(size()); }

		// Returns if the chunk is non-empty.
		constexpr tt_bool has_size() const noexcept { return size() > 0; }

		// Returns if the chunk is empty.
		constexpr tt_bool empty() const noexcept { return size() == 0; }

		// Returns the units of capacity of the chunk.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		constexpr tt_size capacity() const noexcept { return _capacity; }

		// Returns the capacity of the chunk, in bytes.
		constexpr tt_size capacity_bytes() const noexcept { return units_to_bytes(capacity()); }

		// Returns the maximum size the chunk may have, as a function of its allocator.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		inline tt_size max_size() const noexcept { return _al_max_size(); }

		// Returns the maximum size, in bytes, the chunk may have, as a function of its allocator.
		inline tt_size max_size_bytes() const noexcept { return units_to_bytes(max_size()); }

		// Returns if the given unit index ind is in-bounds of the chunk.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		constexpr tt_bool in_bounds(tt_size ind) const noexcept { return ind < size(); }

		// Returns if the given byte (not unit) index byte_ind is in-bounds of the chunk.
		constexpr tt_bool in_bounds_byte(tt_size byte_ind) const noexcept { return byte_ind < size_bytes(); }

		// Returns if an object of type T, at the given unit index ind, is in-bounds, and fits completely within, the chunk.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		template<typename T>
		constexpr tt_bool in_bounds_and_fits(tt_size ind) const noexcept {


			return in_bounds(ind) && (size_bytes() - units_to_bytes(ind)) >= tt::aligned_size_of<tt_size, T>(unit_bytes());
		}

		// Returns a pointer to an individual byte of the chunk at index byte_ind.
		// This index byte_ind is an index into the underlying byte array of the chunk.
		// This does not perform bounds checking.
		inline tt_byte* const get_byte_unchecked(tt_size byte_ind) noexcept {


			return ((tt_byte*)data()) + byte_ind;
		}

		// Returns a pointer to an individual byte of the chunk at index byte_ind.
		// This index byte_ind is an index into the underlying byte array of the chunk.
		// This does not perform bounds checking.
		inline const tt_byte* const get_byte_unchecked(tt_size byte_ind) const noexcept {


			return ((const tt_byte*)data()) + byte_ind;
		}

		// Returns a pointer to an individual byte of the chunk at index byte_ind.
		// This index byte_ind is an index into the underlying byte array of the chunk.
		// Throws tt::out_of_range_error if ind is out-of-range.
		inline tt_byte* const get_byte(tt_size byte_ind) {


			if (!in_bounds_byte(byte_ind))
				TT_THROW(tt::out_of_range_error, "tt::chunk get_byte index byte_ind is out-of-range!");

			return get_byte_unchecked(byte_ind);
		}

		// Returns a pointer to an individual byte of the chunk at index byte_ind.
		// This index byte_ind is an index into the underlying byte array of the chunk.
		// Throws tt::out_of_range_error if ind is out-of-range.
		inline const tt_byte* const get_byte(tt_size byte_ind) const {


			if (!in_bounds_byte(byte_ind))
				TT_THROW(tt::out_of_range_error, "tt::chunk get_byte index byte_ind is out-of-range!");

			return get_byte_unchecked(byte_ind);
		}

		// Returns a pointer to the memory at unit index ind as type T.
		// This does not perform bounds checking.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		template<typename T>
		inline T* const get_unchecked(tt_size ind) noexcept {


			return (T* const)(data() + ind);
		}

		// Returns a pointer to the memory at unit index ind as type T.
		// This does not perform bounds checking.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		template<typename T>
		inline const T* const get_unchecked(tt_size ind) const noexcept {


			return (const T* const)(data() + ind);
		}

		// Returns a pointer to the memory at unit index ind as type T.
		// Throws tt::out_of_range_error if ind is out-of-range, or if an object of type T at ind would exceed the chunk's bounds.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		template<typename T>
		inline T* const get(tt_size ind) {


			if (!in_bounds_and_fits<T>(ind))
				TT_THROW(tt::out_of_range_error, "tt::chunk get index ind is out-of-range!");

			return get_unchecked<T>(ind);
		}

		// Returns a pointer to the memory at unit index ind as type T.
		// Throws tt::out_of_range_error if ind is out-of-range, or if an object of type T at ind would exceed the chunk's bounds.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		template<typename T>
		inline const T* const get(tt_size ind) const {


			if (!in_bounds_and_fits<T>(ind))
				TT_THROW(tt::out_of_range_error, "tt::chunk get index ind is out-of-range!");

			return get_unchecked<T>(ind);
		}

		// Returns a chunk view of the given subsection of the chunk.
		// The portion viewed starts at unit index ind in the chunk and continues for the first n units.
		// If n is zero, the viewed portion will continue to the end of the chunk.
		// If ind is out-of-range, or n makes it extend out-of-range, the subsection viewed will be reduced to fit, or reduced to zero.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		inline chunk_view_t view(tt_size ind = 0, tt_size n = 0) const noexcept {


			if (n == 0)
				n = size(); // <- this'll be reduced to fit below

			if (!in_bounds(ind))
				n = 0;

			else if (size() - ind < n)
				n = size() - ind;

			if (n == 0) // <- post 'n == 0' rule above
				return {};

			return chunk_view_t(get_byte_unchecked(0) + units_to_bytes(ind), n);
		}

		// Returns a subchunk of the given subsection of the chunk.
		// The portion extracted starts at unit index ind in the chunk and continues for the first n units.
		// If n is zero, the extracted portion will continue to the end of the chunk.
		// If ind is out-of-range, or n makes it extend out-of-range, the subsection viewed will be reduced to fit, or reduced to zero.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		inline this_t subchunk(tt_size ind = 0, tt_size n = 0) const {


			if (n == 0)
				n = size(); // <- this'll be reduced to fit below

			if (!in_bounds(ind))
				n = 0;

			else if (size() - ind < n)
				n = size() - ind;

			if (n == 0) // <- post 'n == 0' rule above
				return {};

			auto r = this_t(n);

			tt::copy_block(data() + ind, r.data(), n);

			return r;
		}

		// Returns a string of the contents of the chunk.
		// If byte_order doesn't match up with the user's machine's native endianness, then the bytes of each character of the returned string will have their byte order flipped.
		// The string's null terminator is not included in the section of memory used to build the string.
		// The length of this string is equal to the number of sizeof(Char) blocks which can be fully filled starting from the beginning of the chunk.
		// This function operates upon the entire chunk, and thus view should be used to extract the specific portion desired.
		template<typename Char>
		inline std::basic_string<Char> extract_str(tt::endian byte_order = tt::endian::NATIVE) const {


			return view().extract_str<Char>(byte_order);
		}

		// Returns a string visualization of the chunk.
		// The portion visualized starts at unit index ind in the chunk and continues for the first n units.
		// If n is zero, the visualization will continue to the end of the chunk.
		// If ind is out-of-range, or n makes it extend out-of-range, the block visualized will be reduced to fit, or reduced to zero.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		inline tt_string vis(tt_size ind = 0, tt_size n = 0) const {


			if (n == 0)
				n = size(); // <- this'll be reduced to fit below

			if (!in_bounds(ind))
				n = 0;

			else if (size() - ind < n)
				n = size() - ind;

			tt_size nn = units_to_bytes(n);

			tt_string r{};

			r.reserve(nn * 2U);

			TT_FOR(i, nn)
				r += tt::vis_byte_hex(*get_byte_unchecked(i), true);

			return tt::fprint_s("{ index=", ind, ", bytes=", size_bytes(), ", unit bytes=", unit_bytes(), ", ", r, " }");
		}

		// Returns a hash of the chunk.
		inline tt_size hash() const noexcept {


			return tt::array_hash_of(get_byte_unchecked(0), size_bytes());
		}

		// Resizes the chunk, changing its size to n units, adding or removing uninitialized memory to or from the end of the chunk as needed.
		// The values of any uninitialized memory added to the chunk will be arbitrary.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline this_t& resize(tt_size n) {


			_change_size(n, true);

			TT_RETURN_THIS;
		}

		// Ensures the chunk has exactly n units of capacity allocated, reallocating if needed to meet this value exactly.
		// If n is less than the current size of the chunk, the chunk's size will be reduced to fit.
		// If reallocating would require going below the size of the chunk's SBO (Small Buffer Optimization) size (see SBO_SIZE) then the capacity of the chunk will be set to the SBO's size.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline this_t& change_capacity(tt_size n) {


			_change_capacity(n, true);

			TT_RETURN_THIS;
		}

		// Ensures the chunk has at least n units of capacity allocated, reallocating if needed to satisfy this requirement.
		// If reallocation occurs, the chunk will allocate exactly n units of capacity.
		// If reallocating would require going below the size of the chunk's SBO (Small Buffer Optimization) size (see SBO_SIZE) then the capacity of the chunk will be set to the SBO's size.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline this_t& reserve(tt_size n) {


			_reserve(n, true);

			TT_RETURN_THIS;
		}

		// Ensures the chunk has at least n units of capacity allocated, reallocating if needed to satisfy this requirement.
		// If reallocation occurs, the chunk will allocate according to the container's geometric growth model (aka. new-capacity = capacity * 1.5) or n exactly if geometric growth would not be enough.
		// If reallocating would require going below the size of the chunk's SBO (Small Buffer Optimization) size (see SBO_SIZE) then the capacity of the chunk will be set to the SBO's size.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline this_t& grow_to_contain(tt_size n) {


			_grow_to_contain(n, true);

			TT_RETURN_THIS;
		}

		// Ensures the chunk contains no more units of capacity than are required in order to maintain its current size, reallocating if needed to meet its size exactly.
		// If reallocating would require going below the size of the chunk's SBO (Small Buffer Optimization) size (see SBO_SIZE) then the capacity of the chunk will be set to the SBO's size.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		inline this_t& shrink_to_fit() {


			tt_assert(capacity() >= size());

			_change_capacity(size(), true);

			TT_RETURN_THIS;
		}

		// Grows the size of the chunk by n units, adding uninitialized memory onto the end of it.
		// The values of any uninitialized memory added to the chunk will be arbitrary.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline this_t& grow_by(tt_size n) {


			return resize(size() + n);
		}

		// Shrinks the size of the chunk by n units, removing uninitialized memory from the end of it.
		// If the size of the chunk is less than or exactly n units, the chunk will be cleared.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		inline this_t& shrink_by(tt_size n) {


			if (size() <= n)
				return clear();
			else
				return resize(size() - n);
		}

		// Injects an n unit block of uninitialized memory such that its starting index in the chunk is ind, moving all memory at or beyond that point forward n units.
		// If ind is equal to or greater than the size of the chunk, the chunk will instead by grown by n units.
		// If n is zero, the function fails quietly.
		// The values of any uninitialized memory added to the chunk will be arbitrary.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline this_t& inject(tt_size ind, tt_size n) {


			if (n == 0)
				TT_RETURN_THIS;

			auto cond = ind >= size();

			grow_by(n);

			if (cond)
				TT_RETURN_THIS;

			//if (n >= size())
				//TT_RETURN_THIS;

			tt::copy_block_overlap(data() + ind, data() + ind + n, size() - (ind + n));

			TT_RETURN_THIS;
		}

		// Injects an n unit block of uninitialized memory such that its starting index in the chunk is ind, moving all memory at or beyond that point forward n units, and n units worth of bytes will be copied from x into the gap created.
		// If ind is equal to or greater than the size of the chunk, the chunk will instead by grown by n units, then copied into.
		// If n is zero, the function fails quietly.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		template<typename T>
		inline this_t& inject(tt_size ind, const T* const x, tt_size n) {


			inject(ind, n);

			tt::copy_block((const chunk_unit_t* const)x, data() + ind, n);

			TT_RETURN_THIS;
		}

		// Injects a block of uninitialized memory big enough to contain chunk view x such that its starting index in the chunk is ind, moving all memory at or beyond that point forward, and copying the contents of chunk view x into it.
		// If ind is equal to or greater than the size of the chunk, the chunk will instead by grown, then copied into.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline this_t& inject(tt_size ind, chunk_view_t x) {


			return inject(ind, x.data(), x.size());
		}

		// Injects a block of uninitialized memory big enough to contain chunk x such that its starting index in the chunk is ind, moving all memory at or beyond that point forward, and copying the contents of chunk x into it.
		// If ind is equal to or greater than the size of the chunk, the chunk will instead by grown, then copied into.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		// Provides a strong guarantee if an exception arises.
		// Throws tt::max_size_error if (re)allocating would require more units of space than are allowed by the chunk's allocator.
		inline this_t& inject(tt_size ind, const this_t& x) {


			return inject(ind, x.view());
		}

		inline this_t& operator+=(chunk_view_t rhs) { return inject(size(), rhs); }
		inline this_t& operator+=(const this_t& rhs) { return inject(size(), rhs); }

		inline this_t operator+(chunk_view_t rhs) const { auto r = *this; r += rhs; return r; }
		inline this_t operator+(const this_t& rhs) const { auto r = *this; r += rhs; return r; }

		// Fills the given subsection of the chunk's memory with the given byte value.
		// The portion filled starts at unit index ind in the chunk and continues for the first n units.
		// If n is zero, the filled portion will continue to the end of the chunk.
		// If ind is out-of-range, or n makes it extend out-of-range, the subsection filled will be reduced to fit, or reduced to zero.
		// Unless otherwise specified, indices and sizes in chunks are measured in alignment-sized 'units' of bytes.
		inline this_t& fill(tt_byte x, tt_size ind = 0, tt_size n = 0) noexcept {


			if (n == 0)
				n = size(); // <- this'll be reduced to fit below

			if (!in_bounds(ind))
				n = 0;

			else if (size() - ind < n)
				n = size() - ind;

			if (n == 0) // <- post 'n == 0' rule above
				TT_RETURN_THIS;

			std::memset((void*)(data() + ind), (int)x, units_to_bytes(n));

			TT_RETURN_THIS;
		}

		// Clears the chunk of its contents, maintaining its underlying capacity.
		inline this_t& clear() noexcept {


			_change_size(0, false);

			TT_RETURN_THIS;
		}

		// Resets the chunk of its contents, dumping any allocated capacity it carries.
		inline this_t& reset() noexcept {

			
			_dispose();

			TT_RETURN_THIS;
		}
	};
}

#define _TPARAMS0 <tt_size Alignment>
#define _TPARAMS1 <tt_size Alignment, typename Allocator>

#define _TARGS0 <Alignment>
#define _TARGS1 <Alignment, Allocator>

TT_VISUALIZERS(0) {


	TT_REGISTER_VISUALIZE_T(tt::chunk_view, _TPARAMS0, _TARGS0);
	TT_REGISTER_VISUALIZE_T(tt::chunk, _TPARAMS1, _TARGS1);
}

TT_HASHERS(0) {


	TT_REGISTER_HASH_T(tt::chunk_view, _TPARAMS0, _TARGS0);
	TT_REGISTER_HASH_T(tt::chunk, _TPARAMS1, _TARGS1);
}

#undef _TPARAMS0
#undef _TPARAMS1

#undef _TARGS0
#undef _TARGS1

