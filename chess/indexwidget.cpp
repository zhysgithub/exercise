#include "indexwidget.h"
#include "ui_indexwidget.h"

indexWidget::indexWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::indexWidget)
{
    ui->setupUi(this);

}

indexWidget::~indexWidget()
{
    delete ui;
}
void indexWidget::paintEvent(QPaintEvent *){
    QPainter p(this);//创建画家对象，并指定当前窗口为画图设备

   p.drawPixmap(rect(),QPixmap("D:/Documents/Pictures/chess/index.jpg"));
}
void indexWidget::on_pushButtonFiveGame_clicked()
{
    this->hide();
    fgame.show();
}

void indexWidget::on_pushButtonGo_clicked()
{
    this->hide();
    gogame.show();
}
