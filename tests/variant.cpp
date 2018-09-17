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

#include <loggerpp/logger.h>

#include <gtest/gtest.h>

using namespace charivari_ltd;

class variant_test_suite :
	public testing::Test
{
};

TEST_F(variant_test_suite, check_key_variant_empty)
{
	loggerpp::default_log_traits::key_t root;
	(void)root;
}

TEST_F(variant_test_suite, check_key_variant_char_ptr)
{
	loggerpp::default_log_traits::key_t root("hi");
	EXPECT_EQ(utils::get<std::string>(root), "hi");
}

TEST_F(variant_test_suite, check_key_variant_wchar_ptr)
{
	loggerpp::default_log_traits::key_t root(L"hi");
	EXPECT_EQ(utils::get<std::wstring>(root), L"hi");
}

TEST_F(variant_test_suite, check_key_variant_string)
{
	loggerpp::default_log_traits::key_t root(std::string("hi"));
	EXPECT_EQ(utils::get<std::string>(root), "hi");
}


TEST_F(variant_test_suite, check_key_variant_wstring)
{
	loggerpp::default_log_traits::key_t root(std::wstring(L"hi"));
	EXPECT_EQ(utils::get<std::wstring>(root), L"hi");
}

TEST_F(variant_test_suite, check_value_variant_empty)
{
	loggerpp::default_log_traits::key_t root;
	(void)root;
}

TEST_F(variant_test_suite, check_value_variant_char_ptr)
{
	loggerpp::default_log_traits::value_t root("hi");
	EXPECT_EQ(utils::get<std::string>(root), "hi");
}

TEST_F(variant_test_suite, check_value_variant_wchar_ptr)
{
	loggerpp::default_log_traits::value_t root(L"hi");
	EXPECT_EQ(utils::get<std::wstring>(root), L"hi");
}

TEST_F(variant_test_suite, check_value_variant_string)
{
	loggerpp::default_log_traits::value_t root(std::string("hi"));
	EXPECT_EQ(utils::get<std::string>(root), "hi");
}

TEST_F(variant_test_suite, check_value_variant_wstring)
{
	loggerpp::default_log_traits::value_t root(std::wstring(L"hi"));
	EXPECT_EQ(utils::get<std::wstring>(root), L"hi");
}

TEST_F(variant_test_suite, check_value_variant_nullptr)
{
	loggerpp::default_log_traits::value_t root(nullptr);
	EXPECT_EQ(utils::get<std::nullptr_t>(root), nullptr);
}

TEST_F(variant_test_suite, check_value_variant_bool_t)
{
	loggerpp::default_log_traits::value_t root(utils::true_t);
	EXPECT_EQ(utils::get<utils::bool_t>(root), true);
}

TEST_F(variant_test_suite, check_value_variant_int)
{
	loggerpp::default_log_traits::value_t root(5L);
	EXPECT_EQ(utils::get<std::int64_t>(root), 5);
}

TEST_F(variant_test_suite, check_value_variant_uint)
{
	loggerpp::default_log_traits::value_t root(5UL);
	EXPECT_EQ(utils::get<std::uint64_t>(root), 5);
}

TEST_F(variant_test_suite, check_value_variant_double)
{
	loggerpp::default_log_traits::value_t root(5.0);
	EXPECT_EQ(utils::get<double>(root), 5.0);
}

TEST_F(variant_test_suite, check_value_variant_timepoint)
{
	const auto now = std::chrono::system_clock::now();
	loggerpp::default_log_traits::value_t root(now);
	EXPECT_EQ(utils::get<std::chrono::system_clock::time_point>(root), now);
}

