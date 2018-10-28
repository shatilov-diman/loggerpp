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

#include <utils/utils.h>

#include <string>
#include <string_view>

namespace charivari_ltd::loggerpp
{
	struct default_formatter
	{
		template <typename ... args_t>
		static std::string format(const std::string& format, args_t&& ... args)
		{
			std::basic_string_view<char> view(format.data(), format.size());
			std::string out;
			format_impl(out, view, std::string{"{}"}, std::forward<args_t>(args)...);
			return out;
		}

		template <typename ... args_t>
		static std::wstring format(const std::wstring& format, args_t&& ... args)
		{
			std::basic_string_view<wchar_t> view(format.data(), format.size());
			std::wstring out;
			format_impl(out, view, std::wstring{L"{}"}, std::forward<args_t>(args)...);
			return out;
		}

	protected:
		template <typename string_t, typename string_view_t, typename arg_t, typename ... args_t>
		static void format_impl(string_t& out, const string_view_t& format, const string_t& delim, arg_t&& arg, args_t&& ... args)
		{
			const auto next = format.find(delim);
			if (next == string_t::npos)
				out.append(format);
			else
			{
				out.append(format.substr(0, next));
				out.append(to_string_t<string_t>(arg));
				format_impl(out, format.substr(next+delim.size()), delim, std::forward<args_t>(args)...);
			}
		}

		template <typename string_t, typename string_view_t>
		static void format_impl(string_t& out, const string_view_t& format, const string_t&)
		{
			out.append(format);
		}

	protected:
		template <typename string_t, typename arg_t>
		static string_t to_string_t(arg_t&& arg)
		{
			if constexpr (std::is_same_v<std::decay_t<string_t>, std::string>)
				return utils::to_string(arg);
			else if constexpr (std::is_same_v<std::decay_t<string_t>, std::wstring>)
				return utils::to_wstring(arg);
			return {};
		}
	};
} //namespace charivari_ltd::loggerpp


