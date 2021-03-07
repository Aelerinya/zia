#ifndef ZIA_SERVER_REQUEST_H_
#define ZIA_SERVER_REQUEST_H_

#include <boost/asio/ip/tcp.hpp>

#include "api/http/request.hpp"
#include "api/http/response.hpp"

namespace zia::server
{
class NewHTTPRequest : public api::http::NewHTTPRequest
{
public:
    NewHTTPRequest(const api::http::HTTPRequest &, boost::asio::ip::tcp::socket socket);

    const api::http::HTTPRequest &getRequest() override;
    std::unique_ptr<IEvent>
    createResponse(const api::http::HTTPResponse &response) override;

private:
    api::http::HTTPRequest m_request;
    boost::asio::ip::tcp::socket m_socket;
};
}    // namespace zia::server

#endif    // ZIA_SERVER_REQUEST_H_
