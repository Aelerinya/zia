#include <chrono>
#include <iostream>
#include <thread>

#include "ConfigLoader.hpp"
#include "Executor.hpp"
#include "api/internal/onstart.hpp"

using namespace zia;

void Executor::EventQueue::emit(std::unique_ptr<zia::api::IEvent> event)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if (!m_stopped)
        m_queue.push(std::move(event));
}

void Executor::EventQueue::push(std::unique_ptr<zia::api::IEvent> &&event)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_queue.push(std::move(event));
}

std::optional<std::unique_ptr<zia::api::IEvent>> Executor::EventQueue::pop()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    if (m_queue.empty()) {
        return std::nullopt;
    } else {
        std::unique_ptr<api::IEvent> event;
        m_queue.front().swap(event);
        m_queue.pop();
        return event;
    }
}

std::size_t Executor::EventQueue::size() const
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_queue.size();
}

bool Executor::EventQueue::empty() const
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_queue.empty();
}

void Executor::EventQueue::clear()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_queue = {};
}

void Executor::EventQueue::setStop(bool stop_status)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_stopped = stop_status;
}

Executor::Executor(ModuleHub &hub, ConfigLoader &config)
    : m_module_hub(hub), m_config_loader(config), m_event_queue()
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
    auto event = m_event_queue.pop();
    if (event.has_value())
        this->handleEvent(std::move(*event));
}

void Executor::reloadConfig()
{
    std::clog << "Reloading config" << std::endl;

    std::clog << "Dropping " << m_event_queue.size() << " events" << std::endl;
    m_event_queue.setStop(true);
    m_event_queue.clear();

    std::clog << "Calling config loader" << std::endl;

    m_config_loader.reloadConfig();

    m_event_queue.push(std::make_unique<zia::api::OnStartEvent>());

    // FIXME: Use RAII to stop the queue
    m_event_queue.setStop(false);
}

void Executor::handleEvent(std::unique_ptr<api::IEvent> event)
{
    if (event->getDescriptor() == api::event_descriptor<ReloadConfig>) {
        this->reloadConfig();
        return;
    }

    auto descriptor = event->getDescriptor();
    auto listeners = m_module_hub.getEventListeners(descriptor);
    for (auto listener: listeners) {
        listener(m_event_queue, *event.get());
    }
    auto consumer = m_module_hub.getEventConsumer(descriptor);
    if (consumer.has_value())
        consumer.value()(m_event_queue, std::move(event));
    else
        std::clog << "No consumer found for event " << descriptor.name << std::endl;
}

const Executor::EventQueue &Executor::getEventQueue() const
{
    return m_event_queue;
}
