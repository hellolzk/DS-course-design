#ifndef EDGE_H
#define EDGE_H

#include <QMainWindow>

class Edge : public QMainWindow
{
    Q_OBJECT
public:
    explicit Edge(QWidget *parent = nullptr);
    int from;//起点
    int to;//终点
    int weight;//权
    Edge();//默认构造
    Edge(int from, int to, int weight);//传参构造

signals:

};

#endif // EDGE_H
