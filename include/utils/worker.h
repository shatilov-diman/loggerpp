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

#include "noncopyable.h"

#include <exception>
#include <future>
#include <thread>

namespace charivari_ltd::utils
{
	class worker :
		public noncopyable
	{
	public:
		using callback_t = std::function<void ()>;
		using emergency_t = std::function<void (std::exception_ptr)>;

	public:
		worker() = default;

		worker(emergency_t&& emergency, callback_t&& cb) :
			worker(std::move(cb), std::move(emergency))
		{}

		explicit worker(callback_t&& cb, emergency_t&& emergency = {})
		{
			std::packaged_task<void ()> task {[this, cb{std::move(cb)}, emergency{std::move(emergency)}] () mutable {
				this->run(std::move(cb), std::move(emergency));
			}};
			task_result = task.get_future();
			thread = std::thread(std::move(task));
		}

		void wait_and_rethrow_exception()
		{
			if (thread.joinable())
			{
				thread.join();
				task_result.get();
			}
		}

		~worker()
		{
			try {
				wait_and_rethrow_exception();
			} catch (...) {
				std::terminate();//explicit terminate application if exception wasn't handled by client code
			}
		}

	private:
		void run(callback_t&& cb, emergency_t&& emergency = {})
		{
			if (cb)
			{
				try {
					cb();
				} catch (...) {
					if (emergency == nullptr)
						throw;

					emergency(std::current_exception());
				}
			}
		}

	private:
		std::future<void> task_result;
		std::thread thread;
	};
} //namespace charivari_ltd::utils

