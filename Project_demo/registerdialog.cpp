#include "registerdialog.h"
#include "ui_registerdialog.h"


RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    this->setBoard();
    //1. 创建注册表格
    register_teble();

    thread_rfid= new Thread_rfid();
    connect(thread_rfid,&Thread_rfid::get_rfid,this,&RegisterDialog::getID);

#ifdef Q_OS_LINUX
    thread_rfid->start();
#endif

 //   ui->pushButton__cancel->setEnabled(false);
    this->RFID.clear();
}


/**
 * @brief  获取数据库id
 * @param
 * @return
**/

void RegisterDialog::getID(int ID)
{
    ui->label_reg_id->clear();
    this->RFID = QString::number(ID,16);
    if(!this->RFID.isEmpty())
    ui->label_reg_id->setText(this->RFID);
}


/**
 * @brief  插入数据到注册表格,人员表格
 *
 * @param
 * @return
**/
bool RegisterDialog::reg_insert(QString name ,QString RFID,QString PASS)
{
    //是否已经创建

#ifdef Q_OS_WIN32
    QFile file("tableReg.txt");
#endif


#ifdef Q_OS_LINUX
    QFile file("./txt/tableReg.txt");
#endif

    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open tableReg.txt error";
        return false;
    }
    QByteArray data= file.readAll();
    //空的表示没创建
    if(data.isEmpty())
    {
         file.close();
         return false;//已经创建
    }
    file.close();

    QSqlQuery query;
    QString sql= QString("insert into userRegister(RFID,NAME,PASS) "
                                 "values('%1','%2','%3');")
                    .arg(RFID)
                    .arg(name)
                    .arg(QTime::currentTime().toString("hh:mm:ss"));

    if(!query.exec(sql))
     {
        QMessageBox::warning(this,tr("warning"),tr("insert userLogin error"));
           return false  ;
    }

    return  true ;
}


/**
 * @brief  把注册信息录入到USER表格
 *
 * @param
 * @return  true -- 录入成功
**/
bool RegisterDialog::reg_user(QString name, QString RFID, QString PASS)
{

#ifdef Q_OS_WIN32
      QFile file3("table1.txt");
#endif

#ifdef Q_OS_LINUX
   QFile file3("./txt/table1.txt");
#endif

    QSqlQuery query;

    if(!file3.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open tableReg.txt error";
        return false;
    }
    QString data1= file3.readAll();
    //空的表示没创建
    if(data1.isEmpty())
    {
         file3.close();
         return false;//已经创建
    }
    file3.close();

    QString sql1= QString("insert into USER(RFID,NAME,PASS) "
                         "values('%1','%2','%3');")
            .arg(RFID)
            .arg(name)
            .arg(PASS);
    if(!query.exec(sql1))
    {
        QMessageBox::warning(this,tr("warning"),tr("insert USER error"));
    }
    return true;
}




RegisterDialog::~RegisterDialog()
{
    delete ui;
}


/**
 * @brief  创建注册表格
 *
 * @param
 * @return
**/
void RegisterDialog::register_teble()
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
      QFile file("tableReg.txt");
#endif

#ifdef Q_OS_LINUX
   QFile file("./txt/tableReg.txt");
#endif

    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open tableReg.txt error";
        return ;
    }
     QByteArray data= file.readAll();
     if(!data.isEmpty())
      {
         file.close();
         return;//已经创建
      }

     QSqlQuery query;
     QString sqlTable =QString("CREATE TABLE userRegister("
                                      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                                      "RFID TEXT NOT NULL,"
                                      "NAME TEXT NOT NULL,"
                                      "PASS TEXT NOT NULL)");

     if(!query.exec(sqlTable))
      {
            qDebug()<<"create table failed"<<query.lastError();
            file.close();
                return ;
      }
      else//创建成功
      {
         QString file_word;
         file_word= QString("create userRegister success");
         file.write(file_word.toUtf8());
         file.close();//不管是不是空的，都要关闭
      }
}


/**
 * @brief  录入注册人员信息
 *
 * @param
 * @return
**/
void RegisterDialog::on_pushButton_ok_clicked()
{
    QString name = ui->line_reg_name->text();
    QString pass = ui->line_reg_pass->text();
    if(this->RFID.isEmpty())
    {
        qDebug()<<tr("未获取到ID");
        return ;
    }

    if(name.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请输入"));
        return;
    }
    ui->pushButton__cancel->setText(tr("返回"));
    ui->pushButton_ok->setText(tr("输入完毕"));
  /*  ui->pushButton_ok->setEnabled(false);
    ui->pushButton__cancel->setEnabled(true)*/;


    //是否已经创建
#ifdef Q_OS_WIN32
      QFile file("tableReg.txt");
#endif

#ifdef Q_OS_LINUX
   QFile file("./txt/tableReg.txt");
#endif

    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open tableReg.txt error";
        return ;
    }
    QByteArray data= file.readAll();
    //空的表示没创建
    if(data.isEmpty())
    {
         file.close();
         return;//已经创建
    }
    file.close();



    QSqlQuery query;
    bool ok = query.exec("select * from USER");
    if(!ok)
    {
        QMessageBox::warning(this,tr("提示"),tr("查询数据库失败"));
        return;
    }
    while (query.next())
    {

       QString rfid = query.value("RFID").toString();
       qDebug()<<rfid<<this->RFID;
       if(rfid ==this->RFID)
       {
           QMessageBox::warning(this,"waining",tr("该卡已经注册"));
           return ;
       }
    }

    //把注册信息写入到注册表格和人员信息表
    if(reg_insert(name ,RFID,pass) && reg_user(name,RFID,pass))
    {
         QMessageBox::information(this,tr("提示"),tr("注册成功"),QMessageBox::YesToAll);
    }
    else
    {
        QMessageBox::warning(this,tr("提示"),tr("写入失败"),QMessageBox::Cancel);
        ui->pushButton__cancel->setText(tr("放弃注册"));
        ui->pushButton_ok->setText(tr("输入"));
//        ui->pushButton_ok->setEnabled(true);
//        ui->pushButton__cancel->setEnabled(true);
    }

}


//设置背景
void RegisterDialog::setBoard()
{

    QPixmap pixmap = QPixmap(":/pic/pro/reg.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);

}



void RegisterDialog::on_pushButton__cancel_clicked()
{
#ifdef Q_OS_LINUX
    thread_rfid->stop();
#endif
    this->close();
}
