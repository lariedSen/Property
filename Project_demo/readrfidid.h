#ifndef READRFIDID_H
#define READRFIDID_H
#include<QString>

/*************************************************
Copyright:
Author:Laried
Date:2020年7月27日
Description (描述主要实现的功能):
    [1] 把C语言写的代码封装成类
    [2] readCardId() 是对外接口
**************************************************/


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

#define USART2 "/dev/ttySAC2"


extern void init_tty(int fd);
extern unsigned char calBCC(unsigned char* buf,int n);
extern int PiccRequest(int fd);
extern int PiccAnticoll(int fd);
extern void beep();

//int readCardId(QString);

#ifdef __cplusplus
}
#endif



#endif // READRFIDID_H
