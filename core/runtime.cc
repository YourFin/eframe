#include "runtime.h"

#include "./subscription.h"
#include <forward_list>
#include <functional>
#include <memory>
#include <utility>
#include <variant>
#include <vector>
// Boilerplate for combining lambdas
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace eframe::core::runtime {
template <typename App, typename State, typename Msg>
std::optional<std::unique_ptr<ReifiedSubscription<App, State, Msg>>>
evalSubscriptions(State* state) {
  std::unique_ptr<Subscription<Msg>> topSub = App::subscriptions(state);
  auto sub = std::forward_list<const Subscription<Msg>*>{topSub.get()};
  while (sub) {
    std::unique_ptr<ReifiedSubscription<App, State, Msg>> reifiedSub =
        new std::unique_ptr<ReifiedSubscription<App, State, Msg>>();
    std::visit(
        overloaded{
            [&](Subscription<Msg>::TimerWakeup timerWakeup) {
              reifiedSub->wakeupHandlers.push_back(timerWakeup.fn);
            },
            [&](Subscription<Msg>::TouchWakeup touchWakeup) {
              reifiedSub->subscribedTouchPads.push_back(
                  std::make_pair(touchWakeup.pad, touchWakeup.threshold));
              reifiedSub->touchWakeup.insert({touchWakeup.pad, touchWakeup.fn});
            },
            [&](Subscription<>)},
        sub.get());
  }
}
}; // namespace eframe::core::runtime
