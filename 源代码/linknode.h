#ifndef LINKNODE_H
#define LINKNODE_H

#include <QMainWindow>

class LinkNode : public QMainWindow
{
    Q_OBJECT
public:
    explicit LinkNode(QWidget *parent = nullptr);
    int data;
    LinkNode*next;
    LinkNode(int info,LinkNode*nextValue=NULL);
    LinkNode(LinkNode*nextValue);

signals:

};

#endif // LINKNODE_H
