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

#include "logger.h"

namespace charivari_ltd
{
namespace loggerpp
{
	struct shared_tags_log_traits :
		default_log_traits
	{
		using tags_handle_t = std::shared_ptr<const tags_t>;

		static inline tags_handle_t make_tags_handle(tags_t&& tags)
		{
			return std::make_shared<const tags_t>(std::move(tags));
		}

		static inline const tags_t& extract_tags(const tags_handle_t& tags)
		{
			if (tags == nullptr)
				throw std::runtime_error("extract_tags: got nullptr");
			return *tags;
		}
	};

	inline std::chrono::system_clock::time_point get_time(const shared_tags_log_traits::tags_handle_t& tags)
	{
		return get_time(shared_tags_log_traits::extract_tags(tags));
	}

	inline level get_level(const shared_tags_log_traits::tags_handle_t& tags)
	{
		return get_level(shared_tags_log_traits::extract_tags(tags));
	}

	inline std::string get_message(const shared_tags_log_traits::tags_handle_t& tags)
	{
		return get_message(shared_tags_log_traits::extract_tags(tags));
	}

	inline std::wstring get_wmessage(const shared_tags_log_traits::tags_handle_t& tags)
	{
		return get_wmessage(shared_tags_log_traits::extract_tags(tags));
	}

	template<typename key_t>
	inline auto get_tag(const shared_tags_log_traits::tags_handle_t& tags, const key_t& key)
	{
		return get_tag(shared_tags_log_traits::extract_tags(tags), key);
	}

	template<typename type_t, typename key_t>
	inline auto get_tag(const shared_tags_log_traits::tags_handle_t& tags, const key_t& key)
	{
		return get_tag<type_t>(shared_tags_log_traits::extract_tags(tags), key);
	}

	inline void shared_tags_default_consumer(const shared_tags_log_traits::tags_handle_t& tags_handle)
	{
		base_default_consumer<shared_tags_log_traits>(tags_handle);
	}
} //namespace loggerpp

	using shared_tags_logger = loggerpp::logger_base<loggerpp::shared_tags_log_traits>;
} //namespace charivari_ltd

