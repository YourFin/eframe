#include "simulator.h"

#include "cairo.h"
#include "crow.h"
#include "crow/app.h"
#include "gtkmm/application.h"
#include "gtkmm/box.h"
#include "gtkmm/button.h"
#include "gtkmm/enums.h"
#include "gtkmm/object.h"
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <gtkmm.h>
#include <new>
#include <optional>

namespace simwin {
enum error { INIT_ISSUE };

namespace window::dfault {
const int width = 600;
const int height = 700;
} // namespace window::dfault

class SimWindow : public Gtk::Window {
public:
  SimWindow();

private:
  // Member widgets
};
// TODO: https://docs.gtk.org/gtk4/class.DrawingArea.html

SimWindow::SimWindow() {
  set_title("Eink simulator");
  set_default_size(window::dfault::width, window::dfault::height);

  auto button = Gtk::make_managed<Gtk::Button>("Hello, world!");
  button->signal_clicked().connect([]() { printf("Hello, world!\n"); });
  auto label = Gtk::make_managed<Gtk::Label>("Click me!");

  // auto hbox = Gtk::manage(Gtk::Box(Gtk::Orientation::VERTICAL, 10));
  auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 10);
  hbox->append(*button);
  hbox->append(*label);
  hbox->set_margin(10);

  set_child(*hbox);
}
} // namespace simwin

int main(int argc, char** argv) {
  crow::SimpleApp webapp;
  CROW_ROUTE(webapp, "/")([]() { return "Hello, world"; });
  auto gtkApp = Gtk::Application::create("yf.eframe.simulator");
  auto webappFuture = webapp.port(8080).run_async();

  int exitCode = gtkApp->make_window_and_run<simwin::SimWindow>(argc, argv);
  webapp.stop();
  webappFuture.wait_for(std::chrono::milliseconds(500));
  return exitCode;
}
