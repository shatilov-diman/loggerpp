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

#include "bool_t.h"

#include <exception>
#include <codecvt>
#include <locale>
#include <string>
#include <chrono>

namespace charivari_ltd::utils
{
	namespace details
	{
		static const std::string null_str{"(null)"};
		static const std::wstring null_wstr{L"(null)"};
	}//namespace details

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
		return details::null_str;
	}
	inline std::string to_string(const bool& val)
	{
		return val ? "true" : "false";
	}
	inline std::string to_string(const bool_t& val)
	{
		return to_string(static_cast<bool>(val));
	}
	inline std::string to_string(const char* arg)
	{
		if (arg == nullptr)
			return details::null_str;
		return arg;
	}
	inline const std::string& to_string(const std::string& arg)
	{
		return arg;
	}
	inline std::string to_string(const std::wstring& arg)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(arg);
	}
	inline std::string to_string(const wchar_t* arg)
	{
		if (arg == nullptr)
			return details::null_str;
		return to_string(std::wstring(arg));
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
		return details::null_wstr;
	}
	inline std::wstring to_wstring(const bool& val)
	{
		return val ? L"true" : L"false";
	}
	inline std::wstring to_wstring(const bool_t& val)
	{
		return to_wstring(static_cast<bool>(val));
	}
	inline std::wstring to_wstring(const wchar_t* arg)
	{
		if (arg == nullptr)
			return details::null_wstr;
		return arg;
	}
	inline const std::wstring& to_wstring(const std::wstring& arg)
	{
		return arg;
	}
	inline std::wstring to_wstring(const std::string& arg)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(arg);
	}
	inline std::wstring to_wstring(const char* arg)
	{
		if (arg == nullptr)
			return details::null_wstr;
		return to_wstring(std::string(arg));
	}
} //namespace charivari_ltd::utils

