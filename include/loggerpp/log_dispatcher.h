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

#include "../utils/task_queue.h"
#include "../utils/noncopyable.h"

#include <functional>
#include <future>
#include <set>

namespace charivari_ltd::loggerpp
{
	template <typename tags_t>
	class dispatcher :
		public utils::noncopyable
	{
	public:
		using consumer_fn = std::function<void (const tags_t& tags)>;
		using exception_handler_t = utils::task_queue::exception_handler_t;

		using consumer_ptr = std::shared_ptr<consumer_fn>;

	public:
		dispatcher() :
			dispatcher([] (std::exception_ptr ptr) {
				std::rethrow_exception(ptr);
			})
		{}

		explicit dispatcher(exception_handler_t&& handler) :
			exception_handler(std::move(handler)),
			queue([this] (std::exception_ptr ptr) {
				handle(ptr);
			})
		{}

		auto subscribe(consumer_fn&& consumer)
		{
			auto ptr = std::shared_ptr<consumer_fn>(new consumer_fn{std::move(consumer)}, [this](consumer_fn* ptr) {
				unsubscribe(ptr);
				delete ptr;
			});

			queue.push([this, ptr = ptr.get()] {
				consumers.insert(ptr);
			});
			return ptr;
		}

		void push(tags_t&& tags)
		{
			queue.push([this, tags{std::move(tags)}] {
				dispatch(tags);
			});
		}

	private:
		void unsubscribe(consumer_fn* ptr)
		{
			std::promise<void> promise;
			auto future = promise.get_future();
			queue.push([this, ptr, &promise] () mutable {
				consumers.erase(ptr);
				promise.set_value();
			});
			future.wait();
		}

	private:
		void dispatch(const tags_t& tags)
		{
			for (auto& c : consumers)
			{
				try {
					(*c)(tags);
				} catch (...) {
					handle(std::current_exception());
				}
			}
		}

		void handle(std::exception_ptr ptr)
		{
			exception_handler(ptr);
		}
		
	private:
		exception_handler_t exception_handler;
		std::set<consumer_fn*> consumers;
		utils::task_queue queue;
	};
} //namespace charivari_ltd::loggerpp

