#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFile>
#include<QTimer>
#include<QProcess>
#include<QDir>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void  player_mp4();


public slots:
    void time_slots();

private:
    Ui::MainWindow *ui;
    QFile * qssFile ;
    QTimer *mp4_timer;
    int num;
    QProcess mp4_process;
};
#endif // MAINWINDOW_H
