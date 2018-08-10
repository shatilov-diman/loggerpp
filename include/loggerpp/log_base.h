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

#include "log_level.h"
#include "log_dispatcher.h"

#include "../utils/noncopyable.h"

#include <deque>
#include <chrono>
#include <string>
#include <variant>
#include <optional>
#include <type_traits>

namespace charivari_ltd::loggerpp
{
	namespace constants
	{
		static const std::size_t index_time = 0;
		static const std::size_t index_level = 1;
		static const std::size_t index_message = 2;
		static const std::size_t index_guaratee_size = 3;

		static constexpr const char key_time[] = "time";
		static constexpr const char key_level[] = "level";
		static constexpr const char key_message[] = "message";
		static constexpr const char key_exception_type[] = "exception_type";
		static constexpr const char key_exception_message[] = "exception_message";
	} //namespace constants

	template <typename _traits_t>
	class logger_base :
		public utils::noncopyable
	{
	public:
		using traits_t = _traits_t;

		using key_t = typename traits_t::key_t;
		using value_t =  typename traits_t::value_t;
		using tags_t = typename traits_t::tags_t;
		using formatter_t = typename traits_t::formatter_t;

		using dispatcher_t = dispatcher<tags_t>;
		using dispatcher_ptr = std::shared_ptr<dispatcher_t>;

	public:

		static inline tags_t extend_back(tags_t&& tags, tags_t&& t)
		{
			return traits_t::extend_back(std::move(tags), std::move(t));
		}

		static inline tags_t extend_front(tags_t&& tags, tags_t&& t)
		{
			return traits_t::extend_front(std::move(tags), std::move(t));
		}

		static inline tags_t extend_back(const tags_t& tags, tags_t&& t)
		{
			return extend_back(tags_t{tags}, std::move(t));
		}

		static inline tags_t extend_front(const tags_t& tags, tags_t&& t)
		{
			return extend_front(tags_t{tags}, std::move(t));
		}

	public:

		logger_base() :
			logger_base(std::make_shared<dispatcher_t>(), {})
		{}

		logger_base(const dispatcher_ptr& disp, tags_t&& tags) :
			disp(disp),
			tags(std::move(tags))
		{}

	public:
		dispatcher_ptr get_dispatcher() const
		{
			return disp;
		}

		const tags_t& get_tags() const
		{
			return tags;
		}

	public:
		template <typename string_t, typename ... args_t>
		void log(const level& lvl, string_t&& format, args_t&& ... args) const
		{
			auto&& message = formatter_t::format(std::move(format), std::forward<args_t>(args)...);
			disp->push(extend_front(tags, {
				{constants::key_message, message},
				{constants::key_level, lvl},
				{constants::key_time, std::chrono::system_clock::now()},
			}));
		}

		template <typename ... args_t>
		void log(const level& lvl, const char* message, args_t&& ... args) const
		{
			return log(lvl, std::string(message), std::forward<args_t>(args)...);
		}

		template <typename ... args_t>
		void log(const level& lvl, const wchar_t* message, args_t&& ... args) const
		{
			return log(lvl, std::wstring(message), std::forward<args_t>(args)...);
		}

	public:
		template <typename ... args_t>
		void unknown(args_t&& ... args) const
		{
			log(level::unknown, std::forward<args_t>(args)...);
		}

		template <typename ... args_t>
		void trace(args_t&& ... args) const
		{
			log(level::trace, std::forward<args_t>(args)...);
		}

		template <typename ... args_t>
		void debug(args_t&& ... args) const
		{
			log(level::debug, std::forward<args_t>(args)...);
		}

		template <typename ... args_t>
		void info(args_t&& ... args) const
		{
			log(level::info, std::forward<args_t>(args)...);
		}

		template <typename ... args_t>
		void warning(args_t&& ... args) const
		{
			log(level::warning, std::forward<args_t>(args)...);
		}

		template <typename ... args_t>
		void error(args_t&& ... args) const
		{
			log(level::error, std::forward<args_t>(args)...);
		}

		template <typename ... args_t>
		void critical(args_t&& ... args) const
		{
			log(level::critical, std::forward<args_t>(args)...);
		}

	private:
		dispatcher_ptr disp;
		tags_t tags;
	};

	template <typename traits_t>
	inline logger_base<traits_t> extend_logger(const logger_base<traits_t>& ref, typename traits_t::tags_t&& tags)
	{
		using logger_t = logger_base<traits_t>;
		return {ref.get_dispatcher(), logger_t::extend_back(ref.get_tags(), std::move(tags))};
	}

	template <typename traits_t, typename exception_t>
	inline logger_base<traits_t> extend_exception(const logger_base<traits_t>& ref, const exception_t& t)
	{
		return extend_logger(ref, typename traits_t::tags_t {
			{constants::key_exception_type, std::string{typeid(t).name()}},
			{constants::key_exception_message, std::string{t.what()}},
		});
	}
} //namespace charivari_ltd::loggerpp

