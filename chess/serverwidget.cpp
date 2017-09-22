#include "serverwidget.h"
#include "ui_serverwidget.h"
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonValue>
#include<QTcpServer>//监听套接字
#include<QTcpSocket>//通信套接字
#include<QObject>
#include<QDebug>
#include<QDataStream>
#include<QIODevice>

serverWidget::serverWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serverWidget)
{
    ui->setupUi(this);

    tcpServer=NULL;
    tcpSocket=NULL;

        //监听套接字,指定父对象，让其自动回收空间
    tcpServer=new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);


    socket_list = new QList<QTcpSocket*>;
    tcpServer->listen(QHostAddress::Any,8000);

    setWindowTitle("server(port:8000)");
   connect(tcpServer,&QTcpServer::newConnection,this,&serverWidget::newConnect);

}

serverWidget::~serverWidget()
{
    delete ui;
}



void serverWidget::newConnect()

{
        //取出建立好连接的套接字

        tcpSocket=tcpServer->nextPendingConnection();
        socket_list->append(tcpSocket);


        //获取对方的IP和端口
        QString ip=tcpSocket->peerAddress().toString();

        qint16 port=tcpSocket->peerPort();

        QString temp=QString("[%1:%2]:成功连接 ").arg(ip).arg(port);

        if(socket_list->length()==2)
            sendUserColor();
        ui->textEditRead->append(temp);

         connect(tcpSocket,&QTcpSocket::readyRead,this,&serverWidget::readMessage );

 }


void serverWidget::readMessage()
{
    //从通信套接字中取出内容

    QByteArray array;

    for(int i = 0;i < socket_list->length();i ++)
        {

            array=socket_list->at(i)->readAll();
            if(!(array.isEmpty()))
            {

                break;
            }
        }

    QJsonDocument jsonDoc(QJsonDocument::fromJson(array) );

    if(jsonDoc.isObject() ){

        QJsonObject jsonObj=jsonDoc.object();
        if(jsonObj.contains("continue")){
               if(jsonObj.take("continue").toString()=="BLACK"){
                   QJsonObject jsonObj;
                   jsonObj.insert("continue","BLACK");
                  QJsonDocument jsonDoc;
                  jsonDoc.setObject(jsonObj);
                   socket_list->at(1)->write(jsonDoc.toJson());
                     stopflag1=false;
               }
                 else{
                   QJsonObject jsonObj;
                   jsonObj.insert("continue","WHITE");
                  QJsonDocument jsonDoc;
                  jsonDoc.setObject(jsonObj);
                   socket_list->at(0)->write(jsonDoc.toJson());
                    stopflag2=false;
               }

        }

        else if(jsonObj.contains("gameType")){
           gameType=jsonObj.take("gameType").toString();
           newGame();
        }
        else if(jsonObj.contains("restart")){
            QString callColor;
            callColor=jsonObj.take("restart").toString();
           newGame();
           QJsonObject jsonObj;
           jsonObj.insert("restart",callColor);
          QJsonDocument jsonDoc;
          jsonDoc.setObject(jsonObj);
          for(int i=0;i<socket_list->length();i++)
           socket_list->at(i)->write(jsonDoc.toJson());


        }
        else if(stopflag1||stopflag2)
            return;
        else if(jsonObj.contains("stop")){
            if(jsonObj.take("stop").toString()=="BLACK"){
                stopflag1=true;
                QJsonObject jsonObj;
                jsonObj.insert("stop","BLACK");
               QJsonDocument jsonDoc;
               jsonDoc.setObject(jsonObj);
                socket_list->at(1)->write(jsonDoc.toJson());
            }
            else{
                stopflag2=true;
                QJsonObject jsonObj;
                jsonObj.insert("stop","WHITE");
               QJsonDocument jsonDoc;
               jsonDoc.setObject(jsonObj);
                socket_list->at(0)->write(jsonDoc.toJson());
            }

        }


        else if(gameType=="five")
        {

                if(jsonObj.contains("BLACK")&&"BLACK"==colorTurn)
                {


                    putChess("BLACK",jsonObj,fiveBoard);
                     colorTurn="WHITE";

                }
                else if(jsonObj.contains("WHITE")&&"WHITE"==colorTurn){

                    putChess("WHITE",jsonObj,fiveBoard);
                    colorTurn="BLACK";



                }
        }
        else if(gameType=="go"){

            if(jsonObj.contains("BLACK")&&"BLACK"==colorTurn)
            {

                putChess("BLACK",jsonObj,goBoard);
                 colorTurn="WHITE";

            }
            else if(jsonObj.contains("WHITE")&&"WHITE"==colorTurn){

                putChess("WHITE",jsonObj,goBoard);
                colorTurn="BLACK";



            }

        }



     }
 }



template<int N>
void serverWidget::putChess(QString color,QJsonObject jsonObj,GameBoard<N> &board){
    QJsonArray array= jsonObj.take(color).toArray();
    int x=array[0].toInt();
    int y=array[1].toInt();
    if(x>=board.boardSize()||y>=board.boardSize())
        return;

    if(board.checkStone(x,y)==NONE){
        StoneType stoneColor;
        if(color=="BLACK"){
            stoneColor=BLACK;
            colorTurn="WHITE";
        }
        else{
            stoneColor=WHITE;
            colorTurn="BLACK";
        }

         board.putStone(x,y,stoneColor);
        if(gameType=="go"){
            bool flag=false;
            for(int i=0;i<goBoard.boardSize();i++)
                for(int j=0;j<goBoard.boardSize();j++){
                    QList<QVector<int> > list;
                    if(goBoard.checkStone(i,j)!=NONE&&goBoard.checkStone(i,j)!=stoneColor){

                        findPiece(i,j,list);

                        if(checkGoRemove(list)==true){
                            remove(x,y,color,list);//o add the stone(X,Y,color) and remove the stone in the list
                            flag=true;
                        }
                       \
                    }

                }
            if(flag==false)
                sendToUser(x,y,color);
            else{
                for(int i=0;i<goBoard.boardSize();i++)
                    for(int j=0;j<goBoard.boardSize();j++){
                        QList<QVector<int> > list;
                        if(goBoard.checkStone(i,j)==stoneColor){

                            findPiece(i,j,list);

                            if(checkGoRemove(list)==true){
                                remove(x,y,color,list);//o add the stone(X,Y,color) and remove the stone in the list
                                flag=true;
                            }
                           \
                        }

                    }
            }
        }

        else if(gameType=="five"){

             if(checkFiveWin(fiveBoard,BLACK) ){
                  sendWin(color);
              }
             else
                  sendToUser(x,y,color);
         }


    }

}


bool serverWidget::checkFiveWin(FiveGameBoard a, StoneType COLOR){
    int i,j;//判断横着的5个是否都相等
     for(i=0;i<15;i++)
     for(j=0;j<15;j++)
     if(a.checkStone(i,j)==COLOR&&a.checkStone(i,j+1)==COLOR&&a.checkStone(i,j+2)==COLOR&&a.checkStone(i,j+3)==COLOR&&a.checkStone(i,j+4)==COLOR)
     return 1;
     for(j=0;j<15;j++)//判断竖着的5个是否都相等
     for(i=0;i<15;i++)
     if(a.checkStone(i,j)==COLOR&&a.checkStone(i+1,j)==COLOR&&a.checkStone(i+2,j)==COLOR&&a.checkStone(i+3,j)==COLOR&&a.checkStone(i+4,j)==COLOR)
     return 1;
     for(i=0;i<15;i++)//判断左斜5个
     for(j=0;j<15;j++)
     if(a.checkStone(i,j)==COLOR&&a.checkStone(i+1,j+1)==COLOR&&a.checkStone(i+2,j+2)==COLOR&&a.checkStone(i+3,j+3)==COLOR&&a.checkStone(i+4,j+4)==COLOR)
     return 1;
     for(i=0;i<15;i++)//右斜5个
     for(j=14;j>4;j--)
     if(a.checkStone(i,j)==COLOR&&a.checkStone(i+1,j-1)==COLOR&&a.checkStone(i+2,j-2)==COLOR&&a.checkStone(i+3,j-3)==COLOR&&a.checkStone(i+4,j-4)==COLOR)
     return 1;
     return 0;
  }



void serverWidget::sendToUser(int x, int y, QString COLOR){
       QJsonArray arr;
     arr.insert(0,x);
     arr.insert(1,y);
     QJsonObject jsonObj;

     jsonObj.insert(COLOR,arr);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);

    for(int i = 0;i < socket_list->length();i ++)
    {
        socket_list->at(i)->write(jsonDoc.toJson());

        socket_list->at(i)->waitForBytesWritten();
    }
}

void serverWidget::sendUserColor(){//tell client his stone color
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    for(int i=0;i<=1;i++){
        QString c;
        if(i==0)
            c="BLACK";
        else
            c="WHITE";
        jsonObj.insert("color",c);

        jsonDoc.setObject(jsonObj);

        socket_list->at(i)->write(jsonDoc.toJson());
    }

}
void serverWidget::sendWin(QString winColor){
    QJsonObject jsonObj;
    QString winflag="win";
    jsonObj.insert(winflag,winColor);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);

    for(int i = 0;i < socket_list->length();i ++)
    {

    socket_list->at(i)->write(jsonDoc.toJson());
    socket_list->at(i)->waitForBytesWritten();
    }

}

bool serverWidget::checkGoRemove(QList<QVector<int> > list){

    QList<QVector<int> >::iterator iter=list.begin();
    QVector<int> vec;
    while(iter!=list.end() ){
        vec=*iter;
        if(goBoard.hasNeighbor(vec[0],vec[1],NONE)==true)
            return false;
        iter++;

    }
    return true;//no breath,remove the stone in the list

}




void serverWidget::on_pushButtonClose_clicked()
{
    if(tcpSocket==NULL){
            return;
        }
        //主动和客户端断开连接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpSocket=NULL;
    socket_list->clear();
 }

void serverWidget::remove(int x,int y,QString color,QList<QVector<int> > &list){
    QJsonArray array;
    array.append(x);
    array.append(y);
    array.append(color);

    QList<QVector<int> >::iterator iter=list.begin();
    QVector<int> vec;
    while(iter!=list.end()){
        vec=*iter;
        array.append(vec[0]);
        array.append(vec[1]);
        iter++;
        goBoard.removeStone(vec[0],vec[1]);
    }
    QJsonObject jsonObj;
    QString dieflag="die";
    jsonObj.insert(dieflag,array);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);

    for(int i = 0;i < socket_list->length();i ++)
    {

        socket_list->at(i)->write(jsonDoc.toJson());
        socket_list->at(i)->waitForBytesWritten();
    }
}

void serverWidget:: findPiece(int x,int y,QList<QVector<int> > &list){
    QVector<int> vec;
    vec.append(x);
    vec.append(y);
    list.append(vec);
    if(goBoard.hasNeighbor(x,y,goBoard.checkStone(x,y) ))
     {

         StoneType checkColor=goBoard.checkStone(x,y);
         findNeighbor(x+1,y,checkColor,list);
         findNeighbor(x+1,y+1,checkColor,list);
         findNeighbor(x,y+1,checkColor,list);

     }

 }

 void serverWidget::findNeighbor(int x,int y,StoneType checkColor,QList<QVector<int> > &list){
     if(goBoard.checkStone(x,y)==checkColor){
         QVector<int> vec;
         vec.append(x);
         vec.append(y);
         if(!list.contains(vec))
            findPiece(x,y,list);
     }
 }

void serverWidget::newGame(){
    if(gameType=="go")
        goBoard.initialBoard();
    else
        fiveBoard.initialBoard();
    stopflag1=false;
    stopflag2=false;
    colorTurn="BLACK";

}
