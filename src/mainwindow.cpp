#include "mainwindow.hpp"
#include "webview.hpp"

namespace Mck
{
    MainWindow::MainWindow() : m_webview(new Mck::WebView())
    {
        m_webview->LoadGui("www");
        set_child(*m_webview);
        set_title("MckWebkitTest");
        set_resizable(false);
        set_default_size(800, 480);
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
}