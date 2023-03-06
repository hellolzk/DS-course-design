#include "linknode.h"

LinkNode::LinkNode(QWidget *parent) : QMainWindow(parent)
{

}

LinkNode::LinkNode(int info,LinkNode*nextValue)
{
    data=info;
    this->next=nextValue;
}
LinkNode::LinkNode(LinkNode*nextValue)
{
    this->next=nextValue;
}
