#ifndef PERPROTY_H
#define PERPROTY_H

#include <QWidget>

#include <QTcpSocket>
#include <QFile>

#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>

namespace Ui {
class Perproty;
}

class Perproty : public QWidget
{
    Q_OBJECT

public:
    explicit Perproty(QWidget *parent = nullptr);
    ~Perproty();

    void setHead();
    void sendfile();
    void setBroad();
    void insertTablePerproty(int id ,QString name,QString perproty);

private slots:
    void on_pushButton_property_clicked();
    void on_pushButton_property_return_clicked();


private:
    Ui::Perproty *ui;

    QTcpSocket mscoket;
    QFile file ;
//    QString fileName;
//    quint64 filesize;
//    quint64 sendsize;
};

#endif // PERPROTY_H
