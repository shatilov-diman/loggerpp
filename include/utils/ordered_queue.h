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

#include "optional.h"
#include "utils.h"

#include <atomic>
#include <utility>

namespace charivari_ltd
{
namespace utils
{
namespace lock_free
{
	/* Lock free queue
	 *  - several writeres
	 *  - one reader at each time
	 */
	template <typename item_t>
	class ordered_queue
	{
		struct node_t
		{
			node_t* next;
			node_t* prev;

			item_t item;
		};
		using node_ptr = std::atomic<node_t*>;

	private:
		node_ptr front {nullptr};
		node_t* back {nullptr};

	public:

		ordered_queue()
		{}

		~ordered_queue()
		{
			clear();
		}

		// Thread safe
		void push(item_t&& item)
		{
			push(front, allocate(std::move(item)));
		}

		// Not thread safe!
		optional<item_t> pop()
		{
			if (auto ptr = pop(front, back))
			{
				return deallocate(ptr);
			}
			return {};
		}

		void clear()
		{
			while (pop())
				;
		}

		bool empty() const
		{
			return front.load() == nullptr;
		}

	private:

		static node_t* allocate(item_t&& item)
		{
			return new node_t {
				nullptr,
				nullptr,
				std::move(item),
			};
		}

		static optional<item_t> deallocate(node_t* node)
		{
			optional<item_t> out{std::move(node->item)};
			delete node;
			return out;
		}

	private:
		static void push(node_ptr& front, node_t* node)
		{
			auto ptr = front.load();//speedup
			for (;;)
			{
				node->next = ptr;
				if (front.compare_exchange_weak(ptr, node))
					return;
			}
		}

		static node_t* pop(node_ptr& front, node_t*& back)
		{
			if (back == nullptr)
				back = fill_prev(front);
			if (back == nullptr)
				return nullptr;

			auto tmp = back;
			if (front.compare_exchange_strong(tmp, nullptr))
				return exchange(back, nullptr);

			if (back->prev == nullptr)
				fill_prev(front);
			auto out = exchange(back, back->prev);
			out->prev->next = nullptr;

			return out;
		}

		static node_t* fill_prev(node_ptr& front)
		{
			auto ptr = front.load();
			if (ptr == nullptr)
				return nullptr;

			while (auto n = ptr->next)
			{
				n->prev = ptr;
				ptr = n;
			}
			return ptr;
		}
	};
} //namespace lock_free
} //namespace utils
} //namespace charivari_ltd

