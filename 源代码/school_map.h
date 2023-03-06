#ifndef SCHOOL_MAP_H
#define SCHOOL_MAP_H

#include <QMainWindow>

class school_map : public QMainWindow
{
    Q_OBJECT
public:
    explicit school_map(QWidget *parent = nullptr);

    //重写paintEvent事件
    void paintEvent(QPaintEvent*);

signals:
    //写一个自定义的信号，告诉主场景点击的返回
    void chooseSceneBack();
};

#endif // SCHOOL_MAP_H
