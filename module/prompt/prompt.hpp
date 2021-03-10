#ifndef ZIA_PROMPT_H_
#define ZIA_PROMPT_H_

#include <thread>

#include "api/module.hpp"

namespace zia::prompt
{
class Module : public api::IModule
{
public:
    const std::string &getName() const override;
    void configureModule(const YAML::Node &) override;

    void start(api::IZiaMediator &);

private:
    std::jthread m_prompt_thread;
};
}

#endif    //  ZIA_PROMPT_H_
