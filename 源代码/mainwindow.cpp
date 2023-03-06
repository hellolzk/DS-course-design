#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mypushbutton.h"
#include <QPainter>
#include <QSize>
#include <QTimer>
#include <QLabel>
#include <QDesktopWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //配置主场景
    //设置固定大小
    setFixedSize(320,588);
    //设置软件图标
    setWindowIcon(QIcon(":/res/school_symbol.jfif"));
    //设置标题
    setWindowTitle("校园导游");
    //退出按钮的实现
    connect(ui->actionquit,&QAction::triggered,[=](){
        this->close();
    });
    //实例化地图界面
    sch_map=new school_map;
    //实例化校园信息界面
    sch_info2=new school_info2;
    //实例化校园导航界面
    nav=new navigation;
    //显示地图按钮
    mypushbutton *mapBtn=new mypushbutton(":/res/map_button(2).png");
    mapBtn->setParent(this);
    mapBtn->move(this->width()/2-mapBtn->width()/2,435);
    //校园地图文字
    QLabel *label=new QLabel(this);
    label->setParent(this);
    label->resize(130,20);
    label->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str=QString("校园地图");
    //将字体设置到标签中
    label->setFont(font);
    label->setText(str);
    label->setGeometry(this->width()/2-label->width()/2,mapBtn->y()+50,label->width(),50);
    //显示校园信息按钮
    mypushbutton *school_infoBtn=new mypushbutton(":/res/build_button.png");
    school_infoBtn->setParent(this);
    school_infoBtn->move(this->width()/2-school_infoBtn->width()/2,259);
    //校园信息文字
    QLabel *label2=new QLabel(this);
    label2->setParent(this);
    label2->resize(130,20);
    label2->setAlignment(Qt::AlignCenter);
    QFont font2;
    font2.setFamily("华文新魏");
    font2.setPointSize(20);
    QString str2=QString("楼宇信息");
    //将字体设置到标签中
    label2->setFont(font2);
    label2->setText(str2);
    label2->setGeometry(this->width()/2-label2->width()/2,school_infoBtn->y()+50,label->width(),50);
    //显示导航按钮
    mypushbutton *school_naviBtn=new mypushbutton(":/res/navigation.png");
    school_naviBtn->setParent(this);
    school_naviBtn->move(this->width()/2-school_naviBtn->width()/2,83);
    //校园信息文字
    QLabel *label3=new QLabel(this);
    label3->setParent(this);
    label3->resize(130,20);
    label3->setAlignment(Qt::AlignCenter);
    QFont font3;
    font3.setFamily("华文新魏");
    font3.setPointSize(20);
    QString str3=QString("校园导航");
    //将字体设置到标签中
    label3->setFont(font3);
    label3->setText(str3);
    label3->setGeometry(this->width()/2-label3->width()/2,school_naviBtn->y()+50,label->width(),50);
    QDesktopWidget *pDeskWdg = QApplication::desktop();
    QRect rctAvaild = pDeskWdg->availableGeometry();
    this->move((rctAvaild.width() - this->width()) / 2, (rctAvaild.height() - this->height()) / 2);
    //从校园地图返回主场景
    connect(sch_map,&school_map::chooseSceneBack,[=](){
        //this->setGeometry(sch_map->geometry());
        sch_map->hide();
        this->move((rctAvaild.width() - this->width()) / 2, (rctAvaild.height() - this->height()) / 2);
        this->show();//重新显示主场景
    });
    //从校园信息返回主场景
    connect(sch_info2,&school_info2::chooseSceneBack,[=](){
        //this->setGeometry(sch_info2->geometry());
        sch_info2->hide();
        this->move((rctAvaild.width() - this->width()) / 2, (rctAvaild.height() - this->height()) / 2);
        this->show();//重新显示主场景
    });
    //从校园导航返回主场景
    connect(nav,&navigation::chooseSceneBack,[=](){
        //this->setGeometry(nav->geometry());
        nav->hide();
        this->move((rctAvaild.width() - this->width()) / 2, (rctAvaild.height() - this->height()) / 2);
        this->show();//重新显示主场景
    });
    //显示校园地图界面
    connect(mapBtn,&QPushButton::clicked,[=](){
        mapBtn->zoom1();
        mapBtn->zoom2();
        //延时进入到校园地图
        QTimer::singleShot(100,this,[=](){
            this->hide();
            //显示校园地图界面
            sch_map->move((rctAvaild.width() - sch_map->width()) / 2, (rctAvaild.height() - sch_map->height()) / 2);
            sch_map->show();
        });
    });
    //显示楼宇信息界面
    connect(school_infoBtn,&QPushButton::clicked,[=](){
        school_infoBtn->zoom1();
        school_infoBtn->zoom2();
        //延时进入到选择关卡
        QTimer::singleShot(100,this,[=](){
            this->hide();
            //显示楼宇信息界面
            sch_info2->move((rctAvaild.width() - sch_info2->width()) / 2, (rctAvaild.height() - sch_info2->height()) / 2);
            sch_info2->show();
        });
    });

    //显示校园导航界面
    connect(school_naviBtn,&QPushButton::clicked,[=](){
        school_naviBtn->zoom1();
        school_naviBtn->zoom2();
        //延时进入到选择关卡
        QTimer::singleShot(100,this,[=](){
            this->hide();
            //显示楼宇信息界面
            nav->move((rctAvaild.width() - nav->width()) / 2, (rctAvaild.height() - nav->height()) / 2);
            nav->show();
        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent*)
{

    QPainter painter(this);//创建画家类
    QPixmap pix;//创建图片类
    pix.load(":/res/welcome_bkground.jpg");//上传背景图片
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//画图片
}

