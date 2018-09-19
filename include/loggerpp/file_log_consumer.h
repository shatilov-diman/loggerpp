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
#include "../utils/utils.h"

#include <fstream>

namespace charivari_ltd
{
namespace loggerpp
{
namespace details
{
	template <typename traits_t>
	class file_log_consumer
	{
	public:
		explicit file_log_consumer(const std::string& path) :
			file(path, std::ofstream::app)
		{}

	public:
		void push(const typename traits_t::tags_handle_t& tags_handle)
		{
			const auto& tags = traits_t::extract_tags(tags_handle);

			for (auto iter = traits_t::begin_guaratee_tag(tags); iter != traits_t::end_guaratee_tag(tags); ++iter)
			{
				if (iter != traits_t::begin_guaratee_tag(tags))
					file << '\t';

				file << to_string(iter->value);
			}

			for (auto iter = traits_t::begin_unguaratee_tag(tags); iter != traits_t::end_unguaratee_tag(tags); ++iter)
			{
				file << '\t' << to_string(iter->key) << '=' << to_string(iter->value);
			}

			file << std::endl;
		}

	private:
		std::ofstream file;
	};
}//namespace details

	template <typename traits_t>
	struct file_log_consumer
	{
		file_log_consumer(const std::string& path) :
			consumer(std::make_shared<details::file_log_consumer<traits_t>>(path))
		{
		}

		void operator()(const default_log_traits::tags_handle_t& tags_handle) const
		{
			consumer->push(tags_handle);
		}

	private:
		std::shared_ptr<details::file_log_consumer<traits_t>> consumer;
	};

	inline file_log_consumer<default_log_traits> build_file_log_consumer(const std::string& path)
	{
		return file_log_consumer<default_log_traits>(path);
	}
} //namespace loggerpp
} //namespace charivari_ltd

