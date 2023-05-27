#include "../include/application.h"
#include "../include/affichage.h"
#include <ncurses.h>
#include <unistd.h>

//stratagème pour afficher du printw au milieu sans fenêtre sur l'axe X
void spacementX(int minusSpaceColumn){
    int termWidth, termHeight;
    getmaxyx(stdscr,termHeight, termWidth);
    for(int i=0;i<termWidth/2-minusSpaceColumn;i++){
        printw(" ");
    }
}

//stratagème pour afficher du printw au milieu sans fenêtre sur l'axe Y
void spacementY(int minusSpaceLine){
    int termWidth, termHeight;
    getmaxyx(stdscr,termHeight, termWidth);
    for(int i=0;i<termHeight/2-minusSpaceLine;i++){
        printw("\n");
    }
}

//affichage du tableau complet 
void displayBoard(int **board, int boardNbLine, int boardNbColumn) {
    spacementY(boardNbLine-2);
    spacementX(boardNbColumn+3);

    for (int i = 0; i < boardNbLine; i++) {
        for (int j = 0; j < boardNbColumn; j++) {
            if(board[i][j]==' '){
                printw("|  ");
            }
            else{
                printw("|%lc", board[i][j]);
            }
        }
        printw("|\n");
        spacementX(boardNbColumn+3);
    }

    for(int i=0;i<boardNbColumn;i++){
        printw("___");
    }
    printw("_\n");
    spacementX(boardNbColumn+3);
    for(int i=0;i<boardNbColumn;i++){
        if(i>8){
            printw(" %d",i+1);
        }
        else{
            printw("  %d",i+1);
        }
    }
    printw("\n");
    refresh();
}

//frontend du choix de la colonne à jouer
void displayTokenChoicePosition(int *tokenChoiceBoard, int tokenChoicePosition,int boardNbColumn){
    tokenChoiceBoard[tokenChoicePosition]='^';
    spacementX(boardNbColumn+3);
    for(int i=0;i<boardNbColumn;i++){
        attron(COLOR_PAIR(1));
        printw("  %c",tokenChoiceBoard[i]);
        attroff(COLOR_PAIR(1));
    }
    refresh();
}

//frontend du choix du bloc à faire pivoter
void displayChoiceRotatePosition(int **board,int RotatePositionX,int RotatePositionY,int dimension,int boardNbLine,int boardNbColumn){
    spacementY(boardNbLine-2);
    spacementX(boardNbColumn+3);
    for (int i = 0; i < boardNbLine; i++) {
        for (int j = 0; j < boardNbColumn; j++) {
            if(i>=RotatePositionX && i<RotatePositionX+dimension && j>=RotatePositionY && j<RotatePositionY+dimension){
                printw("|");
                attron(A_REVERSE);
                if(board[i][j]==' '){
                    printw("  ");
                }
                else{
                    printw("%lc",board[i][j]);
                }
                attroff(A_REVERSE);
            }
            else{
                if(board[i][j]==' '){
                    printw("|  ");
                }
                else{
                    printw("|%lc", board[i][j]);
                }
            }
        } 
    printw("|\n");
    spacementX(boardNbColumn+3);
    }
}

//frontend du choix du sens de la rotation
void displayChoiceRotateDirection(int boardChoiceRotateDirection[3],int boardNbColumn){
    printw("\n");
    spacementX(boardNbColumn+3);
    printw("rotation:\n\n");
    spacementX(boardNbColumn+3);
    int emoji = 0x0001F538;
    if(boardChoiceRotateDirection[0]=='>'){
        attron(COLOR_PAIR(2));
        printw("   %lc à gauche %lc\n",emoji,emoji);
        attroff(COLOR_PAIR(2));
        spacementX(boardNbColumn+3);
        printw("   voir l'original\n");
        spacementX(boardNbColumn+3);
        printw("      à droite");
    }
    else if(boardChoiceRotateDirection[1]=='>'){
        printw("      à gauche\n");
        spacementX(boardNbColumn+3);
        attron(COLOR_PAIR(2));
        printw("   voir l'original\n");
        attroff(COLOR_PAIR(2));
        spacementX(boardNbColumn+3);
        printw("      à droite");
    }
    else{
        printw("      à gauche\n");
        spacementX(boardNbColumn+3);
        printw("   voir l'original\n");
        spacementX(boardNbColumn+3);
        attron(COLOR_PAIR(2));
        printw("   %lc à droite %lc",emoji,emoji);
        attroff(COLOR_PAIR(2));
    }
}


//animation lorsqu'il y a un winner
void winnerAnimation(int **board, int win_index[5][2],int actualToken,int boardNbLine,int boardNbColumn){
    displayBoard(board,boardNbLine,boardNbColumn);
    printw("\n");
    spacementX(boardNbColumn-3);
    printw("on a un gagnant !\n");
    refresh();
    sleep(2);
    for(int i=0;i<boardNbColumn;i++){
        for(int j=0;j<boardNbLine;j++){
            board[j][i] = ' ';
        }
        win_add(board,win_index,actualToken);
        clear();
        displayBoard(board,boardNbLine,boardNbColumn);
        refresh();
        usleep(100000);
    }
    printw("\n");
    spacementX(boardNbColumn+boardNbColumn/2);
    printw("le joueur %lc a gagné, bravo à lui\n",actualToken);
    printw("\n\n\n");
    spacementX(boardNbColumn+boardNbColumn/2);
    printw("Appuyez sur une touche pour quitter");
}


//affichage du menu de démarrage
void displayBegin(WINDOW* beginWin) {
    wrefresh(beginWin);

    int boxWidth = 80;  // Largeur de la box augmentée
    int boxHeight = 24; // Hauteur de la box augmentée

    // Calcul de la position de départ pour centrer la fenêtre d'écriture
    int startX = (COLS - boxWidth) / 2;
    int startY = (LINES - boxHeight) / 2;

    // Création d'une nouvelle fenêtre pour la boîte
    WINDOW* boxWin = newwin(boxHeight+3, boxWidth, startY, startX);
    box(boxWin, ACS_VLINE, ACS_HLINE);
    wrefresh(boxWin);

    // Création d'une sous-fenêtre pour le texte à l'intérieur de la boîte
    WINDOW* textWin = derwin(boxWin, boxHeight - 4, boxWidth - 4, 2, 2);

    // Ajustement pour centrer le texte
    int textStartX = (boxWidth - 67) / 2;
    int textStartY = (boxHeight - 21) / 2;

    mvwprintw(textWin, textStartY, textStartX, " __        __     _                                _          ");
    mvwprintw(textWin, textStartY + 1, textStartX, " \\ \\      / /___ | |  ___  ___   _ __ ___    ___  | |_  ___   ");
    mvwprintw(textWin, textStartY + 2, textStartX, "  \\ \\ /\\ / // _ \\| | / __|/ _ \\ | '_ ` _ \\  / _ \\ | __|/ _ \\ ");
    mvwprintw(textWin, textStartY + 3, textStartX, "   \\ V  V /|  __/| || (__| (_) || | | | | ||  __/ | |_| (_) |   ");
    mvwprintw(textWin, textStartY + 4, textStartX, "    \\_/\\_/  \\___||_| \\___|\\___/ |_| |_| |_| \\___|  \\__|\\___/    ");
    mvwprintw(textWin, textStartY + 5, textStartX, "                                                                       ");
    mvwprintw(textWin, textStartY + 6, textStartX, "  ____ __   __        ____                                  _   ");
    mvwprintw(textWin, textStartY + 7, textStartX, " / ___|\\ \\ / /       / ___| ___   _ __   _ __    ___   ___ | |_ ");
    mvwprintw(textWin, textStartY + 8, textStartX, "| |     \\ V /       | |    / _ \\ | '_ \\ | '_ \\  / _ \\ / __|| __|");
    mvwprintw(textWin, textStartY + 9, textStartX, "| |___   | |	  | |___ |(_)| | | | || | | ||  __/| (__ | |_ ");
    mvwprintw(textWin, textStartY + 10, textStartX," \\____|  |_|	   \\____|\\___/ |_| |_||_| |_| \\___| \\___| \\__|");
    wrefresh(textWin);

    // Calcul de la position du centre pour le texte de la boîte
    int textCenterX = (boxWidth - 8) / 2;

    mvwprintw(boxWin, boxHeight / 2 + 6, textCenterX, "NEW GAME");
    mvwprintw(boxWin, boxHeight / 2 + 8, textCenterX, "CONTINUE");
    mvwprintw(boxWin, boxHeight / 2 + 10, textCenterX, "  QUIT");
    wrefresh(boxWin);
    wrefresh(beginWin);
}

//affichage du menu de pause
void displayPause(WINDOW*pauseWin){
    wrefresh(pauseWin);
    mvwprintw(pauseWin, 1, 16, "PAUSE");
    mvwprintw(pauseWin, 3, 13, "Reprendre");
    mvwprintw(pauseWin, 5, 13, "Sauvegarder la partie");
    mvwprintw(pauseWin, 7, 13, "Quitter");
    box(pauseWin, ACS_VLINE, ACS_HLINE);
    refresh();
    wrefresh(pauseWin);
}

//procédure de l'affichage de l'écran de sauvegarde
void displaySave(WINDOW*saveWin){
    box(saveWin, ACS_VLINE, ACS_HLINE);

    refresh();
    wrefresh(saveWin);
    mvwprintw(saveWin, 5, 10, "sauvegarde en cours.  ");
    wrefresh(saveWin);
    usleep(200000);
    mvwprintw(saveWin, 5, 10, "sauvegarde en cours.. ");
    wrefresh(saveWin);
    usleep(200000);
    mvwprintw(saveWin, 5, 10, "sauvegarde en cours...");
    wrefresh(saveWin);
    usleep(200000);
    mvwprintw(saveWin, 5, 10, "sauvegarde en cours.  ");
    wrefresh(saveWin);
    usleep(200000);
}

//procédure de l'affichage de l'écan de chargement
void displayLoading(WINDOW*loadingWin){
    box(loadingWin, ACS_VLINE, ACS_HLINE);

    refresh();
    wrefresh(loadingWin);
    mvwprintw(loadingWin, 5, 10, "chargement en cours.  ");
    wrefresh(loadingWin);
    usleep(200000);
    mvwprintw(loadingWin, 5, 10, "chargement en cours.. ");
    wrefresh(loadingWin);
    usleep(200000);
    mvwprintw(loadingWin, 5, 10, "chargement en cours...");
    wrefresh(loadingWin);
    usleep(200000);
    mvwprintw(loadingWin, 5, 10, "chargement en cours.  ");
    wrefresh(loadingWin);
    usleep(200000);
}


