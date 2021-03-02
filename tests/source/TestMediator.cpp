#include "TestMediator.hpp"

using namespace zia;

void test::Mediator::emit(std::unique_ptr<api::IEvent> event)
{
    this->events.push_back(std::move(event));
}
