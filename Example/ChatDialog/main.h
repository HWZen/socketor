//
// Created by HWZ on 2022/4/12.
//

#ifndef CHATDIALOG_MAIN_H
#define CHATDIALOG_MAIN_H

#include <QObject>
#include "mainwindow.h"
#include "welcome.h"

// MainºÃ≥–QObject£¨“‘±„Ω®¡¢connect
class Main : public QObject
{
Q_OBJECT
public:
    explicit Main(QObject *parent = nullptr): QObject(parent){}

    void run();

    ~Main() override;

private:
    MainWindow *m_mainWindow;
    Welcome *m_welcome;
};

#endif //CHATDIALOG_MAIN_H
