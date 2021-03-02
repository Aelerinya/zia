#ifndef ZIA_TEST_MODULE_H_
#define ZIA_TEST_MODULE_H_

#include "api/module.hpp"

namespace zia::test
{
class Module : public api::IModule
{
public:
    Module();
    const std::string &getName() const override;
    void configureModule(const YAML::Node &) override;

    static inline const std::string module_name = "TestModule";
};

class Event : public api::IEvent
{
public:
    const api::EventDescriptor &getDescriptor() const override;
};

};    // namespace zia::test

template <>
constexpr zia::api::EventDescriptor zia::api::event_descriptor<zia::test::Event> = {
    "TestEvent"};

#endif    // ZIA_TEST_MODULE_H_
