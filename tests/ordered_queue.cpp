// Project tool
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

#include <utils/ordered_queue.h>

#include <gtest/gtest.h>

#include <thread>

using namespace charivari_ltd::utils::lock_free;

class ordered_queue_test_suite :
	public testing::Test
{
};

TEST_F(ordered_queue_test_suite, empty)
{
	ordered_queue<int> queue;
	EXPECT_FALSE(queue.pop());
}

TEST_F(ordered_queue_test_suite, one_element)
{
	ordered_queue<int> queue;
	queue.push(5);
	EXPECT_EQ(5, *queue.pop());

	EXPECT_FALSE(queue.pop());
}

TEST_F(ordered_queue_test_suite, fifo)
{
	ordered_queue<int> queue;
	queue.push(5);
	queue.push(4);
	queue.push(6);

	EXPECT_EQ(5, *queue.pop());
	EXPECT_EQ(4, *queue.pop());
	EXPECT_EQ(6, *queue.pop());

	EXPECT_FALSE(queue.pop());
}

TEST_F(ordered_queue_test_suite, push_pop_push_pop)
{
	ordered_queue<int> queue;

	queue.push(5);
	EXPECT_EQ(5, *queue.pop());

	queue.push(4);
	EXPECT_EQ(4, *queue.pop());

	queue.push(6);
	EXPECT_EQ(6, *queue.pop());

	EXPECT_FALSE(queue.pop());
}

TEST_F(ordered_queue_test_suite, multithread_test)
{
	static const std::size_t c_workers = 100;
	ordered_queue<int> queue;

	std::array<std::thread, c_workers> threads;

	std::atomic<std::size_t> threads_counter {0};

	for (int index = 0; index < c_workers; ++index)
		threads[index] = std::thread([index, &queue, &threads_counter] {
			for (int x = 0; x < c_workers; ++x) {
				queue.push(c_workers * index + x);
			}
			threads_counter++;
		});

	std::map<int, int> counters;

	bool stop = false;
	while (stop == false)
	{
		if (threads_counter == c_workers)
			stop = true;
		while (auto val = queue.pop())
			++counters[*val];
	}

	EXPECT_EQ(counters.size(), c_workers * c_workers);
	for (int index = 0; index < c_workers * c_workers; ++index)
		EXPECT_EQ(counters[index], 1);

	for (auto& thread : threads)
		thread.join();
}

