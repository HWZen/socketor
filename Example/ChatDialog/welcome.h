#ifndef WELCOME_H
#define WELCOME_H

#include <QDialog>

namespace Ui {
class Welcome;
}

class Welcome : public QDialog
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = nullptr);
    ~Welcome() override;

    void showError(const QString &error);
private:
    Ui::Welcome *ui;
public:
signals:
    void connectToServer(const QString& host, const QString& name, int port);
    void createServer(const QString& name, int port);

};

#endif // WELCOME_H
