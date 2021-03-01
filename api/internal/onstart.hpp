#include "../event.hpp"

namespace zia::api {

class OnStartEvent: public IEvent {
    static constexpr const char name[] { "Zia is starting" };
    static const EventDescriptor event;
public:
    const EventDescriptor &getDescriptor()const override;
};

const EventDescriptor OnStartEvent::event { event_base<OnStartEvent, name> };

inline const EventDescriptor &OnStartEvent::getDescriptor() const {
    return event;
}
}
