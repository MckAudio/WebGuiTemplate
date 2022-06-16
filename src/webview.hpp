#pragma once
#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include <thread>
#include <string>
#include <map>

namespace httplib {
    class Server;
}

namespace Mck
{
    typedef std::function<void(int argIdx, const std::string& args, void* userData)> bindingFn;
    typedef std::pair<bindingFn*, void*> bindingCtx;

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

        bool RunCode(const std::string &js);

        bool BindFn(const std::string &name, bindingFn fn, void *arg);

    private:
        void AddScript(const std::string &js);
        void OnMessage(const std::string &msg);

        bool m_initialized;
        std::thread m_thread;
        httplib::Server* m_server;
        std::map<std::string, bindingCtx *> m_bindings;
    };
}