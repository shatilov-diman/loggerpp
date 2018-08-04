
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

