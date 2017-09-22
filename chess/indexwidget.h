#ifndef INDEXWIDGET_H
#define INDEXWIDGET_H

#include <QWidget>
#include<fiveWidget.h>
#include<gowidget.h>
#include<QTcpSocket>//通信套接字
#include<QString>
#include<QPainter>
namespace Ui {
class indexWidget;
}

class indexWidget : public QWidget
{
    Q_OBJECT

public:
    explicit indexWidget(QWidget *parent = 0);

    ~indexWidget();
 protected:
    void paintEvent(QPaintEvent *);
private slots:
    void on_pushButtonFiveGame_clicked();

    void on_pushButtonGo_clicked();

private:
    Ui::indexWidget *ui;
    Widget fgame;
    goWidget gogame;



};

#endif // INDEXWIDGET_H
