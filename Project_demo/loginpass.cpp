#include "loginpass.h"
#include "ui_loginpass.h"
#include"registerdialog.h"
#include"mainfrist.h"
#include<QTime>
#include"login.h"
LoginPass::LoginPass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPass)
{
    ui->setupUi(this);
    this->setBroad();

}

LoginPass::~LoginPass()
{
    delete ui;
}

//录入到登陆表格
void LoginPass::login_insert()
{
    if(ui->lineEdit_name->text().isEmpty() || ui->lineEdit_pass->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请不要空输入"));
        return ;
    }

    //是否已经创建
#ifdef Q_OS_WIN32
    QFile file("tableLogin.txt");
#endif

#ifdef Q_OS_LINUX
    QFile file("./txt/tableLogin.txt");
#endif


    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open tableLogin.txt error";
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
    bool isok = query.exec("SELECT * FROM USER");
    if(!isok)
    {
        qDebug()<<"select error";
        return ;
    }
    while(query.next())
    {
        QString name = query.value("NAME").toString();
        QString pass =query.value("PASS").toString();
        QString RFID = query.value("RFID").toString();
        if(name == ui->lineEdit_name->text() && pass == ui->lineEdit_pass->text())
        {
            qDebug()<<"密码正确";
            QString sql= QString("insert into userLogin(RFID,NAME,TIME) "
                                 "values('%1','%2','%3');")
                    .arg(RFID)
                    .arg(name)
                    .arg(QTime::currentTime().toString("hh:mm:ss"));

            if(!query.exec(sql))
            {
                QMessageBox::warning(this,tr("warning"),tr("insert error"));
            }
           return ;
        }
    }

    QMessageBox::warning(this,tr("warning"),tr("enter error"));



}

//注册窗口
void LoginPass::on_pushButton_reg_clicked()
{
    RegisterDialog * r =new RegisterDialog();
    r->show();
}

//登陆
void LoginPass::on_pushButton_login_clicked()
{

    this->login_insert();
    MainFrist  * m = new MainFrist();
    m->show();
    this->close();
}

void LoginPass::setBroad()
{
    //1.设置背景
    QPixmap pixmap = QPixmap(":/pic/pro/login.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);

    //2. 设置图标
    ui->pushButton_person->setEnabled(false);
    ui->pushButton_password->setEnabled(false);
    ui->pushButton_person->setIcon(QIcon(":/pic/pro/person.png"));
    ui->pushButton_password->setIcon(QIcon(":/pic/pro/pass.png"));

    //3.
    ui->label_passTitle->setText("智能安防登陆系统");

    //
    ui->pushButton_return->setIcon(QIcon(":/pic/pro/return.png"));
    QPalette palette2 = ui->pushButton_return->palette();//获取pushButton的调色板
   // palette2.setColor(QPalette::Text, Qt::red);// 设置按钮文本颜色为红色
    palette2.setColor(QPalette::Background, Qt::green);// 设置按钮背景色为绿色
    ui->pushButton_return->setPalette(palette2);// pushButton使用修改后的调色板

}

//返回
void LoginPass::on_pushButton_return_clicked()
{

    Login * l= new Login();
    l->show();
    this->close();
}
