#ifndef VERIFICATION_H
#define VERIFICATION_H


#define WIN_LENGTH 5

int verifyRotate(int **board,int x,int y,int dimensioin);

int verifyWinner(int **board, int actualToken,int win_index[5][2],int boardNbLine,int boardNbColumn);

int verifyDraw(int **board,int boardNbLine,int boardNbColumn);

int isFileEmpty(const char *filename);

int tokenWinner(int **board, int boardNbLine, int boardNbColumn);


#endif

