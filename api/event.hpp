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
using EventListener = std::function<void(IZiaMediator &, const IEvent &)>;

}    // namespace zia::api

namespace zia::api
{

constexpr bool operator==(const zia::api::EventDescriptor &lhs,
                          const zia::api::EventDescriptor &rhs)
{
    return lhs.name == rhs.name;
}

}    // namespace zia::api

namespace std
{

template <>
struct hash<std::reference_wrapper<const zia::api::EventDescriptor>> {
    std::size_t operator()(const zia::api::EventDescriptor &event) const
    {
        return std::hash<std::string_view>{}(event.name);
    }
};

}    // namespace std

#endif    // ZIA_API_EVENT_H_
