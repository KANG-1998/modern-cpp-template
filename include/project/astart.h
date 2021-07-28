#ifndef _PROJECT_MAP_
#define _PROJECT_MAP_

#include <vector>


namespace Astart{

  //节点结构体
  class Node {
   public:
    Node(int x, int y) { this->x = x; this->y = y;}
   public:
    int x, y;   //此节点对应的mapArr数组的下标<x,y>
    int g;    //起始点到当前节点路径长度/cost
    int h;  //当前节点到终点的路径长度h_cost，即启明函数的值
    int f;  //f = g + h
    Node *parent;
  };

  void init_mapArr(Node * &start, Node * &endNode);
 
  void print_map();

  void print_path (Node * node);

  int isContain(std::vector<Node *> *v, int x, int y);

  int func_h(Node *node, Node *end);

  void modify_node(int x, int y, Node * cur_node, Node *end);

  void releaseNode(Node* n);

  bool compare(Node* n1, Node* n2);

  bool search_path(Node *start, Node *end);

}


#endif