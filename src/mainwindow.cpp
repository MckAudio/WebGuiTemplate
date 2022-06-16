#include "mainwindow.hpp"
#include "webview.hpp"
#include <iostream>

namespace Mck
{
    void RecvGuiMessage(std::string id, std::string msg, void* arg) {
        std::cout << "Msg from GUI: " << id << " : " << msg << std::endl;

        auto win = static_cast<MainWindow *>(arg);

        win->SendGuiMessage("console.log('[ECHO]','" + msg + "');");
    }

    MainWindow::MainWindow() : m_webview(new Mck::WebView())
    {
        m_webview->LoadGui("www");
        set_child(*m_webview);
        set_title("MckWebkitTest");
        set_resizable(false);
        set_default_size(800, 480);

        //m_webview->RunCode("alert('Hello GTK')");

        m_webview->BindFn("SendToBackend", RecvGuiMessage, this);
    }

    MainWindow::~MainWindow() {
        delete m_webview;
    }

    void MainWindow::SetFullscreen(bool fs)
    {
        if (fs)
        {
            fullscreen();
        }
        else
        {
            unfullscreen();
        }
    }

    void MainWindow::SendGuiMessage(const std::string &msg)
    {
        m_webview->RunCode(msg);
    }
}