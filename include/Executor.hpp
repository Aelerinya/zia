#ifndef ZIA_EXECUTOR_H_
#define ZIA_EXECUTOR_H_

#include <mutex>
#include <optional>
#include <queue>

#include "ModuleHub.hpp"
#include "api/event.hpp"
#include "api/mediator.hpp"

namespace zia
{
class Executor
{
public:
    class EventQueue : public api::IZiaMediator
    {
    public:
        void emit(std::unique_ptr<api::IEvent>) override;
        void push(std::unique_ptr<zia::api::IEvent> &&);
        std::optional<std::unique_ptr<zia::api::IEvent>> pop();
        std::size_t size() const;
        bool empty() const;
        void clear();
        void setStop(bool stop_status);

    private:
        std::queue<std::unique_ptr<zia::api::IEvent>> m_queue;
        bool m_stopped = false;
        mutable std::mutex m_mutex;
    };

    Executor(ModuleHub &hub);

    void start();
    void loop();

    void step();
    void handleEvent(std::unique_ptr<api::IEvent>);
    void reloadConfig();

    // Testing
    const EventQueue &getEventQueue() const;

private:
    ModuleHub &m_module_hub;
    EventQueue m_event_queue;
};

}    // namespace zia
#endif    // ZIA_EXECUTOR_H_
