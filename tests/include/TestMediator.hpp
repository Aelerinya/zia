#ifndef __TESTMEDIATOR_H_
#define __TESTMEDIATOR_H_

#include "api/mediator.hpp"

namespace zia::test
{
// HACK: User must clear() the vectors before destroying modules

// FIXME: handle module unloading: events from the module must be destroyed
class Mediator : public api::IZiaMediator
{
public:
    void emit(std::unique_ptr<api::IEvent>) override;

    std::vector<std::unique_ptr<api::IEvent>> events;
};
};    // namespace zia::test

#endif    // __TESTMEDIATOR_H_
