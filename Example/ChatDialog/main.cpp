//
// Created by HWZ on 2022/4/12.
//

#include "main.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    Main m;

    m.run();



    return a.exec();
}

void Main::run()
{
    m_welcome = new Welcome;
    m_mainWindow = new MainWindow;
    connect(m_welcome, &Welcome::connectToServer, m_mainWindow, &MainWindow::tryToConnectToServer);
    connect(m_welcome, &Welcome::createServer, m_mainWindow, &MainWindow::tryToCreateServer);
    connect(m_mainWindow, &MainWindow::connectFailed, m_welcome, &Welcome::showError);
    connect(m_mainWindow, &MainWindow::connectSuccess, m_welcome, &Welcome::hide);
    m_welcome->show();
}

Main::~Main()
{
    delete m_welcome;
    delete m_mainWindow;
}
