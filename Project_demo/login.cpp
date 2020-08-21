#include "login.h"
#include "ui_login.h"
#include"mainwindow.h"
#include<QInputDialog>
#include<QDebug>

#include"loginpass.h"
#include"mainfrist.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setBoard();
    readyDataBase();

    rfid_thread = new Thread_rfid();
    connect(rfid_thread,&Thread_rfid::get_rfid,this,&Login::getID);
    rfid_thread->start();

    //mscoket.connectToHost("192.168.73.54",6666);
}


/**
 * @brief  创建表格
 *
 * @param
 * @return
**/
void Login::readyDataBase()
{
    //创建user 表格  
#ifdef Q_OS_WIN32
    QFile file1("table1.txt");
#endif
#ifdef Q_OS_LINUX
    QFile file1("./txt/table1.txt");
#endif

    if(!file1.open(QIODevice::ReadWrite))
    {
        qDebug()<<"create table1.txt error";
        return ;
    }
     QByteArray data1 = file1.readAll();
     if(data1.isEmpty())
     {
         file1.close();
         isCreateDbTable("USER","RFID","NAME","PASS");
     }
     else file1.close();

     //创建登陆表格

#ifdef Q_OS_WIN32
     QFile file("tableLogin.txt");
#endif
#ifdef Q_OS_LINUX
    QFile file("./txt/tableLogin.txt");
#endif

     if(!file.open(QIODevice::ReadWrite))
     {
         qDebug()<<"create tableLogin.txt error";
         return ;
     }
      QByteArray data = file.readAll();
      if(data.isEmpty())
      {
          file.close();
          CreateTableLogin();
      }
      else file.close();

}


Login::~Login()
{
    delete ui;
}


/**
 * @brief  设置背景
 *
 * @param
 * @return
**/
void Login::setBoard()
{
    //1.设置背景
    QPixmap pixmap = QPixmap(":/pic/pro/showId.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);

    //2. 退出
    ui->pushButton_exit->setIcon(QIcon(":/pic/pro/cancel.png"));
    QPalette palette2 = ui->pushButton_exit->palette();//获取pushButton的调色板
   // palette2.setColor(QPalette::Text, Qt::red);// 设置按钮文本颜色为红色
    palette2.setColor(QPalette::Background, Qt::green);// 设置按钮背景色为绿色
    ui->pushButton_exit->setPalette(palette2);// pushButton使用修改后的调色板

    //3. 刷卡
    ui->label_shua->clear();
    QPixmap pixmap1(":/pic/pro/please.jpg") ;
    ui->label_shua->setPixmap(pixmap1.scaled(ui->label_shua->size()));

    //4.
    ui->label_info->setText(tr("出入请刷卡   安全生活    防范未然"));

}




/**
 * @brief  切换到密码登陆窗口
 *
 * @param
 * @return
**/
void Login::on_pushButton_password_login_clicked()
{
    //不需要停

   rfid_thread->stop();
//#endif
   LoginPass * l = new  LoginPass ();
   l->show();
   this->close();
}


/**
 * @brief  关闭程序
 *
 * @param
 * @return
**/
void Login::on_pushButton_exit_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("系统退出验证"),tr("password"),
                                         QLineEdit::Password,"123", &ok);

   if (ok && !text.isEmpty())
   {
       QApplication *a ;
       a->exit();
   }
}


/**
 * @brief  获取ID，写入到Json
 *
 * @param
 * @return
**/
void Login::getID(int ID)
{
    QFile file("rfid.json");
    file.open(QIODevice::ReadWrite);
    QByteArray data= file.readAll();

    //
    QJsonObject big_obj;
    QJsonObject idObj;
    idObj.insert("id",QString::number(ID,16));
    idObj.insert("time",QTime::currentTime().toString("hh:mm:ss"));

    if(data.isEmpty())
    {
        //创建写入
        QJsonArray array;
        array.append(idObj);
        big_obj.insert("result",array);
    }
    else
    {
        //读取结构
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        QJsonArray get_array = obj.value("result").toArray();
        //写入
        get_array.append(idObj);
        big_obj.insert("result",get_array);
    }
    file.close();

    writeJsonFile(big_obj, "rfid.json");


    QPixmap pixmap(":/pic/pro/gufeng.jpg");
    ui->label->setPixmap(pixmap.scaled(ui->label->size()));

    login_id(ID);

}


/**
 * @brief
 * 获取数据库id，
 *  if(id == ID)
 * {
 *      写入到数据库的登陆表格，
 *      并进入一级界面
 * }
 *
 * @param
 * @return
**/
void Login::login_id(int ID)
{
    qDebug()<<"10位数or16位" <<ID;
    QSqlQuery query;
    bool ok = query.exec("select * from USER");
   if(!ok)
   {
       QMessageBox::warning(this,tr("提示"),tr("查询数据库失败"));
       return;
   }

   while (query.next())
   {
      QString RFID = query.value("RFID").toString();
      qDebug()<<RFID<<QString::number(ID,16);
      if(RFID==QString::number(ID,16))
      {
          QString name = query.value("NAME").toString();
          //录入登陆表格信息 ID,RFID,NAME,Time
          if(login_insert(name ,RFID))
          {
                #ifdef Q_OS_LINUX
                   rfid_thread->stop();
                #endif
              MainFrist * m = new MainFrist();
              m->show();
              this->close();
              return ;
          }
      }
   }

}


/**
 * @brief  插入数据到登陆表格
 *
 * @param
 * @return
**/
bool Login::login_insert(QString name ,QString RFID)
{
    //是否已经创建

#ifdef Q_OS_WIN32
    QFile file("tableLogin.txt");
#endif


#ifdef Q_OS_LINUX
    QFile file("./txt/tableLogin.txt");
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

    QString sql= QString("insert into userLogin(RFID,NAME,TIME) "
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



void Login::writeJsonFile(QJsonObject include_obj, QString pathFile)
{
    QFile filenew(pathFile);
    if (!filenew.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {
       QMessageBox::warning(this,tr("提示"),tr("清空失败"));
    }
    //写入文件
    QJsonDocument docDecu;
    docDecu.setObject(include_obj);
    QByteArray data = docDecu.toJson();
    qint64 if_d = filenew.write(data);
    filenew.close();
    if(if_d == -1)
    {
        QMessageBox::warning(this,tr("温馨提示"),tr("操作失败"),QMessageBox::Close);
    }
}

