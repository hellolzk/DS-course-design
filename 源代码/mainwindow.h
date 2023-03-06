#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "school_map.h"
#include "school_info2.h"
#include "navigation.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //重写paintEvent事件
    void paintEvent(QPaintEvent*);
    //维护一个校园地图类
    school_map*sch_map=NULL;
    school_info2*sch_info2=NULL;
    navigation*nav=NULL;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
