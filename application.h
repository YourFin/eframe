#ifndef EFRAME_APPLICATION_H_
#define EFRAME_APPLICATION_H_
#include "cairo.h"
#include "core/subscription.h"
#include <ionc/ion_types.h>
#include <memory>
#include <utility>
#include <variant>

namespace eframe {

/**
 * Msg and State MUST be dumb data carrier types.
 *
 * State gets stored in RTC Slow Memory during deep sleep,
 * and MUST be:
 *   Self contained (no pointers out to a std::string - use a buffer!)
 *   Less than 8kb
 *
 * Consider using std::variant<...> for both types.
 *
 *
 * Note: In general, the design of this interface tries to save you from
 * doing something stupid. If the runtime detects stupidity (say, entering deep
 * sleep without any wakeup condition) the "crash" mode is a hard-reset calling
 * initialState().
 */
template <typename State, typename Msg> class Application {
public:
  Application() = delete;
  ~Application() = delete;
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;

public:
  /**
   * return value: caller assumes ownership
   */
  static std::pair<std::unique_ptr<State>, std::unique_ptr<Msg>>
  initialState() noexcept = 0;
  inline static int width = 0;
  inline static int height = 0;

  /**
   * previousState: ownership of memory passes to this function
   * event: ownership of memory passes to this function
   *
   * return value: ownership passes out of this function
   */
  static std::unique_ptr<State> update(std::unique_ptr<State> previousState,
                                       std::unique_ptr<Msg> event) noexcept = 0;

  /**
   * "What events should the runtime be listening for given the current state?"
   *
   * Or, in this parlance, what subscriptions should be open given the current
   * state.
   */
  static std::unique_ptr<eframe::core::Subscription<Msg>>
  subscriptions(const State* state);

  /**
   * Render state out to a cairo surface
   *
   * Does not assume ownership of currentState, and must not mutate it.
   *
   * Surface cleaned up by caller
   */
  static void render(const State* currentState,
                     std::unique_ptr<cairo_surface_t> surface) noexcept = 0;

  /**
   * State to present to web UI
   */
  static void writeConfig(const State* state,
                          std::unique_ptr<ION_WRITER> writer) noexcept = 0;

  // Todo:
};
} // namespace eframe

#endif // EFRAME_APPLICATION_H_
