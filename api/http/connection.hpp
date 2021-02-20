#ifndef ZIA_HTTP_CONNECTION_API_H_
#define ZIA_HTTP_CONNECTION_API_H_

#include "../event.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace zia::api
{

class NewHTTPConnectionEvent : public IEvent
{
    // static inline const EventDescriptor event =
    // event_base<NewConnectionEvent, NewConnectionEvent::event_name>;
    // TODO: mettre les trucs de asio
    const EventDescriptor &getDescriptor() const override;
};

inline const EventDescriptor &NewHTTPConnectionEvent::getDescriptor() const
{
    static constexpr char event_name[] = "New connection";
    static const EventDescriptor event =
        event_base<NewHTTPConnectionEvent, event_name>;
    return event;
}

} // namespace zia::api

#endif
