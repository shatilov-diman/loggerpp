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

#include "log_base.h"
#include "log_formatter.h"

#include "../utils/utils.h"

#include <iostream>

namespace charivari_ltd
{
namespace loggerpp
{
	struct default_log_traits
	{
		using formatter_t = default_formatter;
		using key_t = std::variant<std::string, std::wstring>;
		using value_t = std::variant<nullptr_t, bool, char, std::int64_t, std::uint64_t, double, std::string, std::wstring, level, std::chrono::system_clock::time_point>;

		struct tag_t
		{
			key_t key;
			value_t value;
		};

		using tags_t = std::deque<tag_t>;
		using tags_handle_t = tags_t;

		static inline tags_t extend_back(tags_t&& tags, tags_t&& t)
		{
			for (auto&& item : t)
				tags.push_back(std::move(item));
			return tags;
		}

		static inline tags_t extend_front(tags_t&& tags, tags_t&& t)
		{
			for (auto&& item : t)
				tags.push_front(std::move(item));
			return tags;
		}

		static inline tags_handle_t make_tags_handle(tags_t&& tags)
		{
			return std::move(tags);
		}

		static inline const tags_t& extract_tags(const tags_handle_t& tags)
		{
			return tags;
		}
	};


	template<typename tags_t>
	inline void check_guarantee_size(const tags_t& tags)
	{
		if (tags.size() < constants::index_guaratee_size)
			throw std::runtime_error("check_guarantee_size: size(tags) < constants::log_guaratee_size");
	}

	template<typename tags_t>
	inline std::chrono::system_clock::time_point get_time(const tags_t& tags)
	{
		check_guarantee_size(tags);
		return std::get<std::chrono::system_clock::time_point>(tags[constants::index_time].value);
	}

	template<typename tags_t>
	inline level get_level(const tags_t& tags)
	{
		check_guarantee_size(tags);
		return std::get<level>(tags[constants::index_level].value);
	}

	template<typename tags_t>
	inline std::string get_message(const tags_t& tags)
	{
		check_guarantee_size(tags);
		return std::get<std::string>(tags[constants::index_message].value);
	}

	template<typename tags_t>
	inline std::wstring get_wmessage(const tags_t& tags)
	{
		check_guarantee_size(tags);
		return std::get<std::wstring>(tags[constants::index_message].value);
	}

	template<typename tags_t, typename key_t, typename value_t>
	inline std::optional<value_t> get_tag(const tags_t& tags, const key_t& key)
	{
		for (const auto& t : tags)
			if (t.key == key)
				return t.value;
		return {};
	}

	template<typename type_t, typename tags_t,  typename key_t>
	inline std::optional<type_t> get_tag(const tags_t& tags, const key_t& key)
	{
		if (auto opt = get_tag(tags, key))
			return { std::get<type_t>(*opt) };
		return {};
	}

	inline std::string to_string(const default_log_traits::key_t& key)
	{
		return std::visit([] (const auto& value) {
			return utils::to_string(value);
		}, key);
	}
	inline std::wstring to_wstring(const default_log_traits::key_t& key)
	{
		return std::visit([] (const auto& value) {
			return utils::to_wstring(value);
		}, key);
	}
	inline std::string to_string(const default_log_traits::value_t& value)
	{
		return std::visit([] (const auto& value) {
			return utils::to_string(value);
		}, value);
	}
	inline std::wstring to_wstring(const default_log_traits::value_t& value)
	{
		return std::visit([] (const auto& value) {
			return utils::to_wstring(value);
		}, value);
	}

	template <typename traits_t>
	inline void base_default_consumer(const typename traits_t::tags_handle_t& tags_handle)
	{
		const auto& tags = traits_t::extract_tags(tags_handle);
		std::cout
			<< utils::to_string(get_time(tags)) << '\t'
			<< utils::to_string(get_level(tags)) << '\t'
			<< get_message(tags) << std::endl;
	}

	inline void default_consumer(const default_log_traits::tags_handle_t& tags_handle)
	{
		base_default_consumer<default_log_traits>(tags_handle);
	}
} //namespace loggerpp

	using logger = loggerpp::logger_base<loggerpp::default_log_traits>;
} //namespace charivari_ltd

