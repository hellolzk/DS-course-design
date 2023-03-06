#include "edge.h"

Edge::Edge(QWidget *parent) : QMainWindow(parent)
{

}

Edge::Edge()
{
    from = -1;
    to = -1;
    weight = 0;
}

Edge::Edge(int from, int to, int weight)
{
    this->from = from;
    this->to = to;
    this->weight = weight;
}
