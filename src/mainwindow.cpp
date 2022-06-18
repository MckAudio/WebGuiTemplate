#include "mainwindow.hpp"
#include "webview.hpp"
#include <iostream>

namespace Mck
{
    void MessageFromFrontend(int i, const std::string &args, void* userData) {
        auto win = static_cast<MainWindow *>(userData);
        win->MessageFromFrontend(i, args);
    }

    MainWindow::MainWindow() : m_webview(new Mck::WebView())
    {
        m_webview->LoadGui("www");
        set_child(*m_webview);
        set_title("MckWebkitTest");
        set_resizable(false);
        set_default_size(800, 480);

        //m_webview->RunCode("alert('Hello GTK')");

        m_webview->BindFn("SendToBackend", Mck::MessageFromFrontend, this);
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
        m_webview->RunCode("MessageFromBackend('" + msg + "');");
    }

    void MainWindow::MessageFromFrontend(int i, const std::string &args)
    {
        std::cout << "Message from Frontend: " << i << " : " << args << std::endl;
        m_webview->RunCode("console.log('[ECHO]','" + args + "');");
    }
}