#ifndef ZIA_EVENT_H_
#define ZIA_EVENT_H_

#include <functional>
#include <string_view>
#include <typeinfo>

namespace zia::api
{
struct EventDescriptor {
    std::string_view name;
    std::string_view id;
};
class IEvent
{
  public:
    virtual ~IEvent(){};

    virtual const EventDescriptor &getDescriptor() const = 0;
};

template <typename T, auto name = "Unknown event">
const EventDescriptor event_base{name, typeid(T).name()};

class IZiaMediator;

using EventConsumer = std::function<void(IZiaMediator &, const IEvent &)>;
using EventListener = std::function<void(const IZiaMediator &, const IEvent &)>;

} // namespace zia::api

#endif // ZIA_EVENT_H_
