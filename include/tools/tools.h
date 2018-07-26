// Project tool
// Copyright (C) 2016-2017 Dmitry Shatilov
//
// This file is a part of the project tools.
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
//   https://github.com/shatilov-diman/libraries/tools/
//   https://bitbucket.org/charivariltd/libraries/tools/
//
// Author contacts:
//   Dmitry Shatilov (e-mail: shatilov.diman@gmail.com; site: https://www.linkedin.com/in/shatilov)
//
//

#pragma once

#include <exception>
#include <future>
#include <thread>

namespace charivari_ltd::tools
{
	template <typename type_ptr>
	static type_ptr move_nonnullptr_or_die(type_ptr&& ptr)
	{
		if (ptr == nullptr)
			throw std::invalid_argument("move_nonnullptr_or_die got nullptr");
		return std::move(ptr);
	}

	template <typename arg_t>
	inline std::string to_string(const arg_t& arg)
	{
		return std::to_string(arg);
	}
	inline std::string to_string(const std::chrono::system_clock::time_point& time)
	{
		return std::to_string(time.time_since_epoch().count());
	}
	inline std::string to_string(const std::nullptr_t&)
	{
		return "(null)";
	}
	inline std::string to_string(const char* arg)
	{
		return arg;
	}
	inline const std::string& to_string(const std::string& arg)
	{
		return arg;
	}
	inline std::string to_string(const std::wstring& arg)
	{
		return "";
	}

	template <typename arg_t>
	inline std::wstring to_wstring(const arg_t& arg)
	{
		return std::to_wstring(arg);
	}
	inline std::wstring to_wstring(const std::chrono::system_clock::time_point& time)
	{
		return std::to_wstring(time.time_since_epoch().count());
	}
	inline std::wstring to_wstring(const std::nullptr_t&)
	{
		return L"(null)";
	}
	inline std::wstring to_string(const wchar_t* arg)
	{
		return arg;
	}
	inline const std::wstring& to_wstring(const std::wstring& arg)
	{
		return arg;
	}
	inline std::wstring to_wstring(const std::string& arg)
	{
		return L"";
	}
} //namespace charivari_ltd::tools

