#include <wx/wx.h>
#include <wx/timer.h>
#include "AStar.h"
#include "grid.h"
class App:public wxApp{
 public: bool OnInit();
};
wxIMPLEMENT_APP(App);
class Frame:public wxFrame{
 public:
  Frame(const wxString& title);
  void draw_grid(wxDC& dc);
  void draw_path(wxDC& dc);
  void on_timer(wxTimerEvent&);
  void on_paint(wxPaintEvent&);
 private:
  int it;
  Node start, end;
  wxTimer t;
  wxRect grid_rect;
  wxSize grid_size;
  std::vector<std::pair<int, int>> path;
  enum IDs{
   ID_TIMER=1
  };
};
bool App::OnInit(){
 Frame* frame=new Frame(wxT("A* pathfinding"));
 frame->Show(1);
 return 1;
}
Frame::Frame(const wxString& title):t(this,ID_TIMER),wxFrame(NULL, -1,title,wxDefaultPosition, wxSize(640, 480)){
 it=0;
 end.x=0;end.y=0;
 start.x=0; start.y=grid.size()-1;
 path=AStar(&start,&end,grid);
 it=0;
 
 Bind(wxEVT_TIMER, &Frame::on_timer, this, ID_TIMER);
 Bind(wxEVT_PAINT, &Frame::on_paint, this);
 t.Start(1000/15);
}
void Frame::on_timer(wxTimerEvent&){
 if(it<path.size())it++;
 Refresh();
}
void Frame::draw_grid(wxDC& dc){
 dc.SetPen(wxPen(wxColour(0,0,0)));
 wxBrush b_unblocked(wxColour(255,255,255));
 wxBrush b_blocked(wxColour(0,0,0));
 wxSize w_sz=this->GetSize();
 int rows=grid.size();
 int cols=grid[0].size();
 if(w_sz.x>=w_sz.y){		//window height is lower than window width
  float proportion=(float)cols/rows;
  grid_rect.y=0;
  grid_rect.height=w_sz.y;
  grid_rect.width=grid_rect.height*proportion;
  grid_rect.x=(w_sz.x-grid_rect.width)/2;
 }
 else{
  float proportion=(float)rows/cols;
  grid_rect.x=0;
  grid_rect.width=w_sz.x;
  grid_rect.height=grid_rect.width*proportion;
  grid_rect.y=(w_sz.y-grid_rect.height)/2;
 }
 grid_size.x=grid_rect.width/cols;
 grid_size.y=grid_rect.height/rows;
 
 for(int row=0; row<rows; row++){
  for(int col=0; col<cols; col++){
   int x=grid_rect.x+(grid_size.x*col);
   int y=grid_rect.y+(grid_size.y*row);
   if(grid[row][col]==1)dc.SetBrush(b_blocked);
   else{dc.SetBrush(b_unblocked);}
   dc.DrawRectangle(x, y, grid_size.x, grid_size.y);
  }
 }
}
void Frame::draw_path(wxDC& dc){
 dc.SetBrush(wxNullBrush);
 dc.SetPen(wxPen(wxColour(255,0,0), 4));
 
 for(int i=1; i<it&&i<path.size(); i++){
  int prev_row=path[i-1].second;
  int prev_col=path[i-1].first;
  int row=path[i].second;
  int col=path[i].first;
  wxPoint from={grid_rect.x+grid_size.x*prev_col+(grid_size.x/2), grid_rect.y+grid_size.y*prev_row+(grid_size.y/2)};
  wxPoint to={grid_rect.x+grid_size.x*col+(grid_size.x/2), grid_rect.y+grid_size.y*row+(grid_size.y/2)};
  dc.DrawLine(from, to);
 }
}
void Frame::on_paint(wxPaintEvent& evt){
 wxPaintDC dc(this);
 draw_grid(dc);
 draw_path(dc);
}
