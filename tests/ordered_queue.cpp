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

#include <utils/ordered_queue.h>

#include <gtest/gtest.h>

#include <array>
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
	EXPECT_TRUE(queue.empty());
}

TEST_F(ordered_queue_test_suite, one_element)
{
	ordered_queue<int> queue;
	queue.push(5);
	EXPECT_FALSE(queue.empty());
	EXPECT_EQ(5, *queue.pop());
	EXPECT_TRUE(queue.empty());

	EXPECT_FALSE(queue.pop());
	EXPECT_TRUE(queue.empty());
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

