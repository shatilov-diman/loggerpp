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

#include "../utils/utils.h"

#include <string>
#include <algorithm>

namespace charivari_ltd
{
namespace loggerpp
{
	struct default_formatter
	{
		template <typename ... args_t>
		static std::string format(const std::string& format, args_t&& ... args)
		{
			std::string out;
			format_impl(out, std::begin(format), std::end(format), std::string{"{}"}, std::forward<args_t>(args)...);
			return out;
		}

		template <typename ... args_t>
		static std::wstring format(const std::wstring& format, args_t&& ... args)
		{
			std::wstring out;
			format_impl(out, std::begin(format), std::end(format), std::wstring{L"{}"}, std::forward<args_t>(args)...);
			return out;
		}

	protected:
		template <typename string_t, typename iterator_t, typename arg_t, typename ... args_t>
		static void format_impl(string_t& out, const iterator_t& format_begin, const iterator_t format_end, const string_t& placeholder, arg_t&& arg, args_t&& ... args)
		{
			const auto next = std::search(format_begin, format_end, std::begin(placeholder), std::end(placeholder));
			if (next == format_end)
				out.append(format_begin, format_end);
			else
			{
				out.append(format_begin, next);
				out.append(to_string_t<string_t>(arg));
				format_impl(out, next+placeholder.size(), format_end, placeholder, std::forward<args_t>(args)...);
			}
		}

		template <typename string_t, typename iterator_t>
		static void format_impl(string_t& out, const iterator_t& format_begin, const iterator_t format_end, const string_t&)
		{
			out.append(format_begin, format_end);
		}

	protected:
		template <typename string_t, typename arg_t, typename = typename std::enable_if<std::is_same<typename std::decay<string_t>::type, std::string>::value>::type>
		static string_t to_string_t(arg_t&& arg)
		{
			return utils::to_string(arg);
		}
		template <typename string_t, typename arg_t, typename std::enable_if<std::is_same<typename std::decay<string_t>::type, std::wstring>::value>::type>
		static string_t to_string_t(arg_t&& arg)
		{
			return utils::to_wstring(arg);
		}
	};
} //namespace loggerpp
} //namespace charivari_ltd

