// Project loggerpp
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

#pragma once

#include "utils.h"

#include <string>
#include <string_view>

namespace charivari_ltd::loggerpp::log
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

	private:
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

	private:
		template <typename string_t, typename arg_t>
		static string_t to_string_t(arg_t&& arg)
		{
			if constexpr (std::is_same_v<std::decay_t<string_t>, std::string>)
				return to_string(arg);
			else if constexpr (std::is_same_v<std::decay_t<string_t>, std::wstring>)
				return to_wstring(arg);
			return {};
		}
	};
} //namespace charivari_ltd::loggerpp


