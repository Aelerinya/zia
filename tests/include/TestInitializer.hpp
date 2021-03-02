#ifndef __TESTINITIALIZER_H_
#define __TESTINITIALIZER_H_

#include "api/initializer.hpp"

namespace zia::test
{
// HACK: User must clear() the vectors before destroying modules

// FIXME: handle module unloading: handlers from the module must be destroyed
class Initializer : public zia::api::IZiaInitializer
{
public:
    void registerConsumer(const zia::api::EventDescriptor &event,
                          zia::api::EventConsumer consumer) override;

    void registerListener(const zia::api::EventDescriptor &event,
                          zia::api::EventListener listener) override;

    std::vector<std::pair<std::reference_wrapper<const zia::api::EventDescriptor>,
                          zia::api::EventConsumer>>
        consumers;
    std::vector<std::pair<std::reference_wrapper<const zia::api::EventDescriptor>,
                          zia::api::EventListener>>
        listeners;
};
};    // namespace zia::test

#endif    // __TESTINITIALIZER_H_
