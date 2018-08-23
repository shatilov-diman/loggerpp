// Project loggerpp
//
// MIT License
//
// Copyright (C) 2018 Dmitry Shatilov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Original sources:
//   https://github.com/shatilov-diman/loggerpp/
//   https://bitbucket.org/charivariltd/loggerpp/
//
// Author contacts:
//   Dmitry Shatilov (e-mail: shatilov.diman@gmail.com; site: https://www.linkedin.com/in/shatilov)
//
//

#pragma once

#include "ordered_queue.h"
#include "worker.h"
#include "utils.h"

#include <condition_variable>
#include <functional>
#include <atomic>
#include <mutex>

namespace charivari_ltd::utils
{
	class task_queue
	{
	public:
		using task_t = std::function<void ()>;
		using exception_handler_t = std::function<void (std::exception_ptr)>;

	public:
		task_queue() :
			task_queue([] (std::exception_ptr ptr) { std::rethrow_exception(ptr); })
		{}

		explicit task_queue(exception_handler_t&& handler) :
			stop_requested {false},
			exception_handler{move_nonnullptr_or_die(std::move(handler))},
			work([this] { run(); })
		{}

		~task_queue()
		{
			stop();
		}

		void push(task_t&& cb)
		{
			queue.push(std::move(cb));
			wake_up();
		}

	private:
		void stop()
		{
			stop_requested = true;
			wake_up();
		}

		void wake_up()
		{
			signaler.notify_all();
		}

		void wait()
		{
			std::unique_lock<std::mutex> guard(mutex);
			if (stop_requested == false)
				signaler.wait(guard);
		}

		void run() noexcept
		{
			while (stop_requested == false)
			{
				execute_all();
				wait();
			}
			execute_all();
		}

		void execute_all() noexcept
		{
			while (auto&& cb = queue.pop())
				try_execute(std::move(*cb));
		}

		void try_execute(task_t&& cb) noexcept
		{
			try {
				cb();
			} catch (...) {
				exception_handler(std::current_exception());
			}
		}

	private:
		std::atomic_bool stop_requested;

		std::mutex mutex;
		std::condition_variable signaler;

		exception_handler_t exception_handler;
		lock_free::ordered_queue<task_t> queue;
		worker work;
	};
} //namespace charivari_ltd::utils

