#include "fiveWidget.h"
#include "ui_widget.h"
#include<QPainter>
#include<QPen>
#include<QBrush>
#include <QMouseEvent>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QDebug>
#include<QHostAddress>
#include<QIODevice>
#include <QPalette>
#include <QCoreApplication>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);
    newGame();
}
void Widget::readMessage()
 {
     //获取对方发送的内容
     QByteArray array=tcpSocket->readAll();

     QJsonDocument jsonDoc(QJsonDocument::fromJson(array) );
     QJsonObject jsonObj=jsonDoc.object();
     if(jsonObj.contains("stop")){

         ui->continueButton->show();
         ui->stopButton->hide();
        ui->showResult->setText("the other side stoped");
        ui->showResult->show();
     }
     if(jsonObj.contains("continue")){
         ui->stopButton->show();
         ui->continueButton->hide();
         ui->showResult->hide();
     }
     if(jsonObj.contains("restart")){
         QString callColor=jsonObj.take("restart").toString();
         if(color==callColor){

             newGame();
         }
         else{
             ui->showResult->setText("the other side ask for restart!!!");
             ui->showResult->show();
             waitSec(1);
              ui->showResult->hide();
             newGame();
         }
         update();
     }

     if(jsonObj.contains("color")){

         if(jsonObj.take("color").toString()=="BLACK")
             color="BLACK";
         else
             color="WHITE";
     }
     if(jsonObj.contains("win")){
         QString winner=jsonObj.take("win").toString();
         ui->showResult->show();


         if(winner==color)
             ui->showResult->setText("Congratulations!!! You win the game");
         else

            ui->showResult->setText("What a pitty. You lost!!!");

     }
     if(jsonObj.contains("WHITE")){
         if(color=="WHITE")
         {
             ui->connectInfo->setText("wait");
         }
         else{
             ui->connectInfo->setText(color+"   It's your turn");

         }

         QJsonArray jsonArr =jsonObj.take("WHITE").toArray();
         chessX=jsonArr[0].toInt();
         chessY=jsonArr[1].toInt();


         Board.putStone(chessX,chessY,WHITE);


         update();

     }
     if(jsonObj.contains("BLACK")){

         if(color=="BLACK")

             ui->connectInfo->setText("wait");

         else
              ui->connectInfo->setText(color+"   It's your turn");

         QJsonArray jsonArr =jsonObj.take("BLACK").toArray();
         chessX=jsonArr[0].toInt();
         chessY=jsonArr[1].toInt();
         Board.putStone(chessX,chessY,BLACK);
         update();
     }


 }


Widget::~Widget()
{
    delete ui;
}
void Widget::paintEvent(QPaintEvent *)
{

    gridW=32;
    gridH=32;

    startX=gridW;
    startY=gridH;


    QPainter p(this);//创建画家对象，并指定当前窗口为画图设备


     //p.drawPixmap(rect(),QPixmap("D:/Documents/Pictures/chess/empty.PNG"));
    //定义画笔

    //把画笔交给画家
     QBrush brush;

    brush.setColor(QColor(210,125,0));
    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);
    p.drawRect(0,0,startX+gridW*(lineNum),startY+gridH*(lineNum));
     QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(0,0,0));//设置rgb颜色
    p.setPen(pen);
    //画矩形

    p.drawRect(startX,startY,startX+gridW*(lineNum-2),startY+gridH*(lineNum-2));
    pen.setWidth(2);
    p.setPen(pen);
     for(int j=1;j<lineNum;j++){
            p.drawLine(gridW*1,gridH*j,gridW*lineNum,gridH*j);
            p.drawLine(gridW*j,gridH*1,gridW*j,gridH*lineNum);

       }

     brush.setColor(QColor(0,0,0));
     p.setBrush(brush);




     if(lineNum==15){
         for(int i=4;i<lineNum;i+=8){
            for(int j=4;j<lineNum;j+=8)
            p.drawEllipse(QPoint(gridW*i,gridH*j),3,3);
        }
     }

    //if(chessX!=-1&&chessX!=0 && chessY!=0 &&chessX!=lineNum+1 && chessY!=lineNum+1){
     for(int i=0;i<lineNum;i++)
         for(int j=0;j<lineNum;j++)
         {


             if(Board.checkStone(i,j)==NONE)
                    continue;
             if(Board.checkStone(i,j)==WHITE)
                  brush.setColor(QColor(255,255,255));
             else if(Board.checkStone(i,j)==BLACK)
                brush.setColor(QColor(0,0,0));//设置rgb颜色
             brush.setStyle(Qt::SolidPattern);
             p.setBrush(brush);
             p.drawEllipse(QPoint(i*gridW+startX,j*gridH+startY),gridW/2,gridH/2);
         }

}


void Widget::mousePressEvent(QMouseEvent *e)
{
    float  x=e->x();
    float  y=e->y();

    float tempX=(x-startX)/gridW;
    x=qRound(tempX);
    float tempY=(y-startY)/gridH;
    y=qRound(tempY);
    if(x<lineNum&&y<lineNum&&tcpSocket!=NULL)
     writeArray(color,x,y);


}




void Widget::writeArray(QString COLOR, int x, int y){
    QJsonArray arr;
    arr.insert(0,x);
    arr.insert(1,y);
    QJsonObject jsonObj;
    jsonObj.insert(COLOR,arr);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);


    tcpSocket->write(jsonDoc.toJson());

}
void Widget::writeString(QString a, QString b){
    QJsonObject jsonObj;

    jsonObj.insert(a,b);
   QJsonDocument jsonDoc;
   jsonDoc.setObject(jsonObj);

    tcpSocket->write(jsonDoc.toJson());
}

void Widget::on_startButton_clicked()
{
    tcpSocket=NULL;
    tcpSocket=new QTcpSocket(this);
    qint16 port=8000;
    QString ip="127.0.0.1";//""172.17.249.77";

    tcpSocket->connectToHost(QHostAddress(ip),port);
    connect(tcpSocket,&QTcpSocket::connected,
            [=]()
            {
                ui->connectInfo->setText("成功与服务器建立连接");

            }
            );
    writeString("gameType",gameType);
 connect(tcpSocket,&QTcpSocket::readyRead,this,&Widget::readMessage);
 ui->startButton->hide();
}



void Widget::on_stopButton_clicked()
{

        writeString("stop",color);
        ui->continueButton->show();
        ui->stopButton->hide();
   ui->showResult->setText("you stopped");
   ui->showResult->show();

}


void Widget::on_continueButton_clicked()
{
    writeString("continue",color);
    ui->stopButton->show();
    ui->continueButton->hide();
    ui->showResult->hide();

}
void Widget::newGame(){
    Board.initialBoard();
    ui->showResult->hide();
    ui->continueButton->hide();
    ui->stopButton->show();
    lineNum=Board.boardSize();
    gameType="five";
    chessX=-1;
    chessY=-1;
}

void Widget::on_restartButton_clicked()
{
    writeString("restart",color);
}
void Widget::waitSec(int s)
{
    QTime t;
    t.start();
    while(t.elapsed()<1000*s)
        QCoreApplication::processEvents();
}
