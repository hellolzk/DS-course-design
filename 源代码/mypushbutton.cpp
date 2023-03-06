#include "mypushbutton.h"
#include <QDebug>
#include <QPropertyAnimation>
mypushbutton::mypushbutton(QWidget *parent) :QPushButton(parent)
{

}

mypushbutton::mypushbutton(QString normalImg,QString pressImg)
{
    this->normalImgPath=normalImg;
    this->pressImgPath=pressImg;
    QPixmap pix;
    bool ret=pix.load(normalImg);
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    //设置图片固定大小
    this->setFixedSize(pix.width(),pix.height());

    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    this->setIconSize(QSize(pix.width(),pix.height()));
}

void mypushbutton:: zoom1()//向下跳
{
    //创建动态对象
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry");

    //设置时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));

    //结束位置
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();

}

void mypushbutton:: zoom2()//向上跳
{
    //创建动态对象
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry");

    //设置时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    //结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();
}


void mypushbutton:: mousePressEvent(QMouseEvent*e)
{
    if(this->pressImgPath!="")//传入的按下图片不为空，需要有按下的状态，切换图片
    {
        QPixmap pix;
        bool ret=pix.load(this->pressImgPath);
        if(!ret)
        {
            qDebug()<<"图片加载失败";
            return;
        }
        //设置图片固定大小
        this->setFixedSize(pix.width(),pix.height());

        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");

        //设置图标
        this->setIcon(pix);

        //设置图标大小
        this->setIconSize(QSize(pix.width(),pix.height()));
    }

    //让父类执其他内容
    return QPushButton::mousePressEvent(e);
}

void mypushbutton:: mouseReleaseEvent(QMouseEvent*e)
{
    if(this->pressImgPath!="")//传入的按下图片不为空，需要有按下的状态，切换图片
    {
        QPixmap pix;
        bool ret=pix.load(this->normalImgPath);
        if(!ret)
        {
            qDebug()<<"图片加载失败";
            return;
        }
        //设置图片固定大小
        this->setFixedSize(pix.width(),pix.height());

        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");

        //设置图标
        this->setIcon(pix);

        //设置图标大小
        this->setIconSize(QSize(pix.width(),pix.height()));
    }

    //让父类执其他内容
    return QPushButton::mouseReleaseEvent(e);
}
