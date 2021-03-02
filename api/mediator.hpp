#ifndef ZIA_MEDIATOR_API_H_
#define ZIA_MEDIATOR_API_H_

#include <memory>
#include <string>

#include "event.hpp"

namespace zia::api
{

class IZiaMediator
{
  public:
    virtual ~IZiaMediator(){};

    virtual void emit(std::unique_ptr<IEvent>) = 0;
};

} // namespace zia::api

#endif
