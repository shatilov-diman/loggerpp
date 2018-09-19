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

#include <memory>

namespace charivari_ltd
{
namespace utils
{
	template <typename arg_t>
	class optional
	{
		std::shared_ptr<arg_t> _value;

	public:
		optional() = default;
		optional(const optional& opt) = default;
		optional(optional&& opt) = default;

		template <typename type = arg_t>
		optional(type&& arg)
		{
			_value = std::make_shared<arg_t>(std::move(arg));
		}

		optional& operator=(const optional& opt) = default;

		template <typename type = arg_t>
		optional& operator=(type&& arg)
		{
			_value = std::make_shared<arg_t>(std::move(arg));
		}

		bool operator==(const optional& opt) const
		{
			return *_value == *opt._value;
		}

		template <typename type = arg_t>
		bool operator==(const type& val) const
		{
			return *_value == val;
		}

		operator bool() const
		{
			return _value != nullptr;
		}
		const arg_t& operator*() const
		{
			return *_value;
		}
		arg_t& operator*()
		{
			return *_value;
		}
		const arg_t& value() const
		{
			return **this;
		}
		arg_t& value()
		{
			return **this;
		}
	};
} //namespace utils
} //namespace charivari_ltd
