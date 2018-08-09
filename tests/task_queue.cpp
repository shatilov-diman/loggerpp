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

#include <utils/task_queue.h>

#include <gtest/gtest.h>

using namespace charivari_ltd::utils;

class task_queue_test_suite :
	public testing::Test
{
};

TEST_F(task_queue_test_suite, empty)
{
	task_queue queue;
	(void)queue;
}

TEST_F(task_queue_test_suite, default_task_queue_terminate_app_on_exception)
{
	ASSERT_DEATH({
		task_queue queue;
		queue.push([] {
			throw std::runtime_error("exception");
		});
	}, "");
}

TEST_F(task_queue_test_suite, task_queue_with_nullptr_exceptions_handler_throws_exception)
{
	EXPECT_THROW({
		task_queue queue(nullptr);
		(void)queue;
	}, std::invalid_argument);
}

TEST_F(task_queue_test_suite, task_queue_with_pass_all_exceptions_to_exception_handler)
{
	std::size_t value = 0;
	{
		task_queue queue([&value] (std::exception_ptr) {
			value = 1;
		});
		queue.push([] {
			throw std::runtime_error("exception");
		});
	}
	EXPECT_EQ(value, 1);
}

TEST_F(task_queue_test_suite, task_queue_execute_tasks_in_fifo)
{
	std::vector<std::size_t> values;
	{
		task_queue queue;
		queue.push([&values] {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			values.push_back(1);
		});
		queue.push([&values] {
			values.push_back(2);
		});
		queue.push([&values] {
			values.push_back(3);
		});
	}
	EXPECT_EQ(values.size(), 3);
	EXPECT_EQ(values[0], 1);
	EXPECT_EQ(values[1], 2);
	EXPECT_EQ(values[2], 3);
}

