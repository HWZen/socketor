#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <server.h>
#include <client.h>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    // 连接服务器
    void tryToConnectToServer(const QString& host, const QString& name, int port);

    // 创建服务器
    void tryToCreateServer(const QString& name, int port);

private:
    Ui::MainWindow *ui;

    mysock::server* server{nullptr};

public:
signals:
    void connectSuccess();

    void connectFailed(QString err);

    void stopSignal();

};

#endif // MAINWINDOW_H
