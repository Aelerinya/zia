#ifndef __TESTDISPLAY_H_
#define __TESTDISPLAY_H_

#include <ostream>
#include <type_traits>

#include "api/http/request.hpp"

namespace std
{
std::ostream &operator<<(std::ostream &stream, zia::api::http::HTTPMethod method);
std::ostream &operator<<(std::ostream &stream,
                         const zia::api::EventDescriptor &descriptor);
}    // namespace std

#endif    // __TESTDISPLAY_H_
