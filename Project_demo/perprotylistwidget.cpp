#include "perprotylistwidget.h"
#include "ui_perprotylistwidget.h"

perprotyListWidget::perprotyListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::perprotyListWidget)
{
    ui->setupUi(this);
}

perprotyListWidget::~perprotyListWidget()
{
    delete ui;
}

void perprotyListWidget::setData(QString id, QString name,
                                 QString perproty, QString examine, QString opninion)
{
        ui->label_id->setText(id);
        ui->label_name->setText(name);
        ui->label_examine->setText(examine);
        ui->label_perproty->setText(perproty);
        ui->label_opinion->setText(opninion);
}
