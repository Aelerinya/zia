#ifndef ZIA_MEDIATOR_API_H_
#define ZIA_MEDIATOR_API_H_

#include <deque>
#include <string>

#include "event.hpp"

namespace zia::api
{

class IZiaMediator
{
  public:
    virtual ~IZiaMediator(){};

    virtual void emit(const IEvent &) = 0;
    std::deque<std::string> getEventList();
};

} // namespace zia::api

#endif
