#include "event.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace zia::api
{

class NewConnectionEvent : public IEvent
{
    static constexpr const char name[] { "New connection" };
    static const EventDescriptor event;
    // TODO: mettre les trucs de asio
};

const EventDescriptor NewConnectionEvent::event { event_base<NewConnectionEvent, name> };

} // namespace zia::api