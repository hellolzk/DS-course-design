#include "school_info2.h"
#include "ui_school_info2.h"
#include "mypushbutton.h"
#include <QMenuBar>
#include <QTimer>
#include <QPainter>
#include <QTableWidget>
#include <QFileDialog>
#include <ActiveQt/QAxObject>
#include <QMessageBox>
#include <QAction>
#include <QDialog>
#include <QFile>
#include <QString>
#include <QDir>
#include <QHBoxLayout>
#include <QDesktopServices>
school_info2::school_info2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::school_info2)
{
    ui->setupUi(this);
    //初始化固定大小
    this->setFixedSize(475,800);
    //设置软件图标
    this->setWindowIcon(QIcon(":/res/school_symbol.jfif"));
    //设置标题
    this->setWindowTitle("楼宇信息");
    //创建菜单栏
    QMenuBar* bar=menuBar();
    setMenuBar(bar);
    //创建开始菜单
    QMenu*startmenu=bar->addMenu("开始");
    //创建退出 菜单项
    QAction*quitAction=startmenu->addAction("导出到Excel");
    QAction*quitAction2=startmenu->addAction("退出");
    //点击退出，实现退出
    connect(quitAction2,&QAction::triggered,[=](){
       this->close();
    });
    //点击保存，实现保存
    ui->progressBar->hide();
    connect(quitAction,&QAction::triggered,[=](){
       action_save();
    });
    //返回按钮
    mypushbutton*backbtn=new mypushbutton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backbtn->setParent(this);
    backbtn->move(this->width()-backbtn->width(),this->height()-backbtn->height());
    //点击返回
    connect(backbtn,&mypushbutton::clicked,[=](){
        //告诉主场景 我返回了 主场景监听返回按钮
        //延时返回
        //播放返回音效
        //backsound->play();
        QTimer::singleShot(100,this,[=](){
            emit this->chooseSceneBack();
        });
    });
    //设置列数
    ui->tableWidget->setColumnCount(2);
    //设置水平表头
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"楼宇"<<"主要功能");
    //设置行数
    ui->tableWidget->setRowCount(34);
    //设置每列宽度
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,200);
    //设置所有单元格不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置列宽不可拖拽
    //ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //设置行高不可拖拽
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //设置字体样式
    ui->tableWidget->setFont(QFont("宋体",10));
    //随行变色
    ui->tableWidget->setAlternatingRowColors(true);
    //将表格中的值初始化为空，方便设置内容居中时做条件判断
    for(int i=0;i<34;i++){
        for(int j=0;j<2;j++){
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(tr("")));
        }
    }
    //设置正文
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("亚瑟"));
    QStringList namelist;
    namelist<<"第一教学楼"<<"第二教学楼"<<"第三教学楼"<<"第四教学楼"<<"逸夫图书馆"<<"金工楼"<<"人文楼"<<"机电楼"<<"基础楼"<<"建工楼"
           <<"数理楼"<<"材料楼"<<"信息楼"<<"软件楼"<<"城建楼"<<"艺术楼"<<"生命楼"<<"理科楼"<<"环能楼"<<"经管楼"<<"科学楼"<<"建国饭店"<<"学生综合服务楼"
            <<"美食园"<<"礼堂"<<"南操场"<<"北操场"<<"奥林匹克体育馆"<<"游泳馆"<<"篮球场"<<"网球场"<<"排球场"<<"知新园"<<"校医院";
    QStringList funclist;
    funclist<<"授课、自习"<<"授课、自习"<<"授课、自习"<<"授课、自习"<<"阅读、自习"<<"金工实习"<<"授课、办公、展览"<<"授课、办公"<<"授课、办公"
              <<"授课、办公"<<"授课、办公"<<"授课、办公"<<"授课、办公"<<"授课、办公"<<"授课、办公"<<"授课、办公"<<"授课、办公"<<"授课、办公"
                <<"授课、办公"<<"授课、办公"<<"办公"<<"餐饮、住宿"<<"餐饮、学生服务"<<"餐饮"<<"会议"<<"授课、运动"<<"授课、运动（施工中）"<<"羽毛球、核酸检测、展览"
                  <<"授课、游泳"<<"授课、打篮球"<<"授课、打网球"<<"授课、打排球"<<"办公、学生服务"<<"医疗、就诊";
    for (int i=0;i<34;i++) {
        int count=0;
        ui->tableWidget->setItem(i,count++,new QTableWidgetItem(namelist[i]));
        ui->tableWidget->setItem(i,count++,new QTableWidgetItem(funclist[i]));
    }
    //将表格中的内容居中显示
    for( int i = 0;i<34;i++){
        for(int j=0;j<2;j++){
            //如果内容不为空，就设置其内容居中
            if(ui->tableWidget->item(i,j)->text()!=""){
                ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
}

void school_info2:: action_save()
{
    ui->progressBar->setValue(0);   //设置进度条的值为0
    QString fileName = QFileDialog::getSaveFileName(this,tr("Excle file"),QString("./build_info.xlsx"),tr("Excel Files(*.xlsx)"));    //设置保存的文件名
    if(fileName != "")
    {
        ui->progressBar->show();    //进度条需要在ui文件中加个progressBar控件
        ui->progressBar->setValue(10);
        QAxObject *excel = new QAxObject("Excel.Application",this);
        if(excel->setControl("Excel.Application"))
        {
            excel->dynamicCall("SetVisible (bool Visible)",false);
            excel->setProperty("DisplayAlerts",false);
            QAxObject *workbooks = excel->querySubObject("WorkBooks");            //获取工作簿集合
            workbooks->dynamicCall("Add");                                        //新建一个工作簿
            QAxObject *workbook = excel->querySubObject("ActiveWorkBook");        //获取当前工作簿
            QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);
            QAxObject *cell;

            /*添加Excel表头数据*/
            for(int i = 1; i <= ui->tableWidget->columnCount(); i++)
            {
                cell=worksheet->querySubObject("Cells(int,int)", 1, i);
                cell->setProperty("RowHeight", 40);
                cell->dynamicCall("SetValue(const QString&)", ui->tableWidget->horizontalHeaderItem(i-1)->data(0).toString());
                if(ui->progressBar->value()<=50)
                {
                    ui->progressBar->setValue(10+i*5);
                }
            }

            /*将form列表中的数据依此保存到Excel文件中*/
            for(int j = 2; j<=ui->tableWidget->rowCount()+1;j++)
            {
                for(int k = 1;k<=ui->tableWidget->columnCount();k++)
                {
                    cell=worksheet->querySubObject("Cells(int,int)", j, k);
                    cell->dynamicCall("SetValue(const QString&)",ui->tableWidget->item(j-2,k-1)->text()+ "\t");
                }
                if(ui->progressBar->value()<80)
                {
                    ui->progressBar->setValue(50+j*5);
                }
            }

            /*将生成的Excel文件保存到指定目录下*/
            workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName)); //保存至fileName
            workbook->dynamicCall("Close()");                                                   //关闭工作簿
            excel->dynamicCall("Quit()");                                                       //关闭excel
            delete excel;
            excel=NULL;

            ui->progressBar->setValue(100);
            if (QMessageBox::question(NULL,QString::fromUtf8("完成"),QString::fromUtf8("文件已经导出，是否现在打开？"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
            }
            ui->progressBar->setValue(0);
            ui->progressBar->hide();
        }
    }
    ui->progressBar->hide();
}
school_info2::~school_info2()
{
    delete ui;
}
