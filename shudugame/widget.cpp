#include "widget.h"
#include "ui_widget.h"
#include"select.h"
#include<QDebug>
#include<QPushButton>
#include<QColor>
#include<QString>
#include<QDialog>
#include<QAction>
#include<QMessageBox>
#include<QRandomGenerator>
#include<QString>

#define cout qDebug().noquote().nospace()


void Widget::chooseOne(int r,int c)
{
    QPoint point=this->cursor().pos();//获取鼠标位置
    select* s = new select(point,0,this,r,c);//新建一个9选1的
    s->exec();
}

int _num2[9][9]={0};//当前数独的副本，用于实现重玩功能

void Widget::create(int nums)
{
    QRandomGenerator rnd;
    rnd.seed(time(0));
    // 删除原数独
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            _btn[i][j].setStyleSheet("color: blue");
            _btn[i][j].setText("");
            _btn[i][j].setEnabled(true);
            _num[i][j] = 0;
            _num2[i][j]=0;
        }
    }
    // 初始化数独面板
    int row = 0, col = 0;
    // 使用回溯算法生成数独
    bool success = backtrack(_num, row, col);
    // 如果生成成功，则随机保留 nums 个数字
    if(success)
    {
        bool s[81] = {0};
        int count = 0;
        while(count != nums)
        {
            int tmp = (rnd() % 81 + 81) % 81;
            if(s[tmp] == 0) s[tmp] = 1, count++;
        }
        // 设置数独面板的初始状态
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                int tar = i * 9 + j;
                if(s[tar] == 1)
                {
                    _btn[i][j].setStyleSheet("color: blue");
                    _btn[i][j].setText(QString::number(_num[i][j]));
                    _btn[i][j].setEnabled(false);
                    _num2[i][j]=_num[i][j];
                }
            }
        }
    }
}

//回溯法创建/求解数独
bool Widget::backtrack(int board[9][9], int row, int col)
{
  // 如果列数到达9，则表示当前行填写完毕，开始填写下一行
  if(col == 9)
  {
  row++;
  col = 0;
  // 如果当前行也填写完毕，则表示数独生成成功
  if(row == 9)
  {
     return true;
  }
}
// 如果当前位置已经有数，则跳到下一个位置
if(board[row][col] != 0)
{
    return backtrack(board, row, col+1);
}
// 尝试填写当前位置
for(int i = 1;  i<= 9; i++)
{
    int num=(rand()%9+1);//随机生成一个1-9的数字
    if(isValid(board, row, col, num))
    {
        board[row][col] = num;
        if(backtrack(board, row, col+1))
        {
            return true;
        }
        board[row][col] = 0;
    }
}
return false;
}

//验证数独正确性
bool Widget::isValid(int board[9][9], int row, int col, int num)
{
// 判断当前数字在当前行是否出现过
  for(int i = 0; i < 9; i++)
  {
  if(board[row][i] == num)
  {
  return false;
  }
}
// 判断当前数字在当前列是否出现过
  for(int i = 0; i < 9; i++)
  {
    if(board[i][col] == num)
    {
        return false;
    }
}
// 判断当前数字在当前 3x3 的小格子内是否出现过
  int r = row / 3 * 3, c = col / 3 * 3;
  for(int i = r; i < r+3; i++)
  {
    for(int j = c; j < c+3; j++)
    {
        if(board[i][j] == num)
        {
            return false;
        }
    }
  }
 return true;
}

int nums=0;//需要保留的数字数量，用于控制难度
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("数独游戏");
    QStringList color;
    for(int i=0;i<9;i++)
    {
        color.append("background-color:white");
    }

    for(int r=0;r<9;r++)
    {

        for(int c=0;c<9;c++)
        {
            _btn[r][c].setFixedSize(65,65);
            QFont ft(nullptr,35);
            _btn[r][c].setFont(ft);
            _btn[r][c].setText("");
            _btn[r][c].setEnabled(true);
            int tor=r/3,toc=c/3;
            int tar=tor*3+toc;
            _btn[r][c].setStyleSheet(color[tar].append("color:black"));
            ui->gridLayout->addWidget(&_btn[r][c],r,c);
            connect(&_btn[r][c],&QPushButton::clicked,[=](){
                QPoint point=this->cursor().pos();
                select* ss = new select(point,0,this,r,c);
                ss->exec();
            });
        }
    }
    //初始难度为简单，创建初始游戏
    nums=40;
    create(nums);


    //生成新游戏
    connect(ui->btn_create,&QPushButton::clicked,[=]()
    {
        create(nums);
    });

    //检查
    connect(ui->btn_help, &QPushButton::clicked, [=]()
    {
        // 遍历数独面板，检查每个数字是否有误
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                QString text = _btn[i][j].text();
                if(text == "") continue; // 跳过空格
                int num = text.toInt();
                bool conflict = false; // 标记是否有冲突
                // 检查行是否有冲突
                for(int k = 0; k < 9; k++)
                {
                    if(k == j) continue; // 跳过当前位置
                    QString t = _btn[i][k].text();
                    if(t == "") continue; // 跳过空格
                    int n = t.toInt();
                    if(num == n)
                    {
                        _btn[i][k].setStyleSheet("color: red");
                        conflict = true;
                    }
                }
                // 检查列是否有冲突
                for(int k = 0; k < 9; k++)
                {
                    if(k == i) continue; // 跳过当前位置
                    QString t = _btn[k][j].text();
                    if(t == "") continue; // 跳过空格
                    int n = t.toInt();
                    if(num == n)
                    {
                        _btn[k][j].setStyleSheet("color: red");
                        conflict = true;
                    }
                }
                // 检查九宫格是否有冲突
                int m = i / 3 * 3, n = j / 3 * 3;
                for(int p = m; p < m + 3; p++)
                {
                    for(int q = n; q < n + 3; q++)
                    {
                        if(p == i && q == j) continue; // 跳过当前位置
                        QString t = _btn[p][q].text();
                        if(t == "") continue; // 跳过空格
                        int k = t.toInt();
                        if(num == k)
                        {
                            _btn[p][q].setStyleSheet("color: red");
                            conflict = true;
                        }
                    }
                }
                // 没有冲突
                if(!conflict)
                    _btn[i][j].setStyleSheet("color: blue");
            }
        }
    });

    //更换难度
    connect(ui->btn_change, &QPushButton::clicked, [=]()
    {
        QRandomGenerator rnd;
        rnd.seed(time(0));
        // 删除原数独
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                _btn[i][j].setStyleSheet("color: blue");
                _btn[i][j].setText("");
                _btn[i][j].setEnabled(true);
                _num[i][j] = 0;
                _num2[i][j]=0;
            }
        }
        //选择难度
        QMessageBox* choice=new QMessageBox(this);
        choice->setWindowTitle("困难度选择");
        choice->setText("请选择游戏难度");
        choice->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::Ok);
        choice->setButtonText(QMessageBox::Yes,"简单");
        choice->setButtonText(QMessageBox::Cancel,"困难");
        choice->setButtonText(QMessageBox::Ok,"中等");
        choice->setDefaultButton(QMessageBox::Yes);//默认难度为简单
        auto tmp=choice->exec();
        nums=0;
        if(tmp==QMessageBox::Yes)//选择简单难度
        {
            create(40);
            nums=40;
        }
        else if(tmp==QMessageBox::Ok)//选择中等难度
        {
            create(30);
            nums=30;
        }
        else if(tmp==QMessageBox::Cancel)//选择困难难度
        {
            create(20);
            nums=20;
        }
    });


    //显示答案
    connect(ui->btn_answer, &QPushButton::clicked, [=]()
    {
        // 复制当前数独面板
        int board[9][9];
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                board[i][j] = _num[i][j];
            }
        }
        // 使用回溯算法求解数独
        bool success = backtrack(board, 0, 0);
        // 如果求解成功，则将结果显示在界面上
        if(success)
        {
            for(int i = 0; i < 9; i++)
            {
                for(int j = 0; j < 9; j++)
                {
                    _btn[i][j].setStyleSheet("color: black");
                    _btn[i][j].setEnabled(false);
                    setNum(i, j, board[i][j]);
                }
            }
        }
        else
        {
            // 如果求解失败，则弹出提示框
            QMessageBox::information(this, tr("无解"), tr("该数独无解！"));
        }
    });

    //重玩
    connect(ui->btn_replay, &QPushButton::clicked, [=]()
    {
        // 将所有方格设置为可编辑状态
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                _btn[i][j].setEnabled(true);
            }
        }
        // 删除原数独
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                _btn[i][j].setText("");
            }
        }
        // 将拷贝的数独填入方格中
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (_num2[i][j] != 0)
                {
                    _btn[i][j].setStyleSheet("color: blue");
                    _btn[i][j].setText(QString::number(_num2[i][j]));
                    _btn[i][j].setEnabled(false);
                }
            }
        }
    });


    //提交答案
    connect(ui->btn_submit,&QPushButton::clicked,[=]()
    {
        // 用于保存每一行、每一列和每个3x3单元格中出现的数字
        bool row[9][9] = { false };
        bool col[9][9] = { false };
        bool cell[3][3][9] = { false };
        // 遍历数独中的所有单元格
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // 获取当前单元格的数字
                int num = _btn[i][j].text().toInt() - 1;
                // 检查该数字是否已经出现在该行、该列或该单元格中,单元格数字缺失也错误
                if (num < 0||row[i][num] || col[j][num] || cell[i/3][j/3][num])
                {
                    // 如果已经出现，则数独有误
                    QMessageBox::information(this, "遗憾", "数独有误！");
                    return;
                }

                // 如果没有出现，则将该数字添加到该行、该列和该单元格中
                row[i][num] = true;
                col[j][num] = true;
                cell[i/3][j/3][num] = true;
            }
        }
        // 如果遍历完数独中的所有单元格都没有发现错误，则数独正确
        QMessageBox::information(this, "恭喜", "数独正确！");
    });
}

void Widget::setNum(int r, int c, int num)
{
    _btn[r][c].setStyleSheet("color: black");
    _btn[r][c].setText(QString::number(num));
}

void Widget::clearNum(int r, int c)
{
    _btn[r][c].setText("");
}


Widget::~Widget()
{
    delete ui;
}


