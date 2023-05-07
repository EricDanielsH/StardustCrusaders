//player 1 = player on the left down side of the screen = ^ v < >
//player 2 = player on the right up side of the screen = v ^ < >

#include <ncurses.h> //remember that ncurses coordinates are y, x; not x, y
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Creating some constants for the board dimensions
#define BOARD_DIMENSIONS  50
#define BOARD_COLS BOARD_DIMENSIONS
#define BOARD_ROWS BOARD_DIMENSIONS * 2.5  //the ratio of the size of rows to cols is aprox 2->5



struct Rocket {
    int speed; //speed of the rocket ()
    int acc; //acceleration
    int posY;
    int posX;
    int angle;

};

//This function clears the space where the rocket is.
void empty(struct Rocket * r) {
    move(r->posY, r->posX);
    printw(" ");
}

//Functions for the movement of the rocket
void moveUp (struct Rocket * r, WINDOW * screen, int _yMax, int _xMax) {
    empty(r);
    r->posY--;

    if (r->posY < (_yMax - BOARD_COLS)/2 + 1) //Limitations so it doesnt go out of the box
        r->posY = (_yMax - BOARD_COLS)/2 + 1; 


    move(r->posY, r->posX);
    addch('^');


    wrefresh(screen);
    refresh();


}

void moveDown (struct Rocket * r, WINDOW * screen, int _yMax, int _xMax) {
    empty(r);
    r->posY++; //remember that if u add to Y, you go down in lines

    if (r->posY > (_yMax - BOARD_COLS)/2 + BOARD_COLS - 2) //Limitations so it doesnt go out of the box
        r->posY = (_yMax - BOARD_COLS)/2 + BOARD_COLS - 2; 

    move(r->posY, r->posX);
    addch('v');

    wrefresh(screen);
    refresh();
}


void moveLeft (struct Rocket * r, WINDOW * screen, int _yMax, int _xMax) {
    empty(r);
    r->posX--;

    if (r->posX < (_xMax/2) - (BOARD_ROWS/2) + 1) //Limitations so it doesnt go out of the box
        r->posX = (_xMax/2) - (BOARD_ROWS/2) + 1; 

    move(r->posY, r->posX);
    addch('<');

    wrefresh(screen);
    refresh();
}

void moveRight (struct Rocket * r, WINDOW * screen, int _yMax, int _xMax) {
    empty(r);
    r->posX++;

    if (r->posX > (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2) //Limitations so it doesnt go out of the box
        r->posX = (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2; 
    
    move(r->posY, r->posX);
    addch('>');

    wrefresh(screen);
    refresh();
}

//Main movement function, takes the input of rocket 1 and 2. Still working on it.
int getMove(struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax) {
    int KeyPress = wgetch(screen); //waits for input

        switch (KeyPress)
        {
            case KEY_UP:
                moveUp(r1, screen, _yMax, _xMax);
                break;

            case KEY_DOWN:
                moveDown(r1, screen, _yMax, _xMax);
                break;

            case KEY_LEFT:
                moveLeft(r1, screen, _yMax, _xMax);
                break;

            case KEY_RIGHT:
                moveRight(r1, screen, _yMax, _xMax);
                break;

            case 119:
                moveUp(r2, screen, _yMax, _xMax);
                break;

            case 115:
                moveDown(r2, screen, _yMax, _xMax);
                break;

            case 97:
                moveLeft(r2, screen, _yMax, _xMax);
                break;

            case 100:
                moveRight(r2, screen, _yMax, _xMax);
                break;

            default:
                break;
        }

        refresh();
        wrefresh(screen);
        return KeyPress; //this might be necesary if we want to exit the program with a special key (e.g. use 'P' to stop game)
}


int main()
{
    //CREATES THE WINDOW AND THE BOX
    initscr(); //initializates the screen
    refresh(); //this refresh is needed to refresh the parent window
    noecho(); //stops the user input when using getch()
    cbreak(); //alllows the use of all keys, specially control C to stop the program

    int yMax, xMax; //creates the parameters of the size of the window
    getmaxyx(stdscr, yMax, xMax); //gets the max amount that y and x can be in the terminal and saves the values inside of y and x->

    WINDOW  *  win = newwin(BOARD_COLS, BOARD_ROWS, (yMax/2) - (BOARD_COLS/2), (xMax/2) - (BOARD_ROWS/2)); //starts a new window (height, width, starting Y of cursor, starting X of cursor)
    //(yMax/2) - halfHeight->->->->-> This will make it centered on the screen-> Same applies to the width
    box(win, 0, 0); //creates a box inside of window-> arg2 and arg3 are the type of borders that the box has
    wrefresh(win); //refreshes the visuals of the window
    keypad(win, TRUE); //allows the use of arrow keys and F keys

    //INITIALISATION OF ROCKET 1
    struct Rocket * rocket1 = (struct Rocket *) malloc(sizeof(struct Rocket));
    rocket1->posY = (yMax - BOARD_COLS)/2 + BOARD_COLS - 2;
    rocket1->posX = (xMax/2) - (BOARD_ROWS/2) + 1;
    move(rocket1->posY, rocket1->posX ); //moves the cursor 
    printw("^"); //prints the rocket

    //INITIALISATION OF ROCKET 2 
    struct Rocket * rocket2 = (struct Rocket *) malloc(sizeof(struct Rocket));
    rocket2->posY = (yMax - BOARD_COLS)/2 + 1;
    rocket2->posX = (xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2;
    move(rocket2->posY, rocket2->posX ); //moves the cursor 
    printw("v"); //prints the rocket
    refresh();

    //GAME LOOP
    while(1) 
    {
        wrefresh(win);
        getMove(rocket1, rocket2, win,yMax, xMax);
        wrefresh(win);   
     
    }

    //END OF THE GAME
    endwin(); //stops the window, terminates the program
    return 0;
}