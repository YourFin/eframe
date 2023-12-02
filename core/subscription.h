#ifndef SUBSCRIPTION_H_
#define SUBSCRIPTION_H_

#include <functional>
#include <initializer_list>
#include <ionc/ion_types.h>
#include <memory>
#include <optional>
#include <string>
#include <variant>

#include "./runtime.h"
#include "./touch_pad.h"

namespace eframe::core {

template <typename Msg> struct Subscription {
private:
  struct TimerWakeup {
    std::function<Msg()> fn;
  };
  struct TouchWakeup {
    touch_pad_t pad;
    int threshold;
    std::function<Msg()> fn;
  };
  struct HttpCfg {
    std::function<std::optional<Msg>(std::unique_ptr<ION_READER>)> fn;
  };
  using Actual = std::variant<std::initializer_list<Subscription<Msg>>,
                              TimerWakeup, TouchWakeup, HttpCfg>;
  Actual actual;

public:
  /**
   * Triggered when a config update gets sent over the wire
   *
   * @param cfgReader - callback that returns a Msg given the http response
   * body.
   *
   * Note: only the last subscription of this type will be executed
   */
  static Subscription<Msg> httpConfigUpdate(
      std::function<std::optional<Msg>(std::unique_ptr<ION_READER>)>
          handleUpdate) noexcept;

  struct deepSleep {
  public:
    // just a namespace
    deepSleep() = delete;
    ~deepSleep() = delete;
    deepSleep(const deepSleep&) = delete;
    deepSleep& operator=(const deepSleep&) = delete;

  public:
    /**
     * Triggered upon timer wakeup
     */
    static Subscription<Msg> timerWakeup(std::function<Msg()>) noexcept;

    /**
     * Triggered upon touch pad wakeup
     */
    static Subscription<Msg> touchWakeup(touch_pad_t pad, int threshold,
                                         std::function<Msg()>) noexcept;
  };
  friend deepSleep;
  template <typename App1, typename State1, typename Msg1>
  friend class eframe::core::runtime::Engine;

  Subscription<Msg>(std::initializer_list<Subscription<Msg>> subs) noexcept;
};
} // namespace eframe::core

#endif // SUBSCRIPTION_H_
