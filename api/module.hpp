#ifndef ZIA_MODULE_API_H_
#define ZIA_MODULE_API_H_

#include <deque>
#include <memory>
#include <string>

#include "initializer.hpp"

// From yaml-cpp
namespace YAML
{
class Node;
}

namespace zia::api
{
class IModule
{
public:
    virtual ~IModule(){};

    virtual const std::string &getName() const = 0;
    virtual void configureModule(const YAML::Node &) = 0;
};
}

namespace std
{
template class unique_ptr<zia::api::IModule>;
}

namespace zia::api
{
extern "C" std::unique_ptr<IModule> load_module(IZiaInitializer &);
}    // namespace zia::api

#endif
