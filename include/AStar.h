#ifndef __ASTAR_H
#define __ASTAR_H
#include <stdio.h>
#include <vector>
#include <unordered_set>
#include <math.h>
#include <utility>
#include <algorithm>
#define GRID std::vector<std::vector<int>>
struct Node{
 Node(Node* parent=NULL);
 Node(int x, int y);
 bool operator==(Node* node);
 bool operator==(const Node& node);
 int x,y;
 Node* parent;
 void calc_h(Node* target);
 int get_f();
 int h, g;
};
std::vector<std::pair<int, int>> AStar(Node* beg, Node* target, const GRID& grid);
#endif
