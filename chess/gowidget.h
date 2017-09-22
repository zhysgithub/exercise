#ifndef GOWIDGET_H
#define GOWIDGET_H

#include <QWidget>
#include<QTcpSocket>//通信套接字
#include<QString>
#include<QLabel>
#include"gameboard.h"
namespace Ui {
class goWidget;
}

class goWidget : public QWidget
{
    Q_OBJECT

public:
    explicit goWidget(QWidget *parent = 0);
    ~goWidget();


    template <int N>
    void setColor(GameBoard<N> &Board,int i,int j,QBrush &brush);
    void readMessage();
    void writeArray(QString chessColor,int x,int y);
    void writeString(QString a,QString b);
    void waitSec(int s);
    void newGame();
protected:
    //重写绘图事件
    void paintEvent(QPaintEvent *);
    //重写鼠标按下事件
    void mousePressEvent(QMouseEvent *e);


private slots:


    void on_stopButton_clicked();
    void on_continueButton_clicked();
    void on_startButton_clicked();
    void on_restartButton_clicked();

private:
    Ui::goWidget *ui;
    int gridW;//一个格子的宽度
    int gridH;//一个个字的高度
    int startX;//起点横坐标
    int startY;//起点纵坐标

    int chessX,chessY;//棋盘下标
    QTcpSocket *tcpSocket;
    QString userName;
    int lineNum;
    QString color;
    QString gameType;


    GoGameBoard Board;
    QString  turnColor;
};

#endif // GOWIDGET_H
