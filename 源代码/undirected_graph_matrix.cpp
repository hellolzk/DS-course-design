#include "undirected_graph_matrix.h"
#define UNVISITED 0
#define VISITED 1
#define INF 9999
#include "edge.h"
#include <QString>
#include <QDebug>
#include "linklist.h"
unDirected_Graph_matrix::unDirected_Graph_matrix(QWidget *parent) : QMainWindow(parent)
{

}

unDirected_Graph_matrix:: unDirected_Graph_matrix(int numVert)
{
    Verticesnum = numVert;//顶点数
    Edgenum = 0;//边数
    Mark = new int[numVert];//标记数组
    degree = new int[numVert];//度数数组
    matrix = new int* [numVert];//邻接矩阵
    save_position = new int[numVert];//保存路径
    xiaoyuanditu = new int[numVert];
    for (int i = 0; i < numVert; i++)
    {
        Mark[i] = UNVISITED;
        degree[i] = 0;
        matrix[i] = new int[numVert];
        save_position[i] = INF;
        for (int j = 0; j < numVert; j++)
        {
            matrix[i][j] = INF;//初始化图
        }
    }
}

void unDirected_Graph_matrix:: init_edge()
{
    Edgenum = (Verticesnum - 1) * Verticesnum / 2;
    for (int i = 0; i < Verticesnum; i++)
    {
        degree[i] = Verticesnum - 1;
    }
}

void unDirected_Graph_matrix:: set_xiaoyuanditu(LinkList*nodelist)
{
    int value=0;
    for(int i=0;i<nodelist->length();i++)
    {
        nodelist->getValue(i,value);
        xiaoyuanditu[i]=value;
    }
}

void unDirected_Graph_matrix:: tiqu_ditu(int tu[][10])
{
    for (int i = 0; i < Verticesnum; i++)
    {
        int temp = xiaoyuanditu[i];
        for (int j = 0; j < Verticesnum; j++)
        {
            matrix[i][j] = tu[temp-1][xiaoyuanditu[j]-1];
        }
    }
}//提取出相应的邻接矩阵

int unDirected_Graph_matrix:: get_num_of_vertex()
{
    return Verticesnum;
}//提取点的个数

int unDirected_Graph_matrix:: get_num_of_edge()
{
    return Edgenum;
}//提取边的个数

bool unDirected_Graph_matrix:: isEdge(Edge oneEdge)
{
    if (oneEdge.from >= 0 && oneEdge.to >= 0 && oneEdge.weight > 0 && oneEdge.weight < INF)
    {
        return true;
    }
    else
    {
        return false;
    }
}//判断是否为边

void unDirected_Graph_matrix:: setEdge(int from, int to, int weight)
{
    if (matrix[from][to] == INF)
    {
        Edgenum++;
        degree[to]++;
        degree[from]++;
    }
    matrix[from][to] = weight;
    matrix[to][from] = weight;
}//添加边

void unDirected_Graph_matrix:: delEdge(int from, int to)
{
    if (matrix[from][to] > 0)
    {
        Edgenum--;
        degree[to]--;
        degree[from]--;
    }
    matrix[from][to] = INF;
    matrix[to][from] = INF;
}//删除边

void unDirected_Graph_matrix:: initMark()
{
    for (int i = 0; i < Verticesnum; i++)
    {
        Mark[i] = UNVISITED;
    }
}//初始化mark数组

int unDirected_Graph_matrix:: TSP(int current_position, int num_of_entered, int length, int start_position)
{
    /*数组b记录点是否进入路径中，u为当前点编号，m为已进入路径点的个数，
            l为路径长度 ,k为开始点的编号 */
    save_position[num_of_entered - 1] = find_index(current_position);//当前点存入路径
    Mark[find_index(current_position)] = VISITED;//进入路径的的点状态更新
    if (num_of_entered == Verticesnum)
    {//n个点都进入路径后，结束递归
        length = length + matrix[find_index(current_position)][find_index(start_position)]; //路径长度更新
        return length;
    }
    else
    {
        int min = INF;//将最短距离初始值设成一个比较大的数
        int v;
        for (int i = 0; i < Verticesnum; i++)
        {//寻找剩余点中，距离当前点最近的点，并用变量v记录编号
            if (min > matrix[find_index(current_position)][i] && Mark[i] == UNVISITED)
            {//如果某个点距离当前点更近，且不在路径中
                v = i;//记录该点编号
                min = matrix[find_index(current_position)][i];//最短距离更新
            }
        }
        num_of_entered++;//路径中点的个数+1
        length = length + min;//路径长度更新
        return TSP(xiaoyuanditu[v], num_of_entered, length, start_position);//v变为当前点，递归
    }
}//核心算法

int unDirected_Graph_matrix:: find_index(int value)
{
    for (int i = 0; i < Verticesnum; i++)
    {
        if (xiaoyuanditu[i] == value)
        {
            return i;
        }
    }
    return 0;
}//寻找下标

QString unDirected_Graph_matrix:: print_TSP()
{
    QString temp = "";
    QString res  = "";
    for (int i = 0; i < get_num_of_vertex(); i++)
    {
        switch (xiaoyuanditu[save_position[i]])
        {
        case 1:
        {
            if (i == 0)
            {
                temp = "1 14号宿舍楼";
            }
            res=res+"1 14号宿舍楼-->";
            break;
        }

        case 2:
        {
            if (i == 0)
            {
                temp = "2 南操场";
            }
            res=res+"2 南操场-->";
            break;
        }

        case 3:
        {
            if (i == 0)
            {
                temp = "3 信息楼";
            }
            res=res+"3 信息楼-->";
            break;
        }

        case 4:
        {
            if (i == 0)
            {
                temp = "4 羽毛球馆";
            }
            res=res+"4 羽毛球馆-->";
            break;
        }

        case 5:
        {
            if (i == 0)
            {
                temp = "5 图书馆";
            }
            res=res+"5 图书馆-->";
            break;
        }

        case 6:
        {
            if (i == 0)
            {
                temp = "6 美食园";
            }
            res=res+"6 美食园-->";
            break;
        }

        case 7:
        {
            if (i == 0)
            {
                temp = "7 科学楼";
            }
            res=res+"7 科学楼-->";
            break;
        }

        case 8:
        {
            if (i == 0)
            {
                temp = "8 校医院";
            }
            res=res+"8 校医院-->";
            break;
        }

        case 9:
        {
            if (i == 0)
            {
                temp = "9 一教";
            }
            res=res+"9 一教-->";
            break;
        }

        case 10:
        {
            if (i == 0)
            {
                temp = "10 三教";
            }
            res=res+"10 三教-->";
            break;
        }
        default:
            break;
        }
    }
    res=res+temp;
    return res;
}

void unDirected_Graph_matrix:: print_save_pos()
{
    for(int i=0;i<Verticesnum;i++)
    {
        qDebug()<<xiaoyuanditu[save_position[i]];
    }
}








