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

