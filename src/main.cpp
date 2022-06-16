#include <gtkmm.h>
#include "mainwindow.hpp"

int main(int argc, char** argv)
{
    auto app = Gtk::Application::create("com.mckaudio.wktest");
    return app->make_window_and_run<Mck::MainWindow>(argc, argv);
}