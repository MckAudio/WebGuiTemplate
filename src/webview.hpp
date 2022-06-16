#pragma once
#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include <thread>
#include <string>

namespace httplib {
    class Server;
}

namespace Mck
{
    class WebView : public Gtk::Widget
    {
    public:
        WebView();
        virtual ~WebView();

        operator WebKitWebView *()
        {
            return WEBKIT_WEB_VIEW(gobj());
        };

        bool LoadGui(const std::string &path);

    private:
        bool m_initialized;
        std::thread m_thread;
        httplib::Server* m_server;
    };
}