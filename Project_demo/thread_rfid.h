#ifndef THREAD_RFID_H
#define THREAD_RFID_H
#include<QThread>
#include<QDebug>
#include<QDateTime>
#include<QDir>
#include"readrfidid.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef Q_OS_LINUX
#include <assert.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#endif

#ifdef __cplusplus
}
#endif


#include<QMutexLocker>

#define USART "/dev/ttySAC2"

class Thread_rfid :public QThread
{
    Q_OBJECT

public:
    Thread_rfid();

    ~Thread_rfid();

    //执行完成，把状态改为true
    void stop();

   signals:
    void get_rfid(int ID);


public:
    void run() override;


    bool stop_ok=false;
    QMutex mutex;

private:
    QString pic_path;
    int pic_num;

};

#endif // THREAD_RFID_H
