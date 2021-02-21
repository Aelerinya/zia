#include "../event.hpp"

namespace zia::api {

class OnStartEvent: public IEvent {
    static constexpr const char name[] { "Zia is starting" };
    static const EventDescriptor event;
};

const EventDescriptor OnStartEvent::event { event_base<OnStartEvent, name> };
}
