//player 1 = player on the left down side of the screen = ^ v < >
//player 2 = player on the right up side of the screen = v ^ < >

#include <ncurses.h> //remember that ncurses coordinates are y, x; not x, y
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Creating some constants for the board dimensions
#define BOARD_DIMENSIONS  50 //If this number in odd, some problems will happen to the box
#define BOARD_COLS BOARD_DIMENSIONS
#define BOARD_ROWS BOARD_DIMENSIONS * 2.5  //the ratio of the size of rows to cols is aprox 2->5
#define MAX_BULLETS 7



struct Rocket {
    char name[10];
    int number;
    int speed; //speed of the rocket ()
    int posY;
    int posX;
    int angle;
    int score;
    int gravity;
    int fuel;
    bool crashed;
    bool hyperSpace;

    int startY;
    int startX;

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
    bool crashed;
};


struct Rocket * newRocket (char _name[10], int Y, int X, int _angle, int _number) {

    struct Rocket * rocket = (struct Rocket *) malloc(sizeof(struct Rocket));

    strcpy (rocket -> name, _name);
    rocket -> posY = Y;
    rocket -> posX = X;
    rocket -> crashed = false;
    rocket -> angle = _angle;
    rocket -> speed = 2;
    rocket -> score = 0;
    rocket -> gravity = 1;
    rocket -> fuel = 100;
    rocket -> number = _number;
    rocket -> hyperSpace = false;

    rocket -> startY = Y;
    rocket -> startX = X;
    
    return rocket;
}


struct BlackHole * newBlackHole (int Y, int X) {

    struct BlackHole * bh = (struct BlackHole *) malloc(sizeof(struct BlackHole));

   
    bh -> posY = Y;
    bh -> posX = X;
    bh -> status = 0;
    
    return bh;
}



void bulletCollisionBorders (struct Bullet * b, int _yMax, int _xMax) {

    //Add 6 to each one so it doesnt clear the border

    if (b->posY < (_yMax - BOARD_COLS)/2 + 5)  //Limitations so it doesnt go out from the top
        b ->crashed = true;

    if (b->posY > (_yMax - BOARD_COLS)/2 + BOARD_COLS - 6) //Limitations so it doesnt go out from the bottom
        b ->crashed = true;

    if (b->posX < (_xMax/2) - (BOARD_ROWS/2) + 6) //Limitations so it doesnt go out from the left
        b ->crashed = true;

    if (b->posX > (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS - 6) //Limitations so it doesnt go out of from the right
        b ->crashed = true;

}

void bulletCollisionBullets (struct Bullet * b1, struct Bullet * b2) { //WORKING ON THIS

    if ((b1->posX == b2->posX) && (b1->posY == b2->posY) ) 
        b1 ->crashed = true;
        b2 ->crashed = true;


}

void colBulletxRocket (struct Bullet * b, struct Rocket * r) { //WORKING ON THIS

    //Add 6 to each one so it doesnt clear the border
    if ((b->posX  == r->posX) && (b->posY == r->posY) )  
        b ->crashed = true;
        r ->crashed = true;


}

void delBullet (struct Bullet * b) {
    move (b -> posY, b -> posX);
    printw(" ");
    refresh();
}

void printBullet (struct Bullet * b, int _yMax, int _xMax) {
    bulletCollisionBorders(b, _yMax, _xMax);
    if (b -> crashed != true) {
        move (b -> posY, b -> posX);
        printw("*");
        refresh();
        return;
    }

    move (b -> posY, b -> posX);
    delBullet(b);


    
}

void printRocketStart (struct Rocket * r1) {
    switch (r1 -> angle)
    {
    case 0:
        move(r1 -> posY, r1 -> posX);
        printw (">");
        break;
    case 90:
        move(r1 -> posY, r1 -> posX);
        printw ("^");
        break;
    case 180:
        move(r1 -> posY, r1 -> posX);
        printw ("<");
        break;
    case 270:
        move(r1 -> posY, r1 -> posX);
        printw ("v");
        break;
    
    default:
        break;
    }

    refresh();
}



struct Bullet * newBullet (struct Rocket * r, int _yMax, int _xMax) { //bullets are not affected by gravity

    struct Bullet * b = (struct Bullet *) malloc(sizeof(struct Bullet));

    b -> speed = 4;

    b -> direction = r -> angle;

    b -> crashed = false;

    switch (b -> direction) //This will give the start point of the bullet relative to the direction of the rocket
    {
    case 0:
        b -> posY = r -> posY;
        b -> posX = r -> posX + 1;
        printBullet(b, _yMax, _xMax);
        break;

    case 90:
        b -> posY = r -> posY  - 1;
        b -> posX = r -> posX;
        printBullet(b, _yMax, _xMax);
        break;

    case 180:
        b -> posY = r -> posY;
        b -> posX = r -> posX - 1;
        printBullet(b, _yMax, _xMax);
        break;

    case 270:
        b -> posY = r -> posY + 1;
        b -> posX = r -> posX;
        printBullet(b, _yMax, _xMax);
        break;
    
    default:
        break;
    }
    
    return b;
}

void bulletThrust (struct Bullet * b, int _yMax, int _xMax) {

    switch (b -> direction)
    {
    case 0:
        delBullet(b);
        b -> posX = b -> posX + b -> speed;
        printBullet(b, _yMax, _xMax);
        break;
    case 90:
        delBullet(b);        
        b -> posY = b -> posY - b -> speed;
        printBullet(b, _yMax, _xMax);
        break;
    case 180:
        delBullet(b);
        b -> posX = b -> posX - b -> speed;
        printBullet(b, _yMax, _xMax);
        break;
    case 270:
        delBullet(b);
        b -> posY = b -> posY + b -> speed;
        printBullet(b, _yMax, _xMax);
        break;
    
    default:
        break;
    }

    refresh();
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
    if (r->posY < (_yMax - BOARD_COLS)/2 + 1)  //Limitations so it doesnt go out of the box UPPER
        r->posY = (_yMax - BOARD_COLS)/2 + 1; 

    if (r->posY > (_yMax - BOARD_COLS)/2 + BOARD_COLS - 2) //Limitations so it doesnt go out of the box LOWER
        r->posY = (_yMax - BOARD_COLS)/2 + BOARD_COLS - 2; 

    if (r->posX < (_xMax/2) - (BOARD_ROWS/2) + 1) //Limitations so it doesnt go out of the box LEFT
        r->posX = (_xMax/2) - (BOARD_ROWS/2) + 1; 

    if (r->posX > (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS - 2) //Limitations so it doesnt go out of the box RIGHT
        r->posX = (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS - 2; 

}




void delRocketTrail(struct Rocket * r) {
    
    switch (r -> angle)
    {
    case 0:
        move(r -> posY, r -> posX - 1);//delete movement trail
        printw (" ");
        break;
    case 90:
        move(r -> posY + 1, r -> posX);//delete movement trail
        printw (" ");
        break;
    case 180:
        move(r -> posY, r -> posX + 1);//delete movement trail
        printw (" ");
        break;
    case 270:
        move(r -> posY - 1, r -> posX);//delete movement trail
        printw (" ");
        break;
    
    default:
        break;
    }
}

void delRocket(struct Rocket * r) {
    move(r->posY, r->posX);
    printw(" ");
    
    delRocketTrail(r);

}

void printRocketTrail (struct Rocket * r1) {
    switch (r1 -> angle)
    {
    case 0:
        move(r1 -> posY, r1 -> posX - 1);//Add movement trail
        printw (")");
        break;
    case 90:
        move(r1 -> posY + 1, r1 -> posX);//Add movement trail
        printw ("|");
        break;
    case 180:
        move(r1 -> posY, r1 -> posX + 1);//Add movement trail
        printw ("(");
        break;
    case 270:
        move(r1 -> posY - 1, r1 -> posX); //Add movement trail
        printw ("|");
        break;
    
    default:
        break;
    }

    refresh();
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
        printRocketTrail(r1);
        break;
    case 90:
        checkCollisionBorders(r1, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        checkCollisionBH (bh, r1);
        move(r1 -> posY, r1 -> posX);
        printw ("^");
        printRocketTrail(r1);
        break;
    case 180:
        checkCollisionBorders(r1, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        checkCollisionBH (bh, r1);
        move(r1 -> posY, r1 -> posX);
        printw ("<");
        printRocketTrail(r1);
        break;
    case 270:
        checkCollisionBorders(r1, _yMax, _xMax);
        checkCollisionRockets(r1, r2);
        checkCollisionBH (bh, r1);
        move(r1 -> posY, r1 -> posX);
        printw ("v");
        printRocketTrail(r1);
        break;
    
    default:
        break;
    }

    refresh();
}

void updateFuel (struct Rocket * r) { //Print score of rocket 1
    if (r -> number == 1) {
        int y = r -> startY + 3 ;
        int x = r -> startX + 1 + 18;
        move(y, x);
        printw("Fuel = %d ", r -> fuel);
    }

    if (r -> number == 2) {
        int y = r -> startY - 3 ;
        int x = r -> startX - 10 - 18;
        move(y, x);
        printw("Fuel = %d ", r -> fuel);
    }  
}



void rocketThrust (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {

    int newYDown = r1 -> posY + r1 -> speed;
    int newXRight = r1 -> posX + r1 -> speed;
    int newYUp = r1 -> posY - r1 -> speed;
    int newXLeft = r1 -> posX - r1 -> speed;

    if (r1 -> fuel == 0)
        return;

    r1 -> fuel = r1 -> fuel - 5;
    updateFuel(r1);

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
        printw("\\");
        b->status = 1;
        refresh();
        wrefresh(screen);
        return;
    }

    if (b->status == 1){
        printw("|");
        b->status = 2;
        refresh();
        wrefresh(screen);
        return;
    }

    if (b->status == 2){
        printw("/");
        b->status = 3;
        refresh();
        wrefresh(screen);
        return;
    }

    if (b->status == 3){
        printw("=");
        b->status = 0;
        refresh();
        wrefresh(screen);
        return;
    } 
}


//Functions for the rotation of the rocket

void rotateLeft (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {
    
    switch (r1 -> angle)
    {
        case 0:
            delRocketTrail(r1);
            r1 -> angle = 90;
            printRocket(r1, r2, _yMax, _xMax, bh);
            refresh();
            break;

        case 90:
            delRocketTrail(r1);
            r1 -> angle = 180;
            printRocket(r1, r2, _yMax, _xMax, bh);
            refresh();
            break;

        case 180:
            delRocketTrail(r1);
            r1 -> angle = 270;
            printRocket(r1, r2, _yMax, _xMax, bh);
            refresh();
            break;

        case 270:
            delRocketTrail(r1);
            r1 -> angle = 0;
            printRocket(r1, r2, _yMax, _xMax, bh);
            refresh();
            break;
    }


    
}

void rotateRight (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {

    switch (r1 -> angle)
    {
        case 0:
            delRocketTrail(r1);
            r1 -> angle = 270;
            printRocket(r1, r2, _yMax, _xMax, bh);
            refresh();
            break;

        case 270:
            delRocketTrail(r1);
            r1 -> angle = 180;
            printRocket(r1, r2, _yMax, _xMax, bh);
            refresh();
            break;

        case 180:
            delRocketTrail(r1);
            r1 -> angle = 90;
            printRocket(r1, r2, _yMax, _xMax, bh);
            refresh();
            break;

        case 90:
            delRocketTrail(r1);
            r1 -> angle = 0;
            printRocket(r1, r2, _yMax, _xMax, bh);
            refresh();
            break;
    }
}

void hyperSpace(struct Rocket * r, int _yMax, int _xMax, struct BlackHole * bh) { //TP to random location

    if (r -> hyperSpace == true) //If hyperSpace has already been used, leave the function
        return;

    int upperLimit = (_yMax - BOARD_COLS)/2 + 1;
    int lowerLimit = (_yMax - BOARD_COLS)/2 + BOARD_COLS - 2;
    int leftLimit = (_xMax/2) - (BOARD_ROWS/2) + 1;
    int rightLimit = (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS - 2;


    srand(time(NULL)); //Initialization for random, only called once.

    int yRandom;
    int xRandom;

    do {
        yRandom =  rand() % lowerLimit;
        xRandom = rand() % rightLimit;
    }
    while ((upperLimit < yRandom < lowerLimit) && (leftLimit < xRandom < rightLimit) && (yRandom != bh->posY) && (xRandom != bh -> posX)) ;

    delRocket(r); //clear the rocket before changing the new position
    
    r -> posY = yRandom;
    r -> posX = xRandom;

    printRocketStart(r); //print the new rocket

    r -> hyperSpace = true; //hyperSpace has been used, so it cannot be used again later.
}


struct Bullet * getKeys(struct Bullet * b, struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax, struct BlackHole * bh) {

        int KeyPress = wgetch(screen); //waits for input

    
        switch (KeyPress)
        {
            case 119: //W == SHOOT
                b = newBullet(r1, _yMax, _xMax);
                return b;
                break;

            case 115: //S == THRUST
                rocketThrust(r1, r2, _yMax, _xMax, bh);
                break;

            case 97: // A == ROTATE LEFT
                rotateLeft(r1, r2, _yMax, _xMax, bh);
                break;

            case 100: // D == ROTATE RIGHT
                rotateRight(r1, r2, _yMax, _xMax, bh);
                break;

            case 105: //I == SHOOT
                b = newBullet(r2, _yMax, _xMax);
                return b;
                break;

            case 107://K == TRHUST
                rocketThrust(r2, r1, _yMax, _xMax, bh);
                break;

            case 106: //J == ROTATE LEFT
                rotateLeft(r2, r1, _yMax, _xMax, bh);
                break;

            case 108: //L == ROTATE RIGHT
                rotateRight(r2, r1, _yMax, _xMax, bh);
                break;

            case 113: //Q = HYPERSPACE
                hyperSpace(r1, _yMax, _xMax, bh);
                break;

            case 117: //U = HYPERSPACE
                hyperSpace(r2, _yMax, _xMax, bh);
                break;
            

            default:
                break;
        }

        refresh();
        wrefresh(screen); //This refresh in necesary to use the arrow keys, so DO NOT DELETE
        return b; 


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
    halfdelay(1); //Go back to the lowest value of input waiting time`
}

void gravityRocket (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {


    if ((r1 -> posX) < (_xMax/2) && (r1 -> posY) < (_yMax/2)) { //Top Left Quadrant 1

        delRocket(r1);

        r1 -> posY = r1 -> posY + r1 -> gravity;
        r1 -> posX = r1 -> posX + (r1 -> gravity);

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if ((r1 -> posX) > (_xMax/2) && (r1 -> posY) < (_yMax/2)) { //Top Right Quadrant 2

        delRocket(r1);

        r1 -> posY = r1 -> posY + r1 -> gravity;
        r1 -> posX = r1 -> posX - (r1 -> gravity);

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if ((r1 -> posX) < (_xMax/2) && (r1 -> posY) > (_yMax/2)) { //Bottom Left Quadrant 3

        delRocket(r1);

        r1 -> posY = r1 -> posY - r1 -> gravity;
        r1 -> posX = r1 -> posX + (r1 -> gravity);

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if ((r1 -> posX) > (_xMax/2) && (r1 -> posY) > (_yMax/2)) { //Bottom Right Quadrant 4

        delRocket(r1);

        r1 -> posY = r1 -> posY - r1 -> gravity;
        r1 -> posX = r1 -> posX - (r1 -> gravity);

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    /*----------------------------------------------------------------------------------------------------*/
    //This is only for the axis
    if (((r1 -> posX) == (_xMax/2)) && ((r1 -> posY) <= (_yMax/2))) { //Upper Y

        delRocket(r1);

        
        r1 -> posY = (r1 -> posY) + (r1 -> gravity);
        

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if (((r1 -> posX) == (_xMax/2)) && ((r1 -> posY) >= (_yMax/2))) { //Lower Y

        delRocket(r1);

        
        r1 -> posY = r1 -> posY - r1 -> gravity;

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if (((r1 -> posX) <= (_xMax/2)) && ((r1 -> posY) == (_yMax/2))) { //Bottom Left Quadrant 3

        delRocket(r1);

        
        r1 -> posX = r1 -> posX + (r1 -> gravity);
        

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if (((r1 -> posX) >= (_xMax/2)) && ((r1 -> posY) == (_yMax/2))) { //Bottom Right Quadrant 4

        delRocket(r1);

        
        r1 -> posX = r1 -> posX - (r1 -> gravity);
        

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

}

void printScore1 (struct Rocket * r) { //Print score of rocket 1
    int y = r -> posY + 3 ;
    int x = r -> posX + 1;
    move(y, x);
    printw("%s = %d", r -> name, r -> score);
}

void printScore2 (struct Rocket * r) { //Print score of rocket 1
    int y = r -> posY - 3 ;
    int x = r -> posX - 10;
    move(y, x);
    printw("%s = %d", r -> name, r -> score);
}

void printFuel1 (struct Rocket * r) { //Print score of rocket 1
    int y = r -> posY + 3 ;
    int x = r -> posX + 1 + 18;
    move(y, x);
    printw("Fuel = %d", r -> fuel);
}

void printFuel2 (struct Rocket * r) { //Print score of rocket 1
    int y = r -> posY - 3 ;
    int x = r -> posX - 10 - 18;
    move(y, x);
    printw("Fuel = %d", r -> fuel);
}

void addScore (struct Rocket * r) {
    //If something happens, add +10 to the score, and print it again.
}

void gravityBullet (struct Bullet * b, int _yMax, int _xMax) {



    if (((b -> posX) <= (_xMax/2)) && ((b -> posY) <= (_yMax/2))) { //Top Left Quadrant 1

        delBullet(b);

        b -> posY = (b -> posY) + 1;
        b -> posX = (b -> posX) + 1;

        printBullet(b, _yMax, _xMax);
    }

    else if (((b -> posX) >= (_xMax/2)) && ((b -> posY) <= (_yMax/2))) { //Top Right Quadrant 2

        delBullet(b);

        b -> posY = (b -> posY) + 1;
        b -> posX = (b -> posX) - 1;

        printBullet(b, _yMax, _xMax);
    }

    else if (((b -> posX) <= (_xMax/2)) && ((b -> posY) >= (_yMax/2))) { //Bottom Left Quadrant 3

        delBullet(b);

        b -> posY = (b -> posY) - 1;
        b -> posX = (b -> posX) + 1;

        printBullet(b, _yMax, _xMax);
    }

    else if (((b -> posX) >= (_xMax/2)) && ((b -> posY) >= (_yMax/2))) { //Bottom Right Quadrant 4

        delBullet(b);

        b -> posY = (b -> posY) - 1;
        b -> posX = (b -> posX) - 1;

        printBullet(b, _yMax, _xMax);
    }

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

    printw("test 1");
    startScreen(yMax, xMax, win);
    box(win, 0, 0); //when doing startScreen, we delete the box, so we have to recreate it again.


    

    //INITIALISATION OF ROCKET 1 and 2
    struct Rocket * rocket1 = newRocket("Player 1", (yMax - BOARD_COLS)/2 + BOARD_COLS - 3, (xMax/2) - (BOARD_ROWS/2) + 2, 90, 1);
    struct Rocket * rocket2 = newRocket("Player 2", (yMax - BOARD_COLS)/2 + 2, (xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS - 3, 270, 2);

    

    //INITIALISATION OF BLACK HOLE
    struct BlackHole * hole = newBlackHole(yMax/2, xMax/2);

    //MOVE ROCKET 1
    move(rocket1->posY, rocket1->posX ); //moves the cursor 
    printRocketStart(rocket1); //prints the rocket

    printw("test 2");

    //MOVE ROCKET 2
    move(rocket2->posY, rocket2->posX ); //moves the cursor 
    printRocketStart(rocket2); //prints the rocket

    //Print score, when score = 5, stop the game.
    printScore1(rocket1);
    printScore2(rocket2);
    printFuel1(rocket1);
    printFuel2(rocket2);

    struct Bullet * bullet[MAX_BULLETS];


    refresh();

    while (rocket1 -> crashed != true && rocket2 -> crashed != true) //game loop
    {   
        changeBlackHole(win, hole);  //Gives an animation to the black hole
        
        for (int i = 0; i < MAX_BULLETS; i++) {
            bullet[i] = getKeys(bullet[i], rocket1, rocket2, win,  yMax, xMax, hole); //gets input

            if ((bullet[i] != NULL) && (bullet[i] -> crashed == false)) { //if bullet[i] is NULL ignore it
                bulletThrust(bullet[i], yMax, xMax);
            }            
        }

        gravityRocket(rocket1, rocket2, yMax, xMax, hole);
        gravityRocket(rocket2, rocket1, yMax, xMax, hole);
    }

    //END OF THE GAME
    endwin(); //stops the window, terminates the program

    printWinner(rocket1, rocket2);


    return 0;
}