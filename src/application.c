#include "../include/application.h"
#include "../include/affichage.h"
#include "../include/verification.h"
#include <ncurses.h>
#include <unistd.h>

void rotate(int **board, int x, int y, int rotateDirection, int dimension, int boardNbLine, int boardNbColumn) {  
    //création tableau fils "rotatedSonBoard" carré de taille dimension à partir des coordonnées (x,y)
    int rotatedSonBoard[dimension][dimension];

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            rotatedSonBoard[i][j] = board[y + i][x + j];
        }
    }
    
    //1= rotation à droite, autre= rotation à gauche
    if (rotateDirection == 1) {
        //rotation 90° vers la droite
        for (int i = 0; i < dimension / 2; i++) {
            for (int j = i; j < dimension - i - 1; j++) {
                int temp = rotatedSonBoard[i][j];
                rotatedSonBoard[i][j] = rotatedSonBoard[dimension - j - 1][i];
                rotatedSonBoard[dimension - j - 1][i] = rotatedSonBoard[dimension - i - 1][dimension - j - 1];
                rotatedSonBoard[dimension - i - 1][dimension - j - 1] = rotatedSonBoard[j][dimension - i - 1];
                rotatedSonBoard[j][dimension - i - 1] = temp;
            }
        }
    } 
    else {
        //rotation 90° vers la gauche (on applique juste 3x la rotation à droite)
        for(int m=0;m<3;m++){
            for (int i = 0; i < dimension / 2; i++) {
                for (int j = i; j < dimension - i - 1; j++) {
                    int temp = rotatedSonBoard[i][j];
                    rotatedSonBoard[i][j] = rotatedSonBoard[dimension - j - 1][i];
                    rotatedSonBoard[dimension - j - 1][i] = rotatedSonBoard[dimension - i - 1][dimension - j - 1];
                    rotatedSonBoard[dimension - i - 1][dimension - j - 1] = rotatedSonBoard[j][dimension - i - 1];
                    rotatedSonBoard[j][dimension - i - 1] = temp;
                }
            }
        }
    }
  
    //imbriquement tableau fils dans le tableau père 
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            board[y + i][x + j] = rotatedSonBoard[i][j];
        }
    }
}

int playToken(int **board, int playedColumn, int actualToken, int boardNbLine, int boardNbColumn) {
    // Vérifier si la colonne n'est pas obstruée par un bloc
    if (board[0][playedColumn] != ' ') {
        return 0;
    }

    // Parcourir les lignes de la colonne pour simuler le mouvement du jeton
    for (int i = 0; i < boardNbLine; i++) {
        // Placer temporairement le jeton dans la case courante
        board[i][playedColumn] = actualToken;

        // Effacer l'écran et afficher la board modifiée temporairement
        usleep(100000);
        clear();
        displayBoard(board, boardNbLine, boardNbColumn);

        // Retirer temporairement le actualToken de la case courante
        board[i][playedColumn] = ' ';

        // Si la case courante est vide et la case suivante est pleine, ou si nous sommes à la dernière ligne de la colonne
        if (board[i][playedColumn] == ' ' && (i == boardNbLine-1 || board[i+1][playedColumn] != ' ')) {
            // Placer le actualToken dans la case courante
            board[i][playedColumn] = actualToken;

            // Effacer l'écran et retourner 1 pour indiquer que le coup a été joué avec succès
            clear();
            return 1;
        }
    }

}


//procédure qui va appliquer la gravité au tableau de jeu
void gravityApplication(int **board,int boardNbLine,int boardNbColumn){
    //on parcourt tout le tableau
    for(int k=0;k<(boardNbLine-1);k++){
        for(int i=0;i<boardNbColumn;i++){
            //boucle for qui tourne plus grand au plus petit pour appliquer la gravité par le haut
            for(int f=boardNbLine-1;f!=0;f--){
                //vérifie si la case est un espace, que la case au dessus est n'est pas un espace ni un bloc solide
                if(board[f][i]==' ' && board[f-1][i]!=' ' && board[f-1][i]!=0x2B1B){
                    board[f][i] = board[f-1][i];
                    board[f-1][i]=' ';
                }
            }
        }
        //on affiche le tableau après chaque itération de colonne
        clear();
        displayBoard(board,boardNbLine,boardNbColumn);
        usleep(100000);
    }
    clear();
}

//système qui règle le backend du déplacement pour le choix de la colonne de jeu
void moveTokenPlayingCursor(int *tokenChoiceBoard,int getchToken,int *tokenChoicePosition,int*choicePlayTokenNotDone,int boardNbColumn){
    switch (getchToken)
    {
        case KEY_LEFT:
            if((*tokenChoicePosition)>0){
                tokenChoiceBoard[(*tokenChoicePosition)]=' ';
                (*tokenChoicePosition)--;
                tokenChoiceBoard[(*tokenChoicePosition)]='^';
            }
            else{
                tokenChoiceBoard[(*tokenChoicePosition)]=' ';
                (*tokenChoicePosition)=boardNbColumn-1;
            }
            break;

        case KEY_RIGHT:
            if((*tokenChoicePosition)<boardNbColumn-1){
                tokenChoiceBoard[(*tokenChoicePosition)]=' ';
                (*tokenChoicePosition)++;
                tokenChoiceBoard[(*tokenChoicePosition)]='^';
            }
            else{
                tokenChoiceBoard[(*tokenChoicePosition)]=' ';
                (*tokenChoicePosition)=0;
            }
            break;

        case '\n':
            (*choicePlayTokenNotDone)--;
    }
    clear();
}

//système qui règle le backend du déplacement pour le choix de la position de la rotation
void moveRotatePosition(int **board,int getchRota,int *x,int *y,int *choiceRotatePositionNotDone,int dimension,int RotatePositionX,int RotatePositionY,int boardNbLine,int boardNbColumn){
    switch(getchRota) {
        case KEY_LEFT:
            if ((*y) > 0) {
                (*y)--;
            }
            break;

        case KEY_RIGHT:
            if ((*y) < boardNbColumn-dimension) {
                (*y)++;
            }
            break;

        case KEY_UP:
            if ((*x) > 0) {
                (*x)--;
            }
            break;

        case KEY_DOWN:
            if ((*x) < boardNbLine-dimension) {
                (*x)++;
            }
            break;

        case '\n':
            if(verifyRotate(board,RotatePositionX,RotatePositionY,dimension)){
                (*choiceRotatePositionNotDone)--;
                break;
            }
    }
    clear();
}

//système qui règle le backend du déplacement pour le choix de la rotation
void moveChoiceRotateDirection(int boardChoiceRotateDirection[3],int getchChoiceRotateDirection,int *choiceRotateDirection,int *choiceRotateDirectionNotDone){
    switch (getchChoiceRotateDirection){
        case KEY_UP:
            if((*choiceRotateDirection)>0){
                boardChoiceRotateDirection[(*choiceRotateDirection)]=' ';
                (*choiceRotateDirection)--;
                boardChoiceRotateDirection[(*choiceRotateDirection)]='>';
            }
            break;

        case KEY_DOWN:
            if((*choiceRotateDirection)<2){
                boardChoiceRotateDirection[(*choiceRotateDirection)]=' ';
                (*choiceRotateDirection)++;
                boardChoiceRotateDirection[(*choiceRotateDirection)]='>';
            }
            break;

        case '\n':
            if((*choiceRotateDirection)!=1){
                (*choiceRotateDirectionNotDone)--;
                break;
            }
    }
    clear();
}

//système 2 en 1 qui règle l'affichage du menu du début
int choiceBeginWinWin(int getchBegin, int *choiceBeginWin, WINDOW *beginWin, int *specialgame) {
    switch (getchBegin) {
        case KEY_UP:
            // Déplacer le curseur vers le haut
            if (*choiceBeginWin > 1) {
                mvwprintw(beginWin, LINES / 2 + 6 + (*choiceBeginWin - 1) * 2, (COLS - 8) / 2, "%s", (*choiceBeginWin == 2) ? "CONTINUE" : "  QUIT");
                (*choiceBeginWin)--;
                wattron(beginWin, A_BOLD | COLOR_PAIR(2));
                mvwprintw(beginWin, LINES / 2 + 6 + (*choiceBeginWin - 1) * 2, (COLS - 8) / 2, "%s", (*choiceBeginWin == 1) ? "NEW GAME" : "CONTINUE");
                wattroff(beginWin, A_BOLD | COLOR_PAIR(2));
                wrefresh(beginWin);
            }
            break;

        case KEY_DOWN:
            // Déplacer le curseur vers le bas
            if (*choiceBeginWin < 3) {
                mvwprintw(beginWin, LINES / 2 + 6 + (*choiceBeginWin - 1) * 2, (COLS - 8) / 2, "%s", (*choiceBeginWin == 2) ? "CONTINUE" : "NEW GAME");
                (*choiceBeginWin)++;
                wattron(beginWin, A_BOLD| COLOR_PAIR(2));
                mvwprintw(beginWin, LINES / 2 + 6 + (*choiceBeginWin - 1) * 2, (COLS - 8) / 2, "%s", (*choiceBeginWin == 2) ? "CONTINUE" : "  QUIT");
                wattroff(beginWin, A_BOLD| COLOR_PAIR(2));
                wrefresh(beginWin);
            }
            break;

        case KEY_RIGHT:
            //ajout de l'easter egg
            (*specialgame)++;
            break;
        case '\n':  // Touche Entrée
            // Traitement de l'option sélectionnée
            // faire sauvegarde si choix == opt 2+1
            return *choiceBeginWin;
    }
}

//système 2 en 1 qui règle l'affichage du menu du début
int choicePause(int pauseCh,int*pauseChoice,WINDOW*pauseWin,WINDOW*saveWin){
    switch(pauseCh){
            case KEY_DOWN:
                // Déplacer le curseur vers le bas
                if (*pauseChoice < 4) {
                    //attroff(COLOR_PAIR(2));
                    mvwchgat(pauseWin, (*pauseChoice) * 2 - 1, 2, 32, A_NORMAL, 0, NULL);
                    (*pauseChoice)++;
                    mvwchgat(pauseWin, (*pauseChoice) * 2 - 1, 2, 32, A_BOLD , 2, NULL);
                    //attron(COLOR_PAIR(2));
                    wrefresh(pauseWin);
                }
                break;

            case KEY_UP:
                // Déplacer le curseur vers le haut
                if (*pauseChoice >2) {
                    // box(win, ACS_VLINE, ACS_HLINE);
                    // wmove(win, choix * 2 - 1, 2);
                    mvwchgat(pauseWin, (*pauseChoice) * 2 - 1, 2, 32, A_NORMAL, 0, NULL);
                    (*pauseChoice)--;
                    mvwchgat(pauseWin, (*pauseChoice) * 2 - 1, 2, 32, A_BOLD , 2, NULL);
                    
                    wrefresh(pauseWin);
                }
                break;
            case '\n':  // Touche Entrée
                // Traitement de l'option sélectionnée
                // faire sauvegarde si choix == opt 2+1
                if((*pauseChoice)==3){
                    displaySave(saveWin);
                }
                endwin();
                return *pauseChoice;
    }
}

//procédure qui va ajouter les coordonnées des jetons gagnant dans la tableau souhaité
void win_add(int **board,int win_index[5][2],int actualToken){
    for (int i = 0; i < 5; i++){
            board[win_index[i][0]][win_index[i][1]]=actualToken;   
        }
}

//procédure qui copie un tableau dans un autre
void BoardCopy(int** BoardMother, int** BoardSon, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            BoardSon[i][j] = BoardMother[i][j];
        }
    }
}

//fonction 2 en 1 qui règle le choix du nombre de colonne de jeu
int choiceColWin(int getchCol, int *choiceCol, WINDOW *choiceColWindow) {
    switch (getchCol) {
        case KEY_UP:
            // Déplacer le curseur vers le haut
            if (*choiceCol > 1) {
                mvwprintw(choiceColWindow, LINES / 2 + 6 + (*choiceCol - 1) * 2, (COLS - 8) / 2+2, "%s", (*choiceCol == 2) ? "9" : "10");
                (*choiceCol)--;
                wattron(choiceColWindow, A_BOLD | COLOR_PAIR(2));
                mvwprintw(choiceColWindow, LINES / 2 + 6 + (*choiceCol - 1) * 2, (COLS - 8) / 2+2, "%s", (*choiceCol == 1) ? "8" : "9");
                wattroff(choiceColWindow, A_BOLD | COLOR_PAIR(2));
                wrefresh(choiceColWindow);
            }
            break;

        case KEY_DOWN:
            // Déplacer le curseur vers le bas
            if (*choiceCol < 3) {
                mvwprintw(choiceColWindow, LINES / 2 + 6 + (*choiceCol - 1) * 2, (COLS - 8) / 2+2, "%s", (*choiceCol == 2) ? "9" : "8");
                (*choiceCol)++;
                wattron(choiceColWindow, A_BOLD| COLOR_PAIR(2));
                mvwprintw(choiceColWindow, LINES / 2 + 6 + (*choiceCol - 1) * 2, (COLS - 8) / 2+2, "%s", (*choiceCol == 2) ? "9" : "10");
                wattroff(choiceColWindow, A_BOLD| COLOR_PAIR(2));
                wrefresh(choiceColWindow);
            }
            break;

        case '\n':  // Touche Entrée
            // Traitement de l'option sélectionnée
            // faire sauvegarde si choix == opt 2+1
            return *choiceCol+7;
    }
}

//fonction 2 en 1 qui règle le choix du nombre de ligne de jeu
int choiceLineWin(int getchCol, int *choiceCol, WINDOW *choiceColWindow) {
    switch (getchCol) {
        case KEY_UP:
            // Déplacer le curseur vers le haut
            if (*choiceCol > 1) {
                mvwprintw(choiceColWindow, LINES / 2 + 6 + (*choiceCol - 1) * 2, (COLS - 8) / 2+2, "%s", (*choiceCol == 2) ? "7" : "8");
                (*choiceCol)--;
                wattron(choiceColWindow, A_BOLD | COLOR_PAIR(2));
                mvwprintw(choiceColWindow, LINES / 2 + 6 + (*choiceCol - 1) * 2, (COLS - 8) / 2+2, "%s", (*choiceCol == 1) ? "6" : "7");
                wattroff(choiceColWindow, A_BOLD | COLOR_PAIR(2));
                wrefresh(choiceColWindow);
            }
            break;

        case KEY_DOWN:
            // Déplacer le curseur vers le bas
            if (*choiceCol < 3) {
                mvwprintw(choiceColWindow, LINES / 2 + 6 + (*choiceCol - 1) * 2, (COLS - 8) / 2+2, "%s", (*choiceCol == 2) ? "7" : "6");
                (*choiceCol)++;
                wattron(choiceColWindow, A_BOLD| COLOR_PAIR(2));
                mvwprintw(choiceColWindow, LINES / 2 + 6 + (*choiceCol - 1) * 2, (COLS - 8) / 2 +2, "%s", (*choiceCol == 2) ? "7" : "8");
                wattroff(choiceColWindow, A_BOLD| COLOR_PAIR(2));
                wrefresh(choiceColWindow);
            }
            break;

        case '\n':  // Touche Entrée
            // Traitement de l'option sélectionnée
            // faire sauvegarde si choix == opt 2+1
            return *choiceCol+5;
    }
}

//fonction 2 en 1 qui règle le choix du nombre de joueur
int choiceNbPlayerWin(int getchCol, int *choiceCol, WINDOW *choiceColWindow) {
    switch (getchCol) {
        case KEY_UP:
            // Déplacer le curseur vers le haut
            if (*choiceCol > 2) {
                mvwprintw(choiceColWindow, LINES / 2 + 9, (COLS - 8) / 2 + 2, "%s", "3");
                (*choiceCol)--;
                wattron(choiceColWindow, A_BOLD | COLOR_PAIR(2));
                mvwprintw(choiceColWindow, LINES / 2 + 7, (COLS - 8) / 2 + 2, "%s", "2");
                wattroff(choiceColWindow, A_BOLD | COLOR_PAIR(2));
                wrefresh(choiceColWindow);
            }
            break;

        case KEY_DOWN:
            // Déplacer le curseur vers le bas
            if (*choiceCol < 3) {
                mvwprintw(choiceColWindow, LINES / 2 + 7, (COLS - 8) / 2 + 2, "%s", "2");
                (*choiceCol)++;
                wattron(choiceColWindow, A_BOLD | COLOR_PAIR(2));
                mvwprintw(choiceColWindow, LINES / 2 + 9, (COLS - 8) / 2 + 2, "%s", "3");
                wattroff(choiceColWindow, A_BOLD | COLOR_PAIR(2));
                wrefresh(choiceColWindow);
            }
            break;

        case '\n':  // Touche Entrée
            // Traitement de l'option sélectionnée
            // faire sauvegarde si choix == opt 2+1
            return *choiceCol ;
    }
}

