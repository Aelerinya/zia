#ifndef ZIA_HTTP_CONNECTION_API_H_
#define ZIA_HTTP_CONNECTION_API_H_

#include "HTTPParser.hpp"
#include "api/mediator.hpp"
#include <boost/asio.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace zia::server
{

class NewHTTPConnectionEvent : public api::IEvent
{
public:
    NewHTTPConnectionEvent(boost::asio::io_context &ic, boost::asio::ip::tcp::socket s)
        : IEvent(), socket(std::move(s)), context(ic)
    {
    }
    // static inline const EventDescriptor event =
    // event_base<NewConnectionEvent, NewConnectionEvent::event_name>;
    boost::asio::ip::tcp::socket socket;
    std::reference_wrapper<boost::asio::io_context> context;
    const api::EventDescriptor &getDescriptor() const override;
};

inline const api::EventDescriptor &NewHTTPConnectionEvent::getDescriptor() const
{
    return api::event_descriptor<zia::server::NewHTTPConnectionEvent>;
}

class NewHTTPSConnectionEvent : public api::IEvent
{
public:
    NewHTTPSConnectionEvent(boost::asio::io_context &ic, boost::asio::ip::tcp::socket s)
        : IEvent(), socket(std::move(s)), context(ic)
    {
    }
    boost::asio::ip::tcp::socket socket;
    std::reference_wrapper<boost::asio::io_context> context;
    const api::EventDescriptor &getDescriptor() const override;
};

inline const api::EventDescriptor &NewHTTPSConnectionEvent::getDescriptor() const
{
    return api::event_descriptor<zia::server::NewHTTPConnectionEvent>;
}

class HTTPRequestParsed : public api::http::NewHTTPRequest
{
public:
    HTTPRequestParsed(api::http::HTTPRequest r)
        : api::http::NewHTTPRequest(), req(std::move(r))
    {
    }

    const api::http::HTTPRequest &getRequest()
    {
        return req;
    }

public:
    api::http::HTTPRequest req;
};

}    // namespace zia::server

#endif
