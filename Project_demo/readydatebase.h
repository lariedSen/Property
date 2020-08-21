#ifndef READYDATEBASE_H
#define READYDATEBASE_H

#include<QSqlDatabase>
#include<QSqlQuery>
#include<QtDebug>
#include<QFile>
#include <QString>
#include<QSqlError>


/**
 * @brief  打开数据库
 *
 * @param
 * @return
**/
static void createConnection(const QString& dbname)
{
    //打开文件
#ifdef Q_OS_WIN32
    QFile file("./isOpenDb.txt");
#endif


#ifdef Q_OS_LINUX
    QFile file("./txt/isOpenDb.txt");
#endif
    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"create Db fail  in "<<"void createConnection(const QString &dbname)";
    }
    else
    {
        QByteArray data= file.readAll();

        //空的表示没有打开过
        if(data.isEmpty())
        {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(dbname);
            if(!db.open())
            {
                 qDebug()<<"cannot open";
                 return ;
            }
            file.write("ready open");//表示已经打开过来了
            file.close();

        }
    }
}


/**
 * @brief  创建表格：tableName
 *
 * @param
 * @return
**/
static void isCreateDbTable(const  QString & tableName,const QString &rfid,
                       const QString& name,const QString & password)
{
    //应该通过文件去判断数据库或表是否打开了

#ifdef Q_OS_WIN32
    QFile file1("./isOpenDb.txt");
#endif


#ifdef Q_OS_LINUX
    QFile file1("./txt/isOpenDb.txt");
#endif


    file1.open(QIODevice::ReadOnly);
    QByteArray data1= file1.readAll();
    if(data1.isEmpty())
     {
        qDebug()<<"db don`t open";
        file1.close();
        return ;
     }
    file1.close();


    //创建

#ifdef Q_OS_WIN32
    QFile file("table1.txt");
#endif

#ifdef Q_OS_LINUX
    QFile file("./txt/table1.txt");
#endif

    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open openDb.txt error";
        return ;
    }
     QByteArray data= file.readAll();
     if(!data.isEmpty())
      {
         file.close();
         return;
           //已经创建
      }

     QSqlQuery query;
     QString sqlTable =QString("CREATE TABLE %1("
                                      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                                      "%2 TEXT NOT NULL,"
                                      "%3 TEXT NOT NULL,"
                                      "%4 TEXT NOT NULL)")
                    .arg(tableName)
                    .arg(rfid)
                    .arg(name)
                    .arg(password);

     if(!query.exec(sqlTable))
      {
            qDebug()<<"create table failed"<<query.lastError();
            file.close();
                return ;
      }
      else//创建成功
      {
         QString file_word;
         file_word= QString("create %1 success").arg(tableName);
         file.write(file_word.toUtf8());
         file.close();//不管是不是空的，都要关闭
      }

}


/**
 * @brief  创建登陆表格
 *
 * @param
 * @return
**/
static void CreateTableLogin()
{
    //应该通过文件去判断数据库或表是否打开了
#ifdef Q_OS_WIN32
    QFile file1("./isOpenDb.txt");
#endif

#ifdef Q_OS_LINUX
    QFile file1("./txt/isOpenDb.txt");
#endif

    file1.open(QIODevice::ReadOnly);
    QByteArray data1= file1.readAll();
    if(data1.isEmpty())
     {
        qDebug()<<"db don`t open";
        file1.close();
        return ;
     }
    file1.close();


    //创建

#ifdef Q_OS_WIN32
    QFile file("tableLogin.txt");
#endif

#ifdef Q_OS_LINUX
    QFile file("./txt/tableLogin.txt");
#endif

    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open openDb.txt error";
        return ;
    }
     QByteArray data= file.readAll();
     if(!data.isEmpty())
      {
         file.close();
         return;
           //已经创建
      }

     QSqlQuery query;
     QString sqlTable =QString("CREATE TABLE userLogin("
                                      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                                      "RFID TEXT NOT NULL,"
                                      "NAME TEXT NOT NULL,"
                                      "TIME TEXT NOT NULL)");

     if(!query.exec(sqlTable))
      {
            qDebug()<<"create table failed"<<query.lastError();
            file.close();
                return ;
      }
      else//创建成功
      {
         QString file_word;
         file_word= QString("create userLogin success");
         file.write(file_word.toUtf8());
         file.close();//不管是不是空的，都要关闭
      }

}







#endif // READYDATEBASE_H
