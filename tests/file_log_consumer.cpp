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

#include <loggerpp/file_log_consumer.h>

#include <gtest/gtest.h>

#include <sstream>

using namespace charivari_ltd;

class file_log_consumer_test_suite :
	public testing::Test
{
public:
	void SetUp()
	{
		::unlink(log_test_file_name.data());
	}

	void check_data(const std::string& expected)
	{
		std::ifstream file(log_test_file_name);
		std::stringstream check;
		check << file.rdbuf();

		EXPECT_EQ(check.str(), expected);
	}

public:
	const std::string log_test_file_name = "log.log";
};

TEST_F(file_log_consumer_test_suite, empty)
{
	{
		loggerpp::details::file_log_consumer<loggerpp::default_log_traits> consumer(log_test_file_name);
		(void)consumer;
	}
	check_data("");
}

TEST_F(file_log_consumer_test_suite, empty_tag)
{
	{
		loggerpp::details::file_log_consumer<loggerpp::default_log_traits> consumer(log_test_file_name);
		consumer.push({});
	}
	check_data("\n");
}

TEST_F(file_log_consumer_test_suite, guarantee_tag)
{
	{
		loggerpp::details::file_log_consumer<loggerpp::default_log_traits> consumer(log_test_file_name);
		consumer.push({{loggerpp::constants::key_message, std::string("ASDASD")}});
	}
	check_data("ASDASD\n");
}

TEST_F(file_log_consumer_test_suite, not_guarantee_tag)
{
	{
		loggerpp::details::file_log_consumer<loggerpp::default_log_traits> consumer(log_test_file_name);
		consumer.push({
			{std::string("key1"), std::string("value")},
			{std::string("key2"), std::string("value")},
			{std::string("key3"), std::string("value")},
			{std::string("key4"), std::string("value")},
		});
	}
	check_data("value\tvalue\tvalue\tkey4=value\n");
}

TEST_F(file_log_consumer_test_suite, check_common)
{
	{
		loggerpp::details::file_log_consumer<loggerpp::default_log_traits> consumer(log_test_file_name);
		consumer.push(loggerpp::default_log_traits::tags_handle_t{
			{std::string("time"), std::uint64_t{1534704464'123456789LL}},
			{std::string("level"), loggerpp::level::info},
			{std::string("message"), std::wstring{L"Hello, worlds"}},
			{std::string("entity"), std::wstring{L"Test"}},
			{std::wstring(L"color"), std::string{"yellow"}},
		});
	}
	check_data("1534704464123456789\tinfo\tHello, worlds\tentity=Test\tcolor=yellow\n");
}

TEST_F(file_log_consumer_test_suite, check_nullptr)
{
	{
		loggerpp::details::file_log_consumer<loggerpp::default_log_traits> consumer(log_test_file_name);
		consumer.push({{"x", nullptr}});
		consumer.push({{"y", std::nullptr_t{}}});
	}
	check_data("(null)\n(null)\n");
}

TEST_F(file_log_consumer_test_suite, check_bool)
{
	{
		loggerpp::details::file_log_consumer<loggerpp::default_log_traits> consumer(log_test_file_name);
		consumer.push({{"x", utils::true_t}});
		consumer.push({{"y", utils::false_t}});
	}
	check_data("true\nfalse\n");
}

