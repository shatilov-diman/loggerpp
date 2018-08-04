
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

