#include "linklist.h"
#include <QDebug>
#include "linknode.h"
//LinkList::LinkList(QWidget *parent) : QMainWindow(parent)
//{

//}
LinkNode* LinkList:: setPos(const int i)
{
    int count = 0;
    if (i == -1)
    {
        return head;
    }
    LinkNode* p = head->next;
    while (p != NULL && count < i)
    {
        p = p->next;
        count++;
    }
    return p;
}

LinkList:: LinkList()
{
    head = tail = new LinkNode(0,NULL);
}

LinkList:: ~LinkList()
{
    LinkNode* tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        delete tmp;
    }
}

bool LinkList:: isEmpty()
{
    if (head == tail)
    {
        return true;
    }
    return false;
}

void LinkList:: clear_of_list()
{
    LinkNode* tmp1=head->next;
    LinkNode* tmp2;
    tail = head;
    while (tmp1 != NULL)
    {
        tmp2 = tmp1;
        tmp1 = tmp1->next;
        delete tmp2;
    }
    head->next = NULL;
}

int LinkList:: length()
{
    int count = 0;
    LinkNode* tmp = head->next;
    while (tmp != NULL)
    {
        count++;
        tmp = tmp->next;
    }
    return count;
}
bool LinkList:: append(const int value)
{
    LinkNode* tmp = new LinkNode(value, NULL);
    tail->next = tmp;
    tail = tmp;
    return true;
}
bool LinkList:: insert(const int pos, const int value)
{
    LinkNode* p, * q;
    if (pos<0 || pos>length())
    {
        return false;
    }
    p = setPos(pos - 1);
    q = new LinkNode(value, p->next);
    p->next = q;
    if (p == tail)
    {
        tail = q;
    }
    return true;
}

bool LinkList:: del(const int pos)
{
    LinkNode* p, * q;
    if (pos<0 || pos>length() - 1)
    {
        return false;
    }
    p = setPos(pos - 1);
    q = p->next;
    p->next = q->next;
    if (q == tail)
    {
        tail = p;
    }
    delete q;
    return true;
}

bool LinkList:: getValue(const int pos, int& value)
{
    if (pos<0 || pos>length() - 1)
    {
        return false;
    }
    LinkNode* p = setPos(pos);
    value = p->data;
    return true;
}

bool LinkList:: setValue(const int pos, int& value)
{
    if (pos<0 || pos>length() - 1)
    {
        return false;
    }
    LinkNode* p = setPos(pos);
    p->data = value;
    return true;
}

int LinkList:: getPos(const int value)
{
    LinkNode* p = head->next;
    int count = 0;
    if (p == NULL)
    {
        return -1;
    }
    while (p->data != value && p->next!= NULL)
    {
        p = p->next;
        count++;
    }
    if (p->next==NULL&&p->data!=value)
    {
        return -1;
    }
    return count;
}

void LinkList:: printlink()
{
    LinkNode* p = head->next;
    while (p != NULL)
    {
        qDebug()<< p->data;
        p = p->next;
    }
}
