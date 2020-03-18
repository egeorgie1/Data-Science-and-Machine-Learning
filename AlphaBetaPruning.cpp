#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
using namespace std;

int min_value( vector<char>&, int, int);
/* Implementation of the minimax algorithm with alpha-beta pruning from "Artificial Intelligence: A Modern Approach"
by S.Russel and P.Norvig, Chapter 5, for the game Tic Tac Toe */

/* A vector with positions from 0 to 8 represents the board, at the very beginning all positions are empty */
bool allFilled(vector<char>& state){ //if all positions are filled
    for(int i = 0; i<9; i++)
        if(state[i] == 'e')
           return false;
    return true;
}

int utility(vector<char>& state){ //the values (utilities) of the terminal states: 1,-1,0
    char c = 'x';
    int val = 1;

  for(int p=0; p<2; p++){
    //3 in a column
    for(int j = 0; j<3; j++) // all remainders mod 3
        if(state[0+j] == c && state[3+j] == c && state[6+j] == c)
           return val; //win for x (MAX)
    //3 in a row
    for(int j = 0; j<3; j++)
        if(state[3*j + 0] == c && state[3*j + 1] == c && state[3*j + 2] == c)
           return val;

    //3 in a diagonal
    if(state[0] == c && state[4] == c && state[8] == c)
        return val;
    if(state[2] == c && state[4] == c && state[6] == c)
        return val;

    c = 'o';
    val = -1;
  }

    // no winner
    if(allFilled(state))
        return 0;
    // no terminal state
    return 100;
}

bool terminal(vector<char>& state){
    int u = utility(state);
    if(u == 0 || u == 1 || u == -1)
        return true;
    return false;
}


int max_value( vector<char>& state, int alpha, int beta){ //Max puts a 'x'
  if(terminal(state))
      return utility(state);

  int val = INT_MIN;

  //check all the successors
  //we generate a successor by placing a 'x' at an empty position
  for(int i = 0; i<9; i++)
    if(state[i] == 'e'){
       vector<char> succ = state;
       succ[i] = 'x';
       val = max(val,min_value(succ,alpha,beta));
       if(val >= beta) return val;  // cut the other successors, because of the maximizer they will be greater than val
       alpha = max(alpha,val); // the maximizer's goal is to maximize alpha
    }

   return val; //MAX node => return the maximum value among the successors
}

int min_value( vector<char>& state, int alpha, int beta){
   if(terminal(state))
      return utility(state);

   int val = INT_MAX;
   //It is MIN's turn and we generate a successor by placing a 'o' at an empty position
   for(int i = 0; i<9; i++)
     if(state[i] == 'e'){
       vector<char> succ = state;
       succ[i] = 'o';
       val = min(val,max_value(succ,alpha,beta));
       if(val <= alpha) return val; // cut
       beta = min(beta,val); //the minimizer tries to minimize beta
     }

  return val; //MIN node => return the minimum value among the successors
}


int main(){
vector<char> start;
for(int i=0; i<9; i++)
    start.push_back('e');

cout << "The minimax value of the initial state is " << max_value(start,INT_MIN,INT_MAX);
return 0;
}
