#include "simulator.h"

#include "cairo.h"
#include "cairomm/context.h"
#include "crow.h"
#include "crow/app.h"
#include "gtkmm/application.h"
#include "gtkmm/box.h"
#include "gtkmm/button.h"
#include "gtkmm/drawingarea.h"
#include "gtkmm/enums.h"
#include "gtkmm/object.h"
#include "ionc/ion.h"
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <gtkmm.h>
#include <new>
#include <optional>

#include "application.h"

namespace simwin {
enum error { INIT_ISSUE };

namespace window::dfault {
const int width = 600;
const int height = 700;
} // namespace window::dfault

template <typename Application /** subclass of eframe::Application */>
class SimWindow : public Gtk::Window {
public:
  SimWindow();

private:
  // Member widgets
};
// TODO: https://docs.gtk.org/gtk4/class.DrawingArea.html

template <typename Application> SimWindow<Application>::SimWindow() {
  set_title("Eink simulator");
  set_default_size(window::dfault::width, window::dfault::height);

  auto mockScreen = Gtk::make_managed<Gtk::DrawingArea>();
  {
    mockScreen->set_content_width(Application::width);
    mockScreen->set_content_height(Application::height);
    mockScreen->set_draw_func(
        [](const Cairo::RefPtr<Cairo::Context>& cr, int _width, int _height) {
          cairo_t* crRawPtr = cr.get()->cobj();
          Application::cr->save();
          cr->set_source_rgb(1.0, 1.0, 1.0);
          cr->paint();

          cr->set_source_rgb(0.3, 0.3, 0.3);
          int xcenter, ycenter;
          xcenter = Application::width / 2;
          ycenter = Application::height / 2;

          cr->move_to(0, 0);
          cr->line_to(xcenter, ycenter);
          cr->line_to(0, Application::height);
          cr->line_to(

          cr->restore();
        });
  }

  auto button = Gtk::make_managed<Gtk::Button>("Hello, world!");
  button->signal_clicked().connect([]() { printf("Hello, world!\n"); });
  auto label = Gtk::make_managed<Gtk::Label>("Click me!");

  auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 10);
  hbox->append(*button);
  hbox->append(*label);
  hbox->set_margin(10);

  set_child(*hbox);
}
} // namespace simwin

int main(int argc, char** argv) {
  hWRITER writer;
  crow::SimpleApp webapp;
  CROW_ROUTE(webapp, "/")([]() { return "Hello, world"; });
  auto gtkApp = Gtk::Application::create("yf.eframe.simulator");
  auto webappFuture = webapp.port(8080).run_async();

  int exitCode =
      gtkApp->make_window_and_run<simwin::SimWindow<100, 100>>(argc, argv);
  webapp.stop();
  webappFuture.wait_for(std::chrono::milliseconds(500));
  return exitCode;
}
