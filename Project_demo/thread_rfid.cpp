#include "thread_rfid.h"

Thread_rfid::Thread_rfid()
{
     qDebug() << "Worker start Thread : " << QThread::currentThreadId();
}

Thread_rfid::~Thread_rfid()
{
    stop();
    quit();
    wait();
}

void Thread_rfid::stop()
{
    qDebug() << "Worker Stop Thread : " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    stop_ok = true;
}

/*
          // 检测是否停止
          {
              QMutexLocker locker(&mutex);
              if (stop_ok)
                  break;
          }
*/

void Thread_rfid::run()
{

    #ifdef Q_OS_LINUX
    //[1] 打开串口
    int fd = open(USART,O_RDWR | O_NOCTTY);
    if(fd == -1)
    {
        perror("open");
        return ;
    }
    //[2]初始化串口配置
    init_tty(fd);

    while(1)
    {
        qDebug()<<"99999999999999999999999999999999999999999999999";

         //[3]应需求的要求，主机向从机发起请求(request ---- 先写入数据--write，然后再read数据)
         if(PiccRequest(fd)) //请求成功，则返回0，否则，非零
         {
            // printf("request fail\n");

         }
         //[4]根据设备的流程图，进行防碰撞
         unsigned int ID = PiccAnticoll(fd);
         if(ID ==0 )
         {
             printf("Anticoll fail\n");
         }
         else
         {
             emit this->get_rfid(ID);
             printf("The Card ID is %x\n",ID);
             //beep();

         }
          sleep(1);
          // 检测是否停止
          {
              QMutexLocker locker(&mutex);
              if (stop_ok)
                  break;
          }
     qDebug()<<"88888";
    }
#endif
}
