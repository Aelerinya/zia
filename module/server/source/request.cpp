
#include "request.hpp"
#include "response.hpp"

using namespace zia::server;

NewHTTPRequest::NewHTTPRequest(const api::http::HTTPRequest &request,
                               boost::asio::ip::tcp::socket socket)
    : m_request(request), m_socket(std::move(socket))
{
}

const zia::api::http::HTTPRequest &NewHTTPRequest::getRequest()
{
    return m_request;
}

std::unique_ptr<zia::api::IEvent>
NewHTTPRequest::createResponse(const api::http::HTTPResponse &response)
{
    return std::make_unique<NewHTTPResponse>(response, std::move(m_socket));
}
