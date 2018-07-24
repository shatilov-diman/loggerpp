// Project tool
// Copyright (C) 2016-2017 Dmitry Shatilov
//
// This file is a part of the project tools.
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Additional restriction according to GPLv3 pt 7:
// b) required preservation author attributions;
// c) required preservation links to original sources
//
// Original sources:
//   https://github.com/shatilov-diman/libraries/tools/
//   https://bitbucket.org/charivariltd/libraries/tools/
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

namespace charivari_ltd::tools
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

		void rethrow_exception()
		{
			if (thread.joinable())
			{
				thread.join();
				task_result.get();
			}
		}

		~worker()
		{
			rethrow_exception();// Try to rethrow exception. Yes, terminating is OK too.
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
} //namespace charivari_ltd::tools

