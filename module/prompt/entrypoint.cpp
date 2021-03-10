#include "api/internal/onstart.hpp"
#include "prompt.hpp"

ZIA_EXPORT_SYMBOL std::unique_ptr<zia::api::IModule>
load_module(zia::api::IZiaInitializer &initializer)
{
    auto prompt = std::make_unique<zia::prompt::Module>();

    initializer.registerListener(
        zia::api::event_descriptor<zia::api::OnStartEvent>,
        [&prompt = *prompt.get()](zia::api::IZiaMediator &mediator,
                                  const zia::api::IEvent &) { prompt.start(mediator); });
    return prompt;
}
