#ifndef UNDIRECTED_GRAPH_MATRIX_H
#define UNDIRECTED_GRAPH_MATRIX_H
#define INF 9999
#include <QMainWindow>
#include "edge.h"
#include "linklist.h"
class unDirected_Graph_matrix : public QMainWindow
{
    Q_OBJECT
private:
    int Verticesnum;//图顶点个数
    int Edgenum;//边的个数
    int* Mark;//标记图中顶点是否被访问过
    int* degree;//存放图顶点的度数
    int** matrix;//用户选择的相邻矩阵
public:
    int* save_position;//保存路径
    int* xiaoyuanditu;//保存用户输入顶点
    explicit unDirected_Graph_matrix(QWidget *parent = nullptr);
    unDirected_Graph_matrix(int numVert);//传参构造
    void init_edge();//初始化边的信息
    void set_xiaoyuanditu(LinkList*nodelist);//保存用户输入顶点
    void tiqu_ditu(int tu[][10]);//提取出相应的邻接矩阵
    int get_num_of_vertex();//提取点的个数
    int get_num_of_edge();//提取边的个数
    bool isEdge(Edge oneEdge);//判断是否为边
    void setEdge(int from, int to, int weight);//添加边
    void delEdge(int from, int to);//删除边
    void initMark();//初始化mark数组
    int TSP(int current_position, int num_of_entered, int length, int start_position);//核心算法
    int find_index(int value);//寻找下标
    QString print_TSP();//打印路径
    void print_save_pos();


    //~unDirected_Graph_matrix();
signals:

};

#endif // UNDIRECTED_GRAPH_MATRIX_H
