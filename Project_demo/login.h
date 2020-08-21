#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<QPixmap>
#include"readydatebase.h"
#include<QThread>
#include"thread_rfid.h"

#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>


namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    void setBoard();//设置背景
    void readyDataBase();//数据库准备
    bool login_insert(QString ,QString);

private slots:
    void on_pushButton_password_login_clicked();
    void on_pushButton_exit_clicked();
    void getID(int ID);

private:
    Ui::Login *ui;
    //QTcpSocket mscoket;
    //QFile file ;

    void writeJsonFile(QJsonObject include_obj, QString pathFile);
    Thread_rfid * rfid_thread;

    void login_id(int ID);
};

#endif // LOGIN_H
