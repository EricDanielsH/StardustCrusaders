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
    int speed; //speed of the rocket ()
    int acc; //acceleration
    int posY;
    int posX;
    int angle;
    int score;
    bool crashed;
    int hearts;

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
    rocket -> speed = 1;
    rocket -> acc = 1;
    rocket -> score = 0;
    rocket -> hearts = 5;
    
    return rocket;
}


struct BlackHole * newBlackHole (int Y, int X) {

    struct BlackHole * bh = (struct BlackHole *) malloc(sizeof(struct BlackHole));

   
    bh -> posY = Y;
    bh -> posX = X;
    bh -> status = 1;
    
    return bh;
}

void checkCollisionBH (struct BlackHole * b, struct Rocket * r) {
    if (b -> posY == r -> posY && b -> posX == r -> posX) {
        //if this happens, the collision has happened
        r -> crashed = true;
    }
}

void checkCollisionRockets (struct Rocket * r1, struct Rocket * r2) {
    if (r1 -> posY == r2 -> posY && r1 -> posX == r2 -> posX) {
        //if this happens, the collision has happened
        r1 -> crashed = true;
        r2 -> crashed = true;
    }
}

void checkCollisionBorders (struct Rocket * r, WINDOW * screen, int _yMax, int _xMax) {
    if (r->posY < (_yMax - BOARD_COLS)/2 + 1)  //Limitations so it doesnt go out of the box
        r->posY = (_yMax - BOARD_COLS)/2 + 1; 

    if (r->posY > (_yMax - BOARD_COLS)/2 + BOARD_COLS - 2) //Limitations so it doesnt go out of the box
        r->posY = (_yMax - BOARD_COLS)/2 + BOARD_COLS - 2; 

    if (r->posX < (_xMax/2) - (BOARD_ROWS/2) + 1) //Limitations so it doesnt go out of the box
        r->posX = (_xMax/2) - (BOARD_ROWS/2) + 1; 

    if (r->posX > (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2) //Limitations so it doesnt go out of the box
        r->posX = (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2; 

}

void empty(struct Rocket * r) {
    move(r->posY, r->posX);
    printw(" ");
}

void printRocket (struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax) {
    switch (r1 -> angle)
    {
    case 0:
        checkCollisionBorders(r1, screen, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        move(r1 -> posY, r1 -> posX);
        printw (">");
        break;
    case 90:
        checkCollisionBorders(r1, screen, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        move(r1 -> posY, r1 -> posX);
        printw ("^");
        break;
    case 180:
        checkCollisionBorders(r1, screen, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        move(r1 -> posY, r1 -> posX);
        printw ("<");
        break;
    case 270:
        checkCollisionBorders(r1, screen, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        move(r1 -> posY, r1 -> posX);
        printw ("v");
        break;
    
    default:
        break;
    }
}

void rocketThrust (struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax) {

    int newYDown = r1 -> posY + r1 -> speed;
    int newXRight = r1 -> posX + r1 -> speed;
    int newYUp = r1 -> posY - r1 -> speed;
    int newXLeft = r1 -> posX - r1 -> speed;

    switch (r1 -> angle)
    {
    case 0:
        empty(r1);
        move (r1 -> posY, newXRight);
        r1 -> posX = newXRight; //updates the new X
        printRocket(r1, r2, screen, _yMax, _xMax);
        break;
    case 90:
        empty(r1);
        move (newYUp, r1 -> posX);
        r1 -> posY = newYUp; //updates the new Y
        printRocket(r1, r2, screen, _yMax, _xMax);

        break;
    case 180:
        empty(r1);
        move (r1 -> posY, newXLeft);
        r1 -> posX = newXLeft; //updates the new X
        printRocket(r1, r2, screen, _yMax, _xMax);

        break;
    case 270:
        empty(r1);
        move (newYDown, r1 -> posX);
        r1 -> posY = newYDown; //updates the new Y
        printRocket(r1, r2, screen, _yMax, _xMax);

        break;
    
    default:
        break;
    }
}




void printWinner (struct Rocket * r1, struct Rocket * r2) {
    char loser[10];
    char winner[10];
    if (r1 -> crashed == true && r2 -> crashed == true) {
        strcpy(loser, r1 -> name);
        strcpy(winner, r2 -> name);
        printf("The player %s and the player %s have crashed. Both lost.\n\n", loser, winner); //this is temportal, as the winner is the one who has more points
        return;
    }

    else if (r1 -> crashed == true) {
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


//Functions for the movement of the rocket

void rotateUp (struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax) {
    r1 -> angle = 90;
    printRocket(r1, r2, screen, _yMax, _xMax);
    wrefresh(screen);
    refresh();


}

void rotateDown (struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax) {
    r1 -> angle = 270;
    printRocket(r1, r2, screen, _yMax, _xMax);  
    wrefresh(screen);
    refresh();
}


void rotateLeft (struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax) {
    r1 -> angle = 180;
    printRocket(r1, r2, screen, _yMax, _xMax);
    wrefresh(screen);
    refresh();
}

void rotateRight (struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax) {
    r1 -> angle = 0;
    printRocket(r1, r2, screen, _yMax, _xMax);
    wrefresh(screen);
    refresh();
}

int getRotation1(struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax) {

        int KeyPress = wgetch(screen); //waits for input
    
        switch (KeyPress)
        {
            case KEY_UP:
                rotateUp(r2, r1, screen, _yMax, _xMax);
                break;

            case KEY_DOWN:
                rotateDown(r2, r1, screen, _yMax, _xMax);
                break;

            case KEY_LEFT:
                rotateLeft(r2, r1, screen, _yMax, _xMax);
                break;

            case KEY_RIGHT:
                rotateRight(r2, r1, screen, _yMax, _xMax);
                break;

            case 119:
                rotateUp(r1, r2, screen, _yMax, _xMax);
                break;

            case 115:
                rotateDown(r1, r2, screen, _yMax, _xMax);
                break;

            case 97:
                rotateLeft(r1, r2, screen, _yMax, _xMax);
                break;

            case 100:
                rotateRight(r1, r2, screen, _yMax, _xMax);
                break;

            default:
                break;
        }

        refresh();
        wrefresh(screen);
        return KeyPress; //this might be necesary if we want to exit the program with a special key


}

int getRotation2(struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax) {

        int KeyPress = wgetch(screen); //waits for input
    
        switch (KeyPress)
        {
            case KEY_UP:
                rotateUp(r2, r1, screen, _yMax, _xMax);
                break;

            case KEY_DOWN:
                rotateDown(r2, r1, screen, _yMax, _xMax);
                break;

            case KEY_LEFT:
                rotateLeft(r2, r1, screen, _yMax, _xMax);
                break;

            case KEY_RIGHT:
                rotateRight(r2, r1, screen, _yMax, _xMax);
                break;

            case 119:
                rotateUp(r1, r2, screen, _yMax, _xMax);
                break;

            case 115:
                rotateDown(r1, r2, screen, _yMax, _xMax);
                break;

            case 97:
                rotateLeft(r1, r2, screen, _yMax, _xMax);
                break;

            case 100:
                rotateRight(r1, r2, screen, _yMax, _xMax);
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
    halfdelay(1); //this is the amount of time in tenths per second to exit a getch if there is no input, so that the program continues

    //INITIALISATION OF ROCKET 1 and 2
    struct Rocket * rocket1 = newRocket("Eric", (yMax - BOARD_COLS)/2 + BOARD_COLS - 2, (xMax/2) - (BOARD_ROWS/2) + 1, 90);
    struct Rocket * rocket2 = newRocket("Player 2", (yMax - BOARD_COLS)/2 + 1, (xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2, 270);

    //MOVE ROCKET 1
    move(rocket1->posY, rocket1->posX ); //moves the cursor 
    printRocket(rocket1, rocket2, win, yMax, xMax); //prints the rocket

    //MOVE ROCKET 2
    move(rocket2->posY, rocket2->posX ); //moves the cursor 
    printRocket(rocket2, rocket1, win, yMax, xMax); //prints the rocket

    //INITIALISATION OF BLACK HOLE
    struct BlackHole * hole = newBlackHole(yMax/2, xMax/2);
    

    refresh();


    while( rocket1 -> crashed != true && rocket2 -> crashed != true) //game loop
    {   
        changeBlackHole(win, hole);  
        wrefresh(win);
        getRotation1(rocket1, rocket2, win,yMax, xMax);
        getRotation2(rocket1, rocket2, win,yMax, xMax);
        wrefresh(win); 
        checkCollisionBH(hole, rocket1);
        checkCollisionBH(hole, rocket2);
        rocketThrust(rocket1, rocket2,  win, yMax, xMax);
        rocketThrust(rocket2, rocket1, win, yMax, xMax);        
    }

    //END OF THE GAME
    endwin(); //stops the window, terminates the program

    printWinner(rocket1, rocket2);


    return 0;
}