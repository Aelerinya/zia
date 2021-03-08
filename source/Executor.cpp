#include <chrono>
#include <iostream>
#include <thread>

#include "Executor.hpp"
#include "api/internal/onstart.hpp"
#include "config.hpp"

using namespace zia;

Executor::Executor(ModuleHub &hub): m_module_hub(hub), m_event_queue()
{
}

void Executor::start()
{
    m_event_queue.push(std::make_unique<zia::api::OnStartEvent>());
    this->loop();
}

void Executor::loop()
{
    while (true) {
        if (!m_event_queue.empty()) {
            this->step();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            // std::this_thread::yield();
        }
    }
}

void Executor::step()
{
    if (m_event_queue.empty())
        return;
    std::unique_ptr<api::IEvent> event;
    m_event_queue.front().swap(event);
    m_event_queue.pop();
    this->handleEvent(std::move(event));
}

void Executor::reloadConfig()
{
    std::clog << "Reloading config" << std::endl;

    std::clog << "Dropping " << m_event_queue.size() << " events" << std::endl;
    m_event_queue = {};

    std::clog << "Calling config loader" << std::endl;
    // TODO: Call the config loader
}

void Executor::handleEvent(std::unique_ptr<api::IEvent> event)
{
    if (event->getDescriptor() == api::event_descriptor<ReloadConfig>) {
        this->reloadConfig();
        return;
    }

    Mediator mediator(m_event_queue);
    auto descriptor = event->getDescriptor();
    auto listeners = m_module_hub.getEventListeners(descriptor);
    for (auto listener: listeners) {
        listener(mediator, *event.get());
    }
    auto consumer = m_module_hub.getEventConsumer(descriptor);
    if (consumer.has_value())
        consumer.value()(mediator, std::move(event));
    else
        std::clog << "No consumer found for event " << descriptor.name << std::endl;
}

const Executor::EventQueue &Executor::getEventQueue() const
{
    return m_event_queue;
}

Executor::Mediator::Mediator(EventQueue &queue): m_queue(queue)
{
}

void Executor::Mediator::emit(std::unique_ptr<api::IEvent> event)
{
    m_queue.push(std::move(event));
}
