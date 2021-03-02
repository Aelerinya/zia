#include "TestInitializer.hpp"

using namespace zia;

void test::Initializer::registerConsumer(const api::EventDescriptor &event,
                                         api::EventConsumer consumer)
{
    this->consumers.emplace_back(event, consumer);
}

void test::Initializer::registerListener(const api::EventDescriptor &event,
                                         api::EventListener listener)
{
    this->listeners.emplace_back(event, listener);
}
