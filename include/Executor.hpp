#ifndef ZIA_EXECUTOR_H_
#define ZIA_EXECUTOR_H_

#include <queue>

#include "ModuleHub.hpp"
#include "api/event.hpp"
#include "api/mediator.hpp"

namespace zia
{
class Executor
{
public:
    using EventQueue = std::queue<std::unique_ptr<zia::api::IEvent>>;

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

    class Mediator : public api::IZiaMediator
    {
    public:
        Mediator(EventQueue &queue);

        void emit(std::unique_ptr<api::IEvent>) override;

    private:
        EventQueue &m_queue;
    };
};

}    // namespace zia
#endif    // ZIA_EXECUTOR_H_
