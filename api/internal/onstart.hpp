#include "../event.hpp"

namespace zia::api {

class OnStartEvent: public IEvent {
    static constexpr const char name[] { "Zia is starting" };
    static const EventDescriptor event;
public:
    const EventDescriptor &getDescriptor()const override;
};

template<>
constexpr zia::api::EventDescriptor event_descriptor<OnStartEvent> = { "OnStart" };

inline const EventDescriptor &OnStartEvent::getDescriptor() const {
    return api::event_descriptor<zia::api::OnStartEvent>;
}
}

