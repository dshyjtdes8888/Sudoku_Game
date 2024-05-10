#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPushButton>

QT_BEGIN_NAMESPACE

namespace Ui { class Widget; }

QT_END_NAMESPACE
class select;
class Widget : public QWidget
{
    Q_OBJECT
public:
    int _num[9][9]={0};//9*9数独
    QPushButton _btn[9][9];//9*9的按钮
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void create(int nums);//创建局面
    bool backtrack(int board[9][9], int row, int col);//回溯
    bool isValid(int board[9][9], int row, int col, int num);//合法性判断
    void clearNum(int r,int c);//清除按钮上的数字
    void setNum(int r,int c,int num);//设置按钮上的数字
    void chooseOne(int r,int c);//点击格子后触发此函数让玩家9选1
    int get_num(int r,int c)//得到给定位置预设的数字
    {
        return _num[r][c];
    }
    void set_Num(int r,int c,int x)//设定给定位置预设的数字
    {
        _num[r][c]=x;
    }

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
