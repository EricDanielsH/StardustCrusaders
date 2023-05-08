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
    char name[10];
    int speed; //speed of the rocket
    int acc; //acceleration
    int posY;
    int posX;
    int angle;
    bool crashed;

};

struct BlackHole {
    int posY;
    int posX;
    int status;
};

struct Rocket * newRocket (char _name[10], int Y, int X, int _angle) {

    struct Rocket * rocket = (struct Rocket *) malloc(sizeof(struct Rocket));

    strcpy (rocket -> name, _name);
    rocket -> posY = Y;
    rocket -> posX = X;
    rocket -> crashed = false;
    rocket -> angle = _angle;
    
    return rocket;
}

void printWinner (struct Rocket * r1, struct Rocket * r2) {
    char loser[10];
    char winner[10];
    if (r1 -> crashed == true) {
        strcpy(loser, r1 -> name);
        strcpy(winner, r2 -> name);
    }
        
    else {
        strcpy(loser, r2 -> name);
        strcpy(winner, r1 -> name);
    }
        

    printf("The player %s has crashed. %s has won the game\n\n", loser, winner);
}

void changeBlackHole (WINDOW * screen, struct BlackHole * b) {
    move (b->posY, b->posX);
    if (b->status == 0){
        printw("o");
        b->status = 1;
    }
        
    else {
        printw("0");
        b->status = 0;
    }

    refresh();
    wrefresh(screen);
        
}

void collisionBH (struct BlackHole * b, struct Rocket * r) {
    if (b -> posY == r -> posY && b -> posX == r -> posX) {
        //if this happens, the collision has happened
        r -> crashed = true;
    }
}


void empty(struct Rocket * r) {
    move(r->posY, r->posX);
    printw(" ");
}



//Functions for the movement of the rocket

void moveUp (struct Rocket * r, WINDOW * screen, int _yMax, int _xMax) {
    empty(r);
    r->posY--;

    if (r->posY < (_yMax - BOARD_COLS)/2 + 1)  //Limitations so it doesnt go out of the box
        r->posY = (_yMax - BOARD_COLS)/2 + 1; 


    move(r->posY, r->posX);
    addch('^');

    r -> angle = 90;


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

    r -> angle = 270;

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

    r -> angle = 180;

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

    r -> angle = 0;

    wrefresh(screen);
    refresh();
}

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
        return KeyPress; //this might be necesary if we want to exit the program with a special key


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
    struct Rocket * rocket1 = newRocket("Eric", (yMax - BOARD_COLS)/2 + BOARD_COLS - 2, (xMax/2) - (BOARD_ROWS/2) + 1, 90);
    move(rocket1->posY, rocket1->posX ); //moves the cursor 
    printw("^"); //prints the rocket

    //INITIALISATION OF ROCKET 2 
    struct Rocket * rocket2 = newRocket("Player 2", (yMax - BOARD_COLS)/2 + 1, (xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2, 270);
    move(rocket2->posY, rocket2->posX ); //moves the cursor 
    printw("v"); //prints the rocket

    //INITIALISATION OF BLACK HOLE
    struct BlackHole * hole = (struct BlackHole *) malloc(sizeof(struct Rocket));
    hole -> posY = yMax/2;
    hole -> posX = xMax/2;
    hole -> status = 0;
    refresh();

    while( rocket1 -> crashed != true && rocket2 -> crashed != true) //game loop 
    {   
        changeBlackHole(win, hole);  
        wrefresh(win);
        getMove(rocket1, rocket2, win,yMax, xMax);
        wrefresh(win); 
        collisionBH(hole, rocket1);
        collisionBH(hole, rocket2);
        
    }

    //END OF THE GAME
    endwin(); //stops the window, terminates the program

    printWinner(rocket1, rocket2);


    return 0;
}