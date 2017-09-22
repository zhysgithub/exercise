#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QTcpServer>//监听套接字
#include<QTcpSocket>//通信套接字
#include<QList>
#include<QString>
#include"gameboard.h"
#include<QVector>
namespace Ui {
class serverWidget;
}

class serverWidget : public QWidget
{
    Q_OBJECT


public:
    explicit serverWidget(QWidget *parent = 0);
    ~serverWidget();
  //  void sendInfo(int x, int y, StoneType stone);
    //bool putChess(QString color,QJsonObject jsonObj,FiveGameBoard board);
    template <int N>
    void putChess(QString color,QJsonObject jsonObj,GameBoard<N> &board);
    bool checkFiveWin(FiveGameBoard board,StoneType COLOR);

    bool checkGoRemove(QList<QVector<int> > list);
    void sendToUser(int x,int y,QString COLOR);
    void sendUserColor();
    void sendWin(QString winColor);
    //bool checkBreath(int i,int j,StoneType checkColor);
    //void sendDieList(int x,int y,QString color,StoneType checkColor);
    void remove(int x,int y,QString color,QList<QVector<int> > &list);
    void  findPiece(int x,int y,QList<QVector<int> > &list);
    void  dealColor(StoneType stoneColor, bool &flag,QList<QVector<int> > &list);

    void newGame();
private slots:
    void on_pushButtonClose_clicked();
    void newConnect();
    void readMessage();
    void findNeighbor(int x,int y,StoneType checkColor,QList<QVector<int> >  &list);

private:
    Ui::serverWidget *ui;
    QTcpServer *tcpServer;//监听套接字
    QTcpSocket *tcpSocket;
    FiveGameBoard fiveBoard;
    GoGameBoard goBoard;
    QString colorTurn;
    QString gameType;
     QByteArray array;
    QList<QTcpSocket*> *socket_list;
    bool stopflag1;
     bool stopflag2;
};

#endif // SERVERWIDGET_H
