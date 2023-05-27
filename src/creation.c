#include "../include/creation.h"
#include <ncurses.h>


//creation du tableau de jeu avec les bloc solide dans les coins
void initialiseBoard(int **board,int boardNbLine,int boardNbColumn){
    
    for (int i = 0; i < boardNbLine; i++) {
        for (int j = 0; j < boardNbColumn; j++) {
            board[i][j] = ' ';
        }
    }
    
    board[0][0]=0x2B1B;
    board[0][boardNbColumn-1]=0x2B1B;
    board[boardNbLine-1][0]=0x2B1B;
    board[boardNbLine-1][boardNbColumn-1]=0x2B1B;

}

//creation du tableau qui servira au joueur de jouer sur la colonne qu'il souhaite
void createChoiceBoardToken(int *tokenChoiceBoard,int boardNbColumn){
    for(int i=0;i<boardNbColumn;i++){
        tokenChoiceBoard[i]=' ';
    }
}

//creation du tableau qui servira au joueur de jouer le sens de la rotation qu'il souhaite
void createChoiceBoardRotateDirection(int boardChoiceRotateDirection[3]){
    boardChoiceRotateDirection[0]=' ';
    boardChoiceRotateDirection[1]='>';
    boardChoiceRotateDirection[2]=' ';
}




