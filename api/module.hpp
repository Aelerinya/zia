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

extern "C" std::unique_ptr<IModule> load_module(IZiaInitializer &);
} // namespace zia::api