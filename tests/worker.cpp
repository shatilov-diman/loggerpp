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

