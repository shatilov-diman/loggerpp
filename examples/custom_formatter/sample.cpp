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

#include <loggerpp/logger.h>

#include <memory>

struct my_formatter :
	charivari_ltd::loggerpp::default_formatter
{
	template <typename ... args_t>
	static std::string format(const std::string& format, args_t&& ... args)
	{
		std::basic_string_view<char> view(format.data(), format.size());
		std::string out;
		format_impl(out, view, std::string{"69"}, std::forward<args_t>(args)...);
		return "superlogger: " + out;
	}

	template <typename ... args_t>
	static std::wstring format(const std::wstring& format, args_t&& ... args)
	{
		std::basic_string_view<wchar_t> view(format.data(), format.size());
		std::wstring out;
		format_impl(out, view, std::wstring{L"69"}, std::forward<args_t>(args)...);
		return L"superlogger: " + out;
	}
};

struct my_log_traits :
	charivari_ltd::loggerpp::default_log_traits
{
	using formatter_t = my_formatter;
};

using my_logger = charivari_ltd::loggerpp::logger_base<my_log_traits>;

int main () {
	using namespace charivari_ltd;

	my_logger custom(std::make_shared<logger::dispatcher_t>(), {});

	auto subscription = custom.get_dispatcher()->subscribe(loggerpp::default_consumer);

	custom.info("Hello, 69!", "world");

	return 0;
}

