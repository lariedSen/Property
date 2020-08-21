#include "mainfrist.h"
#include "ui_mainfrist.h"
#include<QtDebug>
#include<QDateTime>
#include<QMessageBox>
#include"videodisplay.h"
#include"perproty.h"

MainFrist::MainFrist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainFrist)
{
    ui->setupUi(this);

    //设置背景
    this->setBoard();

    //1. 天气Json，发起请求
    manager = new QNetworkAccessManager ();
    connect(manager,&QNetworkAccessManager::finished,this,&MainFrist::getWeather);
    QUrl  url("http://apis.juhe.cn/simpleWeather/query?"
              "city=%E5%B9%BF%E5%B7%9E&key=813b9882ce5bc87170516a3d7e11026b");
    manager->get(QNetworkRequest(url));


    //2.词条滚动,天气湿度
    timer_humidity  = new QTimer();
    connect(timer_humidity,&QTimer::timeout,this,&MainFrist::word_lopp);
    num_wird =0;
   // timer_humidity->start(2000);

    //词条
    list_word<<"逸品庄园，尚品生活"<<"自然逸天下，尊贵达典雅"
            <<"聚就和谐自然，凝蕴水城深情" <<"体验欧洲风情，尽在逸墅庄园"
            <<"人生是海，逸墅是岸"<<"时尚，尊贵，自然——我永恒的承诺"
            <<"您的安全我们的责任，我们的质量您的选择"
            <<"抗击疫情，人人有责"<<"戴口罩勤洗手少出门，少聚集"
            <<"生命重于泰山，疫情就是命令，防控就是责任";

    list_humidity<<"太阳的怒火，绽放，花儿做不到啊！"<<"用水浇灌给花浇水∶使缓和,使冷静些"
                   <<"给我生命之泉，予你一庭芬芳";


    //3. 播放图片
    thread_pic= new Threadpic();
    connect(thread_pic,&Threadpic::show,this,&MainFrist::pic_show);
    thread_pic->start();
    num_temperture =0;

    //4.播放视频
    //视频播放完成，发射finished()信号，
    connect(&mp4_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(player_auto()));
    mp4_num =0;
    mp4_list.clear();
    player_ready();//读取文件名并开始播放
    bool_mplayer = true ;

    //5.照明
    #ifdef Q_OS_LINUX
    fd = ::open("/dev/gz13_led_drv",O_WRONLY);
    if(fd == -1)
    {
         QMessageBox::warning(this,"warning","open led error");
    }
   #endif

}



/**
 * @brief 循环播放
 *
 * @param
 * @return
**/
void MainFrist::player_auto()
{

        if(mp4_num >= mp4_list.size())
        {
            mp4_num =0;
        }
        if(bool_mplayer)
        {
            player_advert();

        }
        else
        {
            qDebug()<<"关闭进程";
        }
}

/**
 * @brief  播放视频
 *
 * @param
 * @return
**/
void MainFrist::player_advert()
{

  if(mp4_num < mp4_list.size())
  {

     //win系统
    #ifdef Q_OS_WIN32
        QStringList argnments;
        argnments<<"-slave"<<"-quiet"<<"-zoom"<<"-wid"
                 << QString::number(ui->label_mp4->winId())
                 <<mp4_list[mp4_num];
        mp4_process.start("C:/mplayer/mplayer.exe",argnments);
     #endif

    //linux 系统
    #ifdef Q_OS_LINUX
         QString cmd = QString("mplayer -slave -quiet "
                                "-geometry %1:%2 -zoom -x %3 -y %4 "
                                        "%5")
                                       .arg(ui->label_mp4->x())
                                       .arg(ui->label_mp4->y())
                                           .arg(ui->label_mp4->width())
                                           .arg(ui->label_mp4->height())
                                           .arg(mp4_list[mp4_num]) ;

                   qDebug() << "cmd = " << cmd;
                   mp4_process.start(cmd);
     #endif

  }
   mp4_num++;
}

/**
 * @brief  播放准备，读取视频文件名
 *
 * @param
 * @return
**/
void MainFrist::player_ready()
{

    //1. 读取视频文件名
#ifdef Q_OS_WIN32
    QDir dir("G:/QTmake/0727/Project_demo/mp4", "*.mp4");
#endif

#ifdef Q_OS_LINUX
   QDir dir("/project/pro_mp4", "*.mp4");
    //dir.setFilter(QDir::Dirs|QDir::Files);//除了目录或文件，其他的过滤掉
   dir.setFilter(QDir::Files);//除了目录或文件，其他的过滤掉
#endif
   QFileInfoList list = dir.entryInfoList();
   for(int i=0 ;i<list.size();i++)
   {
       mp4_list<<list.at(i).filePath();
       qDebug()<<list.at(i).filePath();
   }

   player_advert();
}


MainFrist::~MainFrist()
{
    delete ui;
}




/**
 * @brief  根据天气设置背景图
 *
 * @param
 * @return
**/
void MainFrist::weather_broad(QString weather)
{

}



/**
 * @brief  湿度+ 广告词
 *
 * @param
 * @return
**/
void MainFrist::word_lopp()
{

    //显示浇水
    if(num_humidity<list_humidity.size())
    {
        ui->label_gun_advert->setText(list_humidity.at(num_humidity));
        num_humidity++;
    }

    //显示广告词
    else
    {
        if(num_wird<list_word.size())
        {
            ui->label_gun_advert->setText(list_word.at(num_wird));
        }
        num_wird++;
        if(num_wird>=list_word.size())
        {
            num_wird =0;
        }
    }


}




/**
 * @brief  图片显示
 * 如果低温就显示低温的图片
 * @param
 * @return
**/
void MainFrist::pic_show(QString pic_path)
{

    //空
    QPixmap pixmap ;
    if(pic_path.isEmpty())
    {
        pixmap.load(":/pic/pro/4.jpg") ;
        ui->label_pic->setPixmap(pixmap.scaled(ui->label_pic->size()));
        return ;
    }

    //显示降温图片
    if(this->temperature.left(2).toInt()<15)
    {

        #ifdef Q_OS_WIN32
            QDir dir("G:/QTmake/0727/Project_demo/cold_wea/", "*.jpg");
        #endif

        #ifdef Q_OS_LINUX
           QDir dir("/project/wea_mp4", "*.jpg");
            //dir.setFilter(QDir::Dirs|QDir::Files);//除了目录或文件，其他的过滤掉
           dir.setFilter(QDir::Files);//除了目录或文件，其他的过滤掉
        #endif
           QFileInfoList list = dir.entryInfoList();
           if(num_temperture<list.size())
           {
               pixmap.load(list.at(num_temperture).filePath());
               ui->label_pic->setPixmap(pixmap.scaled(ui->label_pic->size()));
              return ;
           }

    }

    //显示广告图片
   pixmap.load(pic_path);
   ui->label_pic->setPixmap(pixmap.scaled(ui->label_pic->size()));
}




/**
 * @brief 天气获取和设置
 *
 * @param
 * @return
**/
void MainFrist::getWeather(QNetworkReply *reply)
{
    //1.获取Json数据
    QByteArray array_read  = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(array_read);
    QJsonObject obj = doc.object();

    //城市
    QJsonObject result_obj = obj.value("result").toObject();
    QString city = result_obj.value("city").toString();

    //humidity
    QJsonObject realtime_obj = result_obj.value("realtime").toObject();
    humidity =realtime_obj.value("humidity").toString();

    //
    QJsonArray futrue_array = result_obj.value("future").toArray();
    QJsonObject futrue_frist_obj = futrue_array.at(0).toObject();
    QString date = futrue_frist_obj.value("date").toString();
    temperature = futrue_frist_obj.value("temperature").toString();
    QString weather = futrue_frist_obj.value("weather").toString();

    //
    ui->label_city->setText(city);
    ui->label_date->setText(date);
    ui->label_time->setText(QDateTime::currentDateTime().time().toString("hh:mm:ss"));
    ui->label_weather->setText(weather);
    ui->label_hemidity->setText(QString("湿度：%1").arg(humidity));
    ui->label_temperature->setText(temperature);

    //2.设置天气图标
    QPixmap pixmap;
    if(weather.contains("雪"),Qt::CaseInsensitive)
    {
        pixmap.load(":/weather/xue.png");
    }
    else if(weather.contains(("雨"),Qt::CaseInsensitive))
    {
        pixmap.load(":/weather/rain.png");
    }
    else if(weather.contains(("云"),Qt::CaseInsensitive)){
        pixmap.load(":/weather/duoyun.png");
    }
    else if(weather.contains(("晴"),Qt::CaseInsensitive))
    {
        pixmap.load(":/weather/qing.png");

    }
    else
    {
        pixmap.load(":/weather/wea_no.png");
    }

    ui->label_weather_icon->setPixmap(
                pixmap.scaled(ui->label_weather_icon->size(),Qt::KeepAspectRatio));


    //3.更具天气设置该背景图
   // this->weather_broad(weather);

    //4.空气湿度低于70%，提示天气干燥，给小区园艺浇水 humidity
    if(humidity.left(2).toInt()<70)   num_humidity =0;
    else
    {
        num_wird =0;
        num_humidity = list_humidity.size();
    }
     timer_humidity->start(1500);
}




/**
 * @brief  照明
 *
 * @param
 * @return
**/
void MainFrist::on_lighting_btn_clicked()
{


    QString ok_light =  ui->lighting_btn->text();


#ifdef Q_OS_LINUX

    char databuf[2];
    //根据驱动来定义数据格式
    //databuf[0] --- 代表灯的状态：0 --- 关灯，1 ---开灯
    //databuf[1] ---- 代表灯的编号 ：7 8 9 10

    //开灯
    if(ok_light == "openled")
    {
        databuf[0] = 1;
        databuf[1] = 7;
        write(fd,databuf,sizeof(databuf));

        databuf[0] = 1;
        databuf[1] = 8;
        write(fd,databuf,sizeof(databuf));

        databuf[0] = 1;
        databuf[1] = 9;
        write(fd,databuf,sizeof(databuf));

        databuf[0] = 1;
        databuf[1] = 10;
        write(fd,databuf,sizeof(databuf));
        ui->lighting_btn->setText("closeled");
        return ;
    }
    //关灯
    if(ok_light == "closeled")
    {

        databuf[0] = 0;
        databuf[1] = 10;
        write(fd,databuf,sizeof(databuf));

        databuf[0] = 0;
        databuf[1] = 9;
        write(fd,databuf,sizeof(databuf));

        databuf[0] = 0;
        databuf[1] = 8;
        write(fd,databuf,sizeof(databuf));

        databuf[0] = 0;
        databuf[1] = 7;
        write(fd,databuf,sizeof(databuf));
        ui->lighting_btn->setText("openled");
        return ;
    }

#endif
}


/**
 * @brief  关机
 *
 * @param
 * @return
**/
void MainFrist::on_close_btn_clicked()
{
    timer_humidity->stop();
    thread_pic->stop();

    mp4_process.disconnect();
    if(mp4_process.state() == QProcess::Running)
    {
        mp4_process.kill();
        //mp4_process.waitForFinished();
    }
    this->close();
}


/**
 * @brief  物业服务
 *
 * @param
 * @return
**/
void MainFrist::on_property_btn_clicked()
{
    timer_humidity->stop();
    thread_pic->stop();

    mp4_process.disconnect();
    if(mp4_process.state() == QProcess::Running)
    {
        mp4_process.kill();
    }
    Perproty * p = new Perproty();
    p->show();
    this->close();
}


/**
 * @brief  打开监控
 *
 * @param
 * @return
**/
void MainFrist::on_openCar_btn_clicked()
{


    timer_humidity->stop();
    thread_pic->stop();
    if(mp4_process.state() == QProcess::Running)
    {
        bool_mplayer =false;
        mp4_process.kill();
        //mp4_process.waitForFinished();
    }
    videodisplay * v = new videodisplay();
    v->show();
    this->close();
}


/**
 * @brief  设置皮肤、背景
 *
 * @param
 * @return
**/
void MainFrist::setBoard()
{
    //1.设置背景
    QPixmap pixmap= QPixmap(":pic/pro/hh888.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);

    QPixmap pixmap1(":/pic/pro/hha.jpg");
    QPalette palette1(this->palette());
    palette1.setBrush(QPalette::Background,QBrush(pixmap1));
    ui->label_mp4->setPalette(palette1);


}

