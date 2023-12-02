#ifndef RUNTIME_H_
#define RUNTIME_H_

#include "./touch_pad.h"
#include <forward_list>
#include <functional>
#include <ionc/ion_types.h>
#include <map>
#include <memory>
#include <optional>
#include <vector>

namespace eframe::core::runtime {
template <typename App, typename State, typename Msg>
struct ReifiedSubscription;
template <typename App, typename State, typename Msg> class Engine {
  using ReifSub = ReifiedSubscription<App, State, Msg>;

  /// Subscriptions
  /// On OOM, returns empty optional
  static std::optional<std::unique_ptr<ReifSub>>
  evalSubscriptions(const State* state) noexcept;

  static const std::vector<std::pair<touch_pad_t, int>>*
  subscribedTouchPads(const ReifSub* sub);
  static const std::forward_list<Msg> evalTouched(const ReifSub* sub,
                                                  touch_pad_t pad);
};

/**
 * Reified Subscription
 */
template <typename App, typename State, typename Msg>
struct ReifiedSubscription {
  friend Engine<App, State, Msg>;

private:
  std::vector<std::pair<touch_pad_t, int>> subscribedTouchPads;
  std::multimap<touch_pad_t, std::function<Msg()>> touchPadHandlers;
  std::vector<std::function<Msg()>> wakeupHandlers;
  std::optional<std::function<std::optional<Msg>(std::unique_ptr<ION_READER>)>>
      cfgHandler;
};
} // namespace eframe::core::runtime

#endif // RUNTIME_H_
