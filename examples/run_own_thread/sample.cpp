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

#include <loggerpp/shared_tags_logger.h> //using special header for special logger

#include <memory>

int main () {
	using namespace charivari_ltd;

	shared_tags_logger logger; //using special logger

	std::vector <std::size_t> check;

	{
		auto subscription0 = logger >> loggerpp::run_own_thread([&check] (const auto& tags_handle) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "0: " << loggerpp::get_message(tags_handle) << std::endl;
			check.push_back(0);
		});
		logger.info("Hello, {}!", "world");
	}//waiting here while thread is finishing because subscription0 is destructing

	{
		auto subscription1 = logger >> loggerpp::run_own_thread([&check] (const auto& tags_handle) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "2: " << loggerpp::get_message(tags_handle) << std::endl;
			check.push_back(2);
		});
		auto subscription2 = logger >> loggerpp::run_own_thread() >> [&check] (const auto& tags_handle) {
			std::cout << "1: " << loggerpp::get_message(tags_handle) << std::endl;
			check.push_back(1);
		};
		logger.info("Hello, {}!", "world");
	}//waiting here while threads is finishing because subscription1,2 are destructing

	std::cout << check[0] << check[1] << check[2] << std::endl;
	//Output:
	//0 Hello, worlds!
	//1 Hello, worlds!
	//2 Hello, worlds!
	//

	return 0;
}

