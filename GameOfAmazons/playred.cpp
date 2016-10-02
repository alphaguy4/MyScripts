/*
  Alphaguy : Code Warriors (Blue)
  Bloqueen
*/
#include<bits/stdc++.h>
using namespace std;
#define si(i) scanf("%d",&i)
#define INF 1e7
int dirx[] = {-1,-1,1,1,-1,1,0,0};
int diry[] = {1,-1,-1,1,0,0,1,-1};

typedef vector<int> vi;
typedef vector<string> vs;

//scanning the board
void scanBoard(int b[][12]){
  for(int i=0;i<12;i++){
    for(int j=0;j<12;j++){
      si(b[i][j]);
    }
  }
}

bool checkPosition(int board[][12],int x,int y){
  if(x < 0 || x > 11 || y < 0 || y > 11 || board[x][y] != 0)
    return  false;
  return true;
}
bool checkArrow(int board[][12],int x,int y,int x1,int y1){
  if(x < 0 || x > 11 || y < 0 || y > 11)
    return false;
  if(board[x][y] == 0 || board[x][y] == 5)
    return true;
  
  // if arrow is thrown to place it came from
  if(x1 == x && y1 == y)
    return true;
  return false;
}
int ans1;
void calMoves(int board[][12],vs &move,int player){
  
  vi posx,posy;
  string s;
  for(int i=0;i<12;i++){
    for(int j=0;j<12;j++){
      if(board[i][j] == player){
        posx.push_back(i);
        posy.push_back(j);
      }
    }
  }
  // for all red queen
  for(int i=0;i<4;i++){
    // select one of queen (x,y)
    int x = posx[i];
    int y = posy[i];
    for(int dir=0;dir<8;dir++){
      int x1 = dirx[dir];
      int y1 = diry[dir];
      for(int dist = 1;dist < 17;dist++){

        int xi = dist*x1 + x;  // new position from x to xi 
        int yi = dist*y1 + y;  // new position from y to yi
        
        if(checkPosition(board,xi,yi) == false){
          break;
        }
        
        // throwing arrow from new position (xi,yi)
        for(int dir1 = 0;dir1<8;dir1++){

          //select a direction
          int x1 = dirx[dir1];
          int y1 = diry[dir1];
          
          for(int dist1 = 1;dist1 < 18;dist1++){
            int xarrow = dist1 * x1 + xi;
            int yarrow = dist1 * y1 + yi;
            
            if(checkArrow(board,xarrow,yarrow,x,y) == false){
              break;
            }
            //final move is pushed back
            s = (char)(x+65);
            s += (char)(y+65);
            s += (char)(xi+65);
            s += (char)(yi+65);
            s += (char)(xarrow+65);
            s += (char)(yarrow+65);
            move.push_back(s);

            // cannot go beyond a bomb
            if(board[xarrow][yarrow] == 5)
              break;
          }
        }
      }
    }
  }
}

void copyToFrom(int child[][12],int board[][12],string s,int player){
  for(int i=0;i<12;i++){
    for(int j=0;j<12;j++){
      child[i][j] = board[i][j];
    }
  }
  int x,y;
  
  child[s[0]-65][s[1]-65] = 0;
  child[s[2]-65][s[3]-65] = player;
  
  x = s[4]-65;
  y = s[5]-65;
  // if arrow lands on board
  if(board[x][y] == 5){
    child[x][y] = 0;
    for(int i=0;i<8;i++){
      x = dirx[i];
      y = diry[i];
      child[x][y] = 0;
    }
  }
  else{
    child[x][y] = -1;
  }
}
// Minimax and alpha beta pruning
int scoreEval(int board[][12]){
  vi bluex,bluey,redx,redy;
  for(int i=0;i<12;i++){
    for(int j=0;j<12;j++){
      int p = board[i][j];
      if(p == 1){
        bluex.push_back(i);
        bluey.push_back(j);
      }
      else
      if(p == 2){
        redx.push_back(i);
        redy.push_back(j);
      }
    }
  }
  // diagnol cnt
  int cntRed1=0;
  int cntBlue1=0;
  //non-diagonal cnt
  int cntBlue2=0;
  int cntRed2=0;
  for(int i=0;i<4;i++){
    int x = bluex[i];
    int y = bluey[i];
 //   cout << x << " " << y << endl;
    for(int j=0;j<8;j++){
      int x1 = dirx[j];
      int y1 = diry[j];
      for(int k=1;k<17;k++){
        int x2 = k*x1+x;
        int y2 = k*y1+y;
       // cout << x2 << " " << y2 << endl;
        if(x2 > -1 && x2 < 12 && y2 > -1 && y2 < 12){
          if(board[x2][y2] == 0){
            if(j < 4) 
              cntBlue1++;
            else
              cntBlue2++;        
          //  cout << x2 << " " << y2 << endl;
          }
          else
            break;  
        }
        else
          break;
      }
    }
  }
  for(int i=0;i<4;i++){
    int x = redx[i];
    int y = redy[i];
    for(int j=0;j<8;j++){
      int x1 = dirx[j];
      int y1 = diry[j];
      for(int k=1;k<17;k++){
        int x2 = k*x1 + x;
        int y2 = k*y1 + y;
        if(x2 > -1 && x2 < 12 && y2 > -1 && y2 < 12){
          if(board[x2][y2] == 0){
            if(j < 4) 
              cntRed1++;
            else
              cntRed2++;
          }  
          else
            break;
        }
        else
          break;
      }
    }
  }
//  cout << cntRed << " " << cntBlue << endl;
  //exit(1);
  int p1 = cntRed1-cntBlue1;
  int p2 = cntRed2-cntBlue2;
  if(p1)
    return p1;
  return p2;
//  return rand() % 2020;
}
string ans;
int alphabeta(int board[][12],int depth,int a,int b,bool maximizingPlayer){
  
  if(depth == 0){
    return scoreEval(board);
  }
  vector<int> score;
  vector<string> move;
  int player = maximizingPlayer == true ? 2 : 1;
  calMoves(board,move,player);
  if(move.size() == 0){
    return scoreEval(board);
  }
  int child[12][12];
  if(maximizingPlayer == true){
    for(int moves=0;moves < move.size();moves++){
      
      // create child
      copyToFrom(child,board,move[moves],player);
      int p = alphabeta(child,depth-1,a,b,false);
      if(a < p){
        a = p;
        ans1 = a;
        ans = move[moves];
      }
      if(a >= b)
        break;
    }
    return a;
  }
  else{
    for(int moves=0;moves < move.size();moves++){      
      copyToFrom(child,board,move[moves],player);
      b = min(b,alphabeta(child,depth-1,a,b,true));
      if(a >= b)
        break;
    }
    return b;
  }
}

int main(){
  int turn; 
  si(turn);
  int board[12][12];
  if(turn == 2){
    // take current board input
    scanBoard(board);
    // Minimax 
    
    alphabeta(board,1,-INF,INF,true);
    cout << ans << endl;
  }
  return 0;
}