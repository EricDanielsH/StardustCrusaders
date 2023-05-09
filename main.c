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
    int posY;
    int posX;
    int angle;
    int score;
    int hearts;
    int gravity;
    bool crashed;

};

struct BlackHole {
    int posY;
    int posX;
    int status;
};

struct Bullet {
    int speed;
    int posY;
    int posX;
    int direction;
};


struct Rocket * newRocket (char _name[10], int Y, int X, int _angle) {

    struct Rocket * rocket = (struct Rocket *) malloc(sizeof(struct Rocket));

    strcpy (rocket -> name, _name);
    rocket -> posY = Y;
    rocket -> posX = X;
    rocket -> crashed = false;
    rocket -> angle = _angle;
    rocket -> speed = 3;
    rocket -> score = 0;
    rocket -> hearts = 5;
    rocket -> gravity = 2;
    
    return rocket;
}


struct BlackHole * newBlackHole (int Y, int X) {

    struct BlackHole * bh = (struct BlackHole *) malloc(sizeof(struct BlackHole));

   
    bh -> posY = Y;
    bh -> posX = X;
    bh -> status = 1;
    
    return bh;
}

struct Bullet * newBullet (struct Rocket * r) {

    struct Bullet * b = (struct Bullet *) malloc(sizeof(struct Bullet));

   
    b -> posY = r -> posY;
    b -> posX = r -> posX;
    
    return b;
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

void checkCollisionBorders (struct Rocket * r, int _yMax, int _xMax) {
    if (r->posY < (_yMax - BOARD_COLS)/2 + 1)  //Limitations so it doesnt go out of the box
        r->posY = (_yMax - BOARD_COLS)/2 + 1; 

    if (r->posY > (_yMax - BOARD_COLS)/2 + BOARD_COLS - 2) //Limitations so it doesnt go out of the box
        r->posY = (_yMax - BOARD_COLS)/2 + BOARD_COLS - 2; 

    if (r->posX < (_xMax/2) - (BOARD_ROWS/2) + 1) //Limitations so it doesnt go out of the box
        r->posX = (_xMax/2) - (BOARD_ROWS/2) + 1; 

    if (r->posX > (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2) //Limitations so it doesnt go out of the box
        r->posX = (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2; 

}

void delRocket(struct Rocket * r) {
    move(r->posY, r->posX);
    printw(" ");
}

void printRocket (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {
    switch (r1 -> angle)
    {
    case 0:
        checkCollisionBorders(r1, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        checkCollisionBH (bh, r1);
        move(r1 -> posY, r1 -> posX);
        printw (">");
        break;
    case 90:
        checkCollisionBorders(r1, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        checkCollisionBH (bh, r1);
        move(r1 -> posY, r1 -> posX);
        printw ("^");
        break;
    case 180:
        checkCollisionBorders(r1, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        checkCollisionBH (bh, r1);
        move(r1 -> posY, r1 -> posX);
        printw ("<");
        break;
    case 270:
        checkCollisionBorders(r1, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        checkCollisionBH (bh, r1);
        move(r1 -> posY, r1 -> posX);
        printw ("v");
        break;
    
    default:
        break;
    }

    refresh();
}

void rocketThrust (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {

    int newYDown = r1 -> posY + r1 -> speed;
    int newXRight = r1 -> posX + r1 -> speed;
    int newYUp = r1 -> posY - r1 -> speed;
    int newXLeft = r1 -> posX - r1 -> speed;

    switch (r1 -> angle)
    {
    case 0:
        delRocket(r1);
        move (r1 -> posY, newXRight);
        r1 -> posX = newXRight; //updates the new X
        printRocket(r1, r2, _yMax, _xMax, bh);
        break;
    case 90:
        delRocket(r1);
        move (newYUp, r1 -> posX);
        r1 -> posY = newYUp; //updates the new Y
        printRocket(r1, r2, _yMax, _xMax, bh);

        break;
    case 180:
        delRocket(r1);
        move (r1 -> posY, newXLeft);
        r1 -> posX = newXLeft; //updates the new X
        printRocket(r1, r2, _yMax, _xMax, bh);

        break;
    case 270:
        delRocket(r1);
        move (newYDown, r1 -> posX);
        r1 -> posY = newYDown; //updates the new Y
        printRocket(r1, r2, _yMax, _xMax, bh);

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

void rotateUp (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {
    r1 -> angle = 90;
    printRocket(r1, r2, _yMax, _xMax, bh);
    refresh();


}

void rotateDown (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {
    r1 -> angle = 270;
    printRocket(r1, r2, _yMax, _xMax, bh);  
    refresh();
}


void rotateLeft (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {
    r1 -> angle = 180;
    printRocket(r1, r2, _yMax, _xMax, bh);
    refresh();
}

void rotateRight (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {
    r1 -> angle = 0;
    printRocket(r1, r2, _yMax, _xMax, bh);
    refresh();
}

int getKeys(struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax, struct BlackHole * bh) {

        int KeyPress = wgetch(screen); //waits for input
    
        switch (KeyPress)
        {
            case KEY_UP:
                rotateUp(r2, r1, _yMax, _xMax, bh);
                break;

            case KEY_DOWN:
                rotateDown(r2, r1, _yMax, _xMax, bh);
                break;

            case KEY_LEFT:
                rotateLeft(r2, r1, _yMax, _xMax, bh);
                break;

            case KEY_RIGHT:
                rotateRight(r2, r1, _yMax, _xMax, bh);
                break;

            case 119:
                rotateUp(r1, r2, _yMax, _xMax, bh);
                break;

            case 115:
                rotateDown(r1, r2, _yMax, _xMax, bh);
                break;

            case 97:
                rotateLeft(r1, r2, _yMax, _xMax, bh);
                break;

            case 100:
                rotateRight(r1, r2, _yMax, _xMax, bh);
                break;

            default:
                break;
        }

        refresh();
        wrefresh(screen); //This refresh in necesary to use the arrow keys, so DO NOT DELETE
        return KeyPress; //this might be necesary if we want to exit the program with a special key


}

void startScreen(int _yMax, int _xMax, WINDOW * screen) {
    move(_yMax/2, (_xMax/2) - 30);
    printw("Welcome to the Epic Space Wars!!!! (really epic)");
    move(_yMax/2 + 1, (_xMax/2) - 20);
    printw("Press any key to start the game");
    refresh();
    halfdelay(50); //Gives 5 seconds to wait for any inpuy. Even if no key is pressed, the game will start after 5secs
    getch();
    erase();
    halfdelay(5); //Go back to the lowest value of input waiting time`
}

void gravityRocket (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {


    if ((r1 -> posX) <= (_xMax/2) && (r1 -> posY) <= (_yMax/2)) { //Top Left Quadrant 1

        delRocket(r1);

        r1 -> posY = r1 -> posY + r1 -> gravity;
        r1 -> posX = r1 -> posX + (r1 -> gravity) * 1;

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if ((r1 -> posX) >= (_xMax/2) && (r1 -> posY) <= (_yMax/2)) { //Top Right Quadrant 2

        delRocket(r1);

        r1 -> posY = r1 -> posY + r1 -> gravity;
        r1 -> posX = r1 -> posX - (r1 -> gravity) * 1;

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if ((r1 -> posX) <= (_xMax/2) && (r1 -> posY) >= (_yMax/2)) { //Bottom Left Quadrant 3

        delRocket(r1);

        r1 -> posY = r1 -> posY - r1 -> gravity;
        r1 -> posX = r1 -> posX + (r1 -> gravity) * 1;

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if ((r1 -> posX) >= (_xMax/2) && (r1 -> posY) >= (_yMax/2)) { //Bottom Right Quadrant 4

        delRocket(r1);

        r1 -> posY = r1 -> posY - r1 -> gravity;
        r1 -> posX = r1 -> posX - (r1 -> gravity) * 1;

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    // if ((r1 -> posX) == (_xMax/2) && (r1 -> posY) == (_yMax/2)) {

    // }
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
    // nodelay(win , true);

    startScreen(yMax, xMax, win);
    box(win, 0, 0); 


    //INITIALISATION OF ROCKET 1 and 2
    struct Rocket * rocket1 = newRocket("Eric", (yMax - BOARD_COLS)/2 + BOARD_COLS - 2, (xMax/2) - (BOARD_ROWS/2) + 1, 90);
    struct Rocket * rocket2 = newRocket("Player 2", (yMax - BOARD_COLS)/2 + 1, (xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS + - 2, 270);

    //INITIALISATION OF BLACK HOLE
    struct BlackHole * hole = newBlackHole(yMax/2, xMax/2);

    //MOVE ROCKET 1
    move(rocket1->posY, rocket1->posX ); //moves the cursor 
    printRocket(rocket1, rocket2, yMax, xMax, hole); //prints the rocket

    //MOVE ROCKET 2
    move(rocket2->posY, rocket2->posX ); //moves the cursor 
    printRocket(rocket2, rocket1, yMax, xMax, hole); //prints the rocket


    refresh();

    while( rocket1 -> crashed != true && rocket2 -> crashed != true) //game loop
    {   
        changeBlackHole(win, hole);  
        wrefresh(win);
        getKeys(rocket1, rocket2, win,  yMax, xMax, hole);
        
        wrefresh(win); 
        
        rocketThrust(rocket1, rocket2, yMax, xMax, hole);
        rocketThrust(rocket2, rocket1, yMax, xMax, hole);    

        gravityRocket(rocket1, rocket2, yMax, xMax, hole);
        gravityRocket(rocket2, rocket1, yMax, xMax, hole);

    }

    //END OF THE GAME
    endwin(); //stops the window, terminates the program

    printWinner(rocket1, rocket2);


    return 0;
}