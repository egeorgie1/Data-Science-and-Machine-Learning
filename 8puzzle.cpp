#include <cstdlib>
#include <queue>
#include <list>
#include <iostream>
using namespace std;

struct Table {

 int table[3][3];
 int heuristic;
 int distance;

 int row(int number) const{
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
     if(table[i][j] == number)
        return i;
   return -1;
 }
 int col(int number) const {
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
     if(table[i][j] == number)
        return j;
   return -1;
 }

 Table up(const Table& goal) const{
  int emptyRow = row(0);
  int emptyCol = col(0);
  return (emptyRow - 1) < 0 ? *this : swapTable(emptyRow,emptyCol,emptyRow - 1,emptyCol,goal);
 }

 Table down(const Table& goal) const{
  int emptyRow = row(0);
  int emptyCol = col(0) ;
  return (emptyRow + 1) > 2 ? *this : swapTable(emptyRow,emptyCol,emptyRow + 1, emptyCol,goal);
 }

 Table left(const Table& goal) const {
   int emptyRow = row(0);
   int emptyCol = col(0);
   return (emptyCol - 1) < 0 ? *this : swapTable(emptyRow,emptyCol,emptyRow,emptyCol - 1,goal);
 }

 Table right(const Table& goal) const{
   int emptyRow = row(0);
   int emptyCol= col(0);
   return (emptyCol + 1) > 2 ? *this : swapTable(emptyRow,emptyCol,emptyRow,emptyCol + 1,goal);
 }

 Table swapTable(int i1, int j1, int i2, int j2, const Table& goal) const{
    Table t;
    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++){
        if(i == i1 && j == j1)
        {
            t.table[i][j] = table[i2][j2];
        }else{
           if(i == i2 && j == j2){
            t.table[i][j] = table[i1][j1];
           }else{
            t.table[i][j] = table[i][j];
           }
        }
    }
    t.distance = distance + 1;
    t.heuristic = t.manhattanDist(goal);
    return t;
 }
 int manhattanDist(const Table& t) const{
   int sum = 0;
   for(int num=0; num<9; num++){
     int x1 = row(num);
     int y1 = col(num);
     int x2 = t.row(num);
     int y2 = t.col(num);

     sum += (abs(x2 - x1) + abs(y2 - y1));
   }
   return sum;
 }
bool operator<( const Table& t ) const
    {
        return heuristic + distance > t.heuristic + t.distance;
    }
bool operator==( const Table& t ) const
    {
        for( int i = 0; i < 3; i++ )
            for( int j = 0; j < 3; j++ )
                if( table[i][j] != t.table[i][j] )
                    return false;
        return true;
    }

Table& operator=(const Table& t){
  if(this != &t){
    for( int i = 0; i < 3; i++ )
     for( int j = 0; j < 3; j++ )
        table[i][j] = t.table[i][j];

    heuristic = t.heuristic;
    distance = t.distance;
  }
  return *this;
}

};


bool member( Table& t, list<Table>& visited){
  for(list<Table>::iterator it=visited.begin(); it!=visited.end(); ++it)
    if( *it == t)
      return true;
  return false;
}
int Astar(const Table& start, const Table& goal){
   list<Table> visited;
   priority_queue<Table> pq;

   pq.push(start);

   while(!pq.empty()){
     Table current = pq.top();
     pq.pop();
     visited.push_back(current);

     if(current.heuristic == 0)  // goal reached
         return current.distance;

     //push successors to the priority queue
     Table upNeigh = current.up(goal);
     Table downNeigh = current.down(goal);
     Table leftNeigh = current.left(goal);
     Table rightNeigh = current.right(goal);

     if( !(upNeigh == current) && !member(upNeigh,visited)) //there is such a successor and it's not visited
         pq.push(upNeigh);

     if( !(downNeigh == current) && !member(downNeigh,visited))
        pq.push(downNeigh);

     if( !(leftNeigh == current) && !member(leftNeigh,visited))
        pq.push(leftNeigh);

     if( !(rightNeigh == current) && !member(rightNeigh,visited))
        pq.push(rightNeigh);
   }

   return -1;
}

int main(){
Table goal = {{{1,2,3},
                {4,5,6},
                {7,8,0}}};
Table start = {{{6,5,3},
                {2,4,8},
                {7,0,1}}};
start.heuristic = start.manhattanDist(goal);
start.distance = 0;

cout << "The distance from start to goal is: " << Astar(start,goal) << "\n";

return 0;
}
