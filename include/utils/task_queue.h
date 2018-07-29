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
//   https://github.com/shatilov-diman/libraries/lock_free/
//   https://bitbucket.org/charivariltd/libraries/lock_free/
//
// Author contacts:
//   Dmitry Shatilov (e-mail: shatilov.diman@gmail.com; site: https://www.linkedin.com/in/shatilov)
//
//

#pragma once

#include "ordered_queue.h"
#include "worker.h"
#include "utils.h"

#include <functional>
#include <atomic>

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
		}

	private:
		void stop()
		{
			stop_requested = true;
		}

		void run() noexcept
		{
			while (stop_requested == false)
			{
				execute_all();
				std::this_thread::yield();
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
		exception_handler_t exception_handler;
		lock_free::ordered_queue<task_t> queue;
		worker work;
	};
} //namespace charivari_ltd::utils

