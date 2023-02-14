

#pragma once


// The class encapsulating the Tirous Toolbox thread-pool implementation.


#include <memory>
#include <unordered_set>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "aliases.h"
#include "exceptions.h"
#include "task.h"
#include "regular_task.h"


#if 0
#define _TT_ENABLE_THREAD_POOL_DEBUGGING 1
#endif


#ifdef _TT_ENABLE_THREAD_POOL_DEBUGGING
#include "hash.h"
#include "fprint.h"
#endif


namespace _tt {


	struct thread_pool_state;

	inline void worker_thread_function(std::shared_ptr<thread_pool_state> state);
}

namespace tt {
	

	TT_EXCEPTION_STRUCT(thread_pool_zero_workers_error);

	class thread_pool final {
	public:

		// Initializes a thread-pool of n worker-threads.
		// Throws tt::thread_pool_zero_workers_error if n == 0.
		inline thread_pool(tt_size n);

		// Default initialized thread-pools encapsulate inert thread-pools which cannot be used outside of move-assignment and destruction.
		// Behaviour is undefined if one attempts to use one of these inert thread-pools beyond this vary limited scope.
		thread_pool() = default;

		thread_pool(const thread_pool&) = delete;
		inline thread_pool(thread_pool&& x) noexcept;

		inline ~thread_pool() noexcept;

		thread_pool& operator=(const thread_pool&) = delete;
		inline thread_pool& operator=(thread_pool&& rhs) noexcept;


		// Returns the std::future<void> used to check if the thread-pool's underlying system has fully shutdown.
		// The thread-pool's underlying system may outlive the tt::thread_pool itself, only shutting down fully once ALL outstanding tasks have terminated (discarding any unstarted ones in the task queue.)
		// If any tasks enter a state where they will never terminate, then the this system can never shutdown, and thus a memory leak will occur.
		// The returned future is intended to make the detection/debugging of these shutdown failures easier.
		// Behaviour is undefined if get_shutdown_future is called more than once for any given tt::thread_pool.
		inline std::future<void> get_shutdown_future();

		// Prematurely shuts down the thread-pool (but the underlying system may not, see tt::thread_pool::get_shutdown_future for details.)
		// Hereafter, until move-assigned, the thread-pool will encapsulate an inert thread-pool which cannot be used outside of move-assignment and destruction.
		// Behaviour is undefined if one attempts to use the thread-pool beyond this vary limited scope.
		inline void shutdown();


		// Returns the number of worker-threads which the thread-pool is designated to have.
		// The number of worker-threads designated reflects the desired number, but the actual number active may take some time to align with this number.
		inline tt_size get_worker_threads() const noexcept;

		// Sets the amount of worker-threads which the thread-pool is designated to have.
		// Having zero worker-threads is valid, but no tasks will be performed while the number remains zero.
		// The number of worker-threads designated reflects the desired number, but the actual number active may take some time to align with this number.
		inline void set_worker_threads(tt_size n);


		// Returns the number of unfinished tasks in the thread-pool.
		inline tt_size get_tasks() const noexcept;

		// Returns the number of exceptions counted by the thread-pool.
		// If an otherwise uncaught exception arises from the execution of a task, the thread-pool will count this, but otherwise catch it and discard it.
		// Tasks who's execution terminates due to an exception like this will still be deemed 'complete'.
		// Notice that due to how tt::regular_task works, exceptions which arise in tt::thread_pool::dispatch dispatched tasks should never increment this counter.
		inline tt_size get_exceptions() const noexcept;

		// Dispatches task x, adding it to the task queue of the thread-pool.
		// Fails quietly if x is nullptr.
		inline void dispatch_task(std::unique_ptr<tt::task> x);

		// Dispatches a tt::regular_task<FType> of function f, using fargs, adding it to the task queue of the thread-pool.
		// Returns the std::future associated with this tt::regular_task<FType> object.
		template<typename FType, typename F, typename... FArgs>
		inline typename tt::regular_task<FType>::future_t dispatch(F&& f, FArgs&&... fargs);


	private:

		std::shared_ptr<_tt::thread_pool_state> _state;
	};
}

namespace _tt {


	// NOTE: this encapsulates the main state of the thread-pool's underlying system

	struct thread_pool_state final {

		// NOTE: made designated_workers atomic for tt::thread_pool::get_worker_threads
		// NOTE: made tasks atomic for tt::thread_pool::get_tasks
		// NOTE: made exceptions atomic for tt::thread_pool::get_exceptions
		// NOTE: active_workers is incremented by add_workers_unsafe, but decremented by worker thread

		std::mutex											mtx					= {};
		std::condition_variable								cv					= {};
		tt_size												active_workers		= 0;
		std::atomic_size_t									designated_workers	= 0;
		std::atomic_size_t									tasks				= 0;
		std::atomic_size_t									exceptions			= 0;
		std::unordered_map<std::thread::id, std::thread>	worker_threads		= {};
		std::queue<std::unique_ptr<tt::task>>				task_queue			= {};
		std::promise<void>									shutdown_promise	= {};
		std::weak_ptr<thread_pool_state>					weak_this			= {};

#ifdef _TT_ENABLE_THREAD_POOL_DEBUGGING
		std::mutex											debug_mtx			= {};
#endif


		// NOTE: the tt::thread_pool and its worker-thread functions both carrying strong references
		//		 to the system's state object, keeping the system alive
		// 
		//		 upon system shutdown, the tt::thread_pool is being destroyed, and the number of 
		//		 designated worker-threads will be set to zero, meaning that the former will release
		//		 its strong reference, and so too will be ladder, one-by-one as they each terminate
		//		 themselves to make active_workers == designated_workers true
		//
		//		 once both of these two have fully shutdown, this destructor will be called, which
		//		 will cleanup the system as a whole, and use shutdown_promise

		inline ~thread_pool_state() noexcept;


		// NOTE: the *_unsafe methods below presume that mtx is locked

		inline void add_workers_unsafe(tt_size n);
		inline void remove_workers_unsafe(tt_size n);

		inline void add_workers(tt_size n);
		inline void remove_workers(tt_size n);

		inline void dispatch_task(std::unique_ptr<tt::task> x);

		inline void startup(tt_size n, std::weak_ptr<thread_pool_state> weak_this);
		inline void shutdown() noexcept;


		template<typename... Args>
		inline void debug_echo(Args&&... args);
	};


	inline _tt::thread_pool_state::~thread_pool_state() noexcept {


		std::scoped_lock lk(mtx);

		tt_assert(designated_workers == 0);
		tt_assert(active_workers == 0);
		tt_assert(worker_threads.empty());
		tt_assert(task_queue.empty());

		shutdown_promise.set_value();
	}

	inline void _tt::thread_pool_state::add_workers_unsafe(tt_size n) {


		debug_echo("adding ", n, " new workers");

		active_workers += n;
		designated_workers += n;

		TT_FOR(i, n) {


			std::thread _worker(worker_thread_function, std::move(weak_this.lock()));

			worker_threads[_worker.get_id()] = std::move(_worker);
		}
	}

	inline void _tt::thread_pool_state::remove_workers_unsafe(tt_size n) {


		debug_echo("removing ", n, " existing workers");

		if (n <= designated_workers)
			designated_workers -= n;
		else
			designated_workers = 0;

		// NOTE: this'll cause ALL our sleeping threads to awaken, with the 
		//		 first ~n or so then shutting down in response to the change
		//		 in designated_workers, and the rest going back to sleep
		//
		//		 there's probably a WAY better way of doing this, but I just
		//		 want this to work, so I'm gonna be *safe* and just wake
		//		 them all up, so we know we didn't miss any
		
		cv.notify_all();
	}

	inline void _tt::thread_pool_state::add_workers(tt_size n) {


		std::scoped_lock lk(mtx);

		add_workers_unsafe(n);
	}

	inline void _tt::thread_pool_state::remove_workers(tt_size n) {


		std::scoped_lock lk(mtx);

		remove_workers_unsafe(n);
	}

	inline void _tt::thread_pool_state::dispatch_task(std::unique_ptr<tt::task> x) {


		tt_assert(x);

		debug_echo("enqueueing new task");

		{
			std::scoped_lock lk(mtx);

			task_queue.push(std::move(x));

			++tasks;
		}

		cv.notify_one();
	}

	inline void _tt::thread_pool_state::startup(tt_size n, std::weak_ptr<thread_pool_state> weak_this) {


		debug_echo("thread-pool startup");

		std::scoped_lock lk(mtx);

		tt_assert(designated_workers == 0);
		tt_assert(active_workers == 0);

		this->weak_this = weak_this;

		add_workers_unsafe(n);
	}

	inline void _tt::thread_pool_state::shutdown() noexcept {


		debug_echo("thread-pool shutdown");

		std::scoped_lock lk(mtx);

		// NOTE: discard any tasks in the task queue

		while (!task_queue.empty())
			task_queue.pop();

		remove_workers_unsafe(designated_workers);
	}

	template<typename... Args>
	inline void _tt::thread_pool_state::debug_echo(Args&&... args) {


#ifdef _TT_ENABLE_THREAD_POOL_DEBUGGING
		std::scoped_lock lk(debug_mtx);

		tt::fprintl("[tt::thread_pool debug] [tt::thread::id hash ", tt::hash_of(std::this_thread::get_id()), "] \"", TT_FMOVE_N(Args, args), "\"");
#endif
	}


	// NOTE: these manage the internal logic of each worker-thread
	// NOTE: the state parameter will hold the thread's strong reference to the system state

	inline void worker_thread_function(std::shared_ptr<thread_pool_state> state) {


		tt_assert(state);

		state->debug_echo("starting up");

		// NOTE: if a spurious wakeup occurs such that the spuriously waking-up thread
		//		 ends up locking mtx before a properly notified one has a chance to, causing
		//		 the former to block, our code here should be robust enough that the two
		//		 threads will essentially *switch roles*, and no issue should arise

		// NOTE: need to put this outside the loop since it'll be reaquired when the thread
		//		 awakens each time already

		std::unique_lock lk(state->mtx);

		// NOTE: this loop handles the worker-thread responding-to/affecting the state of the system

		while (true) {


			state->debug_echo("thinking");


			// if the system wants to shutdown some worker-threads, and this one isn't busy on
			// a task, then this worker-thread should shutdown

			if (state->designated_workers < state->active_workers) {


				state->debug_echo("decided to shutdown (", tt_size(state->designated_workers), " designated workers) (", state->active_workers, " active workers)");

				--(state->active_workers);

				auto _thread = std::move(state->worker_threads.at(std::this_thread::get_id()));

				state->worker_threads.erase(_thread.get_id());

				_thread.detach();

				break;
			}


			// if the task queue is empty, and we've not shutdown, go back to sleep

			else if (state->task_queue.empty()) {


				state->debug_echo("decided to sleep (", state->task_queue.size(), " tasks)");

				state->cv.wait(lk);

				continue;
			}


			// if this thread is not to shutdown, and there's a task in the queue to perform,
			// then pop the task and begin performing it, being sure to unlock lk during the
			// interim when we're performing the task to let other threads use the system state

			else {


				state->debug_echo("decided to work (", state->task_queue.size(), " tasks)");

				tt_assert(!state->task_queue.empty());

				auto _task = std::move(state->task_queue.front());

				tt_assert(_task);

				state->task_queue.pop();

				--(state->tasks);

				lk.unlock();

				// NOTE: to keep our threads from crashing, catch any exceptions which arise

				try {


					_task->perform();
				}

				catch (...) {


					state->debug_echo("task threw an exception");

					// NOTE: count exceptions which arise so end-user can still tell that they happened
					// NOTE: no need to re-lock yet, as this is atomic

					++(state->exceptions);
				}

				lk.lock();

				continue;
			}
		}

		// NOTE: worker-thread terminates hereafter, unlocking lk

		state->debug_echo("shutting down");
	}
}

namespace tt {


	inline tt::thread_pool::thread_pool(tt_size n) {


		if (n == 0)
			TT_THROW(thread_pool_zero_workers_error, "tt::thread_pool::thread_pool n may not be 0!");

		_state = std::make_shared<_tt::thread_pool_state>();

		_state->startup(n, _state);
	}

	inline tt::thread_pool::thread_pool(thread_pool&& x) noexcept
		: _state(TT_FMOVE(std::shared_ptr<_tt::thread_pool_state>, x._state)) {}

	inline tt::thread_pool::~thread_pool() noexcept {


		// NOTE: remember that an 'inert' thread-pool still needs a working destructor

		if (_state)
			_state->shutdown();
	}

	inline thread_pool& tt::thread_pool::operator=(thread_pool&& rhs) noexcept {


		TT_SELF_MOVE_TEST(rhs);

		TT_MOVESET(_state, rhs, nullptr);

		TT_RETURN_THIS;
	}

	inline std::future<void> tt::thread_pool::get_shutdown_future() {


		tt_assert(_state);

		return _state->shutdown_promise.get_future();
	}

	inline void tt::thread_pool::shutdown() {


		tt_assert(_state);

		_state->shutdown();

		_state = nullptr;
	}

	inline tt_size tt::thread_pool::get_worker_threads() const noexcept {


		tt_assert(_state);

		return _state->designated_workers;
	}

	inline void tt::thread_pool::set_worker_threads(tt_size n) {


		tt_assert(_state);

		if (n > _state->designated_workers)
			_state->add_workers(n - _state->designated_workers);

		else if (n < _state->designated_workers)
			_state->remove_workers(_state->designated_workers - n);
	}

	inline tt_size tt::thread_pool::get_tasks() const noexcept {


		tt_assert(_state);

		return _state->tasks;
	}

	inline tt_size tt::thread_pool::get_exceptions() const noexcept {


		tt_assert(_state);

		return _state->exceptions;
	}

	inline void tt::thread_pool::dispatch_task(std::unique_ptr<tt::task> x) {


		tt_assert(_state);

		if (x)
			_state->dispatch_task(std::move(x));
	}

	template<typename FType, typename F, typename... FArgs>
	inline typename tt::regular_task<FType>::future_t tt::thread_pool::dispatch(F&& f, FArgs&&... fargs) {


		auto _task = std::make_unique<tt::regular_task<FType>>(f, fargs...);
		auto _future = _task->get_future();

		dispatch_task(std::move(_task));

		return _future;
	}
}

