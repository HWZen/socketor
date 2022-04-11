//
// Created by HWZ on 2022/4/12.
//
#include "mainwindow.h"
#include "welcome.h"
#include <QApplication>

int main(int argc, char **argv){
    QApplication a(argc,argv);
    MainWindow m;
    Welcome wel;
    m.show();
    wel.show();
    return a.exec();
}
