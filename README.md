# loggerpp

It's a framework for manage log messages

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
