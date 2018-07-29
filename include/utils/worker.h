// Project loggerpp
// Copyright (C) 2018 Dmitry Shatilov
//
// This file is a part of the project loggerpp.
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

