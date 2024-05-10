#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include<QPushButton>
#include<QDialog>

namespace Ui
{
    class select;
}
class Widget;

class select : public QDialog
{
    Q_OBJECT
public:
    Ui::select *ui;
    QPushButton _btn[9];//9个选数字按钮
    Widget* _widget;
    int _r,_c;
    QPoint _point;
    //第一个参数是鼠标的位置，第三个参数是主窗口，最后两个参数确定玩家点击的按钮的位置
    explicit select(QPoint point,QWidget *parent = nullptr,Widget* widget=0,int r=0,int c=0);
    ~select();
    void clearNumWhenClicked();//配合清除当前方格的槽函数
};

#endif // FORM_H
