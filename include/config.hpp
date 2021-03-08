#ifndef ZIA_CONFIG_H_
#define ZIA_CONFIG_H_

#include "api/event.hpp"

namespace zia
{
class ReloadConfig : public api::IEvent
{
public:
    const api::EventDescriptor &getDescriptor() const final;
};

template <>
constexpr api::EventDescriptor api::event_descriptor<ReloadConfig> = {"ReloadConfig"};

inline const api::EventDescriptor &ReloadConfig::getDescriptor() const
{
    return api::event_descriptor<ReloadConfig>;
}
}    // namespace zia

#endif    // ZIA_CONFIG_H_
