

#pragma once


// The abstract base class used to derive task types for tt::thread_pool.


namespace tt {


	class task {
	public:

		virtual ~task() noexcept = default;


		// NOTE: this *performs* the encapsulated task
		// NOTE: once performed, a task object should not be used again

		virtual void perform() = 0;
	};
}

