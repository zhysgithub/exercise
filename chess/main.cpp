#include "fiveWidget.h"
#include "indexwidget.h"

#include <QApplication>
#include"serverwidget.h"
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   indexWidget w1;
   w1.show();

   indexWidget w2;
   w2.show();
   serverWidget ser;
    ser.show();


    return a.exec();
}
