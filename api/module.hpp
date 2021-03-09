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

#ifdef WIN32
#define ZIA_EXPORT_SYMBOL extern "C" __declspec(dllexport)
#else
#define ZIA_EXPORT_SYMBOL extern "C"
#endif

namespace zia::api
{
ZIA_EXPORT_SYMBOL std::unique_ptr<IModule> load_module(IZiaInitializer &);
}    // namespace zia::api

#endif
