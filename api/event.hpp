#ifndef ZIA_API_EVENT_H_
#define ZIA_API_EVENT_H_

#include <functional>
#include <memory>
#include <string_view>
#include <typeinfo>

namespace zia::api
{
struct EventDescriptor {
    std::string_view name;
};

class IEvent
{
public:
    virtual ~IEvent(){};

    virtual const EventDescriptor &getDescriptor() const = 0;
};

template <std::derived_from<IEvent> T>
constexpr EventDescriptor event_descriptor;

class IZiaMediator;

using EventConsumer = std::function<void(IZiaMediator &, std::unique_ptr<IEvent>)>;
using EventListener = std::function<void(const IZiaMediator &, const IEvent &)>;

}    // namespace zia::api

#endif    // ZIA_API_EVENT_H_
