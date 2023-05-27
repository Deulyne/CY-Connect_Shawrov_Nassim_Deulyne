#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <ncurses.h>


void displayBoard(int **board, int boardNbLine, int boardNbColumn);

void displayTokenChoicePosition(int *tokenChoiceBoard, int tokenChoicePosition,int boardNbColumn);

void displayChoiceRotatePosition(int **board,int RotatePositionX,int RotatePositionY,int dimension,int boardNbLine,int boardNbColumn);

void displayChoiceRotateDirection(int boardChoiceRotateDirection[3],int boardNbColumn);

void winnerAnimation(int **board, int win_index[5][2],int actualToken,int boardNbLine,int boardNbColumn);

void displayBegin(WINDOW*beginWin);

void displayPause(WINDOW*pauseWin);

void displaySave(WINDOW*saveWin);

void displayLoading(WINDOW*loadingWin);

void displayChoiceColWin(WINDOW* choiceColWin);

void spacementY(int minusSpaceLine);

void spacementX(int minusSpaceColumn);

#endif
