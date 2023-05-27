#ifndef APPLICATION_H
#define APPLICATION_H

#include <ncurses.h>


void rotate(int **board, int x, int y, int rotateDirection, int dimension, int boardNbLine, int boardNbColumn);

int playToken(int **board, int playedColumn, int actualToken, int boardNbLine, int boardNbColumn);

void gravityApplication(int **board,int boardNbLine,int boardNbColumn);

void moveRotatePosition(int **board,int getchRota,int *x,int *y,int *choiceRotatePositionNotDone,int dimension,int RotatePositionX,int RotatePositionY,int boardNbLine,int boardNbColumn);

void moveChoiceRotateDirection(int boardChoiceRotateDirection[2],int getchChoiceRotateDirection,int *choiceRotateDirection,int *choiceRotateDirectionNotDone);

void moveTokenPlayingCursor(int *tokenChoiceBoard,int getchToken,int *tokenChoicePosition,int*choicePlayTokenNotDone,int boardNbColumn);

int choiceBeginWinWin(int getchBegin, int *choiceBeginWin, WINDOW *beginWin, int *specialgame);

int choicePause(int pauseCh,int*pauseChoice,WINDOW*pauseWin,WINDOW*saveWin);

void win_add(int **board,int win_index[5][2],int actualToken);

void BoardCopy(int** BoardMother, int** BoardSon, int rows, int columns);

int choiceColWin(int getchCol, int *choiceCol, WINDOW *choiceColWindow);

int choiceLineWin(int getchLine, int *choiceLine, WINDOW *choiceLineWindow);

int choiceNbPlayerWin(int getchCol, int *choiceCol, WINDOW *choiceColWindow);

#endif
