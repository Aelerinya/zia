#include "response.hpp"

using namespace zia::server;

NewHTTPResponse::NewHTTPResponse(api::http::HTTPMethod method,
                                 const api::http::HTTPResponse &response,
                                 boost::asio::ip::tcp::socket socket)
    : m_request_method(method), m_response(response), m_socket(std::move(socket))
{
}

const zia::api::http::HTTPResponse &NewHTTPResponse::getResponse() const
{
    return m_response;
}

boost::asio::ip::tcp::socket &NewHTTPResponse::getSocket()
{
    return m_socket;
}

zia::api::http::HTTPMethod NewHTTPResponse::getRequestMethod() const
{
    return m_request_method;
}
