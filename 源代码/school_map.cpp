#include "school_map.h"
#include "mypushbutton.h"
#include <QMenuBar>
#include <QPainter>
#include <QTimer>
#include <QFileDialog>
#include <QGuiApplication>
#include <QScreen>
school_map::school_map(QWidget *parent) : QMainWindow(parent)
{
    //初始化固定大小
    this->setFixedSize(1355,806);
    //设置软件图标
    this->setWindowIcon(QIcon(":/res/school_symbol.jfif"));
    //设置标题
    this->setWindowTitle("校园地图");
    //创建菜单栏
    QMenuBar* bar=menuBar();
    setMenuBar(bar);
    //创建开始菜单
    QMenu*startmenu=bar->addMenu("开始");
    //创建保存
    QAction*saveAction=startmenu->addAction("保存");
    //创建退出 菜单项
    QAction*quitAction=startmenu->addAction("退出");
    //点击退出，实现退出
    connect(quitAction,&QAction::triggered,[=](){
       this->close();
    });
    //点击保存，实现保存本地
    connect(saveAction,&QAction::triggered,[=](){
        QString filename1 = QFileDialog::getSaveFileName(this,tr("保存校园地图"),QString("./school_map.png"),tr("Images (*.png *.bmp *.jpg)")); //选择路径
        QScreen *screen = QGuiApplication::primaryScreen();
        screen->grabWindow(this->winId()).save(filename1);
    });
    //返回按钮
    mypushbutton*backbtn=new mypushbutton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backbtn->setParent(this);
    backbtn->move(this->width()-backbtn->width(),this->height()-backbtn->height());
    //点击返回
    connect(backbtn,&mypushbutton::clicked,[=](){
        QTimer::singleShot(100,this,[=](){
            emit this->chooseSceneBack();
        });
    });
}

void school_map:: paintEvent(QPaintEvent*)
{
    QPainter painter(this);//创建画家类
    QPixmap pix;//创建图片类
    pix.load(":/res/school_map(2).png");//上传背景图片
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//画图片

}
