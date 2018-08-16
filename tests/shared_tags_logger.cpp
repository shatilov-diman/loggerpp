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

#include <loggerpp/shared_tags_logger.h>
#include <utils/task_queue.h>

#include <gtest/gtest.h>

using namespace charivari_ltd;

class shared_tags_logger_test_suite :
	public testing::Test
{
};

TEST_F(shared_tags_logger_test_suite, empty)
{
	shared_tags_logger root;
	(void)root;
}

TEST_F(shared_tags_logger_test_suite, check_shared_tags_logger)
{
	shared_tags_logger root;
	auto x = extend_logger(root, shared_tags_logger::tags_t {
		{"aaa", std::wstring{L"BBB"}},
	});

	{
		auto subscription = root >> loggerpp::shared_tags_default_consumer;
		(void)subscription;
		root.debug("AAA {} CCC", "BBB");
	}

}

TEST_F(shared_tags_logger_test_suite, check_shared_tags_logger_with_consumer)
{
	std::vector<shared_tags_logger::traits_t::tags_handle_t> check1;
	std::vector<shared_tags_logger::traits_t::tags_handle_t> check2;

	shared_tags_logger root;
	root.debug("1");
	{
		auto subscription1 = root >> [&check1] (const auto& tags_handle) {
			check1.push_back(tags_handle);
		};
		auto subscription2 = root >> [&check2] (const auto& tags_handle) {
			check2.push_back(tags_handle);
		};
		root.debug("2");
	}
	root.debug("3");

	EXPECT_EQ(check1.size(), 1);
	EXPECT_EQ(get_message(check1[0]), "2");

	EXPECT_EQ(check2.size(), 1);
	EXPECT_EQ(get_message(check2[0]), "2");

	EXPECT_EQ(check1[0], check2[0]);
}

TEST_F(shared_tags_logger_test_suite, check_shared_tags_logger_with_own_thread_consumer)
{
	std::vector<shared_tags_logger::traits_t::tags_handle_t> check1;
	std::vector<shared_tags_logger::traits_t::tags_handle_t> check2;

	shared_tags_logger root;
	root.debug("1");
	{
		auto subscription1 = root >> loggerpp::run_own_thread([&check1] (const auto& tags_handle) {
			check1.push_back(tags_handle);
		});
		auto subscription2 = root >> loggerpp::run_own_thread([&check2] (const auto& tags_handle) {
			check2.push_back(tags_handle);
		});
		root.debug("2");
	}
	root.debug("3");

	EXPECT_EQ(check1.size(), 1);
	EXPECT_EQ(get_message(check1[0]), "2");

	EXPECT_EQ(check2.size(), 1);
	EXPECT_EQ(get_message(check2[0]), "2");

	EXPECT_EQ(check1[0], check2[0]);
}

TEST_F(shared_tags_logger_test_suite, check_order_of_own_threads)
{
	std::vector<std::size_t> check;

	shared_tags_logger root;
	root.debug("1");
	{
		auto subscription1 = root >> loggerpp::run_own_thread([&check] (const auto& tags_handle) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			check.push_back(1);
		});
		auto subscription2 = root >> loggerpp::run_own_thread([&check] (const auto& tags_handle) {
			check.push_back(2);
		});
		root.debug("2");
	}
	root.debug("3");

	EXPECT_EQ(check.size(), 2);
	EXPECT_EQ(check[0], 2);
	EXPECT_EQ(check[1], 1);
}


TEST_F(shared_tags_logger_test_suite, check_order_on_destruct_own_threads)
{
	std::vector<std::size_t> check;

	shared_tags_logger root;
	root.debug("0");
	{
		auto subscription1 = root >> loggerpp::run_own_thread([&check] (const auto& tags_handle) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			check.push_back(1);
		});
		root.debug("5");
	}//wait while thread is finishing

	{
		auto subscription2 = root >> loggerpp::run_own_thread([&check] (const auto& tags_handle) {
			check.push_back(2);
		});
		root.debug("5");
	}//wait while thread is finishing
	root.debug("3");

	EXPECT_EQ(check.size(), 2);
	EXPECT_EQ(check[0], 1);
	EXPECT_EQ(check[1], 2);
}

