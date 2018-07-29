
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

