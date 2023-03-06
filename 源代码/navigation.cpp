#include "navigation.h"
#include "ui_navigation.h"
#include "mypushbutton.h"
#include "undirected_graph_matrix.h"
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QScreen>
#include <QTime>
#include <QButtonGroup>
navigation::navigation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::navigation)
{
    ui->setupUi(this);

    //初始化固定大小
    this->setFixedSize(1600,806);

    //设置软件图标
    this->setWindowIcon(QIcon(":/res/school_symbol.jfif"));

    //设置标题
    this->setWindowTitle("校园导航");

    //返回按钮
    mypushbutton*backbtn=new mypushbutton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backbtn->setParent(this);
    backbtn->move(this->width()-backbtn->width(),this->height()-backbtn->height());

    //点击返回
    connect(backbtn,&mypushbutton::clicked,[=](){
        //告诉主场景 我返回了 主场景监听返回按钮
        //延时返回
        QTimer::singleShot(100,this,[=](){
            emit this->chooseSceneBack();
        });
    });
    btnGroup= new QButtonGroup(this);
    btnGroup->addButton(ui->radioButton_walk);
    btnGroup->addButton(ui->radioButton_bike);
    ui->radioButton_walk->setChecked(1);
    btnGroup->setId(ui->radioButton_walk,0);
    btnGroup->setId(ui->radioButton_bike,1);
    connect(ui->radioButton_walk,&QPushButton::clicked,[=](){
        speed_of_user=2;
        if(flag==1)
        {
            ui->textBrowser->setText("最佳路径：");
            ui->textBrowser->append(most_best_travel);
            ui->textBrowser->append("路径长度：");
            ui->textBrowser->append(QString("%1").arg(length));
            time_of_travel=length/speed_of_user/60;
            ui->textBrowser->append("预估时间：");
            ui->textBrowser->append(QString("%1min").arg(time_of_travel));
        }
        qDebug()<<speed_of_user;
    });
    connect(ui->radioButton_bike,&QPushButton::clicked,[=](){
        speed_of_user=5;
        if(flag==1)
        {
            ui->textBrowser->setText("最佳路径：");
            ui->textBrowser->append(most_best_travel);
            ui->textBrowser->append("路径长度：");
            ui->textBrowser->append(QString("%1").arg(length));
            time_of_travel=length/speed_of_user/60;
            ui->textBrowser->append("预估时间：");
            ui->textBrowser->append(QString("%1min").arg(time_of_travel));
        }
        qDebug() << speed_of_user;
    });

    ui->groupBox->setGeometry(1378,80,111,79);
    ui->search_button->setGeometry(1378,30+15+100+40+10,93,28);//查询按钮
    ui->label->setGeometry(1378,70+15+100+50+10,72,15);//查询结果字符
    ui->textBrowser->setGeometry(1378,90+105+50+30,200,192);//查询结果显示窗口
    ui->clear_search->setGeometry(1378,535,93,28);//清空按钮
    ui->save_button->setGeometry(1378,608,93,28);//保存按钮
    ui->use_help->setGeometry(1378,636+45,93,28);//使用指南按钮
    ui->quit_button->setGeometry(1378,754,93,28);//退出按钮


    //查询按钮
    connect(ui->search_button,&QPushButton::clicked,[=](){
        if(num_of_node!=0&&start_pos!=0&&num_of_node!=1)
        {
            hide_of_label_num();
            have_searched=1;
            unDirected_Graph_matrix*school_nav=new unDirected_Graph_matrix(num_of_node);
            school_nav->init_edge();
            school_nav->set_xiaoyuanditu(nodelist);
            school_nav->tiqu_ditu(ceshi);
            length=school_nav->TSP(start_pos,1,0,start_pos);
            //school_nav->print_TSP();
            ui->textBrowser->setText("最佳路径：");
            most_best_travel=school_nav->print_TSP();
            ui->textBrowser->append(most_best_travel);
            ui->textBrowser->append("路径长度：");
            ui->textBrowser->append(QString("%1").arg(length));
            time_of_travel=length/speed_of_user/60;
            ui->textBrowser->append("预估时间：");
            ui->textBrowser->append(QString("%1min").arg(time_of_travel));
            school_nav->print_save_pos();
            if(xiaoyuanditu!=NULL)
            {
                delete [] xiaoyuanditu;
                xiaoyuanditu=NULL;
            }
            if(save_position!=NULL)
            {
                delete [] save_position;
                save_position=NULL;
            }
            xiaoyuanditu=new int[num_of_node];
            save_position=new int[num_of_node];
            for(int i=0;i<num_of_node;i++)
            {
                xiaoyuanditu[i]=school_nav->xiaoyuanditu[i];
                save_position[i]=school_nav->save_position[i];
            }
            //xiaoyuanditu=school_nav->xiaoyuanditu;
            //save_position=school_nav->save_position;
            flag=1;
            QWidget::update();
            //qDebug()<<length;
            delete school_nav;
        }
        else if(num_of_node==1)
        {
            hide_of_label_num();
            flag=0;
            QWidget::update();
            ui->textBrowser->clear();
            QMessageBox::warning(this,"warning","至少选择两个地点");
        }
        else if(num_of_node==0)
        {
            hide_of_label_num();
            flag=0;
            QWidget::update();
            ui->textBrowser->clear();
            QMessageBox::warning(this,"warning","请选择地点");
        }
        else if(start_pos==0)
        {
            hide_of_label_num();
            flag=0;
            QWidget::update();
            ui->textBrowser->clear();
            QMessageBox::warning(this,"warning","请选择起点");
        }
    });

    //使用指南按钮
    connect(ui->use_help,&QPushButton::clicked,[=](){
        QString dlgTitle="使用指南";
        QString strInfo="地点选择： \n单击鼠标左键选择地点 \n单击鼠标右键取消选择 \n未选中状态编号显示黑色 \n"
                        "选中状态编号显示红色 \n\n起点选择：\n双击鼠标左键选择起点，再次双击或单击右键取消选择 \n\n点击查询，即可查询路径（支持步行和骑行） \n"
                        "点击清空，即可清空选项 \n点击保存，即可将地图与路径保存至本地";
        QMessageBox::about(this, dlgTitle, strInfo);
    });

    //清空按钮
    connect(ui->clear_search,&QPushButton::clicked,[=](){
        clear_select();
        flag=0;
        QWidget::update();//刷新界面
    });

    //保存按钮
    connect(ui->save_button,&QPushButton::clicked,[=](){
        QString filename1 = QFileDialog::getSaveFileName(this,tr("保存路径"),QString("./navigation.png"),tr("Images (*.png *.bmp *.jpg)")); //选择路径
        QScreen *screen = QGuiApplication::primaryScreen();
        screen->grabWindow(this->winId()).save(filename1);
    });

    //退出按钮
    connect(ui->quit_button,&QPushButton::clicked,[=](){
        this->close();
    });

    setlabel();
}

navigation::~navigation()
{
    delete ui;
}

void navigation:: onButtonClicked(QAbstractButton *button)
{
    Q_UNUSED(button)
    int sigChoose = btnGroup->checkedId();  //选择信号
    if(sigChoose==0) qDebug() << sigChoose;
    else if(sigChoose==1) qDebug() << sigChoose;
}

void navigation:: paintEvent(QPaintEvent*)
{
    QPainter painter(this);//创建画家类
    QPixmap pix;//创建图片类
    pix.load(":/res/school_map(2).png");//上传背景图片
    painter.drawPixmap(0,0,1355,806,pix);//画图片
    QPen pen(QColor(0,255,0));
    pen.setWidth(7);
    //使用画笔
    painter.setPen(pen);
    if(flag==1)
    {
        int start_x=0;
        int start_y=0;
        int end_x=0;
        int end_y=0;
        int head_x=0;
        int head_y=0;
        int tail_x=0;
        int tail_y=0;
        for(int i=0;i<num_of_node-1;i++)
        {
            switch(xiaoyuanditu[save_position[i]])
            {
            case 1:
                start_x=1082;
                start_y=425;
                break;
            case 2:
                start_x=778;
                start_y=523;
                break;
            case 3:
                start_x=459;
                start_y=318;
                break;
            case 4:
                start_x=609;
                start_y=632;
                break;
            case 5:
                start_x=553;
                start_y=350;
                break;
            case 6:
                start_x=551;
                start_y=452;
                break;
            case 7:
                start_x=367;
                start_y=438;
                break;
            case 8:
                start_x=1114;
                start_y=263;
                break;
            case 9:
                start_x=772;
                start_y=203;
                break;
            case 10:
                start_x=716;
                start_y=396;
                break;
            default:
                break;
            }

            switch(xiaoyuanditu[save_position[i+1]])
            {
            case 1:
                end_x=1082;
                end_y=425;
                break;
            case 2:
                end_x=778;
                end_y=523;
                break;
            case 3:
                end_x=459;
                end_y=318;
                break;
            case 4:
                end_x=609;
                end_y=632;
                break;
            case 5:
                end_x=553;
                end_y=350;
                break;
            case 6:
                end_x=551;
                end_y=452;
                break;
            case 7:
                end_x=367;
                end_y=438;
                break;
            case 8:
                end_x=1114;
                end_y=263;
                break;
            case 9:
                end_x=772;
                end_y=203;
                break;
            case 10:
                end_x=716;
                end_y=396;
                break;
            default:
                break;
            }
            painter.setRenderHint(QPainter::Antialiasing,true);
            painter.drawLine(start_x,start_y,end_x,end_y);
            switch(i)
            {
            case 0:
                label1num->setGeometry((start_x+end_x)/2,(start_y+end_y)/2,120,50);
                label1num->show();
                break;
            case 1:
                label2num->setGeometry((start_x+end_x)/2,(start_y+end_y)/2,120,50);
                label2num->show();
                break;
            case 2:
                label3num->setGeometry((start_x+end_x)/2,(start_y+end_y)/2,120,50);
                label3num->show();
                break;
            case 3:
                label4num->setGeometry((start_x+end_x)/2,(start_y+end_y)/2,120,50);
                label4num->show();
                break;
            case 4:
                label5num->setGeometry((start_x+end_x)/2,(start_y+end_y)/2,120,50);
                label5num->show();
                break;
            case 5:
                label6num->setGeometry((start_x+end_x)/2,(start_y+end_y)/2,120,50);
                label6num->show();
                break;
            case 6:
                label7num->setGeometry((start_x+end_x)/2,(start_y+end_y)/2,120,50);
                label7num->show();
                break;
            case 7:
                label8num->setGeometry((start_x+end_x)/2,(start_y+end_y)/2,120,50);
                label8num->show();
                break;
            case 8:
                label9num->setGeometry((start_x+end_x)/2,(start_y+end_y)/2,120,50);
                label9num->show();
                break;
            default:
                break;
            }

            if(i==0)//保存起点
            {
                head_x=start_x;
                head_y=start_y;
            }
            if(i==num_of_node-2)//保存最后一个点
            {
                tail_x=end_x;
                tail_y=end_y;
            }
        }
        painter.setRenderHint(QPainter::Antialiasing,true);
        painter.drawLine(tail_x,tail_y,head_x,head_y);
        switch(num_of_node)
        {
        case 2:
            label2num->setGeometry((tail_x+head_x)/2,(tail_y+head_y)/2,120,50);
            label2num->show();
            break;
        case 3:
            label3num->setGeometry((tail_x+head_x)/2,(tail_y+head_y)/2,120,50);
            label3num->show();
            break;
        case 4:
            label4num->setGeometry((tail_x+head_x)/2,(tail_y+head_y)/2,120,50);
            label4num->show();
            break;
        case 5:
            label5num->setGeometry((tail_x+head_x)/2,(tail_y+head_y)/2,120,50);
            label5num->show();
            break;
        case 6:
            label6num->setGeometry((tail_x+head_x)/2,(tail_y+head_y)/2,120,50);
            label6num->show();
            break;
        case 7:
            label7num->setGeometry((tail_x+head_x)/2,(tail_y+head_y)/2,120,50);
            label7num->show();
            break;
        case 8:
            label8num->setGeometry((tail_x+head_x)/2,(tail_y+head_y)/2,120,50);
            label8num->show();
            break;
        case 9:
            label9num->setGeometry((tail_x+head_x)/2,(tail_y+head_y)/2,120,50);
            label9num->show();
            break;
        case 10:
            label10num->setGeometry((tail_x+head_x)/2,(tail_y+head_y)/2,120,50);
            label10num->show();
            break;
        default:
            break;
        }
    }
}

void navigation:: setlabel()
{
    QStringList numlist;
    numlist<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10";
    label1=new QLabel(this);
    label1->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    //将字体设置到标签中
    label1->setFont(font);
    label1->setText(numlist[0]);
    label1->setGeometry((1053+1111)/2-10,(399+452)/2-30,120,50);

    label2=new QLabel(this);
    label2->setParent(this);
    //将字体设置到标签中
    label2->setFont(font);
    label2->setText(numlist[1]);
    label2->setGeometry((748+808)/2-10,(479+568)/2-20,120,50);

    label3=new QLabel(this);
    label3->setParent(this);
    //将字体设置到标签中
    label3->setFont(font);
    label3->setText(numlist[2]);
    label3->setGeometry((449+470)/2-10,(285+351)/2-20,120,50);

    label4=new QLabel(this);
    label4->setParent(this);
    //将字体设置到标签中
    label4->setFont(font);
    label4->setText(numlist[3]);
    label4->setGeometry((556+653)/2-10,(597+667)/2-20,120,50);

    label5=new QLabel(this);
    label5->setParent(this);
    //将字体设置到标签中
    label5->setFont(font);
    label5->setText(numlist[4]);
    label5->setGeometry((530+577)/2-10,(303+398)/2-20,120,50);

    label6=new QLabel(this);
    label6->setParent(this);
    //将字体设置到标签中
    label6->setFont(font);
    label6->setText(numlist[5]);
    label6->setGeometry((540+562)/2-10,(426+478)/2-20,120,50);

    label7=new QLabel(this);
    label7->setParent(this);
    //将字体设置到标签中
    label7->setFont(font);
    label7->setText(numlist[6]);
    label7->setGeometry((336+398)/2-10,(389+488)/2-20,120,50);

    label8=new QLabel(this);
    label8->setParent(this);
    //将字体设置到标签中
    label8->setFont(font);
    label8->setText(numlist[7]);
    label8->setGeometry((1100+1129)/2-10,(240+286)/2-20,120,50);

    label9=new QLabel(this);
    label9->setParent(this);
    //将字体设置到标签中
    label9->setFont(font);
    label9->setText(numlist[8]);
    label9->setGeometry((728+817)/2-10,(169+237)/2-20,120,50);

    label10=new QLabel(this);
    label10->setParent(this);
    //将字体设置到标签中
    label10->setFont(font);
    label10->setText(numlist[9]);
    label10->setGeometry((678+755)/2-10,(341+451)/2-20,120,50);

    label1st=new QLabel(this);
    label2st=new QLabel(this);
    label3st=new QLabel(this);
    label4st=new QLabel(this);
    label5st=new QLabel(this);
    label6st=new QLabel(this);
    label7st=new QLabel(this);
    label8st=new QLabel(this);
    label9st=new QLabel(this);
    label10st=new QLabel(this);

    label1num=new QLabel(this);
    label2num=new QLabel(this);
    label3num=new QLabel(this);
    label4num=new QLabel(this);
    label5num=new QLabel(this);
    label6num=new QLabel(this);
    label7num=new QLabel(this);
    label8num=new QLabel(this);
    label9num=new QLabel(this);
    label10num=new QLabel(this);

    QPalette mype_num;
    mype_num.setColor(QPalette::WindowText, Qt::yellow);
    label1num->setFont(font);
    label1num->setText(numlist[0]);
    label1num->setPalette(mype_num);

    label2num->setFont(font);
    label2num->setText(numlist[1]);
    label2num->setPalette(mype_num);

    label3num->setFont(font);
    label3num->setText(numlist[2]);
    label3num->setPalette(mype_num);

    label4num->setFont(font);
    label4num->setText(numlist[3]);
    label4num->setPalette(mype_num);

    label5num->setFont(font);
    label5num->setText(numlist[4]);
    label5num->setPalette(mype_num);

    label6num->setFont(font);
    label6num->setText(numlist[5]);
    label6num->setPalette(mype_num);

    label7num->setFont(font);
    label7num->setText(numlist[6]);
    label7num->setPalette(mype_num);

    label8num->setFont(font);
    label8num->setText(numlist[7]);
    label8num->setPalette(mype_num);

    label9num->setFont(font);
    label9num->setText(numlist[8]);
    label9num->setPalette(mype_num);

    label10num->setFont(font);
    label10num->setText(numlist[9]);
    label10num->setPalette(mype_num);

    if(flag!=1)
    {
        label1num->hide();
        label2num->hide();
        label3num->hide();
        label4num->hide();
        label5num->hide();
        label6num->hide();
        label7num->hide();
        label8num->hide();
        label9num->hide();
        label10num->hide();
    }
    nodelist=new LinkList();
}
void navigation:: mousePressEvent(QMouseEvent*e)
{
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);

    QPalette pe_unselect;
    pe_unselect.setColor(QPalette::WindowText, Qt::black);

    QPalette pe_select;
    pe_select.setColor(QPalette::WindowText, Qt::red);

    QStringList numlist;
    numlist<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10";

    if (e->button() == Qt::LeftButton)
    {                // 发生鼠标左击事件
        int x = this->mapFromGlobal(QCursor().pos()).x();   // 获取鼠标返回的横坐标
        int y = this->mapFromGlobal(QCursor().pos()).y();
        qDebug()<<x<<","<<y;
        if(x>=1053&&x<=1111&&y>=399&&y<=452)
        {
            delete label1;
            label1=NULL;
            label1=new QLabel(this);
            label1->setParent(this);
            //将字体设置到标签中
            label1->setFont(font);
            label1->setText(numlist[0]);
            label1->setPalette(pe_select);
            label1->setGeometry((1053+1111)/2-10,(399+452)/2-30,120,50);
            label1->show();
            if(is_select1==0)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node++;
                is_select1=1;
                nodelist->append(1);
            }
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=748&&x<=808&&y>=479&&y<=568)
        {
            delete label2;
            label2=NULL;
            label2=new QLabel(this);
            label2->setParent(this);
            //将字体设置到标签中
            label2->setFont(font);
            label2->setText(numlist[1]);
            label2->setPalette(pe_select);
            label2->setGeometry((748+808)/2-10,(479+568)/2-20,120,50);
            label2->show();
            if(is_select2==0)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node++;
                is_select2=1;
                nodelist->append(2);
            }
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=449&&x<=470&&y>=285&&y<=351)
        {
            delete label3;
            label3=NULL;
            label3=new QLabel(this);
            label3->setParent(this);
            //将字体设置到标签中
            label3->setFont(font);
            label3->setText(numlist[2]);
            label3->setPalette(pe_select);
            label3->setGeometry((449+470)/2-10,(285+351)/2-20,120,50);
            label3->show();
            if(is_select3==0)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node++;
                is_select3=1;
                nodelist->append(3);
            }
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=566&&x<=653&&y>=597&&y<=667)
        {
            delete label4;
            label4=NULL;
            label4=new QLabel(this);
            label4->setParent(this);
            //将字体设置到标签中
            label4->setFont(font);
            label4->setText(numlist[3]);
            label4->setPalette(pe_select);
            label4->setGeometry((556+653)/2-10,(597+667)/2-20,120,50);
            label4->show();
            if(is_select4==0)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node++;
                is_select4=1;
                nodelist->append(4);
            }
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=530&&x<=577&&y>=303&&y<=398)
        {
            delete label5;
            label5=NULL;
            label5=new QLabel(this);
            label5->setParent(this);
            //将字体设置到标签中
            label5->setFont(font);
            label5->setText(numlist[4]);
            label5->setPalette(pe_select);
            label5->setGeometry((530+577)/2-10,(303+398)/2-20,120,50);
            label5->show();
            if(is_select5==0)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node++;
                is_select5=1;
                nodelist->append(5);
            }
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=540&&x<=562&&y>=426&&y<=478)
        {
            delete label6;
            label6=NULL;
            label6=new QLabel(this);
            label6->setParent(this);
            //将字体设置到标签中
            label6->setFont(font);
            label6->setText(numlist[5]);
            label6->setPalette(pe_select);
            label6->setGeometry((540+562)/2-10,(426+478)/2-20,120,50);
            label6->show();
            if(is_select6==0)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node++;
                is_select6=1;
                nodelist->append(6);
            }
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=336&&x<=398&&y>=389&&y<=488)
        {
            delete label7;
            label7=NULL;
            label7=new QLabel(this);
            label7->setParent(this);
            //将字体设置到标签中
            label7->setFont(font);
            label7->setText(numlist[6]);
            label7->setPalette(pe_select);
            label7->setGeometry((336+398)/2-10,(389+488)/2-20,120,50);
            label7->show();
            if(is_select7==0)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node++;
                is_select7=1;
                nodelist->append(7);
            }
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=1100&&x<=1129&&y>=240&&y<=286)
        {
            delete label8;
            label8=NULL;
            label8=new QLabel(this);
            label8->setParent(this);
            //将字体设置到标签中
            label8->setFont(font);
            label8->setText(numlist[7]);
            label8->setPalette(pe_select);
            label8->setGeometry((1100+1129)/2-10,(240+286)/2-20,120,50);
            label8->show();
            if(is_select8==0)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node++;
                is_select8=1;
                nodelist->append(8);
            }
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=728&&x<=817&&y>=169&&y<=237)
        {
            delete label9;
            label9=NULL;
            label9=new QLabel(this);
            label9->setParent(this);
            //将字体设置到标签中
            label9->setFont(font);
            label9->setText(numlist[8]);
            label9->setPalette(pe_select);
            label9->setGeometry((728+817)/2-10,(169+237)/2-20,120,50);
            label9->show();
            if(is_select9==0)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node++;
                is_select9=1;
                nodelist->append(9);
            }
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=678&&x<=755&&y>=341&&y<=451)
        {
            delete label10;
            label10=NULL;
            label10=new QLabel(this);
            label10->setParent(this);
            //将字体设置到标签中
            label10->setFont(font);
            label10->setText(numlist[9]);
            label10->setPalette(pe_select);
            label10->setGeometry((678+755)/2-10,(341+451)/2-20,120,50);
            label10->show();
            if(is_select10==0)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node++;
                is_select10=1;
                nodelist->append(10);
            }
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
    }
    if(e->button()==Qt::RightButton)
    {
        int x = this->mapFromGlobal(QCursor().pos()).x();   // 获取鼠标返回的横坐标
        int y = this->mapFromGlobal(QCursor().pos()).y();
        qDebug()<<x<<","<<y;
        if(x>=1053&&x<=1111&&y>=399&&y<=452)
        {
            delete label1;
            label1=NULL;
            label1=new QLabel(this);
            label1->setParent(this);
            //将字体设置到标签中
            label1->setFont(font);
            label1->setText(numlist[0]);
            label1->setPalette(pe_unselect);
            label1->setGeometry((1053+1111)/2-10,(399+452)/2-30,120,50);
            label1->show();
            if(lock1==1)
            {
                symbol=0;
                lock1=0;
                label1st->hide();
                if(start_pos==1)
                {
                    start_pos=0;
                }
            }
            if(is_select1==1)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node--;
                is_select1=0;
            }
            nodelist->del(nodelist->getPos(1));
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=748&&x<=808&&y>=479&&y<=568)
        {
            delete label2;
            label2=NULL;
            label2=new QLabel(this);
            label2->setParent(this);
            //将字体设置到标签中
            label2->setFont(font);
            label2->setText(numlist[1]);
            label2->setPalette(pe_unselect);
            label2->setGeometry((748+808)/2-10,(479+568)/2-20,120,50);
            label2->show();
            if(lock2==1)
            {
                symbol=0;
                lock2=0;
                label2st->hide();
                if(start_pos==2)
                {
                    start_pos=0;
                }
            }
            if(is_select2==1)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node--;
                is_select2=0;
            }
            nodelist->del(nodelist->getPos(2));
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=449&&x<=470&&y>=285&&y<=351)
        {
            delete label3;
            label3=NULL;
            label3=new QLabel(this);
            label3->setParent(this);
            //将字体设置到标签中
            label3->setFont(font);
            label3->setText(numlist[2]);
            label3->setPalette(pe_unselect);
            label3->setGeometry((449+470)/2-10,(285+351)/2-20,120,50);
            label3->show();
            if(lock3==1)
            {
                symbol=0;
                lock3=0;
                label3st->hide();
                if(start_pos==3)
                {
                    start_pos=0;
                }
            }
            if(is_select3==1)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node--;
                is_select3=0;
            }
            nodelist->del(nodelist->getPos(3));
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=566&&x<=653&&y>=597&&y<=667)
        {
            delete label4;
            label4=NULL;
            label4=new QLabel(this);
            label4->setParent(this);
            //将字体设置到标签中
            label4->setFont(font);
            label4->setText(numlist[3]);
            label4->setPalette(pe_unselect);
            label4->setGeometry((556+653)/2-10,(597+667)/2-20,120,50);
            label4->show();
            if(lock4==1)
            {
                symbol=0;
                lock4=0;
                label4st->hide();
                if(start_pos==4)
                {
                    start_pos=0;
                }
            }
            if(is_select4==1)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node--;
                is_select4=0;
            }
            nodelist->del(nodelist->getPos(4));
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=530&&x<=577&&y>=303&&y<=398)
        {
            delete label5;
            label5=NULL;
            label5=new QLabel(this);
            label5->setParent(this);
            //将字体设置到标签中
            label5->setFont(font);
            label5->setText(numlist[4]);
            label5->setPalette(pe_unselect);
            label5->setGeometry((530+577)/2-10,(303+398)/2-20,120,50);
            label5->show();
            if(lock5==1)
            {
                symbol=0;
                lock5=0;
                label5st->hide();
                if(start_pos==5)
                {
                    start_pos=0;
                }
            }
            if(is_select5==1)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node--;
                is_select5=0;
            }
            nodelist->del(nodelist->getPos(5));
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=540&&x<=562&&y>=426&&y<=478)
        {
            delete label6;
            label6=NULL;
            label6=new QLabel(this);
            label6->setParent(this);
            //将字体设置到标签中
            label6->setFont(font);
            label6->setText(numlist[5]);
            label6->setPalette(pe_unselect);
            label6->setGeometry((540+562)/2-10,(426+478)/2-20,120,50);
            label6->show();
            if(lock6==1)
            {
                symbol=0;
                lock6=0;
                label6st->hide();
                if(start_pos==6)
                {
                    start_pos=0;
                }
            }
            if(is_select6==1)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node--;
                is_select6=0;
            }
            nodelist->del(nodelist->getPos(6));
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=336&&x<=398&&y>=389&&y<=488)
        {
            delete label7;
            label7=NULL;
            label7=new QLabel(this);
            label7->setParent(this);
            //将字体设置到标签中
            label7->setFont(font);
            label7->setText(numlist[6]);
            label7->setPalette(pe_unselect);
            label7->setGeometry((336+398)/2-10,(389+488)/2-20,120,50);
            label7->show();
            if(lock7==1)
            {
                symbol=0;
                lock7=0;
                label7st->hide();
                if(start_pos==7)
                {
                    start_pos=0;
                }
            }
            if(is_select7==1)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node--;
                is_select7=0;
            }
            nodelist->del(nodelist->getPos(7));
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=1100&&x<=1129&&y>=240&&y<=286)
        {
            delete label8;
            label8=NULL;
            label8=new QLabel(this);
            label8->setParent(this);
            //将字体设置到标签中
            label8->setFont(font);
            label8->setText(numlist[7]);
            label8->setPalette(pe_unselect);
            label8->setGeometry((1100+1129)/2-10,(240+286)/2-20,120,50);
            label8->show();
            if(lock8==1)
            {
                symbol=0;
                lock8=0;
                label8st->hide();
                if(start_pos==8)
                {
                    start_pos=0;
                }
            }
            if(is_select8==1)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node--;
                is_select8=0;
            }
            nodelist->del(nodelist->getPos(8));
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=728&&x<=817&&y>=169&&y<=237)
        {
            delete label9;
            label9=NULL;
            label9=new QLabel(this);
            label9->setParent(this);
            //将字体设置到标签中
            label9->setFont(font);
            label9->setText(numlist[8]);
            label9->setPalette(pe_unselect);
            label9->setGeometry((728+817)/2-10,(169+237)/2-20,120,50);
            label9->show();
            if(lock9==1)
            {
                symbol=0;
                lock9=0;
                label9st->hide();
                if(start_pos==9)
                {
                    start_pos=0;
                }
            }
            if(is_select9==1)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node--;
                is_select9=0;
            }
            nodelist->del(nodelist->getPos(9));
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
        else if(x>=678&&x<=755&&y>=341&&y<=451)
        {
            delete label10;
            label10=NULL;
            label10=new QLabel(this);
            label10->setParent(this);
            //将字体设置到标签中
            label10->setFont(font);
            label10->setText(numlist[9]);
            label10->setPalette(pe_unselect);
            label10->setGeometry((678+755)/2-10,(341+451)/2-20,120,50);
            label10->show();
            if(lock10==1)
            {
                symbol=0;
                lock10=0;
                label10st->hide();
                if(start_pos==10)
                {
                    start_pos=0;
                }
            }
            if(is_select10==1)
            {
                flag=0;
                QWidget::update();//刷新界面
                hide_of_label_num();
                num_of_node--;
                is_select10=0;
            }
            nodelist->del(nodelist->getPos(10));
            nodelist->printlink();
            qDebug()<<num_of_node;
        }
    }
}
void navigation:: mouseDoubleClickEvent(QMouseEvent* e)
{
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);

    QPalette pe_select;
    pe_select.setColor(QPalette::WindowText, Qt::blue);

    QPalette pe_unselect;
    pe_unselect.setColor(QPalette::WindowText, Qt::black);
    QStringList numlist;
    numlist<<"起点";

    if(e->button()==Qt::LeftButton)
    {
        int x = this->mapFromGlobal(QCursor().pos()).x();   // 获取鼠标返回的横坐标
        int y = this->mapFromGlobal(QCursor().pos()).y();
        qDebug()<<x<<","<<y;
        if(x>=1053&&x<=1111&&y>=399&&y<=452)
        {
            label1st->setParent(this);
            //将字体设置到标签中
            label1st->setFont(font);
            label1st->setText(numlist[0]);
            label1st->setPalette(pe_select);
            label1st->setGeometry((1053+1111)/2,(399+452)/2-30,120,50);
            if(symbol==0)
            {
                label1st->show();
                start_pos=1;
                symbol=1;
                lock1=1;
            }
            else if(symbol==1)
            {
                label1st->hide();
                if(lock1==1)
                {
                    symbol=0;
                    lock1=0;
                }
                if(start_pos==1)
                {
                    start_pos=0;
                }
            }
        }
        else if(x>=748&&x<=808&&y>=479&&y<=568)
        {
            label2st->setParent(this);
            //将字体设置到标签中
            label2st->setFont(font);
            label2st->setText(numlist[0]);
            label2st->setPalette(pe_select);
            label2st->setGeometry((748+808)/2+5,(479+568)/2-20,120,50);
            if(symbol==0)
            {
                label2st->show();
                start_pos=2;
                symbol=1;
                lock2=1;
            }
            else if(symbol==1)
            {
                label2st->hide();
                if(lock2==1)
                {
                    symbol=0;
                    lock2=0;
                }
                if(start_pos==2)
                {
                    start_pos=0;
                }
            }
        }
        else if(x>=449&&x<=470&&y>=285&&y<=351)
        {
            label3st->setParent(this);
            //将字体设置到标签中
            label3st->setFont(font);
            label3st->setText(numlist[0]);
            label3st->setPalette(pe_select);
            label3st->setGeometry((449+470)/2+5,(285+351)/2-20,120,50);
            if(symbol==0)
            {
                label3st->show();
                start_pos=3;
                symbol=1;
                lock3=1;
            }
            else if(symbol==1)
            {
                label3st->hide();
                if(lock3==1)
                {
                    symbol=0;
                    lock3=0;
                }
                if(start_pos==3)
                {
                    start_pos=0;
                }
            }
        }
        else if(x>=566&&x<=653&&y>=597&&y<=667)
        {
            label4st->setParent(this);
            //将字体设置到标签中
            label4st->setFont(font);
            label4st->setText(numlist[0]);
            label4st->setPalette(pe_select);
            label4st->setGeometry((556+653)/2+5,(597+667)/2-20,120,50);
            if(symbol==0)
            {
                label4st->show();
                start_pos=4;
                symbol=1;
                lock4=1;
            }
            else if(symbol==1)
            {
                label4st->hide();
                if(lock4==1)
                {
                    symbol=0;
                    lock4=0;
                }
                if(start_pos==4)
                {
                    start_pos=0;
                }
            }

        }
        else if(x>=530&&x<=577&&y>=303&&y<=398)
        {
            label5st->setParent(this);
            //将字体设置到标签中
            label5st->setFont(font);
            label5st->setText(numlist[0]);
            label5st->setPalette(pe_select);
            label5st->setGeometry((530+577)/2+5,(303+398)/2-20,120,50);
            if(symbol==0)
            {
                label5st->show();
                start_pos=5;
                symbol=1;
                lock5=1;
            }
            else if(symbol==1)
            {
                label5st->hide();
                if(lock5==1)
                {
                    symbol=0;
                    lock5=0;
                }
                if(start_pos==5)
                {
                    start_pos=0;
                }
            }
        }
        else if(x>=540&&x<=562&&y>=426&&y<=478)
        {
            label6st->setParent(this);
            //将字体设置到标签中
            label6st->setFont(font);
            label6st->setText(numlist[0]);
            label6st->setPalette(pe_select);
            label6st->setGeometry((540+562)/2+5,(426+478)/2-20,120,50);
            if(symbol==0)
            {
                label6st->show();
                start_pos=6;
                symbol=1;
                lock6=1;
            }
            else if(symbol==1)
            {
                label6st->hide();
                if(lock6==1)
                {
                    symbol=0;
                    lock6=0;
                }
                if(start_pos==6)
                {
                    start_pos=0;
                }
            }
        }
        else if(x>=336&&x<=398&&y>=389&&y<=488)
        {
            label7st->setParent(this);
            //将字体设置到标签中
            label7st->setFont(font);
            label7st->setText(numlist[0]);
            label7st->setPalette(pe_select);
            label7st->setGeometry((336+398)/2+5,(389+488)/2-20,120,50);
            if(symbol==0)
            {
                label7st->show();
                start_pos=7;
                symbol=1;
                lock7=1;
            }
            else if(symbol==1)
            {
                label7st->hide();
                if(lock7==1)
                {
                    symbol=0;
                    lock7=0;
                }
                if(start_pos==7)
                {
                    start_pos=0;
                }
            }
        }
        else if(x>=1100&&x<=1129&&y>=240&&y<=286)
        {
            label8st->setParent(this);
            //将字体设置到标签中
            label8st->setFont(font);
            label8st->setText(numlist[0]);
            label8st->setPalette(pe_select);
            label8st->setGeometry((1100+1129)/2+5,(240+286)/2-20,120,50);
            if(symbol==0)
            {
                label8st->show();
                start_pos=8;
                symbol=1;
                lock8=1;
            }
            else if(symbol==1)
            {
                label8st->hide();
                if(lock8==1)
                {
                    symbol=0;
                    lock8=0;
                }
                if(start_pos==8)
                {
                    start_pos=0;
                }
            }
        }
        else if(x>=728&&x<=817&&y>=169&&y<=237)
        {
            label9st->setParent(this);
            //将字体设置到标签中
            label9st->setFont(font);
            label9st->setText(numlist[0]);
            label9st->setPalette(pe_select);
            label9st->setGeometry((728+817)/2+5,(169+237)/2-20,120,50);
            if(symbol==0)
            {
                label9st->show();
                start_pos=9;
                symbol=1;
                lock9=1;
            }
            else if(symbol==1)
            {
                label9st->hide();
                if(lock9==1)
                {
                    symbol=0;
                    lock9=0;
                }
                if(start_pos==9)
                {
                    start_pos=0;
                }
            }
        }
        else if(x>=678&&x<=755&&y>=341&&y<=451)
        {
            label10st->setParent(this);
            //将字体设置到标签中
            label10st->setFont(font);
            label10st->setText(numlist[0]);
            label10st->setPalette(pe_select);
            label10st->setGeometry((678+755)/2+18,(341+451)/2-20,120,50);
            if(symbol==0)
            {
                label10st->show();
                start_pos=10;
                symbol=1;
                lock10=1;
            }
            else if(symbol==1)
            {
                label10st->hide();
                if(lock10==1)
                {
                    symbol=0;
                    lock10=0;
                }
                if(start_pos==10)
                {
                    start_pos=0;
                }
            }
        }
    }
}

void navigation:: clear_select()
{
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);

    QPalette pe_unselect;
    pe_unselect.setColor(QPalette::WindowText, Qt::black);

    lock1=0;
    lock2=0;
    lock3=0;
    lock4=0;
    lock5=0;
    lock6=0;
    lock7=0;
    lock8=0;
    lock9=0;
    lock10=0;

    //标记是否被选中
    is_select1=0;
    is_select2=0;
    is_select3=0;
    is_select4=0;
    is_select5=0;
    is_select6=0;
    is_select7=0;
    is_select8=0;
    is_select9=0;
    is_select10=0;

    num_of_node=0;//顶点个数

    if(nodelist!=NULL)
    {
        nodelist->clear_of_list();
    }

    label1->setPalette(pe_unselect);
    label2->setPalette(pe_unselect);
    label3->setPalette(pe_unselect);
    label4->setPalette(pe_unselect);
    label5->setPalette(pe_unselect);
    label6->setPalette(pe_unselect);
    label7->setPalette(pe_unselect);
    label8->setPalette(pe_unselect);
    label9->setPalette(pe_unselect);
    label10->setPalette(pe_unselect);

    label1st->hide();
    label2st->hide();
    label3st->hide();
    label4st->hide();
    label5st->hide();
    label6st->hide();
    label7st->hide();
    label8st->hide();
    label9st->hide();
    label10st->hide();

    hide_of_label_num();


    ui->textBrowser->clear();
    start_pos=0;
    symbol=0;


}
void navigation:: hide_of_label_num()
{
    label1num->hide();
    label2num->hide();
    label3num->hide();
    label4num->hide();
    label5num->hide();
    label6num->hide();
    label7num->hide();
    label8num->hide();
    label9num->hide();
    label10num->hide();
}
