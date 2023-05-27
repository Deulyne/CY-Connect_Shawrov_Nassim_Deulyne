#include "../include/verification.h"
#include "../include/creation.h"
#include "../include/application.h"
#include "../include/affichage.h"

#include <ncurses.h>
#include <unistd.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

int main(){
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    assume_default_colors(COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);



    //initialisation des différentes fenêtres
    WINDOW* beginWin = newwin(LINES, COLS, 0,0);
    WINDOW* pauseWin = newwin(12, 40, LINES / 2 - 6, COLS / 2 - 16);
    WINDOW* saveWin = newwin(12, 40, LINES / 2 - 6, COLS / 2 - 16);
    WINDOW* loadingWin = newwin(12, 40, LINES / 2 - 6, COLS / 2 - 16);

    //variables permettant  de rediriger le joueur après qu'il ait choisi ce qu'il veut faire
    int choiceBeginWin = 1;
    int choiceCol=1;
    int choiceLine=1;
    int choiceNbPlayer=2;

    //initialisation des variables qui vont permettre de récupérer les touches saisies par l'utilisateur
    int getchBegin;
    int getchCol;
    int getchLine;
    int getchNbPlayer;
    int getchToken;

    //variables permettant de rediriger le joueur après qu'il ait choisi s'il veut jouer continuer ou quitter
    int WhatToDo;

    //initialisation des différents actualTokens
    int tokenP1 = 0x0001F535; //actualToken bleu
    int tokenP2 = 0x0001F534; //actualToken rouge
    int tokenP3 = 0x0001F7E2; //actualToken vert
    int tokenP4 = 0x0001F7E1; //actualToken jaune

    //initialisation des variables pour premettre le jeu
    int nbPlayer;
    int boardNbLine,boardNbColumn;

    int specialgame=0;

    refresh();
    
    //affichage de la premlière fenêtre et écriture de "NEW GAME" en couleur
    displayBegin(beginWin);
    wattron(beginWin, A_BOLD| COLOR_PAIR(2));
    mvwprintw(beginWin, LINES / 2 + 6 , (COLS - 8) / 2, "NEW GAME");
    wattroff(beginWin, A_BOLD| COLOR_PAIR(2));
    wrefresh(beginWin);



    while (getchBegin!='\n') {
        getchBegin = getch();         // Lecture de l'entrée utilisateur

        WhatToDo=choiceBeginWinWin(getchBegin,&choiceBeginWin,beginWin,&specialgame);

        //on vérifie si le fichier est vide (si c'est le cas, il ne se passe rien)
        const char *saveVerification = "./save/save1.txt";
        int empty = isFileEmpty(saveVerification);
        if(WhatToDo==2 && empty){
            getchBegin=0;
        }
    }
    if(WhatToDo==1 || WhatToDo==2){

        

        int actualToken = tokenP1; //initialisation avec le actualToken bleu (joueur 1)


        
        int **TempBoard;

        if(WhatToDo==2){
            clear();
            FILE *save = fopen("./save/save1.txt", "r");
            fscanf(save,"%d",&boardNbLine);
            fscanf(save,"%d",&boardNbColumn);
            fscanf(save,"%d",&nbPlayer);
            fscanf(save,"%x",&actualToken);
            
            TempBoard = malloc(boardNbLine * sizeof(int *));
            for(int i = 0; i < boardNbLine; i++){
                TempBoard[i] = malloc(boardNbColumn * sizeof(int));
            }
            for (int i = 0; i < boardNbLine; i++) {
                for (int j = 0; j < boardNbColumn; j++) {
                    fscanf(save, "%d", &TempBoard[i][j]);
                }
            }

            fclose(save);
            displayLoading(loadingWin);
        }
        else if(specialgame!=5){
            //effacement des affichages du beginWin
            mvwprintw(beginWin, LINES / 2 + 6 , (COLS - 8) / 2, "                      ");
            mvwprintw(beginWin, LINES / 2 + 8 , (COLS - 8) / 2, "                      ");
            mvwprintw(beginWin, LINES / 2 + 10 , (COLS - 8) / 2, "                      ");

            //choix du nb de joueur
            mvwprintw(beginWin, LINES / 2 + 4 , (COLS - 20) / 2, "Combien êtes vous ?");
            wattron(beginWin, A_BOLD| COLOR_PAIR(2));
            mvwprintw(beginWin, LINES / 2 + 7 , (COLS - 8) / 2, "  2");
            wattroff(beginWin, A_BOLD| COLOR_PAIR(2));
            mvwprintw(beginWin, LINES / 2 + 9 , (COLS - 8) / 2, "  3");
            wrefresh(beginWin);
            while(getchNbPlayer!='\n'){
                getchNbPlayer=getch();
                nbPlayer= choiceNbPlayerWin(getchNbPlayer,&choiceNbPlayer,beginWin);
                wrefresh(beginWin);
            }


            mvwprintw(beginWin, LINES / 2 + 7 , (COLS - 8) / 2, "   ");
            mvwprintw(beginWin, LINES / 2 + 9 , (COLS - 8) / 2, "   ");



            //choix du nombre de colonne du plateau
            mvwprintw(beginWin, LINES / 2 + 4 , (COLS - 45) / 2, "choisissez le nombre de colonnes du plateau");
            wattron(beginWin, A_BOLD| COLOR_PAIR(2));
            mvwprintw(beginWin, LINES / 2 + 6 , (COLS - 8) / 2, "  8");
            wattroff(beginWin, A_BOLD| COLOR_PAIR(2));
            mvwprintw(beginWin, LINES / 2 + 8 , (COLS - 8) / 2, "  9");
            mvwprintw(beginWin, LINES / 2 + 10 , (COLS - 8) / 2, "  10");
            wrefresh(beginWin);
            while(getchCol!='\n'){
                getchCol=getch();
                boardNbColumn= choiceColWin(getchCol,&choiceCol,beginWin);
                wrefresh(beginWin);
            }

            //choix du nombre de ligne du plateau
            mvwprintw(beginWin, LINES / 2 + 4 , (COLS - 45) / 2, "  choisissez le nombre de ligne du plateau    ");
            wattron(beginWin, A_BOLD| COLOR_PAIR(2));
            mvwprintw(beginWin, LINES / 2 + 6 , (COLS - 8) / 2, "  6");
            wattroff(beginWin, A_BOLD| COLOR_PAIR(2));
            mvwprintw(beginWin, LINES / 2 + 8 , (COLS - 8) / 2, "  7");
            mvwprintw(beginWin, LINES / 2 + 10 , (COLS - 8) / 2, "  8 ");
            wrefresh(beginWin);
            while(getchLine!='\n'){
                getchLine=getch();
                boardNbLine= choiceLineWin(getchLine,&choiceLine,beginWin);
                wrefresh(beginWin);
            }



        }
        else{
            boardNbLine=12;
            boardNbColumn=14;
            nbPlayer=4;
        }

        //attribution de taille dans la ram pour le tableau 2D
        int **board = malloc(boardNbLine * sizeof(int *));
        for(int i = 0; i < boardNbLine; i++){
            board[i] = malloc(boardNbColumn * sizeof(int));
        }


        //si le joueur veut continuer, on copie le board dans le tempboard, sinon on créer un nouveau plateau de jeu avec les paramètres qu'il a demandé
        if(WhatToDo==2){
            BoardCopy(TempBoard,board,boardNbLine,boardNbColumn);
            for (int i = 0; i < boardNbLine; i++) {
                free(TempBoard[i]);
            }
            free(TempBoard);
        }
        else{
            initialiseBoard(board,boardNbLine,boardNbColumn);
        }

        int **rotatedBoard = malloc(boardNbLine * sizeof(int *));
        for(int i = 0; i < boardNbLine; i++){
            rotatedBoard[i] = malloc(boardNbColumn * sizeof(int));
        }

        int is_win =0;

        int win_index[5][2];

        displayBoard(board,boardNbLine,boardNbColumn);
        int tokenChoicePosition=0;
        int draw=0;


        //tant qu'il n'y a pas de gagnant
        while (!is_win) {

            int tokenChoiceBoard[boardNbColumn];
            
            int choicePlayTokenNotDone=1;
            int choiceRotatePositionNotDone=1;
            int choiceRotateDirectionNotDone=1;
            int choiceRotateDirection =1;


            int boardChoiceRotateDirection[2];
            createChoiceBoardToken(tokenChoiceBoard,boardNbColumn);
            createChoiceBoardRotateDirection(boardChoiceRotateDirection);

            int RotatePositionX=0;
            int RotatePositionY=0;
            
            clear();

            //initialisation de la dimension        
            srand(time(NULL));
            int dimension= rand() % 2;
            if(dimension==0){
                dimension=3;
            }
            else{
                dimension=5;
            }

            

            while(choicePlayTokenNotDone){
                clear();
                displayBoard(board,boardNbLine,boardNbColumn);
                displayTokenChoicePosition(tokenChoiceBoard,tokenChoicePosition,boardNbColumn);
                printw("\n\n");
                spacementX(boardNbColumn+boardNbColumn/2);
                printw("joueur %lc, à vous de jouer \n",actualToken);
                printw("\n\n\n\n");
                spacementX(boardNbColumn+boardNbColumn/2);
                printw("'e' pour mettre en pause");
                getchToken= getch();



                //système de pause
                int pauseChoice=2;
                int getchPause=0;
                int WhatToDoPause=0;
                if(getchToken== 'e' ){
                    clear();
                    displayPause(pauseWin);
                    mvwchgat(pauseWin, pauseChoice * 2 - 1, 2, 32, A_BOLD, 2, NULL);
                    wrefresh(pauseWin);
                    //sleep(2);
                    while(getchPause!='\n'){
                        getchPause=getch();
                        if(getchPause=='e'){
                            break;
                        }
                        WhatToDoPause = choicePause(getchPause,&pauseChoice,pauseWin,saveWin);
                    }
                    if(getchPause!='e'){

                        // 3=système de sauvegarde
                        if(WhatToDoPause==3){
                            FILE *save = fopen("./save/save1.txt", "w");
                            fprintf(save, "%d\n", boardNbLine);
                            fprintf(save, "%d\n", boardNbColumn);
                            fprintf(save, "%d\n", nbPlayer);
                            fprintf(save, "%x\n", actualToken);

                            for (int i = 0; i < boardNbLine; i++) {
                                for (int j = 0; j < boardNbColumn; j++) {
                                    fprintf(save, "%d ", board[i][j]);
                                    }
                                fprintf(save, "\n");
                            }
                            fclose(save);

                        }

                        //4=le joueur veut quitter le jeu
                        if(WhatToDoPause==4){
                            //libérer la mémoire allouée avec malloc
                            for (int i = 0; i < boardNbLine; i++) {
                                free(board[i]);
                            }
                            free(board);

                            // Libérer la mémoire allouée avec malloc
                            for (int i = 0; i < boardNbLine; i++) {
                                free(rotatedBoard[i]);
                            }
                            free(rotatedBoard);
                            endwin();
                            return 0;
                        }
                    }
                    //rénitialisation de la variable pour ne pas qu'il ne pose pas problème lors de la prochaine itération
                    WhatToDoPause=0;
                    
                }
                //si le joueur ne va pas dans le menu option, on le laisse jouer
                else{
                    moveTokenPlayingCursor(tokenChoiceBoard,getchToken,&tokenChoicePosition,&choicePlayTokenNotDone,boardNbColumn);
                }
            }

            if(playToken(board,tokenChoicePosition,actualToken,boardNbLine,boardNbColumn)){

                while(choiceRotatePositionNotDone){
                    displayChoiceRotatePosition(board,RotatePositionX,RotatePositionY,dimension,boardNbLine,boardNbColumn);
                    printw("\n\n");
                    spacementX(boardNbColumn+boardNbColumn/2);
                    printw("joueur %lc,\n",actualToken);
                    spacementX(boardNbColumn+boardNbColumn/2);
                    printw("vous avez obtenu la dimension %d\n",dimension);
                    spacementX(boardNbColumn+boardNbColumn/2);
                    printw("choisissez où vous voulez l'appliquer.");
                    printw("\n");
                    int getchRota= getch();

                    //x,y inversé
                    moveRotatePosition(board,getchRota,&RotatePositionX,&RotatePositionY,&choiceRotatePositionNotDone,dimension,RotatePositionY,RotatePositionX,boardNbLine,boardNbColumn);
                }
                BoardCopy(board,rotatedBoard,boardNbLine,boardNbColumn);

                while(choiceRotateDirectionNotDone){
                    displayChoiceRotatePosition(rotatedBoard,RotatePositionX,RotatePositionY,dimension,boardNbLine,boardNbColumn);
                    printw("\n");
                    displayChoiceRotateDirection(boardChoiceRotateDirection,boardNbColumn);
                    printw("\n");
                    int getchChoiceRotateDirection=getch();
                    moveChoiceRotateDirection(boardChoiceRotateDirection,getchChoiceRotateDirection,&choiceRotateDirection,&choiceRotateDirectionNotDone);
                    if(choiceRotateDirection!=1){
                        BoardCopy(board,rotatedBoard,boardNbLine,boardNbColumn);
                        if(choiceRotateDirection==2){
                            rotate(rotatedBoard,RotatePositionY,RotatePositionX,1,dimension,boardNbLine,boardNbColumn);
                        }
                        else{
                            rotate(rotatedBoard,RotatePositionY,RotatePositionX,0,dimension,boardNbLine,boardNbColumn);
                        }
                    }
                    else{
                        BoardCopy(board,rotatedBoard,boardNbLine,boardNbColumn);
                    }
                }
                BoardCopy(rotatedBoard,board,boardNbLine,boardNbColumn);
                gravityApplication(board,boardNbLine,boardNbColumn);


                is_win = verifyWinner(board, tokenP1,win_index,boardNbLine,boardNbColumn)+verifyWinner(board, tokenP2,win_index,boardNbLine,boardNbColumn)+verifyWinner(board, tokenP3,win_index,boardNbLine,boardNbColumn)+verifyWinner(board, tokenP4,win_index,boardNbLine,boardNbColumn);
                if(is_win){
                    break;
                }
            }
            else{
                continue;
            }

            // Changer le jeton pour le prochain joueur
            if(nbPlayer==2){
                if(actualToken== tokenP1){
                    actualToken=tokenP2;
                }
                else{
                    actualToken= tokenP1;
                }
            };


            if(nbPlayer==3){
                if(actualToken==tokenP1){
                    actualToken=tokenP2;
                }
                else if(actualToken==tokenP2){
                    actualToken=tokenP3;
                }
                else {
                    actualToken=tokenP1;
                }
            }
            if(nbPlayer==4){
                if(actualToken==tokenP1){
                    actualToken=tokenP2;
                }
                else if(actualToken==tokenP2){
                    actualToken=tokenP3;
                }
                else if(actualToken==tokenP3){
                    actualToken=tokenP4;
                }
                else{
                    actualToken= tokenP1;
                }
            }

            //on vérifie le match nul et si c'est le cas on quitte la boucle de jeu
            if(verifyDraw(board,boardNbLine,boardNbColumn)){
                draw++;
                break;
            }
        }
        //s'il ya match nul
        if(draw){
            displayBoard(board,boardNbLine,boardNbColumn);
            printw("\n\n");
            spacementX(boardNbColumn+boardNbColumn/2);
            printw("match nul, vous ne pouvez plus jouer\n\n\n\n");
            spacementX(boardNbColumn+boardNbColumn/2);
            printw("appuyez sur une touche pour quitter");
            refresh();
            getch();
            endwin();
        }

        //s'il y a un gagnant
        if(is_win==1){
            actualToken = tokenWinner(board, boardNbLine, boardNbColumn);
            winnerAnimation(board,win_index,actualToken,boardNbLine, boardNbColumn);
            
            refresh();
            getch();
        }

        //s'il y a plusieurs gagnants
        else if(is_win>1){
            displayBoard(board,boardNbLine,boardNbColumn);
            printw("\n\n");
            spacementX(boardNbColumn+boardNbColumn/2);
            printw("match nul, il y %d win en même temps\n\n\n\n",is_win);
            spacementX(boardNbColumn+boardNbColumn/2);
            printw("appuyez sur une touche pour quitter");
            refresh();
            getch();
            
        }

        //libérer la mémoire allouée avec malloc
        for (int i = 0; i < boardNbLine; i++) {
            free(board[i]);
            }
        free(board);

        // Libérer la mémoire allouée avec malloc
        for (int i = 0; i < boardNbLine; i++) {
            free(rotatedBoard[i]);
        }
        free(rotatedBoard);
        endwin();
        return 0;
    }

    //si le joueur appuie sur quitter dès l'écran de démarrage
    else{
        endwin();
        return 0;
    }
   
}
