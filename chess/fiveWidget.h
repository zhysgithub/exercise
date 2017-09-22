#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTcpSocket>//通信套接字
#include<QString>
#include<QTime>
#include<QLabel>
#include"gameboard.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


    void readMessage();
    void writeArray(QString chessColor,int x,int y);
    void writeString(QString a,QString b);
    void newGame();
    void waitSec(int s);
protected:
    //重写绘图事件
    void paintEvent(QPaintEvent *);
    //重写鼠标按下事件
    void mousePressEvent(QMouseEvent *e);


private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_continueButton_clicked();

    void on_restartButton_clicked();


private:
    int gridW;//一个格子的宽度
    int gridH;//一个个字的高度
    int startX;//起点横坐标
    int startY;//起点纵坐标
    int chessX,chessY;//棋盘下标


    QTcpSocket *tcpSocket;
    QString userName;
    int lineNum;
    QString color;//自己棋子的颜色
    QString gameType;
    Ui::Widget *ui;
    FiveGameBoard Board;
    QString  turnColor;//当前应该下棋的颜色


};

#endif // WIDGET_H
