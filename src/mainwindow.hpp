#pragma once

#include <gtkmm.h>

namespace Mck {
    class WebView;

    class MainWindow : public Gtk::Window
    {
        public:
            MainWindow();
            ~MainWindow();

            void SetFullscreen(bool fs = true);

        private:
            Mck::WebView *m_webview;
    };
}