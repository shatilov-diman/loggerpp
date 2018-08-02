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
// along with this program.  If nqueue, see <http://www.gnu.org/licenses/>.
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

