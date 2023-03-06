#ifndef LINKLIST_H
#define LINKLIST_H

#include <QMainWindow>
#include "linknode.h"
class LinkList : public QMainWindow
{
    Q_OBJECT
public:
    //explicit LinkList(QWidget *parent = nullptr);
    LinkList();
    ~LinkList();
    bool isEmpty();
    void clear_of_list();
    int length();
    bool append(const int value);
    bool insert(const int pos, const int value);
    bool del(const int pos);
    bool getValue(const int pos, int& value);
    bool setValue(const int pos, int& value);
    int getPos(const int value);
    void printlink();

private:
    LinkNode* head, *tail;
    LinkNode* setPos(const int i);

signals:

};

#endif // LINKLIST_H
