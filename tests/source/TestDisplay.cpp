#include "TestDisplay.hpp"

std::ostream &std::operator<<(std::ostream &stream, zia::api::http::HTTPMethod method)
{
    switch (method) {
        case zia::api::http::HTTPMethod::GET: stream << "GET"; break;
        case zia::api::http::HTTPMethod::HEAD: stream << "HEAD"; break;
        case zia::api::http::HTTPMethod::POST: stream << "POST"; break;
        case zia::api::http::HTTPMethod::PUT: stream << "PUT"; break;
        case zia::api::http::HTTPMethod::DELETE: stream << "DELETE"; break;
        case zia::api::http::HTTPMethod::CONNECT: stream << "CONNECT"; break;
        case zia::api::http::HTTPMethod::OPTIONS: stream << "OPTIONS"; break;
        case zia::api::http::HTTPMethod::TRACE: stream << "TRACE"; break;
        case zia::api::http::HTTPMethod::PATCH: stream << "PATCH"; break;
        default: stream << "Unknown method " << static_cast<unsigned>(method);
    }
    return stream;
}

std::ostream &std::operator<<(std::ostream &stream,
                              const zia::api::EventDescriptor &descriptor)
{
    stream << "EventDescriptor{" << descriptor.name << "}";
    return stream;
}
