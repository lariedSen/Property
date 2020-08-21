#ifndef THREADPIC_H
#define THREADPIC_H

#include<QMutexLocker>
#include<QThread>
#include<QDebug>
#include<QDateTime>
#include<QDir>
class Threadpic :public QThread
{
    Q_OBJECT
public:
    Threadpic(QObject *parent = 0);

    ~Threadpic();

    //执行完成，把状态改为true
    void stop();
    // QMutex mutex;
    //#include<QMutexLocker>

signals:
    void show(QString );


public:
    void run() override;

private:
    bool stop_ok;
    QMutex mutex;

private:
    QString pic_path;
    int pic_num;
};

#endif // THREADPIC_H
