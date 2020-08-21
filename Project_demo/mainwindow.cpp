#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QtDebug>
#include"login.h"
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //开始计时
    num =0;
    mp4_timer = new QTimer();
    connect(mp4_timer,&QTimer::timeout,this,&MainWindow::time_slots);
    mp4_timer->start(1000);
    player_mp4();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::player_mp4()
{

    QString filename ;

//win系统
#ifdef Q_OS_WIN32
        //获取文件明
        QDir dir("../Project_demo/pro","*.mp4");
        QFileInfoList list = dir.entryInfoList();
        for(int i=0 ;i<list.count();i++)
        {
            if(list.at(i).fileName() == "need.mp4")
            {
                filename= list.at(i).filePath();
                break;
            }
        }
        qDebug()<<list.count();
        if(filename.isEmpty())
        {
            QMessageBox::warning(this,tr("提示"),tr("播放失败"),QMessageBox::Close);
            return ;
        }
        //播放
        QStringList argments ;
        argments<<"-slave"<<"-quiet"<<"-zoom"<<"-wid"
               <<QString::number(ui->label->winId())<<filename;
        mp4_process.start("C:/mplayer/mplayer.exe",argments);//进程开启
        if(mp4_process.state() == QProcess::Running)
        {
            qDebug()<<" mp4_process runing";
        }
#endif

    //linux 系统

    #ifdef Q_OS_LINUX

        QDir dir("/project/mp4_frist","*.mp4");
        QFileInfoList list = dir.entryInfoList();
        for(int i=0 ;i<list.count();i++)
        {
            if(list.at(i).fileName() == "need.mp4")
            {
                filename= list.at(i).filePath();
                break;
            }
        }
        qDebug()<<list.count();
        if(filename.isEmpty())
        {
            QMessageBox::warning(this,tr("提示"),tr("播放失败"),QMessageBox::Close);
            return ;
        }

        QString cmd = QString("mplayer -slave -quiet "
                              "-geometry %1:%2 -zoom -x %3 -y %4 "
                              "%5")
                            .arg(ui->label->x())
                            .arg(ui->label->y())
                                .arg(ui->label->width())
                                .arg(ui->label->height())
                                .arg(filename) ;

        qDebug() << "cmd = " << cmd;
        mp4_process.start(cmd);
        if(mp4_process.state() == QProcess::Running)
        {
            qDebug()<<"8************"<<"runing";
        }


    #endif
}

void MainWindow::time_slots()
{
    ui->label_calu->setText(QString("%1%2").arg(10-num).arg("秒后进入"));
    num++;
    if(num == 5)
    {
        mp4_process.kill();
    }
    if(num > 7)
    {
        //kill播放进程
        mp4_timer->stop();
        ui->label->clear();
        Login * l = new Login();
        l->show();
        this->close();
    }
}


