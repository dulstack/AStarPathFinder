#include "AStar.h"
Node::Node(Node* parent):parent(parent){
 if(parent==NULL){g=0;}
 else{g=parent->g+1;}
}
bool Node::operator==(Node* node){return ((x==node->x)&&(y==node->y));}
bool Node::operator==(const Node& node){return ((x==node.x)&&(y==node.y));}
void Node::calc_h(Node* target){
 int dx=x-target->x;
 int dy=y-target->y;
 h=dx*dx+dy*dy;
}
Node::Node(int x, int y):Node(NULL){
 this->x=x;
 this->y=y;
}

int Node::get_f(){return h+g;}
std::unordered_set<Node*>::iterator get_lowest(std::unordered_set<Node*>& list){
 auto lowest=list.begin();
 for(auto it=list.begin(); it!=list.end(); it++){
  if((*lowest)->get_f()>(*it)->get_f())lowest=it;
 }
 return lowest;
}
bool is_closed(Node* n, std::unordered_set<Node*>& list){
 for(auto el: list){
  if(*n==el)return 1;
 }
 return 0;
}

std::vector<std::pair<int, int>> AStar(Node* beg, Node* target, const GRID& grid){
 std::unordered_set<Node*> open_list={beg};
 std::unordered_set<Node*> closed_list;
 std::vector<std::pair<int,int>> res;
 std::vector<Node*> allocated_nodes;
 if(*beg==*target)return res;
 bool found=0;
 Node* last_node=NULL;
 while(open_list.size()>0&&found==0){
  std::unordered_set<Node*>::iterator q_it=get_lowest(open_list);
  Node* q=*q_it;
  int x=q->x,y=q->y;
  //get successors
  if(x-1>=0&&grid[y][x-1]==0){
   Node* child=new Node(q);
   child->x=x-1;child->y=y;
   allocated_nodes.push_back(child);
   if(!is_closed(child, closed_list)){child->calc_h(target);open_list.insert(child);if(*child==*target){found=1;last_node=child;break;}}
  }
  if(y-1>=0&&grid[y-1][x]==0){
   Node* child=new Node(q);
   child->x=x;child->y=y-1;
   allocated_nodes.push_back(child);
   if(!is_closed(child, closed_list)){child->calc_h(target);open_list.insert(child);if(*child==*target){found=1;last_node=child;break;}}
  }
  if(x+1<grid[0].size()&&grid[y][x+1]==0){
   Node* child=new Node(q);
   child->x=x+1;child->y=y;
   allocated_nodes.push_back(child);
   if(!is_closed(child, closed_list)){child->calc_h(target);open_list.insert(child);if(*child==*target){found=1;last_node=child;break;}}
  }
  if(y+1<grid.size()&&grid[y+1][x]==0){
   Node* child=new Node(q);
   child->x=x;child->y=y+1;
   allocated_nodes.push_back(child);
   if(!is_closed(child, closed_list)){child->calc_h(target);open_list.insert(child);if(*child==*target){found=1;last_node=child;break;}}
  }
  closed_list.insert(q);
  open_list.erase(q_it);
 }
 if(found){
  Node* n=last_node;
  while(n!=NULL){
   std::pair<int,int> coord;
   coord.first=n->x;
   coord.second=n->y;
   res.push_back(coord);
   n=n->parent;
  }
 }
 //deallocate
 for(int i=0; i<allocated_nodes.size(); i++){delete allocated_nodes[i];}
 std::reverse(res.begin(), res.end());
 return res;
}
