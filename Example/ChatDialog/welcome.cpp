#include <QMessageBox>
#include "welcome.h"
#include "ui_welcome.h"

Welcome::Welcome(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);

    connect(ui->clientModeButton, &QPushButton::clicked, this, [=](){
        ui->serverModeButton->setChecked(false);
        ui->hostEdit->setText(QString::fromLocal8Bit("服务器地址(e.g 127.0.0.1:5150 端口号默认5150)"));
        ui->connectButton->setText(QString::fromLocal8Bit("连接"));
    });

    connect(ui->serverModeButton, &QPushButton::clicked, this, [=](){
        ui->clientModeButton->setChecked(false);
        ui->hostEdit->setText(QString::fromLocal8Bit("服务器监听端口号(默认5150)"));
        ui->connectButton->setText(QString::fromLocal8Bit("创建服务器"));
    });

    connect(ui->connectButton, &QPushButton::clicked, this, [=](){
        if(ui->clientModeButton->isChecked())
        {
            auto str = ui->hostEdit->text();
            auto parm = str.split(":");
            int port{5150};
            if(parm.size() == 2)
            {
                port = parm[1].toInt();
            }
            if(port < 1024 || port > 49151)
            {
                QMessageBox::warning(this, "Warning", "Server listen port number must be between 1024 and 49151");
                return;
            }
            emit connectToServer(parm[0], ui->nameEdit->text(), port);
        }
        else if(ui->serverModeButton->isChecked())
        {
            int port{5150};
            if(ui->hostEdit->text().size() > 0)
            {
                port = ui->hostEdit->text().toInt();
                if(port < 1024 || port > 49151)
                {
                    QMessageBox::warning(this, "Warning", "Listen port number must be between 1024 and 49151");
                    return;
                }
                emit createServer(ui->nameEdit->text(), port);
            }
        }
        else
        {
            QMessageBox::warning(this, "Warning", "Please select a mode.");
        }

    });
}

Welcome::~Welcome()
{
    delete ui;
}

void Welcome::showError(const QString& error)
{
    QMessageBox::critical(this, "Error", error);
}

