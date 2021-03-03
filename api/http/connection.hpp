#ifndef ZIA_HTTP_CONNECTION_API_H_
#define ZIA_HTTP_CONNECTION_API_H_

#include "api/mediator.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <boost/asio.hpp>

namespace zia::api
{

class NewHTTPConnectionEvent : public IEvent
{
public:
    NewHTTPConnectionEvent(boost::asio::io_context &ic, boost::asio::ip::tcp::socket s):
        IEvent(), socket(std::move(s)), context(ic)
    {}
    // static inline const EventDescriptor event =
    // event_base<NewConnectionEvent, NewConnectionEvent::event_name>;
    boost::asio::ip::tcp::socket socket;
    std::reference_wrapper<boost::asio::io_context> context;
    const EventDescriptor &getDescriptor() const override;
};

inline const EventDescriptor &NewHTTPConnectionEvent::getDescriptor() const
{
    return api::event_descriptor<zia::api::NewHTTPConnectionEvent>;
}

} // namespace zia::api

#endif
