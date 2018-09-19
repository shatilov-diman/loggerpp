# loggerpp

It's a framework for manage log messages. C++17 and above

For C++11 & C++14 you may use c++11 branch

## Notes

All log messages are accumulated in one-thread log dispatcher and next each messages is forwarded to each consumer one by one.

So, do not log too much time, or copy message to another thread.

## Exceptions

By default any exception in consumer will terminate application.

You may override this behavior by pass handler of std::exception_ptr to dispatcher.

## Thread safety

You are allowed to call any methods from any threads while logger object is alive.

Even if you try to add log messages from consumer's thread. But it's not a good idea due to recurtion.

You may subscribe and unsubscribe to dispatcher from any threads.

But unsubscribe method block current thread will blocked while all previous messages are passed to all consumers.

Also you may extend tags by call extend_logger & extend_exception from any threads.

## simple example

```cpp
#include <loggerpp/logger.h>

#include <memory>

int main () {
        using namespace charivari_ltd;

        logger simple;

        auto subscription = simple >> loggerpp::default_consumer;

        simple.info("Hello, {}!", "world");

        return 0;
}
```

## extended_tags example

```cpp

#include <loggerpp/logger.h>

#include <memory>

using namespace charivari_ltd;

class Application :
	public utils::noncopyable
{
public:
	Application() = delete;

	explicit Application(const logger& root) :
		app_logger(root | logger::tags_t {
			{"entity", std::string{"Application"}},
			{"instance", reinterpret_cast<std::uint64_t>(this)}
		}))
	{
		app_logger.debug("Application constructed");
	}

	~Application()
	{
		app_logger.debug("Application destructed ");
	}

private:
	logger app_logger;
};

int main () {
	logger root;

	auto subscription = root >> [](const auto& tags) {
		for (const auto& tag : tags)
			std::cout << loggerpp::to_string(tag.key) << ":" << loggerpp::to_string(tag.value) << '\t';
		std::cout << std::endl;
	};

	Application app(root);

	return 0;
}

```

## thread_safe example

```cpp
#include <loggerpp/logger.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

using namespace charivari_ltd;

class Application :
	public utils::noncopyable
{
public:
	Application() = delete;

	explicit Application(const logger& root) :
		stop {false},
		app_logger(root | logger::tags_t {
			{"entity", std::string{"Application"}}
		})),
		thread1([this] { this->run("x1"); }),
		thread2([this] { this->run("x2"); })
	{
		app_logger.debug("Application constructed");
	}

	~Application()
	{
		stop = true;
		thread1.join();
		thread2.join();
		app_logger.debug("Application destructed ");
	}

private:
	void run(const std::string& name)
	{
		while (stop == false)
		{
			auto local = app_logger | logger::tag_t {"name", name};
			local.debug("Before extend");
			{
				//this subscribtion catch only 'extend message' message
				auto subscription = local >> [name](const auto& tags) {
					std::cerr << name << ": " << loggerpp::get_message(tags) << std::endl;
				};

				(local | logger::tag_t {"timer", std::chrono::system_clock::now()})
					.info("extend message");
			}
			local.debug("After extend");
		}
	}

private:
	std::atomic_bool stop;
	logger app_logger;
	std::thread thread1;
	std::thread thread2;
};

int main () {
	logger root;

	auto subscription = root >> [](const auto& tags) {
		for (const auto& tag : tags)
			std::cout << loggerpp::to_string(tag.value) << '\t';
		std::cout << std::endl;
	};

	Application app(root);

	std::this_thread::sleep_for(std::chrono::seconds(5));

	return 0;
}
```

## bench example

```cpp
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
```

## custom formatter example

```cpp
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

	my_logger custom;

	auto subscription = custom >> loggerpp::default_consumer;

	custom.info("Hello, 69!", "world");

	return 0;
}
```

## run_own_thread example

Logger is asynchron by default, in another words logger::info(and other) method is not block the thread.

All log consumers are executed in a separate thread.

From time to time it's required a lot of time to process some consumers(restore db connection for example)

and it blocks all the other consumers.

For solve this issue you can wrap your logs consumer with loggerpp::run_own_thread function which holds

separate thread and will copy all tags for you.

Because copying tags is a slow operation, we recomends using shared_tags_logger.

It just wrap original tags with shared_ptr.

```cpp
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
	//012

	return 0;
}
```
