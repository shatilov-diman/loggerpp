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

#include <string>

namespace charivari_ltd
{
namespace loggerpp
{
	enum class level
	{
		unknown,
		trace,
		debug,
		info,
		warning,
		error,
		critical,
	};
} //namespace loggerpp

namespace utils
{
	inline std::string to_string(const loggerpp::level& l)
	{
		using namespace loggerpp;
		switch (l)
		{
			case level::unknown: return "unknown";
			case level::trace: return "trace";
			case level::debug: return "debug";
			case level::info: return "info";
			case level::warning: return "warning";
			case level::error: return "error";
			case level::critical: return "critical";
			default: return "undefined";
		}
	}
	inline std::wstring to_wstring(const loggerpp::level& l)
	{
		using namespace loggerpp;
		switch (l)
		{
			case level::unknown: return L"unknown";
			case level::trace: return L"trace";
			case level::debug: return L"debug";
			case level::info: return L"info";
			case level::warning: return L"warning";
			case level::error: return L"error";
			case level::critical: return L"critical";
			default: return L"undefined";
		}
	}
} //namespace utils
} //namespace charivari_ltd

