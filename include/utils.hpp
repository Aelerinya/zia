#ifndef ZIA_UTILS_H_
#define ZIA_UTILS_H_

#include <sstream>
#include <string>
#include <yaml-cpp/yaml.h>

namespace zia
{
template <typename T>
inline std::string toStringWithStream(T v)
{
    std::ostringstream oss;

    oss << v;
    return oss.str();
}

struct InvalidConfiguration : public std::runtime_error {
public:
    InvalidConfiguration(const std::string &module_name, const YAML::Node &config,
                         const std::string_view &error)
        : std::runtime_error(module_name + ": Invalid configuration: " +
                             std::string(error) + ": " + toStringWithStream(config))
    {
    }
};

template <typename Derived, typename Base>
std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base> &&p)
{
    if (Derived *result = dynamic_cast<Derived *>(p.get())) {
        p.release();
        return std::unique_ptr<Derived>(result);
    }
    return std::unique_ptr<Derived>(nullptr);
}
// template <typename Derived, typename Base, typename Del>
// std::unique_ptr<Derived, Del> dynamic_unique_ptr_cast(std::unique_ptr<Base, Del> &&p)
// {
//     if (Derived *result = dynamic_cast<Derived *>(p.get())) {
//         p.release();
//         return std::unique_ptr<Derived, Del>(result, std::move(p.get_deleter()));
//     }
//     return std::unique_ptr<Derived, Del>(nullptr, p.get_deleter());
// }
}    // namespace zia
#endif    // ZIA_UTILS_H_
