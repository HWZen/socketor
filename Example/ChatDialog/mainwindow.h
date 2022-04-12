#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <sock_server.h>
#include <sock_client.h>
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

    void tryToConnectToServer(const QString& host, const QString& name, int port);

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
