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

#include <utils/worker.h>

#include <gtest/gtest.h>

using namespace charivari_ltd::utils;

class worker_test_suite :
	public testing::Test
{
};

TEST_F(worker_test_suite, empty)
{
	worker w;
	(void)w;
}

TEST_F(worker_test_suite, check_fn)
{
	std::size_t value = 0;
	{
		worker w([&value] {
			value = 42;
		});
		(void)w;
	}
	EXPECT_EQ(value, 42);
}

TEST_F(worker_test_suite, check_no_rethrow_exception)
{
	std::size_t value = 0;
	{
		worker w([&value] {
			value = 42;
		});
		EXPECT_NO_THROW(w.wait_and_rethrow_exception());
	}
	EXPECT_EQ(value, 42);
}

TEST_F(worker_test_suite, check_null_fn)
{
	std::size_t value = 0;
	{
		worker w(nullptr);
		(void)w;
	}
	EXPECT_EQ(value, 0);
}

TEST_F(worker_test_suite, check_app_is_terminating_if_exception_have_not_taken)
{
	ASSERT_DEATH({
		worker w([] {
			throw std::runtime_error("error");
		});
		(void)w;
	}, "");
}

TEST_F(worker_test_suite, check_exception_without_emergency_bug_get_exception)
{
	worker w([] {
		throw std::runtime_error("error");
	});
	EXPECT_THROW(w.wait_and_rethrow_exception(), std::runtime_error);
}

TEST_F(worker_test_suite, check_emergency_handler)
{
	std::size_t value = 0;
	std::string err;
	{
		worker w([&value] {
			value = 41;
			throw std::runtime_error("error");
		}, [&value, &err] (std::exception_ptr e) {
			try {
				std::rethrow_exception(e);
			} catch (const std::exception& ex) {
				err = ex.what();
				value = 42;
			}
		});
		(void)w;
	}
	EXPECT_EQ(value, 42);
	EXPECT_EQ(err, "error");
}

TEST_F(worker_test_suite, check_emergency_handler_another_order)
{
	std::size_t value = 0;
	std::string err;
	auto emergency_handler = [&value, &err] (std::exception_ptr e) {
		try {
			std::rethrow_exception(e);
		} catch (const std::exception& ex) {
			err = ex.what();
			value = 42;
		}
	};

	{
		worker w(emergency_handler, [&value] {
			value = 41;
			throw std::runtime_error("error");
		});
		(void)w;
	}

	EXPECT_EQ(value, 42);
	EXPECT_EQ(err, "error");
}

TEST_F(worker_test_suite, check_app_is_terminating_if_exception_while_emergency_have_not_taken)
{
	ASSERT_DEATH({
		worker w([] {
			throw std::runtime_error("error");
		}, [] (std::exception_ptr e) {
			throw std::domain_error("error_2");
		});
		(void)w;
	}, "");
}

TEST_F(worker_test_suite, check_exception_at_emergency_bug_get_exception)
{
	worker w([] {
		throw std::runtime_error("error");
	}, [] (std::exception_ptr e) {
		throw std::domain_error("error_2");
	});
	EXPECT_THROW(w.wait_and_rethrow_exception(), std::domain_error);
}

