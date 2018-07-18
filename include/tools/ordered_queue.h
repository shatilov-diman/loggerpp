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

#include <atomic>
#include <utility>
#include <optional>

namespace charivari_ltd::tools::lock_free
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

		// Thread safe
		void push(item_t&& item)
		{
			push(front, allocate(std::move(item)));
		}

		// Not thread safe!
		std::optional<item_t> pop()
		{
			if (auto ptr = pop(front, back))
			{
				return deallocate(ptr);
			}
			return {};
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

		static std::optional<item_t> deallocate(node_t* node)
		{
			std::optional<item_t> out{std::move(node->item)};
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
				return std::exchange(back, nullptr);

			if (back->prev == nullptr)
				fill_prev(front);
			auto out = std::exchange(back, back->prev);
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
} //namespace charivari_ltd::tools::lock_free

