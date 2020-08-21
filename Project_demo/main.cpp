#include "mainwindow.h"
#include<QFile>
#include <QApplication>
#include"login.h"
#include"loginpass.h"
#include "mainfrist.h"
#include"readydatebase.h"
#include"keyBoard.h"
#include"perproty.h"
void readyDataBase()
{

#ifdef Q_OS_WIN32
    QFile file("./isOpenDb.txt");
#endif


#ifdef Q_OS_LINUX
    QFile file("./txt/isOpenDb.txt");
#endif


   if(!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
   {
       qDebug()<<"open isOpenDb.txt error";
       return ;
   }
   QByteArray data = file.readAll();
    if(data.isEmpty())
    {
        file.close();
        createConnection("people.db");
    }
    else    file.close();

}


//摄像头
void a1()
{
    QFile file("./txt/opdev.txt");
    if(!file.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        return ;
    }
    QByteArray data =  file.readAll();
    if(!data.isEmpty())
    {
        qDebug()<<QString::fromUtf8(data);
        return ;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    readyDataBase();

    a1() ;
    keyBoard k;
    k.hide();


    //设置皮肤样式
    QFile* qssFile = new QFile(":/qss/pifu.qss");
    qssFile->open(QFile::ReadOnly);
    qApp->setStyleSheet(QString::fromLatin1(qssFile->readAll())); // 为QApplication设置样式表
    qssFile->close();

    MainWindow m;
    m.show();
    return a.exec();
}
