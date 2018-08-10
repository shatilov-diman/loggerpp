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

#include <chrono>

using namespace charivari_ltd;

static const std::size_t messages_count = 1'000'000;

int main () {
	logger root;

	std::size_t total_size = 0;
	const auto start = std::chrono::system_clock::now();
	{
		auto counter_subscription = root >> [&total_size](const auto& tags) {
			for (const auto& tag : tags)
				total_size += loggerpp::to_string(tag.value).size();
		};

		for (std::size_t index = 0; index < messages_count; ++index)
			root.info("msg: {}", index);
	}
	const auto end = std::chrono::system_clock::now();

	const auto delta = end - start;
	const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(delta);
	const auto mills = std::chrono::duration_cast<std::chrono::milliseconds>(delta - seconds);

	auto subscription = root >> [](const auto& tags) {
		for (const auto& tag : tags)
			std::cout << loggerpp::to_string(tag.value) << '\t';
		std::cout << std::endl;
	};

	root.info("Total value size for {} messages is {}; calculated for {}.{}", messages_count, total_size, seconds.count(), mills.count());

	return 0;
}

