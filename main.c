//player 1 = player on the left down side of the screen = ^ v < >
//player 2 = player on the right up side of the screen = v ^ < >

#include <ncurses.h> //remember that ncurses coordinates are y, x; not x, y
#include <stdio.h>
#include <string.h>

//Creating some constants for the board dimensions
#define BOARD_DIMENSIONS  30
#define BOARD_COLS BOARD_DIMENSIONS
#define BOARD_ROWS BOARD_DIMENSIONS * 2.5  //the ratio of the size of rows to cols is aprox 2.5

struct Rocket {
    int speed; //speed of the rocket ()
    int acc; //acceleration
    int posY;
    int posX;
    int angle;
    char * symbol[1];
};


int main()
{
    //CREATES THE WINDOW AND THE BOX

    initscr(); //initializates the screen
    refresh(); //this refresh is needed to refresh the parent window
    noecho(); //stops the user input when using getch();s
    cbreak(); //alllows the use of all keys, specially control C to stop the program

    int yMax, xMax; //creates the parameters of the size of the window
    getmaxyx(stdscr, yMax, xMax); //gets the max amount that y and x can be in the terminal and saves the values inside of y and x.

    WINDOW  *  win = newwin(BOARD_COLS, BOARD_ROWS, (yMax/2) - (BOARD_COLS/2), (xMax/2) - (BOARD_ROWS/2)); //starts a new window (height, width, starting Y of cursor, starting X of cursor)
    //(yMax/2) - halfHeight..... This will make it centered on the screen. Same applies to the width
    box(win, 0, 0); //creates a box inside of window. arg2 and arg3 are the type of borders that the box has
    wrefresh(win); //refreshes the visuals of the window
    keypad(win, TRUE); //allows the use of arrow keys and F keys

    //INITIALISATION OF ROCKET 2 
    struct Rocket rocket1;
    rocket1.symbol[0] = "^"; //this rocket starts at the bottom left of the window
    rocket1.posY = (yMax - BOARD_COLS)/2 + BOARD_COLS - 2;
    rocket1.posX = (xMax/2) - (BOARD_ROWS/2) + 1;
    move(rocket1.posY, rocket1.posX ); //moves the cursor 
    printw(rocket1.symbol[0]); //prints the rocket

    //INITIALISATION OF ROCKET 2 
    struct Rocket rocket2;
    rocket2.symbol[0] = "v"; //this rocket starts at the top right of the window
    rocket2.posY = (yMax - BOARD_COLS)/2 + 1;
    rocket2.posX = (xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2;
    move(rocket2.posY, rocket2.posX ); //moves the cursor 
    printw(rocket2.symbol[0]); //prints the rocket


    getch(); //temporal getch so that the screen doesnt disapear 

    //END OF THE GAME
    endwin(); //stops the window, terminates the program

    return 0;
}