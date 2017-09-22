#include "gameboard.h"
template <int N>
GameBoard<N>::GameBoard()
{

}
template <int N>
 void  GameBoard<N>::initialBoard(){
     for(int i=0;i<N;i++){
         for(int j=0;j<N;j++)

            board[i][j]=NONE;
     }
 }

template <int N>
 bool  GameBoard<N>:: putStone (int x, int y, StoneType stone ){
    if(board[x][y]!=NONE)
        return false;
    board[x][y]=stone;

    return true;

}
template <int N>
bool GameBoard<N>:: removeStone (int x, int y){
    if(N==19){

    }

}
template <int N>
StoneType  GameBoard<N>::checkStone (int x, int y) const
{
    return board[x][y];

}
