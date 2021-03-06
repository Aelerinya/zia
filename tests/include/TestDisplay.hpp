#ifndef __TESTDISPLAY_H_
#define __TESTDISPLAY_H_

#include <ostream>
#include <type_traits>

#include "api/http/request.hpp"

namespace std
{
std::ostream &operator<<(std::ostream &stream, zia::api::http::HTTPMethod method);
}

#endif    // __TESTDISPLAY_H_
