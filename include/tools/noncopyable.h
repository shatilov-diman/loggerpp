// Project tools
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

namespace charivari_ltd::tools
{
	class noncopyable
	{
	public:
		noncopyable() = default;
		noncopyable(const noncopyable&) = delete;
		noncopyable(noncopyable&&) = default;
		noncopyable& operator=(const noncopyable&) = delete;
		noncopyable& operator=(noncopyable&&) = default;
	};

	class nonmoveable
	{
	public:
		nonmoveable() = default;
		nonmoveable(const nonmoveable&) = default;
		nonmoveable(nonmoveable&&) = delete;
		nonmoveable& operator=(const nonmoveable&) = default;
		nonmoveable& operator=(nonmoveable&&) = delete;
	};
} //namespace charivari_ltd::tools

