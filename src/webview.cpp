#include "webview.hpp"
#include <httplib.h>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <JavaScriptCore/JavaScript.h>

#include <rapidjson/document.h>

namespace fs = std::filesystem;

namespace Mck
{
    WebView::WebView()
        : Gtk::Widget{(webkit_web_view_new())}, m_initialized{false}, m_server{new httplib::Server()}
    {
        WebKitUserContentManager *manager = webkit_web_view_get_user_content_manager(*this);
        g_signal_connect(manager,
                         "script-message-received::external",
                         G_CALLBACK(+[](WebKitUserContentManager *,
                                        WebKitJavascriptResult *res, gpointer arg)
                                    {
                                        auto *w = static_cast<WebView *>(arg);

                                        assert(WEBKIT_MAJOR_VERSION >= 2 && WEBKIT_MINOR_VERSION >= 22);

                                        JSCValue *value = webkit_javascript_result_get_js_value(res);
                                        char *str = jsc_value_to_string(value);

                                        w->OnMessage(str);
                                        g_free(str);
                                    }),
                         this);

        webkit_user_content_manager_register_script_message_handler(manager, "external");
        AddScript("window.external={invoke:function(s){window.webkit.messageHandlers."
                  "external.postMessage(s);}}");
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

    bool WebView::LoadGui(const std::string &path, unsigned port)
    {
        if (m_initialized)
        {
            return false;
        }

        fs::path p(path);

        if (p.is_absolute() == false) {
            p = fs::current_path();
            p.append(path);
        }

        if (m_server->set_mount_point("/", p.c_str()) == false)
        {
            std::printf("Failed to load gui from path \"%s\"\n", path.c_str());
        }

        m_thread = std::thread([this, &port]()
                               { m_server->listen("localhost", port); });

        std::string uri = "http://localhost:" + std::to_string(port);
        webkit_web_view_load_uri(*this, uri.c_str());

        m_initialized = true;

        return true;
    }

    bool WebView::RunCode(const std::string &js)
    {
        if (m_initialized == false)
        {
            return false;
        }

        webkit_web_view_run_javascript(*this, js.c_str(), NULL, NULL, NULL);

        return true;
    }

    bool WebView::BindFn(const std::string &name, bindingFn fn, void *arg)
    {
        if (m_initialized == false)
        {
            return false;
        }

        auto js = "(function() { var name = '" + name + "';" + R"(
          var RPC = window._rpc = (window._rpc || {nextSeq: 1});
          window[name] = function() {
            var seq = RPC.nextSeq++;
            var promise = new Promise(function(resolve, reject) {
              RPC[seq] = {
                resolve: resolve,
                reject: reject,
              };
            });
            window.external.invoke(JSON.stringify({
              id: seq,
              method: name,
              params: Array.prototype.slice.call(arguments),
            }));
            return promise;
          }
        })())";

        AddScript(js);

        m_bindings[name] = std::pair<bindingFn *, void *>(new bindingFn(fn), arg);

        return true;
    }

    void WebView::AddScript(const std::string &js)
    {
        WebKitUserContentManager *manager = webkit_web_view_get_user_content_manager(*this);
        webkit_user_content_manager_add_script(manager, webkit_user_script_new(js.c_str(), WEBKIT_USER_CONTENT_INJECT_TOP_FRAME, WEBKIT_USER_SCRIPT_INJECT_AT_DOCUMENT_START, NULL, NULL));
    }

    void WebView::OnMessage(const std::string &msg)
    {
        rapidjson::Document doc;
        doc.Parse(msg.c_str());

        auto method = std::string{doc["method"].GetString()};
        auto params = doc["params"].GetArray();

        if (m_bindings.contains(method))
        {
            for (size_t i = 0; i < params.Size(); i++)
            {
                (*m_bindings[method].first)(i, params[i].GetString(), m_bindings[method].second);
            }
        }
    }
}