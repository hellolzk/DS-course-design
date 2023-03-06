#ifndef SCHOOL_INFO2_H
#define SCHOOL_INFO2_H

#include <QMainWindow>

namespace Ui {
class school_info2;
}

class school_info2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit school_info2(QWidget *parent = nullptr);
    ~school_info2();
    void action_save();

private:
    Ui::school_info2 *ui;

signals:
    //写一个自定义的信号，告诉主场景点击的返回
    void chooseSceneBack();
};

#endif // SCHOOL_INFO2_H
