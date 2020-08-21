#ifndef LOGINPASS_H
#define LOGINPASS_H

#include <QWidget>
#include<QFile>
#include<QDebug>
#include<QSqlQuery>
#include<QtDebug>
#include <QString>
#include<QSqlError>
#include<QMessageBox>

namespace Ui {
class LoginPass;
}

class LoginPass : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPass(QWidget *parent = nullptr);
    ~LoginPass();

    void login_insert();

private slots:
//    void on_pushButton_forget_pass_clicked();

    void on_pushButton_reg_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_return_clicked();

private:
    Ui::LoginPass *ui;

    void setBroad();
};

#endif // LOGINPASS_H
