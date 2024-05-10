#include "select.h"
#include "ui_select.h"
#include<QPushButton>
#include"widget.h"
#define cout qDebug().noquote().nospace()

select::select(QPoint point,QWidget *parent,Widget* widget,int r,int c) :
    QDialog(parent),_widget(widget),_r(r),_c(c),
    ui(new Ui::select)
{
    ui->setupUi(this);
    this->setGeometry(point.x(),point.y(),this->width(),this->height());
    //但是如果一move的话还是从0，0开始move
    this->move(point.x()+65,point.y()-this->height()/2);
    for(int i=0;i<9;i++)//设置9个按钮
    {
        _btn[i].setFixedSize(45,45);
        _btn[i].setStyleSheet("color:green");
        QFont font(nullptr,25);
        _btn[i].setFont(font);
        _btn[i].setText(QString::number(i+1));
        ui->gridLayout->addWidget(&_btn[i],i/3,i%3);
        connect(&_btn[i],&QPushButton::clicked,[=]()
        {//点击按钮就填入为对应的数字
            _widget->setNum(_r,_c,i+1);
            this->close();
        });
    }
    connect(ui->btn_clear,&QPushButton::clicked,this,&select::clearNumWhenClicked);//点击清除当前方格就清除数字
}

select::~select()
{
    delete ui;
}

void select::clearNumWhenClicked()
{
    _widget->clearNum(_r,_c);
    this->close();
}
