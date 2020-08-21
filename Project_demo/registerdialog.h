#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include<QFile>
#include<QDebug>
#include<QSqlQuery>
#include<QtDebug>
#include <QString>
#include<QSqlError>
#include<QMessageBox>
#include"thread_rfid.h"
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

    void register_teble();//注册表格

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton__cancel_clicked();
    void getID(int ID);

private:
    Ui::RegisterDialog *ui;
    void setBoard();
    Thread_rfid * thread_rfid ;

    bool reg_insert(QString name ,QString RFID,QString PASS);
    bool reg_user(QString name ,QString RFID,QString PASS);

    QString RFID;

};

#endif // REGISTERDIALOG_H
