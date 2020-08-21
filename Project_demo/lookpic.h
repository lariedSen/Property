#ifndef LOOKPIC_H
#define LOOKPIC_H

#include <QWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QtDebug>
#include <QFile>

namespace Ui {
class LookPic;
}

class LookPic : public QWidget
{
    Q_OBJECT

public:
    explicit LookPic(QWidget *parent = nullptr);
    ~LookPic();

private slots:
    void browseBtn_Slots();
    void listWdiget_itemDoubleClicked(QListWidgetItem* item);


    void on_return_btn_clicked();

private:
    Ui::LookPic *ui;
};

#endif // LOOKPIC_H
