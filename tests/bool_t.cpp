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

#include <utils/bool_t.h>

#include <gtest/gtest.h>

using namespace charivari_ltd;

class bool_test_suite :
	public testing::Test
{
};

TEST_F(bool_test_suite, check_default)
{
	utils::bool_t b;
	EXPECT_FALSE(static_cast<bool>(b));
}

TEST_F(bool_test_suite, check_explicit_false)
{
	utils::bool_t b{false};
	EXPECT_FALSE(static_cast<bool>(b));
}

TEST_F(bool_test_suite, check_explicit_true)
{
	utils::bool_t b{true};
	EXPECT_TRUE(static_cast<bool>(b));
}

TEST_F(bool_test_suite, check_copy_false)
{
	utils::bool_t b{true};
	b = false;
	EXPECT_FALSE(static_cast<bool>(b));
}

TEST_F(bool_test_suite, check_copy_true)
{
	utils::bool_t b{false};
	b = true;
	EXPECT_TRUE(static_cast<bool>(b));
}

TEST_F(bool_test_suite, check_compare_false)
{
	utils::bool_t b{false};
	EXPECT_TRUE(b == false);
	EXPECT_FALSE(b != false);
	EXPECT_FALSE(b == true);
	EXPECT_TRUE(b != true);
}

TEST_F(bool_test_suite, check_compare_true)
{
	utils::bool_t b{true};
	EXPECT_TRUE(b == true);
	EXPECT_FALSE(b != true);
	EXPECT_FALSE(b == false);
	EXPECT_TRUE(b != false);
}

TEST_F(bool_test_suite, check_compare_bool_t_false)
{
	utils::bool_t b{false};
	EXPECT_TRUE(b == utils::bool_t{false});
	EXPECT_FALSE(b != utils::bool_t{false});
	EXPECT_FALSE(b == utils::bool_t{true});
	EXPECT_TRUE(b != utils::bool_t{true});
}

TEST_F(bool_test_suite, check_compare_bool_t_true)
{
	utils::bool_t b{true};
	EXPECT_TRUE(b == utils::bool_t{true});
	EXPECT_FALSE(b != utils::bool_t{true});
	EXPECT_FALSE(b == utils::bool_t{false});
	EXPECT_TRUE(b != utils::bool_t{false});
}

