#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include<QList>
enum StoneType {
    BLACK , WHITE , NONE
};
template <int N>
class GameBoard
{
    StoneType board[N][N];
public:
    GameBoard();

public :
    bool putStone (int x, int y, StoneType stone );
    bool removeStone (int x, int y);
    StoneType checkStone (int x, int y) const ;
    static const int boardSize () {
        return N;
    }
    void initialBoard();

    bool hasNeighbor(int i,int j,StoneType color);

};
typedef GameBoard <15> FiveGameBoard ;
typedef GameBoard <19> GoGameBoard ;

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
    if(board[x][y]!=NONE){
        board[x][y]=NONE;
        return true;
    }
    return false;

}
template <int N>
StoneType  GameBoard<N>::checkStone (int x, int y) const
{
    return board[x][y];

}
template <int N>
bool GameBoard<N>::hasNeighbor(int i,int j,StoneType color){

    if(checkStone(i-1,j)==color||checkStone(i,j-1)==color||checkStone(i+1,j)==color||checkStone(i,j+1)==color)
        return true;
    else
        return false;

}

#endif // GAMEBOARD_H
