#ifndef MAINFRIST_H
#define MAINFRIST_H

#include <QWidget>
#include"threadpic.h"


//包含C的头文件和宏定义
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef __cplusplus
}
#endif



//网络请求
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QUrl>
#include<QNetworkReply>

//Json获取
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>

#include<QTimer>
#include<QThread>
#include<QProcess>
namespace Ui {
class MainFrist;
}

class MainFrist : public QWidget
{
    Q_OBJECT

public:
    explicit MainFrist(QWidget *parent = nullptr);
    ~MainFrist();

private :
    void weather_broad( QString weather);
    void humidity_warn(QString humidity);
    void temperature_warn(QString temperature);
    void word_lopp();



private slots:

    void pic_show(QString pic_path);
    void player_auto();

    void getWeather(QNetworkReply * reply);

    void on_lighting_btn_clicked();

    void on_close_btn_clicked();

    void on_property_btn_clicked();

    void on_openCar_btn_clicked();

private:
    Ui::MainFrist *ui;
    void setBoard();//设置背景皮肤
    QNetworkAccessManager *manager;

    QStringList list_word;
    QStringList list_humidity;
    QString humidity;
    int num_humidity=0;
    int num_wird;
    QTimer*timer_humidity ;


    QString temperature;
    int num_temperture;

    Threadpic * thread_pic;

    //播放视频
    bool bool_mplayer ;
    QProcess mp4_process;
    void player_advert(); //开始播放
    void player_ready(); //准备播放
   // void player_auto();槽 播放完成
    int mp4_num;
    QStringList mp4_list;

    //灯
    int fd;
};

#endif // MAINFRIST_H
