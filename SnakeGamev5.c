#define _XOPEN_SOURCE_EXTENDED
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "SnakeCommonv5.h"

int maxY, maxX;
inputInfo gameInfo;
int numTrophy;
int tail;
trophyCell t;
void drawScreen(snakeCell snake[], unsigned int delay);

//Read in int value for x or y
//Use atoi() from stdlib.h to parse int from char []
int getxy() {
    char xyStr[3];
    getstr(xyStr);
    int xy = atoi(xyStr); 
    return xy;
}

//use ncurses mvaddch to display the matrix included from the header file
//created for this project: snakeMatrix.
//A char type is taken as a parameter for displaying the snake
void displayMatrix(snakeCell snake, const chtype c) {
    mvaddch(snake.y,snake.x,c);
}

//Replaced by drawPart
void drawSnake(snakeCell snake[]) {
    for (int i = tail-1; i > -1; i--) {
        snake[i] = snakeInit(snake[tail].x-i,snake[tail].y, snake[tail].pointNum--);
        displayMatrix(snake[i],ACS_DIAMOND);
    }
}

//Initialize or update the trophy variable (global)
trophyCell createTrophyCell(int maxY, int maxX, bool isTrophy) {
t.x = (rand() % maxX);
t.y = (rand() % maxY);
t.value = (rand() % 9);
t.display = 48 + t.value;
return t;
}


//Display trophy to screen
void drawTrophy(trophyCell t) {
    mvaddch(t.y,t.x,t.display);
}


//Initialize ncurses library
void initNcurses() {
    initscr();
    //raw();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    
    //Bhawana  
    getmaxyx(stdscr, maxY, maxX);
}

void shiftSnake(snakeCell snake[]) {
    snakeCell tmp = snake[tail - 1];

    for(int i = tail - 1; i > 0; i--) {
        snake[i] = snake[i-1];
    }

    snake[0] = tmp;

    // Add to the game score if snake picks up a trophy
    if((snake[0].x == t.x) && (snake[0].y == t.y))
        gameInfo.score += t.value;
}

void drawPart(snakeCell drawPoint, _Bool isHead) {
    if(isHead == TRUE)
        mvprintw(drawPoint.y, drawPoint.x, "@");
    else
        mvprintw(drawPoint.y, drawPoint.x, "♦");
}

void drawScreen(snakeCell snake[], unsigned int delay) {
    //Clears the screen - put all draw functions after this
    clear();

    drawSnakePit(maxX, maxY, gameInfo.score);

    //Alana
    if (snake[0].x == t.x && snake[0].y == t.y) {
          tail += t.value;
          numTrophy--;
          if (gameInfo.delay > 150000) {gameInfo.delay -= 20000;}
    }
    
    if (gameInfo.gameOver == TRUE) {
        mvprintw(maxY / 2, maxX / 2, "Game Over!");        
    }

    //Draw the snake to the screen
    for(int i = 0; i < tail; i++) {
        if(i == 0)
            drawPart(snake[i], TRUE);
        else
            drawPart(snake[i], FALSE);
    }
    if (numTrophy != 0) {
        drawTrophy(t);
    }
    if (numTrophy == 0) {
            t = createTrophyCell(maxY,maxX, true);
            drawTrophy(t);
            numTrophy++;
            refresh();
    }     

    //ncurses refresh
    refresh();

    //Delay between movements
    usleep(delay);
}

//Initialize ncurses
//Initialize pit struct
//getmaxyx of stdscr
int main() {
    // Initial setup
    setlocale(LC_ALL, "");
    initNcurses();
    int x;
    int y;
    numTrophy = 0;
    tail = 5;
    snakeCell snake[100];
    snake[tail] = snakeInit(15,1,5);
    snakeCell tailPoint = snake[tail];
    
    // Game info default settings
    gameInfo.currentDir = RIGHT;
    gameInfo.prevDir = RIGHT;  
    gameInfo.gameOver = FALSE;
    gameInfo.delay = 500000;
    gameInfo.prevSpeed= 500000;
    gameInfo.score = 0;
    gameInfo.nextX = 0;
    gameInfo.nextY = 0;
    clear();
    
    // Prepare the game
    drawSnakePit(maxX, maxY, gameInfo.score);     // Draws the snakepit
    drawSnake(snake);
    mvprintw(x/2,y/2,"%d",tailPoint.pointNum);
    refresh();

    int nextX = 0;
    int nextY = 0;

    //Changing the while condition to meet movement needs
    while(!gameInfo.gameOver) {
        // Bhawana and Ricardo
        gameInfo.nextX = snake[0].x;
        gameInfo.nextY = snake[0].y;

        // Updated input handler        
        int ch = getch();
        gameInfo = inputHandler(gameInfo, ch);

        nextX = snake[0].x;
        nextY = snake[0].y;

        if(gameInfo.currentDir == RIGHT) nextX++;
        else if(gameInfo.currentDir == LEFT) nextX--;
        else if(gameInfo.currentDir == UP) nextY--;
        else if(gameInfo.currentDir == DOWN) nextY++;

        snake[tail - 1].x = nextX;
        snake[tail - 1].y = nextY;

        shiftSnake(snake);

        //Snake dies when it crosses itself 
        for (int i = 1; i < tail; i++) {
            if((snake[0].x == snake[i].x) && (snake[0].y == snake[i].y)) {
                gameInfo.gameOver = true;
                //drawScreen(snake, gameInfo.delay);
                break;
            }
        }
   
        //Game Over if the player hits the screen edges
		if((nextX >= maxX || nextX <= 0) || (nextY >= maxY || nextY <= 0)) {
			gameInfo.gameOver = TRUE;
        }

        // Redraw the screen        
        drawScreen(snake, gameInfo.delay);
    }

    getchar();    // Wait for user input - End of game pause
    endwin();
    return 0;
}