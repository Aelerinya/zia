#include "../event.hpp"

namespace zia::api {

class OnStartEvent: public IEvent {
public:
    const EventDescriptor &getDescriptor()const final;
};

template<>
constexpr zia::api::EventDescriptor event_descriptor<OnStartEvent> = { "OnStart" };

inline const EventDescriptor &OnStartEvent::getDescriptor() const {
    return api::event_descriptor<zia::api::OnStartEvent>;
}
}

