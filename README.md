# loggerpp

It's a framework for manage log messages.

## Notes

All log messages are accumulated in one-thread log dispatcher and next each messages is forwarded to each consumer one by one.

So, do not log too much time, or copy message to another thread.

## Exceptions

By default any exception in consumer will terminate application.

You may override this behavior by pass handler of std::exception_ptr to dispatcher.

## Thread safety

You are allowed to call any methods from any threads while logger object is alive.

Even if you try to add log messages from consumer's thread. But it's not a good idea due to recurtion.

You may subscribe and and unsubscribe to dispatcher from any threads.

Also you may extend tags by call extend_logger & extend_exception from any threads.

## simple example

```cpp
#include <loggerpp/logger.h>

#include <memory>

int main () {
        using namespace charivari_ltd;

        logger simple(std::make_shared<logger::dispatcher_t>(), {});

        auto subscription = simple.get_dispatcher()->subscribe(loggerpp::default_consumer);

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
		app_logger(loggerpp::extend_logger(root, {
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
	logger root(std::make_shared<logger::dispatcher_t>(), {});

	auto subscription = root.get_dispatcher()->subscribe([](const auto& tags) {
		for (const auto& tag : tags)
			std::cout << loggerpp::to_string(tag.key) << ":" << loggerpp::to_string(tag.value) << '\t';
		std::cout << std::endl;
	});

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
		app_logger(loggerpp::extend_logger(root, {
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
			auto local = loggerpp::extend_logger(app_logger, { {"name", name} });
			local.debug("Before extend");
			{
				//this subscribtion catch only 'extend message' message
				auto subscription = local.get_dispatcher()->subscribe([name](const auto& tags) {
					std::cerr << name << ": " << loggerpp::get_message(tags) << std::endl;
				});

				loggerpp::extend_logger(local, { {"timer", std::chrono::system_clock::now()} })
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
	logger root(std::make_shared<logger::dispatcher_t>(), {});

	auto subscription = root.get_dispatcher()->subscribe([](const auto& tags) {
		for (const auto& tag : tags)
			std::cout << loggerpp::to_string(tag.value) << '\t';
		std::cout << std::endl;
	});

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
	logger root(std::make_shared<logger::dispatcher_t>(), {});

	std::size_t total_size = 0;
	const auto start = std::chrono::system_clock::now();
	{
		auto counter_subscription = root.get_dispatcher()->subscribe([&total_size](const auto& tags) {
			for (const auto& tag : tags)
				total_size += loggerpp::to_string(tag.value).size();
		});

		for (std::size_t index = 0; index < messages_count; ++index)
			root.info("msg: {}", index);
	}
	const auto end = std::chrono::system_clock::now();

	const auto delta = end - start;
	const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(delta);
	const auto mills = std::chrono::duration_cast<std::chrono::milliseconds>(delta - seconds);

	auto subscription = root.get_dispatcher()->subscribe([](const auto& tags) {
		for (const auto& tag : tags)
			std::cout << loggerpp::to_string(tag.value) << '\t';
		std::cout << std::endl;
	});

	root.info("Total value size for {} messages is {}; calculated for {}.{}", messages_count, total_size, seconds.count(), mills.count());

	return 0;
}
```

