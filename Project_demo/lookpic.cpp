#include "lookpic.h"
#include "ui_lookpic.h"
#include"videodisplay.h"
LookPic::LookPic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LookPic)
{
    ui->setupUi(this);
    connect(ui->browseBtn,&QAbstractButton::clicked,
            this,&LookPic::browseBtn_Slots);
    connect(ui->listWidget,&QListWidget::itemDoubleClicked,
            this,&LookPic::listWdiget_itemDoubleClicked);

}

LookPic::~LookPic()
{
    delete ui;
}


/**
 * @brief  浏览图片
 *
 * @param
 * @return
**/
void LookPic::browseBtn_Slots()
{
     QString namefile =QFileDialog::getOpenFileName(this,"choice pictrue","./",
                                                          "Images (*.png *.jpg)");

     ui->listWidget->addItem(namefile);
}


/**
 * @brief  双击显示图片
 *
 * @param
 * @return
**/
void LookPic::listWdiget_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug()<<"***************************************";

    //使用QPixmap，指定文件其路径
    QString namefile = item->text();
    qDebug()<<namefile;
    QPixmap map(namefile);
    map = map.scaled(ui->label->width(),ui->label->height());
    ui->label->setPixmap(map);
}

void LookPic::on_return_btn_clicked()
{
    this->close();
}
