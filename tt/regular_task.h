

#pragma once


// A derivative of tt::task encapsulating an asynchronous function call.

// This is the type used to define tt::thread_pool::dispatch, and so is the 'regular' type
// of task used to hide the guts of the task system from the end-user most of the time.


#include <tuple>
#include <future>

#include "task.h"


namespace tt {


	// NOTE: using this specialization trick I learned from reading std::packaged_task in order
	//		 to let us use a function type as our type argument, then use the specialization to
	//		 *extract* Result and Args

	template<typename>
	class regular_task final {};

	template<typename Result, typename... Args>
	class regular_task<Result(Args...)> final : public task {
	public:

		using tuple_t			= std::tuple<Args...>;
		using packaged_task_t	= std::packaged_task<Result(Args...)>;
		using future_t			= std::future<Result>;

		using this_t			= regular_task<Result(Args...)>;


		template<typename F, typename... FArgs>
		inline regular_task(F&& f, FArgs&&... fargs) noexcept;

		regular_task() = delete;
		regular_task(const this_t&) = delete;
		regular_task(this_t&&) noexcept = delete;

		virtual ~regular_task() noexcept = default;

		this_t& operator=(const this_t&) = delete;
		this_t& operator=(this_t&&) noexcept = delete;


		// Returns an std::future used to await the completion of the task.
		// Behaviour is undefined if get_future is called more than once for any given tt::regular_task.
		inline future_t get_future() { return pt.get_future(); }


		inline void perform() override final { (void)std::apply(pt, args); }


	private:

		tuple_t args;
		packaged_task_t pt;
	};


	template<typename Result, typename... Args>
	template<typename F, typename... FArgs>
	inline tt::regular_task<Result(Args...)>::regular_task(F&& f, FArgs&&...fargs) noexcept 
		: args(std::make_tuple(fargs...)), 
		pt(packaged_task_t(f)) {}
}

