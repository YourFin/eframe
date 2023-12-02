#include "subscription.h"
#include <cstddef>
#include <ionc/ion_types.h>

namespace eframe::core {

template <typename Msg>
Subscription<Msg> Subscription<Msg>::httpConfigUpdate(
    std::function<std::optional<Msg>(std::unique_ptr<ION_READER> cfgReader)>
        handleUpdate) noexcept {
  return Subscription<Msg>{.actual = HttpCfg{.fn = handleUpdate}};
}
template <typename Msg>
Subscription<Msg> Subscription<Msg>::deepSleep::timerWakeup(
    std::function<Msg()> handleWakeup) noexcept {
  return Subscription<Msg>{.actual = TimerWakeup{.fn = handleWakeup}};
}

template <typename Msg>
Subscription<Msg> Subscription<Msg>::deepSleep::touchWakeup(
    touch_pad_t pad, int threshold, std::function<Msg()> handler) noexcept {
  return Subscription<Msg>{.actual = TouchWakeup{
                               .pad = pad,
                               .threshold = threshold,
                               .fn = handler,
                           }};
}

template <typename Msg>
Subscription<Msg>::Subscription(
    std::initializer_list<Subscription<Msg>> subs) noexcept {
  actual = subs;
}

} // namespace eframe::core
