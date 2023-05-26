#include "../include/verification.h"
#include <ncurses.h>


//returne 1 si la rotation est accepté
int verifyRotate(int **board,int x,int y,int dimensioin){
    for(int i=y;i<y+dimensioin;i++){
        for(int j=x;j<x+dimensioin;j++){
            if(board[i][j]!=' '){
                return 1;
            }
        }
    }
    return 0;
}


// Vérifier s'il y a un gagnant
int verifyWinner(int **board, int actualToken,int win_index[5][2],int boardNbLine,int boardNbColumn) {
    // Vérifier les lignes
    for (int i = 0; i < boardNbLine; i++) {
        for (int j = 0; j <= boardNbColumn - WIN_LENGTH; j++) {
            int k;
            for (k = 0; k < WIN_LENGTH; k++) {
                if (board[i][j+k] != actualToken) {
                    break;
                }
            }
            if (k == WIN_LENGTH) {
                // Stocker les indices des actualTokens gagnants
                for (int l = 0; l < WIN_LENGTH; l++) {
                    win_index[l][0] = i;
                    win_index[l][1] = j+l;
                }
                return 1;
            }
        }
    }
   
    // Vérifier les colonnes
    for (int i = 0; i <= boardNbLine - WIN_LENGTH; i++) {
        for (int j = 0; j < boardNbColumn; j++) {
            int k;
            for (k = 0; k < WIN_LENGTH; k++) {
                if (board[i+k][j] != actualToken) {
                    break;
                }
            }
            if (k == WIN_LENGTH) {
                // Stocker les indices des actualTokens gagnants
                for (int l = 0; l < WIN_LENGTH; l++) {
                    win_index[l][0] = i+l;
                    win_index[l][1] = j;
                }
                return 1;
            }
        }
    }

    // Vérifier les diagonales 
    for (int i = WIN_LENGTH - 1; i < boardNbLine; i++) {
        for (int j = 0; j <= boardNbColumn - WIN_LENGTH; j++) {
            int k;
            for (k = 0; k < WIN_LENGTH; k++) {
                if (board[i-k][j+k] != actualToken) {
                break;
                }
            }
            if (k == WIN_LENGTH) {
                // Stocker les indices des actualTokens gagnants
                for (int l = 0; l < WIN_LENGTH; l++) {
                    win_index[l][0] = i-l;
                    win_index[l][1] = j+l;
                }
                return 1;
            }
        }
    }

    // Vérifier les diagonales 
    for (int i = WIN_LENGTH - 1; i < boardNbLine; i++) {
        for (int j = WIN_LENGTH - 1; j < boardNbColumn; j++) {
            int k;
            for (k = 0; k < WIN_LENGTH; k++) {
                if (board[i-k][j-k] != actualToken) {
                    break;
                }
            }
            if (k == WIN_LENGTH) {
                // Stocker les indices des actualTokens gagnants
                for (int l = 0; l < WIN_LENGTH; l++) {
                    win_index[l][0] = i-l;
                    win_index[l][1] = j-l;
                }
                return 1;
            }
        }
    }

    return 0;
}

int tokenWinner(int **board, int boardNbLine, int boardNbColumn) {

    // Vérifier les colonnes
    for (int i = 0; i < boardNbLine; i++) {
        for (int j = 0; j <= boardNbColumn - WIN_LENGTH; j++) {
            int k;
            for (k = 0; k < WIN_LENGTH; k++) {
                /*
                if (board[i][j+k] != ' ' && board[i][j+k] == board[i][j]) {
                    break;
                }
                */
                if (board[i][j+k] == ' ' || board[i][j+k] != board[i][j] ) {break;}

            }
            if (k == WIN_LENGTH) {
                return board[i][j];
            }
        }
    }

    // Vérifier les lignes
    for (int i = 0; i <= boardNbLine - WIN_LENGTH; i++) {
        for (int j = 0; j < boardNbColumn; j++) {
            int k;
            for (k = 0; k < WIN_LENGTH; k++) {
                /*
                if (board[i+k][j] != ' ' && board[i+k][j] == board[i][j]) {
                    break;
                }
                */
                if (board[i+k][j] == ' ' || board[i+k][j] != board[i][j] ) {break;}
            }
            if (k == WIN_LENGTH) {
                return board[i][j];
            }
        }
    }

    // Vérifier les diagonales /
    for (int i = WIN_LENGTH - 1; i < boardNbLine; i++) {
        for (int j = 0; j <= boardNbColumn - WIN_LENGTH; j++) {
            int k;
            for (k = 0; k < WIN_LENGTH; k++) {
                if (board[i-k][j+k] == ' ' || board[i-k][j+k] != board[i][j]) {
                    break;
                }
            }
            if (k == WIN_LENGTH) {
                return board[i][j];
            }
        }
    }

    // Vérifier les diagonales \    /
    for (int i = WIN_LENGTH - 1; i < boardNbLine; i++) {
        for (int j = WIN_LENGTH - 1; j < boardNbColumn; j++) {
            int k;
            for (k = 0; k < WIN_LENGTH; k++) {
                if (board[i-k][j-k] == ' ' || board[i-k][j-k] != board[i][j]) {
                    break;
                }
            }
            if (k == WIN_LENGTH) {
                return board[i][j];
            }
        }
    }

    return 0;
}



int verifyDraw(int **board,int boardNbLine,int boardNbColumn){
    int draw=0;
    for(int i=0;i<boardNbColumn;i++){
        if(board[0][i]==' '){
            draw++;
        }
    }
    if(draw){
        return 0;
    }
    else{
        return 1;
    }
}

int isFileEmpty(const char *filename) {
    FILE *file = fopen(filename, "r");

    
    int isEmpty = (fgetc(file) == EOF);

    fclose(file);
    return isEmpty;
}