#include "project/tmp.hpp"

int tmp::add(int a, int b) { return a + b; }

int tmp::sub(int a, int b) { return a - b; }

/*-------------------------------------------------------*/
#include "../include/project/astart.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace Astart;

std::vector<Node *> openList; //将当前点紧邻的八个方向点Node
std::vector<Node *> closeList; //将障碍点和已在路径上的点放置在close列表中

//用数组保存每个图即可，用于数据处理的节点point
char mapArr[4][8] = {
    'S','-','-','-','X','X','X','X',
    '-','-','-','-','-','X','X','X',
    'X','X','-','X','X','-','-','-',
    'X','-','-','-','-','-','-','E'
};

int col_mapArr = sizeof(mapArr[0]) / sizeof(mapArr[0][0]);  //一维数组的长度
int row_mapArr = sizeof(mapArr) / sizeof(mapArr[0]);    //一维数组的个数

//
void Astart::init_mapArr(Node * &start, Node * &endNode) {
    std::cout << "x < " << col_mapArr << ", y < " << row_mapArr << std::endl;
    start = new Node(0, 0);
    endNode = new Node(3, 7);
}


//打印输出路径
void Astart::print_map () {
    for (int i = 0; i < row_mapArr; i++) {
        for (int j = 0; j < col_mapArr; j++)
            std::cout << mapArr[i][j] << " ";
        std::cout << std::endl;
    }
}

void Astart::print_path (Node * node) { //使用递归，以使得从起点开始输出路径坐标
    if (node->parent != NULL) 
        Astart::print_path(node->parent);

    std::cout << "<" << (node->x) << ", " << (node->y) << "> ";
}

int Astart::isContain(std::vector<Node *> *v, int x, int y) {
    for (int i = 0; i < v->size(); i++)
        if ( v->at(i)->x == x && v->at(i)->y == y )
            return true;
    return false;
}

//启明函数：估算当前节点到终点的距离
int Astart::func_h(Node *node, Node *end) {
    return abs(end->x - node->x) + abs(end->y - node->y);
}


//操作每个节点周围节点的节点属性值
void Astart::modify_node(int x, int y, Node * cur_node, Node *end) {
    if (mapArr[x][y] == 'X')    //要遍历的节点是障碍物
        return;

    if ( isContain(&closeList, x, y) )    //要访问的节点是已包含在路径中
        return;
    
    if ( !Astart::isContain(&openList, x, y) ) { //还没被添加到openList队列中时
        //创建新结点并赋值
        Node *n = new Node(x, y);
        n->g = cur_node->g + 1;
        n->h = func_h(n, end);
        n->f = n->g + n->h;
        n->parent = cur_node;
        //将新结点加入列表
        openList.push_back(n);
    }
}

//释放申请的节点
void Astart::releaseNode(Node* n) {     
    if (n->parent != NULL)
        Astart::releaseNode(n->parent);
    delete n;
}

bool Astart::compare(Node* n1, Node* n2) {//vector对象的sort函数降序排序
    return n1->f < n2->f;
}

//查找最短路径
bool Astart::search_path(Node *start, Node *end) {
    if (mapArr[start->x][start->y] == 'X' || mapArr[end->x][end->y] == 'X')
        return -1;
    
    //为起始节点赋值
    start->g = 0;
    start->h = Astart::func_h(start, end);
    start->f = start->g + start->h;
    start->parent = NULL;
    //将起始节点加入到列表中
    openList.push_back(start);

    Node * cur_node = NULL; //保存当前访问的节点

    bool find_path = false;  //收否找到一条路径

    int x, y;
    while (!openList.empty()) {
        cur_node = openList[0]; //openList的对首元素就是路径要访问的节点。

        if (cur_node->x == end->x && cur_node->y == end->y) {//当前节点是否为终点
            find_path = true;
            break;
        }
        x = cur_node->x;
        y = cur_node->y;

        if (mapArr[x][y] != 'S' && mapArr[x][y] != 'E') //路径节点不是起始和终点时，置为【*】
            mapArr[x][y] = '*';

        //访问当前节点的周围的八个节点
        if ( (x + 1) <= row_mapArr - 1 )  //当前节点的右边节点
            Astart::modify_node(x+1, y, cur_node, end);
        if ( x - 1 >= 0 )                 //当前节点的左边节点  
            Astart::modify_node(x-1, y, cur_node, end);
        if ( y - 1 >= 0 )                 //当前节点的上边节点
            Astart::modify_node(x, y-1, cur_node, end); 
        if ( y + 1 <= col_mapArr -1 )     //当前节点的下边节点
            Astart::modify_node(x, y+1, cur_node, end);

        if ( x + 1 <= row_mapArr - 1 && y + 1 <= col_mapArr -1 )     //当前节点的右下节点
            Astart::modify_node(x + 1, y + 1, cur_node, end);
        if ( x + 1 <= row_mapArr && y - 1 >= 0 )                     //当前节点的右上节点
            Astart::modify_node(x + 1, y - 1, cur_node, end);
        if ( x - 1 >= 0 && y + 1 <= col_mapArr -1 )                  //当前节点的左下节点
            Astart::modify_node(x - 1, y + 1, cur_node, end);
        if ( x - 1 >= 0 && y - 1 >= 0 )                              //当前节点的左上节点
            Astart::modify_node(x - 1, y - 1, cur_node, end);


        //将当前节点的八个方向节点访问一遍之后，要将当前节点从openList删除，加入closeList中

        closeList.push_back(cur_node);
        openList.erase(openList.begin());
        std::sort(openList.begin(), openList.end(), Astart::compare);//通过sort操作，将f=g+h的最小值放在openList对首
    }

    if (find_path) {
            print_path(cur_node);
            std::cout << std::endl;
    }

    releaseNode(cur_node);
    openList.clear();
    closeList.clear();
    return find_path;
}