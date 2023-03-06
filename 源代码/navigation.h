#ifndef NAVIGATION_H
#define NAVIGATION_H
#define UNVISITED 0
#define VISITED 1
#define INF 9999
#include <QMainWindow>
#include <QLabel>
#include "linklist.h"
#include <QAbstractButton>
namespace Ui {
class navigation;
}

class navigation : public QMainWindow
{
    Q_OBJECT

public:
    explicit navigation(QWidget *parent = nullptr);
    ~navigation();
    //重写paintEvent事件
    void paintEvent(QPaintEvent*);
    //事件过滤器
    //重写按下和释放
    void mousePressEvent(QMouseEvent*e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void setlabel();
    void clear_select();
    void onButtonClicked(QAbstractButton *button);
    void hide_of_label_num();
    QButtonGroup *btnGroup;  //添加buttongroup
    int speed_of_user=2;//默认为步行，步行2m/s，骑行7m/s
    int symbol=0;
    int flag=0;//控制是否绘制路径
    int length=0;//路径长度
    int time_of_travel=1;//周游时间
    int *xiaoyuanditu=NULL;
    int *save_position=NULL;
    QString most_best_travel="";//最佳路径
    //选择顶点
    QLabel*label1=NULL;
    QLabel*label2=NULL;
    QLabel*label3=NULL;
    QLabel*label4=NULL;
    QLabel*label5=NULL;
    QLabel*label6=NULL;
    QLabel*label7=NULL;
    QLabel*label8=NULL;
    QLabel*label9=NULL;
    QLabel*label10=NULL;
    //选择起点
    QLabel*label1st=NULL;
    QLabel*label2st=NULL;
    QLabel*label3st=NULL;
    QLabel*label4st=NULL;
    QLabel*label5st=NULL;
    QLabel*label6st=NULL;
    QLabel*label7st=NULL;
    QLabel*label8st=NULL;
    QLabel*label9st=NULL;
    QLabel*label10st=NULL;

    //路径编号
    QLabel*label1num=NULL;
    QLabel*label2num=NULL;
    QLabel*label3num=NULL;
    QLabel*label4num=NULL;
    QLabel*label5num=NULL;
    QLabel*label6num=NULL;
    QLabel*label7num=NULL;
    QLabel*label8num=NULL;
    QLabel*label9num=NULL;
    QLabel*label10num=NULL;

    int lock1=0;
    int lock2=0;
    int lock3=0;
    int lock4=0;
    int lock5=0;
    int lock6=0;
    int lock7=0;
    int lock8=0;
    int lock9=0;
    int lock10=0;

    //标记是否被选中
    int is_select1=0;
    int is_select2=0;
    int is_select3=0;
    int is_select4=0;
    int is_select5=0;
    int is_select6=0;
    int is_select7=0;
    int is_select8=0;
    int is_select9=0;
    int is_select10=0;


    int num_of_node=0;//顶点个数
    int have_searched=0;
    LinkList*nodelist=NULL;

    int start_pos=0;

    //地图邻接矩阵
    int ceshi[10][10] = { {INF,420,650,600,550,640,860,410,460,350},
          {420,INF,420,300,240,230,440,660,580,180},
    {650,420,INF,610,220,300,460,760,310,220
} ,{600,300,610,INF,480,310,290,1000,880,480
},{550,240,220,480,INF,170,330,750,390,130
} ,{640,230,300,310,170,INF,230,890,560,300
} ,{860,440,460,290,330,230,INF,1100,720,460},
    {410,660,760,1000,750,890,1100,INF,350,550 },
    {460,580,310,880,390,560,720,350,INF,330},
    {350,180,220,480,130,300,460,550,330,INF }
    };


private:
    Ui::navigation *ui;

signals:
    //写一个自定义的信号，告诉主场景点击的返回
    void chooseSceneBack();
};

#endif // NAVIGATION_H
