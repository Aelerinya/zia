#include "event.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <boost/asio.hpp>

namespace zia::api
{

class NewConnectionEvent : public IEvent
{
    static constexpr const char name[] { "New connection" };
    static const EventDescriptor event;
    boost::asio::ip::tcp::socket socket;
    std::reference_wrapper<boost::asio::io_context> context;
    std::thread context_thread;
};

const EventDescriptor NewConnectionEvent::event { event_base<NewConnectionEvent, name> };

} // namespace zia::api
