#ifndef PERPROTYLISTWIDGET_H
#define PERPROTYLISTWIDGET_H

#include <QWidget>

namespace Ui {
class perprotyListWidget;
}

class perprotyListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit perprotyListWidget(QWidget *parent = nullptr);
    ~perprotyListWidget();
    void setData(QString id,QString name,QString perproty,QString examine ,QString opninion);

private:
    Ui::perprotyListWidget *ui;
    //bool ok;
};

#endif // PERPROTYLISTWIDGET_H
