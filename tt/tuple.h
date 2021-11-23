

#pragma once


// The Tirous Toolbox tuple implementation.


#include "aliases.h"
#include "macros.h"
#include "debug.h"

#include "forward_declarations.h"

#include "visualize.h"

#include "hash.h"
#include "hash_factory.h"


namespace tt {


#define HASH_0 113190263, 2907030853, (tt_size)5475681401113217209, (tt_size)14675788808751262303
#define HASH_1 3740273923, 4208862061, (tt_size)9177768853909198597, (tt_size)1309847167325433301
#define HASH_2 269017513, 652733729, (tt_size)13764402442208241959, (tt_size)18324144342180106883
#define HASH_3 805334473, 287226371, (tt_size)3467070816987863467, (tt_size)2192987852956786601
#define HASH_4 1202150093, 2106865963, (tt_size)10839095753339024287, (tt_size)113972877175268753
#define HASH_5 3137527051, 385084111, (tt_size)10583190091343792743, (tt_size)11879059580920384459
#define HASH_6 2603537359, 3187682909, (tt_size)3760655961987104419, (tt_size)9310968019156321079
#define HASH_7 110499709, 2192861053, (tt_size)11525568597581361047, (tt_size)16361807409629797103

	// TODO: In the future we'll likely benefit from making our tuples more easily able to interact with std::tuple.

	// In order to reduce the complexity of the below classes, take note that we're NOT going to
	// define explicit constructors, destructors, or assignment operators, so as to ensure that
	// we can use aggregate initialization, and take advantage of the defaults generated for us.

	// The Tirous Toolbox tuple implementation.
	template<typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H>
	struct tuple_struct final {

		using a_type = typename A;
		using b_type = typename B;
		using c_type = typename C;
		using d_type = typename D;
		using e_type = typename E;
		using f_type = typename F;
		using g_type = typename G;
		using h_type = typename H;

		using this_t = tt::tuple_struct<a_type, b_type, c_type, d_type, e_type, f_type, g_type, h_type>;

		// The first value of the tuple.
		a_type a = {};

		// The second value of the tuple.
		b_type b = {};

		// The third value of the tuple.
		c_type c = {};

		// The fourth value of the tuple.
		d_type d = {};

		// The fifth value of the tuple.
		e_type e = {};

		// The sixth value of the tuple.
		f_type f = {};

		// The seventh value of the tuple.
		g_type g = {};

		// The eighth value of the tuple.
		h_type h = {};

		// Returns if this and the given tuple are equal.
		inline tt_bool equal(const this_t& x) const noexcept {


			return a == x.a && b == x.b && c == x.c && d == x.d && e == x.e && f == x.f && g == x.g && h == x.h;
		}

		inline tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		inline tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// lexicographical comparison function

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			if (a < rhs.a)
				return true;

			else if (rhs.a < a)
				return false;

			else if (b < rhs.b)
				return true;

			else if (rhs.b < b)
				return false;

			else if (c < rhs.c)
				return true;

			else if (rhs.c < c)
				return false;

			else if (d < rhs.d)
				return true;

			else if (rhs.d < d)
				return false;

			else if (e < rhs.e)
				return true;

			else if (rhs.e < e)
				return false;

			else if (f < rhs.f)
				return true;

			else if (rhs.f < f)
				return false;

			else if (g < rhs.g)
				return true;

			else if (rhs.g < g)
				return false;

			else if (h < rhs.h)
				return true;

			else if (rhs.h < h)
				return false;

			else return false;
		}

		// Returns a string visualization of the tuple.
		inline tt_string visualize() const {


			tt_string r = "{ ";

			r += tt::visualize(a) + ", ";
			r += tt::visualize(b) + ", ";
			r += tt::visualize(c) + ", ";
			r += tt::visualize(d) + ", ";
			r += tt::visualize(e) + ", ";
			r += tt::visualize(f) + ", ";
			r += tt::visualize(g) + ", ";
			r += tt::visualize(h) + " }";

			return r;
		}

		// Returns a hash of the tuple.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(HASH_0);

			h.add(a);
			h.add(b);
			h.add(c);
			h.add(d);
			h.add(e);
			h.add(f);
			h.add(g);
			h.add(h);

			return h.get();
		}

		template<typename A0, typename B0, typename C0, typename D0, typename E0, typename F0, typename G0, typename H0>
		constexpr operator tt::tuple_struct<A0, B0, C0, D0, E0, F0, G0, H0>() const {


			return { (A0)a, (B0)b, (C0)c, (D0)d, (E0)e, (F0)f, (G0)g, (G0)h };
		}
	};

	// The Tirous Toolbox tuple implementation.
	template<typename A, typename B, typename C, typename D, typename E, typename F, typename G>
	struct tuple_struct<A, B, C, D, E, F, G> final {

		using a_type = typename A;
		using b_type = typename B;
		using c_type = typename C;
		using d_type = typename D;
		using e_type = typename E;
		using f_type = typename F;
		using g_type = typename G;

		using this_t = tt::tuple_struct<a_type, b_type, c_type, d_type, e_type, f_type, g_type>;

		// The first value of the tuple.
		a_type a = {};

		// The second value of the tuple.
		b_type b = {};

		// The third value of the tuple.
		c_type c = {};

		// The fourth value of the tuple.
		d_type d = {};

		// The fifth value of the tuple.
		e_type e = {};

		// The sixth value of the tuple.
		f_type f = {};

		// The seventh value of the tuple.
		g_type g = {};

		// Returns if this and the given tuple are equal.
		inline tt_bool equal(const this_t& x) const noexcept {


			return a == x.a && b == x.b && c == x.c && d == x.d && e == x.e && f == x.f && g == x.g;
		}

		inline tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		inline tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// lexicographical comparison function

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			if (a < rhs.a)
				return true;

			else if (rhs.a < a)
				return false;

			else if (b < rhs.b)
				return true;

			else if (rhs.b < b)
				return false;

			else if (c < rhs.c)
				return true;

			else if (rhs.c < c)
				return false;

			else if (d < rhs.d)
				return true;

			else if (rhs.d < d)
				return false;

			else if (e < rhs.e)
				return true;

			else if (rhs.e < e)
				return false;

			else if (f < rhs.f)
				return true;

			else if (rhs.f < f)
				return false;

			else if (g < rhs.g)
				return true;

			else if (rhs.g < g)
				return false;

			else return false;
		}

		// Returns a string visualization of the tuple.
		inline tt_string visualize() const {


			tt_string r = "{ ";

			r += tt::visualize(a) + ", ";
			r += tt::visualize(b) + ", ";
			r += tt::visualize(c) + ", ";
			r += tt::visualize(d) + ", ";
			r += tt::visualize(e) + ", ";
			r += tt::visualize(f) + ", ";
			r += tt::visualize(g) + " }";

			return r;
		}

		// Returns a hash of the tuple.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(HASH_1);

			h.add(a);
			h.add(b);
			h.add(c);
			h.add(d);
			h.add(e);
			h.add(f);
			h.add(g);

			return h.get();
		}

		template<typename A0, typename B0, typename C0, typename D0, typename E0, typename F0, typename G0>
		constexpr operator tt::tuple_struct<A0, B0, C0, D0, E0, F0, G0>() const {


			return { (A0)a, (B0)b, (C0)c, (D0)d, (E0)e, (F0)f, (G0)g };
		}
	};

	// The Tirous Toolbox tuple implementation.
	template<typename A, typename B, typename C, typename D, typename E, typename F>
	struct tuple_struct<A, B, C, D, E, F> final {

		using a_type = typename A;
		using b_type = typename B;
		using c_type = typename C;
		using d_type = typename D;
		using e_type = typename E;
		using f_type = typename F;

		using this_t = tt::tuple_struct<a_type, b_type, c_type, d_type, e_type, f_type>;

		// The first value of the tuple.
		a_type a = {};

		// The second value of the tuple.
		b_type b = {};

		// The third value of the tuple.
		c_type c = {};

		// The fourth value of the tuple.
		d_type d = {};

		// The fifth value of the tuple.
		e_type e = {};

		// The sixth value of the tuple.
		f_type f = {};

		// Returns if this and the given tuple are equal.
		inline tt_bool equal(const this_t& x) const noexcept {


			return a == x.a && b == x.b && c == x.c && d == x.d && e == x.e && f == x.f;
		}

		inline tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		inline tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// lexicographical comparison function

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			if (a < rhs.a)
				return true;

			else if (rhs.a < a)
				return false;

			else if (b < rhs.b)
				return true;

			else if (rhs.b < b)
				return false;

			else if (c < rhs.c)
				return true;

			else if (rhs.c < c)
				return false;

			else if (d < rhs.d)
				return true;

			else if (rhs.d < d)
				return false;

			else if (e < rhs.e)
				return true;

			else if (rhs.e < e)
				return false;

			else if (f < rhs.f)
				return true;

			else if (rhs.f < f)
				return false;

			else return false;
		}

		// Returns a string visualization of the tuple.
		inline tt_string visualize() const {


			tt_string r = "{ ";

			r += tt::visualize(a) + ", ";
			r += tt::visualize(b) + ", ";
			r += tt::visualize(c) + ", ";
			r += tt::visualize(d) + ", ";
			r += tt::visualize(e) + ", ";
			r += tt::visualize(f) + " }";

			return r;
		}

		// Returns a hash of the tuple.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(HASH_2);

			h.add(a);
			h.add(b);
			h.add(c);
			h.add(d);
			h.add(e);
			h.add(f);

			return h.get();
		}

		template<typename A0, typename B0, typename C0, typename D0, typename E0, typename F0>
		constexpr operator tt::tuple_struct<A0, B0, C0, D0, E0, F0>() const {


			return { (A0)a, (B0)b, (C0)c, (D0)d, (E0)e, (F0)f };
		}
	};

	// The Tirous Toolbox tuple implementation.
	template<typename A, typename B, typename C, typename D, typename E>
	struct tuple_struct<A, B, C, D, E> final {

		using a_type = typename A;
		using b_type = typename B;
		using c_type = typename C;
		using d_type = typename D;
		using e_type = typename E;

		using this_t = tt::tuple_struct<a_type, b_type, c_type, d_type, e_type>;

		// The first value of the tuple.
		a_type a = {};

		// The second value of the tuple.
		b_type b = {};

		// The third value of the tuple.
		c_type c = {};

		// The fourth value of the tuple.
		d_type d = {};

		// The fifth value of the tuple.
		e_type e = {};

		// Returns if this and the given tuple are equal.
		inline tt_bool equal(const this_t& x) const noexcept {


			return a == x.a && b == x.b && c == x.c && d == x.d && e == x.e;
		}

		inline tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		inline tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// lexicographical comparison function

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			if (a < rhs.a)
				return true;

			else if (rhs.a < a)
				return false;

			else if (b < rhs.b)
				return true;

			else if (rhs.b < b)
				return false;

			else if (c < rhs.c)
				return true;

			else if (rhs.c < c)
				return false;

			else if (d < rhs.d)
				return true;

			else if (rhs.d < d)
				return false;

			else if (e < rhs.e)
				return true;

			else if (rhs.e < e)
				return false;

			else return false;
		}

		// Returns a string visualization of the tuple.
		inline tt_string visualize() const {


			tt_string r = "{ ";

			r += tt::visualize(a) + ", ";
			r += tt::visualize(b) + ", ";
			r += tt::visualize(c) + ", ";
			r += tt::visualize(d) + ", ";
			r += tt::visualize(e) + " }";

			return r;
		}

		// Returns a hash of the tuple.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(HASH_3);

			h.add(a);
			h.add(b);
			h.add(c);
			h.add(d);
			h.add(e);

			return h.get();
		}

		template<typename A0, typename B0, typename C0, typename D0, typename E0>
		constexpr operator tt::tuple_struct<A0, B0, C0, D0, E0>() const {


			return { (A0)a, (B0)b, (C0)c, (D0)d, (E0)e };
		}
	};

	// The Tirous Toolbox tuple implementation.
	template<typename A, typename B, typename C, typename D>
	struct tuple_struct<A, B, C, D> final {

		using a_type = typename A;
		using b_type = typename B;
		using c_type = typename C;
		using d_type = typename D;

		using this_t = tt::tuple_struct<a_type, b_type, c_type, d_type>;

		// The first value of the tuple.
		a_type a = {};

		// The second value of the tuple.
		b_type b = {};

		// The third value of the tuple.
		c_type c = {};

		// The fourth value of the tuple.
		d_type d = {};

		// Returns if this and the given tuple are equal.
		inline tt_bool equal(const this_t& x) const noexcept {


			return a == x.a && b == x.b && c == x.c && d == x.d;
		}

		inline tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		inline tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// lexicographical comparison function

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			if (a < rhs.a)
				return true;

			else if (rhs.a < a)
				return false;

			else if (b < rhs.b)
				return true;

			else if (rhs.b < b)
				return false;

			else if (c < rhs.c)
				return true;

			else if (rhs.c < c)
				return false;

			else if (d < rhs.d)
				return true;

			else if (rhs.d < d)
				return false;

			else return false;
		}

		// Returns a string visualization of the tuple.
		inline tt_string visualize() const {


			tt_string r = "{ ";

			r += tt::visualize(a) + ", ";
			r += tt::visualize(b) + ", ";
			r += tt::visualize(c) + ", ";
			r += tt::visualize(d) + " }";

			return r;
		}

		// Returns a hash of the tuple.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(HASH_4);

			h.add(a);
			h.add(b);
			h.add(c);
			h.add(d);

			return h.get();
		}

		template<typename A0, typename B0, typename C0, typename D0>
		constexpr operator tt::tuple_struct<A0, B0, C0, D0>() const {


			return { (A0)a, (B0)b, (C0)c, (D0)d };
		}
	};

	// The Tirous Toolbox tuple implementation.
	template<typename A, typename B, typename C>
	struct tuple_struct<A, B, C> final {

		using a_type = typename A;
		using b_type = typename B;
		using c_type = typename C;

		using this_t = tt::tuple_struct<a_type, b_type, c_type>;

		// The first value of the tuple.
		a_type a = {};

		// The second value of the tuple.
		b_type b = {};

		// The third value of the tuple.
		c_type c = {};

		// Returns if this and the given tuple are equal.
		inline tt_bool equal(const this_t& x) const noexcept {


			return a == x.a && b == x.b && c == x.c;
		}

		inline tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		inline tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// lexicographical comparison function

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			if (a < rhs.a)
				return true;

			else if (rhs.a < a)
				return false;

			else if (b < rhs.b)
				return true;

			else if (rhs.b < b)
				return false;

			else if (c < rhs.c)
				return true;

			else if (rhs.c < c)
				return false;

			else return false;
		}

		// Returns a string visualization of the tuple.
		inline tt_string visualize() const {


			tt_string r = "{ ";

			r += tt::visualize(a) + ", ";
			r += tt::visualize(b) + ", ";
			r += tt::visualize(c) + " }";

			return r;
		}

		// Returns a hash of the tuple.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(HASH_5);

			h.add(a);
			h.add(b);
			h.add(c);

			return h.get();
		}

		template<typename A0, typename B0, typename C0>
		constexpr operator tt::tuple_struct<A0, B0, C0>() const {


			return { (A0)a, (B0)b, (C0)c };
		}
	};

	// The Tirous Toolbox tuple implementation.
	template<typename A, typename B>
	struct tuple_struct<A, B> final {

		using a_type = typename A;
		using b_type = typename B;

		using this_t = tt::tuple_struct<a_type, b_type>;

		// The first value of the tuple.
		a_type a = {};

		// The second value of the tuple.
		b_type b = {};

		// Returns if this and the given tuple are equal.
		inline tt_bool equal(const this_t& x) const noexcept {


			return a == x.a && b == x.b;
		}

		inline tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		inline tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// lexicographical comparison function

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			if (a < rhs.a)
				return true;

			else if (rhs.a < a)
				return false;

			else if (b < rhs.b)
				return true;

			else if (rhs.b < b)
				return false;

			else return false;
		}

		// Returns a string visualization of the tuple.
		inline tt_string visualize() const {


			tt_string r = "{ ";

			r += tt::visualize(a) + ", ";
			r += tt::visualize(b) + " }";

			return r;
		}

		// Returns a hash of the tuple.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(HASH_6);

			h.add(a);
			h.add(b);

			return h.get();
		}

		template<typename A0, typename B0>
		constexpr operator tt::tuple_struct<A0, B0>() const {


			return { (A0)a, (B0)b };
		}
	};

	// The Tirous Toolbox tuple implementation.
	template<typename A>
	struct tuple_struct<A> final {

		using a_type = typename A;

		using this_t = tt::tuple_struct<a_type>;

		// The first value of the tuple.
		a_type a = {};

		// Returns if this and the given tuple are equal.
		inline tt_bool equal(const this_t& x) const noexcept {


			return a == x.a;
		}

		inline tt_bool operator==(const this_t& rhs) const noexcept { return equal(rhs); }
		inline tt_bool operator!=(const this_t& rhs) const noexcept { return !equal(rhs); }

		// lexicographical comparison function

		inline tt_bool operator<(const this_t& rhs) const noexcept {


			if (a < rhs.a)
				return true;

			else if (rhs.a < a)
				return false;

			else return false;
		}

		// Returns a string visualization of the tuple.
		inline tt_string visualize() const {


			tt_string r = "{ ";

			r += tt::visualize(a) + " }";

			return r;
		}

		// Returns a hash of the tuple.
		inline tt_size hash() const noexcept {


			tt::hash_factory h(HASH_7);

			h.add(a);

			return h.get();
		}

		template<typename A0>
		constexpr operator tt::tuple_struct<A0>() const {


			return { (A0)a };
		}
	};

#undef HASH_0
#undef HASH_1
#undef HASH_2
#undef HASH_3
#undef HASH_4
#undef HASH_5
#undef HASH_6
#undef HASH_7

	// NOTE: Remember that we can't use *const* here, as it'll mess up with std::decay_t when
	//		 one tries to use things like C-strings, which'll have their const-ness removed incorrectly.

	// NOTE: Got a REALLY bizzare error trying to use 'tt_tuple' instead of 'tt::tuple_struct' below...

	// Returns a Tirous Toolbox tuple of the given values, implying the types involved.
	template<typename... Args>
	constexpr tt::tuple_struct<std::decay_t<Args>...> tuple_of(Args... args) {


		return { std::move(args)... };
	}
}

#define __TPARAMS1 <typename A>
#define __TPARAMS2 <typename A, typename B>
#define __TPARAMS3 <typename A, typename B, typename C>
#define __TPARAMS4 <typename A, typename B, typename C, typename D>
#define __TPARAMS5 <typename A, typename B, typename C, typename D, typename E>
#define __TPARAMS6 <typename A, typename B, typename C, typename D, typename E, typename F>
#define __TPARAMS7 <typename A, typename B, typename C, typename D, typename E, typename F, typename G>
#define __TPARAMS8 <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H>

#define __TARGS1 <A>
#define __TARGS2 <A, B>
#define __TARGS3 <A, B, C>
#define __TARGS4 <A, B, C, D>
#define __TARGS5 <A, B, C, D, E>
#define __TARGS6 <A, B, C, D, E, F>
#define __TARGS7 <A, B, C, D, E, F, G>
#define __TARGS8 <A, B, C, D, E, F, G, H>

TT_VISUALIZERS(0) {


	TT_REGISTER_VISUALIZE_T(tt::tuple_struct, __TPARAMS1, __TARGS1);
	TT_REGISTER_VISUALIZE_T(tt::tuple_struct, __TPARAMS2, __TARGS2);
	TT_REGISTER_VISUALIZE_T(tt::tuple_struct, __TPARAMS3, __TARGS3);
	TT_REGISTER_VISUALIZE_T(tt::tuple_struct, __TPARAMS4, __TARGS4);
	TT_REGISTER_VISUALIZE_T(tt::tuple_struct, __TPARAMS5, __TARGS5);
	TT_REGISTER_VISUALIZE_T(tt::tuple_struct, __TPARAMS6, __TARGS6);
	TT_REGISTER_VISUALIZE_T(tt::tuple_struct, __TPARAMS7, __TARGS7);
	TT_REGISTER_VISUALIZE_T(tt::tuple_struct, __TPARAMS8, __TARGS8);
}

TT_HASHERS(0) {


	TT_REGISTER_HASH_T(tt::tuple_struct, __TPARAMS1, __TARGS1);
	TT_REGISTER_HASH_T(tt::tuple_struct, __TPARAMS2, __TARGS2);
	TT_REGISTER_HASH_T(tt::tuple_struct, __TPARAMS3, __TARGS3);
	TT_REGISTER_HASH_T(tt::tuple_struct, __TPARAMS4, __TARGS4);
	TT_REGISTER_HASH_T(tt::tuple_struct, __TPARAMS5, __TARGS5);
	TT_REGISTER_HASH_T(tt::tuple_struct, __TPARAMS6, __TARGS6);
	TT_REGISTER_HASH_T(tt::tuple_struct, __TPARAMS7, __TARGS7);
	TT_REGISTER_HASH_T(tt::tuple_struct, __TPARAMS8, __TARGS8);
}

#undef __TPARAMS1
#undef __TPARAMS2
#undef __TPARAMS3
#undef __TPARAMS4
#undef __TPARAMS5
#undef __TPARAMS6
#undef __TPARAMS7
#undef __TPARAMS8

#undef __TARGS1
#undef __TARGS2
#undef __TARGS3
#undef __TARGS4
#undef __TARGS5
#undef __TARGS6
#undef __TARGS7
#undef __TARGS8

