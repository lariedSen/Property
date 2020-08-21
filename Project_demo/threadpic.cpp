#include "threadpic.h"


Threadpic::Threadpic(QObject *parent): QThread(parent),stop_ok(false)
{
     qDebug() << "Worker start Thread : " << QThread::currentThreadId();
}


/**
 * @brief 析构函数，退出线程
 *
 * @param
 * @return
**/
Threadpic::~Threadpic()
{
    stop();
    quit();
    wait();
}



/**
 * @brief  把状态改为true,
 *
 * @param
 * @return
**/
void Threadpic::stop()
{
    qDebug() << "Worker Stop Thread : " << QThread::currentThreadId();
    QMutexLocker locker(&mutex);
    stop_ok = true;
}

void Threadpic::run()
{
    pic_num =0;

#ifdef Q_OS_WIN32
    QDir dir("G:/QTmake/0727/Project_demo/frist", "*.jpg");
#endif

#ifdef Q_OS_LINUX
    QDir dir("/project/frist", "*.jpg");
    //dir.setFilter(QDir::Dirs|QDir::Files);//除了目录或文件，其他的过滤掉
   dir.setFilter(QDir::Files);//除了目录或文件，其他的过滤掉
#endif
   QFileInfoList list = dir.entryInfoList();
   while (1)
   {
        pic_num =0 ;
      //照片
       for( ;pic_num<list.size();pic_num++)
       {
           qDebug()<<pic_num;

           pic_path = list.at(pic_num).filePath();
           qDebug()<<pic_path;
           emit show(pic_path);
           QThread::sleep(2);
       }
       qDebug()<<list.size();
       if(pic_num >= list.size())
       {
           qDebug()<<"--------------------";
           pic_path.clear();
           emit show(pic_path);
           QThread::sleep(2);

       }
       // 检测是否停止
       {
           QMutexLocker locker(&mutex);
           if (stop_ok)
               break;
       }
   }


}
