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

            void SendGuiMessage(const std::string &msg);

            void MessageFromFrontend(int i, const std::string &args);

        private:
            Mck::WebView *m_webview;
    };
}