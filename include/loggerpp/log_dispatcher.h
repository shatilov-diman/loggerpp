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

#include "../utils/task_queue.h"
#include "../utils/noncopyable.h"

#include <functional>
#include <future>
#include <set>

namespace charivari_ltd
{
namespace loggerpp
{
	template <typename tags_handle_t>
	class dispatcher :
		public utils::noncopyable
	{
	public:
		using consumer_fn = std::function<void (const tags_handle_t& tags)>;
		using exception_handler_t = utils::task_queue::exception_handler_t;

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

		void push(tags_handle_t&& tags)
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
		void dispatch(const tags_handle_t& tags)
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
} //namespace loggerpp
} //namespace charivari_ltd

