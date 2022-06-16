#include "webview.hpp"
#include <httplib.h>
#include <cstdio>

namespace Mck
{
    WebView::WebView()
        : Gtk::Widget{(webkit_web_view_new())}, m_initialized{false}, m_server{new httplib::Server()}
    {
    }

    WebView::~WebView()
    {
        if (m_thread.joinable())
        {
            m_server->stop();
            m_thread.join();
        }

        delete m_server;
    }

    bool WebView::LoadGui(const std::string &path)
    {
        if (m_initialized) {
            return false;
        }

        unsigned port = 7689;
        if(m_server->set_mount_point("/", path.c_str()) == false) {
            std::printf("Failed to load gui from path \"%s\"\n", path.c_str());
        }

        m_thread = std::thread([this, &port]() {
            m_server->listen("localhost", port);
        });

        std::string uri = "http://localhost:" + std::to_string(port);
        webkit_web_view_load_uri(*this, uri.c_str());

        m_initialized = true;

        return true;
    }
}