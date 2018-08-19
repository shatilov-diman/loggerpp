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

#include <utils/utils.h>

#include <gtest/gtest.h>

using namespace charivari_ltd;

class to_string_test_suite :
	public testing::Test
{
};

TEST_F(to_string_test_suite, check_nullptr)
{
	EXPECT_EQ("(null)", utils::to_string(nullptr));
	EXPECT_EQ("(null)", utils::to_string(std::nullptr_t{}));

	EXPECT_EQ(L"(null)", utils::to_wstring(nullptr));
	EXPECT_EQ(L"(null)", utils::to_wstring(std::nullptr_t{}));
}

TEST_F(to_string_test_suite, check_bool)
{
	EXPECT_EQ("false", utils::to_string(false));
	EXPECT_EQ("true", utils::to_string(true));

	EXPECT_EQ(L"false", utils::to_wstring(false));
	EXPECT_EQ(L"true", utils::to_wstring(true));
}

TEST_F(to_string_test_suite, check_char)
{
	EXPECT_EQ("97", utils::to_string('a'));
	EXPECT_EQ("65", utils::to_string('A'));
	EXPECT_EQ("97", utils::to_string(L'a'));
	EXPECT_EQ("65", utils::to_string(L'A'));

	EXPECT_EQ(L"97", utils::to_wstring('a'));
	EXPECT_EQ(L"65", utils::to_wstring('A'));
	EXPECT_EQ(L"97", utils::to_wstring(L'a'));
	EXPECT_EQ(L"65", utils::to_wstring(L'A'));
}

TEST_F(to_string_test_suite, check_int)
{
	EXPECT_EQ("-1", utils::to_string(-1));
	EXPECT_EQ("0", utils::to_string(0));
	EXPECT_EQ("1", utils::to_string(1));

	EXPECT_EQ(L"-1", utils::to_wstring(-1));
	EXPECT_EQ(L"0", utils::to_wstring(0));
	EXPECT_EQ(L"1", utils::to_wstring(1));
}

TEST_F(to_string_test_suite, check_longlong)
{
	EXPECT_EQ("-1", utils::to_string(-1LL));
	EXPECT_EQ("0", utils::to_string(0LL));
	EXPECT_EQ("1", utils::to_string(1LL));

	EXPECT_EQ(L"-1", utils::to_wstring(-1LL));
	EXPECT_EQ(L"0", utils::to_wstring(0LL));
	EXPECT_EQ(L"1", utils::to_wstring(1LL));
}

TEST_F(to_string_test_suite, check_double)
{
	EXPECT_EQ("-1.100000", utils::to_string(-1.1));
	EXPECT_EQ("0", utils::to_string(0));
	EXPECT_EQ("1.100000", utils::to_string(1.1));

	EXPECT_EQ(L"-1.100000", utils::to_wstring(-1.1));
	EXPECT_EQ(L"0", utils::to_wstring(0));
	EXPECT_EQ(L"1.100000", utils::to_wstring(1.1));
}

TEST_F(to_string_test_suite, check_char_pointer)
{
	EXPECT_EQ("a", utils::to_string("a"));
	EXPECT_EQ("A", utils::to_string("A"));

	EXPECT_EQ(L"a", utils::to_wstring("a"));
	EXPECT_EQ(L"A", utils::to_wstring("A"));
}

TEST_F(to_string_test_suite, check_wchar_pointer)
{
	EXPECT_EQ("a", utils::to_string(L"a"));
	EXPECT_EQ("A", utils::to_string(L"A"));

	EXPECT_EQ(L"a", utils::to_wstring(L"a"));
	EXPECT_EQ(L"A", utils::to_wstring(L"A"));
}

TEST_F(to_string_test_suite, check_std_string)
{
	EXPECT_EQ("a", utils::to_string(std::string("a")));
	EXPECT_EQ("A", utils::to_string(std::string("A")));

	EXPECT_EQ(L"a", utils::to_wstring(std::string("a")));
	EXPECT_EQ(L"A", utils::to_wstring(std::string("A")));
}

TEST_F(to_string_test_suite, check_std_wstring)
{
	EXPECT_EQ("a", utils::to_string(std::wstring(L"a")));
	EXPECT_EQ("A", utils::to_string(std::wstring(L"A")));

	EXPECT_EQ(L"a", utils::to_wstring(std::wstring(L"a")));
	EXPECT_EQ(L"A", utils::to_wstring(std::wstring(L"A")));
}

