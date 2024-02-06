#include <ncurses.h>

//Initializes movement enum for the snake
//Called with ncurses getch/etc. input functions
typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN
}snakeDir;

// This is the matrix for the snake game
// [x,y] matrix.
typedef struct {
    int pointNum;
    int x;
    int y;
}snakeCell;

// Trophy Cell's matrix values and 
// Char display/score value
typedef struct {
    int x;
    int y;
    int value;
    char display;
}trophyCell;

// Structure for holding input info
typedef struct {
    snakeDir currentDir;
    snakeDir prevDir;   
    bool gameOver;
    unsigned int delay;
    unsigned int prevSpeed;
    unsigned int score;
    int nextX;
    int nextY;
}inputInfo;

snakeCell snakeInit( int x, int y, int pointNum) {
    snakeCell s1;
    s1.pointNum = pointNum;
    s1.x = x;
    s1.y = y;
    return s1;
}

/* **** GAME SUPPORT FUNCTIONS **** */

//Input Handler
inputInfo inputHandler(inputInfo gameInfo, int ch) {
    switch(ch) { 
        case KEY_UP:
            if(gameInfo.currentDir == DOWN)
                gameInfo.gameOver = TRUE;
            else
                gameInfo.currentDir = UP;
            
            //if(gameInfo.currentDir != UP && gameInfo.currentDir != DOWN) { gameInfo.currentDir = UP; }
            break;
        case KEY_DOWN:
            if(gameInfo.currentDir == UP)
                gameInfo.gameOver = TRUE;
            else
                gameInfo.currentDir = DOWN;        
            
            //if(gameInfo.currentDir != UP && gameInfo.currentDir != DOWN) { gameInfo.currentDir = DOWN; }
            break;
        case KEY_LEFT:
            if(gameInfo.currentDir == RIGHT)
                gameInfo.gameOver = TRUE;
            else
                gameInfo.currentDir = LEFT;
            
            //if(gameInfo.currentDir != RIGHT && gameInfo.currentDir != LEFT) { gameInfo.currentDir = LEFT; }
            break;
        case KEY_RIGHT:
            if(gameInfo.currentDir == LEFT)
                gameInfo.gameOver = TRUE;
            else
                gameInfo.currentDir = RIGHT;

            //if(gameInfo.currentDir != RIGHT && gameInfo.currentDir != LEFT) { gameInfo.currentDir = RIGHT; }
            break;
        case 81:
        case 113:
            gameInfo.gameOver = TRUE;
            break;
    }
    return gameInfo;
}

// This function draws the snakepit borders
void drawSnakePit(int maxX, int maxY, unsigned int currentScore) {
    int i, j;
    // Border characters
   
     wchar_t tlc[] = L"\u2554", 
            trc[] = L"\u2557",
            blc[] = L"\u255A",
            brc[] = L"\u255D",
            hln[] = L"\u2550",
            vln[] = L"\u2551";

    for(i = 0; i < maxY - 1; i++) {
        if(i == 0) {
            j = 0;
            mvaddwstr(i, j, tlc);
            j++;
            while(j < (maxX - 1)) {
                mvaddwstr(i, j, hln);
                j++;
            }
            mvaddwstr(i, j, trc);
        }
        else if (i == (maxY - 2)) {
            j = 0;
            mvaddwstr(i, j, blc);
            j++;
            while(j < (maxX - 1)) {
                mvaddwstr(i, j, hln);
                j++;
            }
            mvaddwstr(i, j, brc);
        }
        else {
            j = 0;
            mvaddwstr(i, j, vln);
            j = maxX - 1;
            mvaddwstr(i, j, vln);
        }
    }

    char score[100];
    sprintf(score, "CURRENT SCORE: %d", currentScore);
    mvaddstr(maxY - 1, 0, score);
}