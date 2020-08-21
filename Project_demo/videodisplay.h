#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QWidget>
#include<v4l2.h>
#include <QTimer>
#include<QTcpSocket>
/***************************/
//封装接口，

static pass_data pd;

static bool initDev();

/****************************/


namespace Ui {
class videodisplay;
}

#define Path_pic "/project/pic"

class videodisplay : public QWidget
{
    Q_OBJECT

public:
    explicit videodisplay(QWidget *parent = nullptr);
    ~videodisplay();


private slots:
    void on_startBtn_clicked();
    void flushButff();

    void on_Btn_cat_clicked();

   // void on_Btn_stop_clicked();

    void on_Btn_broser_clicked();

    void on_Btn_return_clicked();

    void on_waitBtn_clicked();

private:
    Ui::videodisplay *ui;
     unsigned char *bufrgb; //获取摄像头数据指针
     QTimer timer;
     bool open_ok=false;
};

#endif // VIDEODISPLAY_H
