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

#pragma once

namespace charivari_ltd::utils
{
	class bool_t
	{
	public:
		bool_t():
			value{false}
		{}

		bool_t(const bool_t&) = default;
		bool_t(bool_t&&) = default;
		constexpr bool_t(const bool v):
			value{v}
		{}

		bool_t& operator=(const bool_t&) = default;
		bool_t& operator=(bool_t&&) = default;
		bool_t& operator=(const bool& v)
		{
			value = v;
			return *this;
		}

		operator bool() const
		{
			return value;
		}

		bool operator==(const bool_t& b) const
		{
			return value == b.value;
		}
		bool operator==(const bool& b) const
		{
			return value == b;
		}
		friend bool operator==(const bool& l, const bool_t& r)
		{
			return r == l;
		}
		bool operator!=(const bool_t& b) const
		{
			return !(*this == b);
		}
		bool operator!=(const bool& b) const
		{
			return !(*this == b);
		}
		friend bool operator!=(const bool& l, const bool_t& r)
		{
			return !(r == l);
		}

	private:
		bool value;
	};

	constexpr bool_t false_t{false};
	constexpr bool_t true_t{true};
} //namespace charivari_ltd::utils

