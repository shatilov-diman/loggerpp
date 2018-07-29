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

#include <loggerpp/logger.h>

#include <gtest/gtest.h>

using namespace charivari_ltd;

using dispatcher = logger::dispatcher_t;

class logger_test_suite :
	public testing::Test
{
};

TEST_F(logger_test_suite, empty)
{
	logger root{std::make_shared<dispatcher>(), {}};
	(void)root;
}

TEST_F(logger_test_suite, add_remove_subscription)
{
	logger root{std::make_shared<dispatcher>(), {}};
	{
		auto subscription = root.get_dispatcher()->subscribe([] (const auto& tags) {
		});
		(void)subscription;
	}
}

TEST_F(logger_test_suite, log_without_consumer)
{
	logger root{std::make_shared<dispatcher>(), {}};
	root.debug("Test");
}

TEST_F(logger_test_suite, log_messages_with_consumer)
{
	std::vector<std::string> check;
	logger root{std::make_shared<dispatcher>(), {}};
	root.debug("1");
	{
		auto subscription = root.get_dispatcher()->subscribe([&check] (const auto& tags) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			check.push_back(get_message(tags));
		});
		root.debug("2");
	}
	root.debug("3");
	EXPECT_EQ(check.size(), 1);
	EXPECT_EQ(check[0], "2");
}

TEST_F(logger_test_suite, log_two_consumers)
{
	std::vector<std::string> check1;
	std::vector<std::string> check2;
	logger root{std::make_shared<dispatcher>(), {}};
	{
		auto subscription1 = root.get_dispatcher()->subscribe([&check1] (const auto& tags) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			check1.push_back(get_message(tags));
		});
		root.debug("1");
		{
			auto subscription2 = root.get_dispatcher()->subscribe([&check2] (const auto& tags) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				check2.push_back(get_message(tags));
			});
			root.debug("2");
		}
		root.debug("3");
	}
	EXPECT_EQ(check1.size(), 3);
	EXPECT_EQ(check1[0], "1");
	EXPECT_EQ(check1[1], "2");
	EXPECT_EQ(check1[2], "3");
	EXPECT_EQ(check2.size(), 1);
	EXPECT_EQ(check2[0], "2");
}

TEST_F(logger_test_suite, check_extend_tags)
{
	logger root{std::make_shared<dispatcher>(), {}};
	auto x = extend_logger(root, logger::tags_t {
		{"aaa", std::wstring{L"BBB"}},
	});

	std::vector<logger::tags_t> check;
	{
		auto subscription = root.get_dispatcher()->subscribe([&check] (const auto& tags) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			check.push_back(tags);
		});

		x.debug("test");
	}

	EXPECT_EQ(check.size(), 1);
	EXPECT_EQ(get_message(check[0]), "test");
	EXPECT_EQ(get_level(check[0]), loggerpp::level::debug);
	//EXPECT_EQ(get_tag<std::wstring>(check[0], "aaa"), L"BBB");
	//EXPECT_EQ(std::get<std::wstring>(*get_tag(check[0], "aaa")), L"BBB");
}

TEST_F(logger_test_suite, check_extend_exception)
{
	logger root{std::make_shared<dispatcher>(), {}};
	auto x = extend_logger(root, logger::tags_t {
		{"aaa", std::wstring{L"BBB"}},
	});

	std::vector<logger::tags_t> check;
	{
		auto subscription = root.get_dispatcher()->subscribe([&check] (const auto& tags) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			check.push_back(tags);
		});

		extend_exception(x, std::runtime_error("AAAA"))
			.error("fail");
	}

	EXPECT_EQ(check.size(), 1);
	EXPECT_EQ(get_message(check[0]), "fail");
	EXPECT_EQ(get_level(check[0]), loggerpp::level::error);
	//EXPECT_EQ(get_tag<std::string>(check[0], constants::key_exception_type), std::string{"St13runtime_error"});
	//EXPECT_EQ(get_tag<std::string>(check[0], constants::key_exception_message), std::string{"AAAA"});
}

TEST_F(logger_test_suite, check_formatter_no_placeholder)
{
	std::vector<std::string> check;
	logger root{std::make_shared<dispatcher>(), {}};
	{
		auto subscription = root.get_dispatcher()->subscribe([&check] (const auto& tags) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			check.push_back(get_message(tags));
		});
		root.debug("CCC", "BBB");
	}
	EXPECT_EQ(check.size(), 1);
	EXPECT_EQ(check[0], "CCC");
}

TEST_F(logger_test_suite, check_formatter_no_arg)
{
	std::vector<std::string> check;
	logger root{std::make_shared<dispatcher>(), {}};
	{
		auto subscription = root.get_dispatcher()->subscribe([&check] (const auto& tags) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			check.push_back(get_message(tags));
		});
		root.debug("{}");
	}
	EXPECT_EQ(check.size(), 1);
	EXPECT_EQ(check[0], "{}");
}

TEST_F(logger_test_suite, check_formatter)
{
	std::vector<std::string> check;
	logger root{std::make_shared<dispatcher>(), {}};
	{
		auto subscription = root.get_dispatcher()->subscribe([&check] (const auto& tags) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			check.push_back(get_message(tags));
		});
		root.debug("{}", "BBB");
	}
	EXPECT_EQ(check.size(), 1);
	EXPECT_EQ(check[0], "BBB");
}

TEST_F(logger_test_suite, check_formatter_ex)
{
	std::vector<std::string> check;
	logger root{std::make_shared<dispatcher>(), {}};
	{
		auto subscription = root.get_dispatcher()->subscribe([&check] (const auto& tags) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			check.push_back(get_message(tags));
		});
		root.debug("AAA {} CCC", "BBB");
	}
	EXPECT_EQ(check.size(), 1);
	EXPECT_EQ(check[0], "AAA BBB CCC");
}

TEST_F(logger_test_suite, check_default_consumer)
{
	logger root{std::make_shared<dispatcher>(), {}};
	auto x = extend_logger(root, logger::tags_t {
		{"aaa", std::wstring{L"BBB"}},
	});

	{
		auto subscription = root.get_dispatcher()->subscribe(loggerpp::default_consumer);
		root.debug("AAA {} CCC", "BBB");
		extend_exception(x, std::runtime_error("AAAA"))
			.error("fail");
	}

}

