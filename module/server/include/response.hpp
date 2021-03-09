#ifndef ZIA_SERVER_RESPONSE_H_
#define ZIA_SERVER_RESPONSE_H_

#include <boost/asio/ip/tcp.hpp>

#include "api/http/request.hpp"
#include "api/http/response.hpp"

namespace zia::server
{
class NewHTTPResponse : public api::http::NewHTTPResponse
{
public:
    NewHTTPResponse(api::http::HTTPMethod, const api::http::HTTPResponse &,
                    boost::asio::ip::tcp::socket);

    const api::http::HTTPResponse &getResponse() const override;
    boost::asio::ip::tcp::socket &getSocket();
    api::http::HTTPMethod getRequestMethod() const;

private:
    api::http::HTTPMethod m_request_method;
    api::http::HTTPResponse m_response;
    boost::asio::ip::tcp::socket m_socket;
};
}    // namespace zia::server

#endif    // ZIA_SERVER_RESPONSE_H_
