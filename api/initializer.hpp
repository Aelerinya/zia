#include "event.hpp"

namespace zia::api
{
class IZiaInitializer
{
  public:
    virtual ~IZiaInitializer(){};

    virtual void registerConsumer(const EventDescriptor &event,
                                  EventConsumer consumer) = 0;

    virtual void registerListener(const EventDescriptor &event,
                                  EventListener listener) = 0;
};
} // namespace zia::api