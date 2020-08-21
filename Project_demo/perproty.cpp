#include "perproty.h"
#include "ui_perproty.h"
#include<QMessageBox>
#include <QFileInfo>
#include<QSqlQuery>
#include"mainfrist.h"
#include<QDebug>
#include<QMessageBox>
#include<QDebug>
#include<QSqlError>
#include<QListWidgetItem>
#include"perprotylistwidget.h"

Perproty::Perproty(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Perproty)
{
    ui->setupUi(this);
    mscoket.connectToHost("192.168.73.54",6666);
    setHead();
    setBroad();
}

Perproty::~Perproty()
{
    delete ui;
}

void Perproty::setHead()
{
//    ui->listWidget->clear();
//    QListWidgetItem * item = new QListWidgetItem();
//    perprotyListWidget * p = new perprotyListWidget();
//    p->setData("id","名字","报修业务","审核进度","审核意见");
//    ui->listWidget->addItem(item);
//    item->setSizeHint(QSize(800,50));
//    ui->listWidget->setItemWidget(item,p);
}

//

/**
 * @brief  成员函数,把数据写入到临时文件
 *
 * @param
 * @return
**/
void Perproty::sendfile()
{
    QSqlQuery query ;
    if(!query.exec("SELECT * FROM USER"))
    {
        QMessageBox::warning(this,tr("warning"),tr("select user error"));
        return ;
    }
    while(query.next())
    {
        QString name =  query.value("NAME").toString();
        int id=  query.value("ID").toInt();
        qDebug()<<id<<name;
        if(id == ui->lineEdit_property_id->text().toInt() &&
                name == ui->lineEdit_property_name->text())
        {
            QString text = ui->lineEdit_property_1->text();
            QJsonObject include_obj ;
            include_obj.insert("service","perproty");
            include_obj.insert("id",id);
            include_obj.insert("name",name);
            include_obj.insert("property",text);
            qDebug()<<id<<name<<text;
            //写入到数据库
            insertTablePerproty(id,name,text);

            //更新到listWidget
//            QListWidgetItem * item = new QListWidgetItem();
//            perprotyListWidget * p = new perprotyListWidget();
//            p->setData(QString::number(id),name,text,"未审核","无");
//            ui->listWidget->addItem(item);
//            item->setSizeHint(QSize(800,50));
//            ui->listWidget->setItemWidget(item,p);

            //发送到服务器
            QJsonDocument doc;
            mscoket.write(QJsonDocument(include_obj).toJson());
            mscoket.flush();
            return ;
        }
    }


        QMessageBox::warning(this,"提示","输入错误");
        return ;


}

void Perproty::setBroad()
{
    //1.设置背景
    QPixmap pixmap = QPixmap(":/pic/pro/group.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);

//    //2. 退出
//    ui->pushButton_exit->setIcon(QIcon(":/pic/pro/cancel.png"));
//    QPalette palette2 = ui->pushButton_exit->palette();//获取pushButton的调色板
//   // palette2.setColor(QPalette::Text, Qt::red);// 设置按钮文本颜色为红色
//    palette2.setColor(QPalette::Background, Qt::green);// 设置按钮背景色为绿色
//    ui->pushButton_exit->setPalette(palette2);// pushButton使用修改后的调色板

        ui->pushButton_property_return->setIcon(QIcon(":/pic/pro/return.png"));
        QPalette palette2 = ui->pushButton_property_return->palette();//获取pushButton的调色板
       // palette2.setColor(QPalette::Text, Qt::red);// 设置按钮文本颜色为红色
        palette2.setColor(QPalette::Background, Qt::green);// 设置按钮背景色为绿色
        ui->pushButton_property_return->setPalette(palette2);// pushButton使用修改后的调色板

}

void Perproty::insertTablePerproty(int id, QString name, QString perproty)
{
    QSqlQuery query ;
    query.prepare("insert into Perproty(ID,NAME,PERPROTY,EXAMINE,OPINION) "
                  "values(?,?,?,?,?)");
    query.addBindValue(id);
    query.addBindValue(name);
    query.addBindValue(perproty);
    query.addBindValue(false);
    query.addBindValue("0");

    if(!query.exec())
    {
        QMessageBox::warning(this,tr("提示"),tr("物业报修写入数据库失败"));
        qDebug()<<"create table failed"<<query.lastError();
           return ;
    }

}


/**
 * @brief  发送
 *
 * @param
 * @return
**/
void Perproty::on_pushButton_property_clicked()
{
    if(mscoket.state()==QAbstractSocket::UnconnectedState)
    {
        mscoket.connectToHost("192.168.73.54",6666);
    }

    if(mscoket.state() == QTcpSocket::ConnectedState)
    {
        //发送
        sendfile();
    }
     ui->lineEdit_property_1->clear();

}



/**
 * @brief  关闭窗口，返回上一级界面
 *
 * @param
 * @return
**/
void Perproty::on_pushButton_property_return_clicked()
{
    MainFrist * m = new MainFrist();
    m->show();
    this->close();
}

