#include "videodisplay.h"
#include "ui_videodisplay.h"
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
#include<QDateTime>
#include<QtDebug>
#include"lookpic.h"
#include<QFileInfoList>
#include<v4l2.h>
#include"mainfrist.h"

/*************************************************
Copyright:
Author:Laried
Date:2020年7月31日
Description (描述主要实现的功能):
    [1]实现视频的监控。
    [2]抓拍照片和浏览。
//#define Path_pic "/0801/pic"  若要移植，更改图片路径
**************************************************/


videodisplay::videodisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::videodisplay)
  ,bufrgb(nullptr)
{
    ui->setupUi(this);
    connect(&timer,&QTimer::timeout,this,&videodisplay::flushButff);
    timer.setInterval(20);  
    ui->startBtn->setEnabled(true);//开始
    ui->waitBtn->setEnabled(false);
    ui->Btn_cat->setEnabled(false);

//    //tcp传输摄像头数据
//    //当客户端在连接成功时，会发送一个信号：connected,
//    connect(&mscoket,&QTcpSocket::connected,
//            this,&Perproty::Perproty::sendFilehead);

//    //有数据写入就发送此信号 ： bytesWritten
//    connect(&mscoket,&QTcpSocket::bytesWritten,
//            this,&Perproty::sendFileText);


}

videodisplay::~videodisplay()
{
    delete ui;
}


/**
 * @brief  开始摄像头监控
 *
 * 不会重复操作了，只会开启定时器来开始监控
 *
 * @param
 * @return
**/
void videodisplay::on_startBtn_clicked()
{

    bool init_ok =initDev();
    if(!init_ok)
    {
        qDebug()<<"init error";
        return ;
    }

    timer.start(); //开启捕获数据
    open_ok =true;
    ui->startBtn->setEnabled(false);
    ui->Btn_cat->setEnabled(true);
    ui->waitBtn->setEnabled(open_ok);

}

void videodisplay::flushButff()
{

    read_frame(&pd); //读取数据
    if(!bufrgb)
        bufrgb = (unsigned char *)malloc(640*480*3);
    memset(bufrgb,0,640*480*3);

    convert_yuv_to_rgb_buffer((unsigned char*)pd.buffers[pd.buf.index].start,
            bufrgb,640,480);

    QImage image(bufrgb,640,480,QImage::Format_RGB888);
         ui->displaytLabel->setPixmap(QPixmap::fromImage(image));
    return_data(&pd);

}


/**
 * @brief  抓拍并保存图片
 *
 * @param
 * @return
**/
void videodisplay::on_Btn_cat_clicked()
{
    //将摄像头数据保存为图片的本质就是将bufrgb地址的数据保存为一个jpg图片
    QImage image(bufrgb,640,480,QImage::Format_RGB888);
    //使用系统的日期和时间来设置文件名
    QDateTime dt = QDateTime::currentDateTime();
    QDate d = dt.date();
    QTime t = dt.time();
    QString fileName =
            QString("%1/%2%3-%4-%5-%6.jpg")
            .arg(Path_pic)
            .arg(d.month())
            .arg(d.day())
            .arg(t.hour())
            .arg(t.minute())
            .arg(t.second());
    qDebug() << fileName;
    image.save(fileName);
}




/**
 * @brief  前往浏览窗口
 *
 * @param
 * @return
**/
void videodisplay::on_Btn_broser_clicked()
{
    LookPic* l= new LookPic();
    l->show();
}


void videodisplay::on_Btn_return_clicked()
{
    this->close();
    MainFrist * f = new  MainFrist();
    f->show();

}


/**
 * @brief  静态函数 ---
 *  -- 初始化摄像头工作。
 * 作业：避免 当前窗口close(),再show()时出现的重复操作 出现的错误
 *
 * @param
 * @return
**/
bool initDev()
{
    QFile file("./txt/opdev.txt");
    if(!file.open(QIODevice::ReadWrite))
    {
        return false;
    }
    QByteArray data =  file.readAll();

    //init new
    if(data.isEmpty())
    {
        pd.dev_name = (char*)"/dev/video7";
        int flag = init_dev(&pd);
        if(flag == -1)
        {
            qDebug()<<"no device";
           // QMessageBox::information(this,"tip","no device");
            exit(1);
        }else if (flag == -2)
        {
            qDebug()<<"device is worng";
            //QMessageBox::information(this,"tip"," device is worng");
            exit(2);
        } else if (flag == -3)
        {
            qDebug()<<"can not open device";
            //QMessageBox::information(this,"tip"," can not open device");
            exit(3);
        }
        file.write("init finished");
    }

    file.close();

    return true;

}
void videodisplay::on_waitBtn_clicked()
{
    if(open_ok)
    {
        if(timer.isActive())
        {
            timer.stop();
           open_ok = false;
        }
    }
    else
    {
        if(!timer.isActive())
        {
           timer.start();
           open_ok = true;
        }
    }
}
