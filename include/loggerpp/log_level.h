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

#include <string>

namespace charivari_ltd
{
namespace loggerpp::log
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
} //namespace loggerpp::log

namespace utils
{
	inline std::string to_string(const loggerpp::log::level& l)
	{
		using namespace loggerpp::log;
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
	inline std::wstring to_wstring(const loggerpp::log::level& l)
	{
		using namespace loggerpp::log;
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

