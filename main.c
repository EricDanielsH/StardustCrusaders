// player 1 = player on the left down side of the screen = ^ v < >
// player 2 = player on the right up side of the screen = v ^ < >

#include <ncurses.h> // remember that ncurses coordinates are y, x; not x, y
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Creating some constants for the board dimensions
#define BOARD_DIMENSIONS  50 // If this number in odd, some problems will happen to the box
#define BOARD_COLS BOARD_DIMENSIONS
#define BOARD_ROWS BOARD_DIMENSIONS * 2.5  // the ratio of the size of rows to cols is aprox 2->5
#define MAX_BULLETS 50 // This is the max amount of bullet that a rocket can shoot in total
#define MAX_POINTS 5
#define MIN_FIRE_RATE 1000 // This is equivalent to 1 sec


/////////////////////////
/////* BlackHole *///////
/////////////////////////


struct BlackHole {
    int posY;
    int posX;
    int status;
};

// Create an instance of a blackhole
struct BlackHole * newBlackHole (int Y, int X) {

    struct BlackHole * bh = (struct BlackHole *) malloc(sizeof(struct BlackHole));
   
    bh -> posY = Y;
    bh -> posX = X;
    bh -> status = 0;
    
    return bh;
}

// Gives a small animation to the blackhole
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


/////////////////////////
/////* Rocket *//////////
/////////////////////////


struct Rocket {
    char name[10];
    int number;
    int speed; // speed of the rocket ()
    int posY;
    int posX;
    int angle;
    int score;
    int gravity;
    int fuel;
    int acc; // acceleration, used when thrusting
    bool crashed;
    bool hyperSpace;

    int startY;
    int startX;

};

// Create an instace of a Rocket
struct Rocket * newRocket (char _name[10], int Y, int X, int _angle, int _number) {

    struct Rocket * rocket = (struct Rocket *) malloc(sizeof(struct Rocket));

    strcpy (rocket -> name, _name);
    rocket -> posY = Y;
    rocket -> posX = X;
    rocket -> crashed = false;
    rocket -> angle = _angle;
    rocket -> speed = 1;
    rocket -> acc = 4;
    rocket -> score = 0;
    rocket -> gravity = 1;
    rocket -> fuel = 100;
    rocket -> number = _number;
    rocket -> hyperSpace = false;

    rocket -> startY = Y;
    rocket -> startX = X;
    
    return rocket;
}

// Print the rocket without checking any collisions
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

// Checks the collisions between the rocket and the blackhole
void checkCollisionBH (struct BlackHole * b, struct Rocket * r) {
    if (b -> posY == r -> posY && b -> posX == r -> posX) {
        // If this happens, the collision has happened
        r -> crashed = true;
    }
}

// Checks collisions between rockets
void checkCollisionRockets (struct Rocket * r1, struct Rocket * r2) {
    if ((r1 -> posY == r2 -> posY) && (r1 -> posX == r2 -> posX)) {
        // If this happens, the collision has happened
        r1 -> crashed = true;
        r2 -> crashed = true;
    }
}

// Checks collisions between rockets and borders
void checkCollisionBorders (struct Rocket * r, int _yMax, int _xMax) {
    if (r->posY < (_yMax - BOARD_COLS)/2 + 2)  // Limitations so it doesnt go out of the box UPPER
        r->posY = (_yMax - BOARD_COLS)/2 + 2; 

    if (r->posY > (_yMax - BOARD_COLS)/2 + BOARD_COLS - 3) // Limitations so it doesnt go out of the box LOWER
        r->posY = (_yMax - BOARD_COLS)/2 + BOARD_COLS - 3; 

    if (r->posX < (_xMax/2) - (BOARD_ROWS/2) + 2) // Limitations so it doesnt go out of the box LEFT
        r->posX = (_xMax/2) - (BOARD_ROWS/2) + 2; 

    if (r->posX > (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS - 3) // Limitations so it doesnt go out of the box RIGHT
        r->posX = (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS - 3; 

}

// Deletes the visuals of the rocket trail
void delRocketTrail(struct Rocket * r) {
    
    switch (r -> angle)
    {
    case 0:
        move(r -> posY, r -> posX - 1);// Delete movement trail
        printw (" ");
        break;
    case 90:
        move(r -> posY + 1, r -> posX);// Delete movement trail
        printw (" ");
        break;
    case 180:
        move(r -> posY, r -> posX + 1);// Delete movement trail
        printw (" ");
        break;
    case 270:
        move(r -> posY - 1, r -> posX);// Delete movement trail
        printw (" ");
        break;
    
    default:
        break;
    }
}

// Deletes the visuals of the rocket
void delRocket(struct Rocket * r) {
    move(r->posY, r->posX);
    printw(" ");
    
    delRocketTrail(r);
}

// Prints the visuals of the rocket trail
void printRocketTrail (struct Rocket * r1) {
    switch (r1 -> angle)
    {
    case 0:
        move(r1 -> posY, r1 -> posX - 1);// Add movement trail
        printw (")");
        break;
    case 90:
        move(r1 -> posY + 1, r1 -> posX);// Add movement trail
        printw ("|");
        break;
    case 180:
        move(r1 -> posY, r1 -> posX + 1);// Add movement trail
        printw ("(");
        break;
    case 270:
        move(r1 -> posY - 1, r1 -> posX); // Add movement trail
        printw ("|");
        break;
    
    default:
        break;
    }

    refresh();
}

// Prints the rocket in the screen, checking all collisions
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

// Updates the visuals of the fuel
void updateFuel (struct Rocket * r) { // Print score of rocket 1
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

// Allows the rocket to rotate to the left
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

// Allows the rocket to rotate to the right
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

// When a certain key is pressed, thrust the rocket by the Rocket Acceleration, and subtracts fuel from the rocket
void rocketThrust (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {

    int newYDown = r1 -> posY + r1 -> acc;
    int newXRight = r1 -> posX + r1 -> acc;
    int newYUp = r1 -> posY - r1 -> acc;
    int newXLeft = r1 -> posX - r1 -> acc;

    if (r1 -> fuel == 0)
        return;

    r1 -> fuel = r1 -> fuel - 5;
    updateFuel(r1);

    switch (r1 -> angle)
    {
    case 0:
        delRocket(r1);
        move (r1 -> posY, newXRight);
        r1 -> posX = newXRight; // Updates the new X
        printRocket(r1, r2, _yMax, _xMax, bh);
        break;
    case 90:
        delRocket(r1);
        move (newYUp, r1 -> posX);
        r1 -> posY = newYUp; // Updates the new Y
        printRocket(r1, r2, _yMax, _xMax, bh);

        break;
    case 180:
        delRocket(r1);
        move (r1 -> posY, newXLeft);
        r1 -> posX = newXLeft; // Updates the new X
        printRocket(r1, r2, _yMax, _xMax, bh);

        break;
    case 270:
        delRocket(r1);
        move (newYDown, r1 -> posX);
        r1 -> posY = newYDown; // Updates the new Y
        printRocket(r1, r2, _yMax, _xMax, bh);

        break;
    
    default:
        break;
    }
}

// HyperSpace is a random TP inside of the map that it can only be used once every round
void hyperSpace(struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) { 

    if (r1 -> hyperSpace == true) //If hyperSpace has already been used, leave the function
        return;

    int upperLimit = (_yMax - BOARD_COLS)/2 + 2;
    int lowerLimit = (_yMax - BOARD_COLS)/2 + BOARD_COLS - 3; 
    int leftLimit = (_xMax/2) - (BOARD_ROWS/2) + 2;
    int rightLimit = (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS - 3; 

    int yRandom = 0;
    int xRandom = 0;

    int count=0;

    while ((yRandom < upperLimit || yRandom > lowerLimit) || (xRandom < leftLimit || xRandom > rightLimit) || (yRandom == bh->posY) || (xRandom == bh->posX) || (yRandom == r2->posY) || (xRandom == r2->posX)) {
        yRandom = (rand() % (lowerLimit - upperLimit + 1)) + upperLimit;
        xRandom = (rand() % (rightLimit - leftLimit + 1)) + leftLimit;
        if(yRandom < upperLimit)
            yRandom += upperLimit;
        if (xRandom < leftLimit)
            xRandom += leftLimit;
        count++;
        
    }

    delRocket(r1); // clear the rocket before changing the new position
    
    r1 -> posY = yRandom;
    r1 -> posX = xRandom;

    printRocketStart(r1); // print the new rocket
    printRocketTrail(r1); // print the rocket trails

    r1 -> hyperSpace = true; // hyperSpace has been used, so it cannot be used again later.
}


/////////////////////////
/////* Bullet *//////////
/////////////////////////


struct Bullet {
    int speed;
    int posY;
    int posX;
    int direction;
    bool active;
    bool crashed;
};

// Delete the visuals of a bullet
void delBullet (struct Bullet * b) {
    move (b -> posY, b -> posX);
    printw(" ");
    refresh();
}

// Checks that the bullets dont go out of the borders
void bulletCollisionBorders (struct Bullet * b, int _yMax, int _xMax) {

    // Add 6 to each one so it doesnt clear the border

    if (b->posY < (_yMax - BOARD_COLS)/2 + 4)  // Limitations so it doesnt go out from the top
        b -> active = false;

    if (b->posY > (_yMax - BOARD_COLS)/2 + BOARD_COLS - 4) // Limitations so it doesnt go out from the bottom
        b ->active = false;

    if (b->posX < (_xMax/2) - (BOARD_ROWS/2) + 4) // Limitations so it doesnt go out from the left
        b ->active = false;

    if (b->posX > (_xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS - 4) // Limitations so it doesnt go out of from the right
        b ->active = false;

    if (!b -> active)
        delBullet(b);
}

// Print a bullet if the bullet is active
void printBullet (struct Bullet * b, int _yMax, int _xMax) {
    bulletCollisionBorders(b, _yMax, _xMax);
    if (b -> active == true) {
        move (b -> posY, b -> posX);
        printw("*");
        refresh();
        return;
    }

    move (b -> posY, b -> posX);
    delBullet(b);
}

// Check collisions between bullets, and deactivates them when true
bool colBulxBul (struct Bullet * b1, struct Bullet * b2) {

    if (!b1 -> active || !b2 -> active) // If one of the bullets is not active, exit the function
        return false;
    
    return (b1 ->posX == b2 -> posX && b1 -> posY == b2 -> posY); // If the position of the bullets is the same, return true, else return false
}

// Create an instance of a bullet
struct Bullet * newBullet (struct Rocket * r, int _yMax, int _xMax) { // Bullets are not affected by gravity

    struct Bullet * b = (struct Bullet *) malloc(sizeof(struct Bullet));

    b -> speed = 2;

    b -> direction = r -> angle;

    b -> crashed = false;

    switch (b -> direction) // This will give the start point of the bullet relative to the direction of the rocket
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

// Moves the bullets fowards
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

// Gives gravity to the rocket
void gravityRocket (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh) {

    if ((r1 -> posX) < (_xMax/2) && (r1 -> posY) < (_yMax/2)) { // Top Left Quadrant 1

        delRocket(r1);

        r1 -> posY = r1 -> posY + r1 -> gravity;
        r1 -> posX = r1 -> posX + (r1 -> gravity);

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if ((r1 -> posX) > (_xMax/2) && (r1 -> posY) < (_yMax/2)) { // Top Right Quadrant 2

        delRocket(r1);

        r1 -> posY = r1 -> posY + r1 -> gravity;
        r1 -> posX = r1 -> posX - (r1 -> gravity);

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if ((r1 -> posX) < (_xMax/2) && (r1 -> posY) > (_yMax/2)) { // Bottom Left Quadrant 3

        delRocket(r1);

        r1 -> posY = r1 -> posY - r1 -> gravity;
        r1 -> posX = r1 -> posX + (r1 -> gravity);

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if ((r1 -> posX) > (_xMax/2) && (r1 -> posY) > (_yMax/2)) { // Bottom Right Quadrant 4

        delRocket(r1);

        r1 -> posY = r1 -> posY - r1 -> gravity;
        r1 -> posX = r1 -> posX - (r1 -> gravity);

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    /*-------------------This is only for the axis--------------------------------------*/

    if (((r1 -> posX) == (_xMax/2)) && ((r1 -> posY) <= (_yMax/2))) { // Upper Y

        delRocket(r1);
        
        r1 -> posY = (r1 -> posY) + (r1 -> gravity);
        
        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if (((r1 -> posX) == (_xMax/2)) && ((r1 -> posY) >= (_yMax/2))) { // Lower Y

        delRocket(r1);

        r1 -> posY = r1 -> posY - r1 -> gravity;

        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if (((r1 -> posX) <= (_xMax/2)) && ((r1 -> posY) == (_yMax/2))) { // Bottom Left Quadrant 3

        delRocket(r1);

        r1 -> posX = r1 -> posX + (r1 -> gravity);
        
        printRocket(r1, r2, _yMax, _xMax, bh);
    }

    else if (((r1 -> posX) >= (_xMax/2)) && ((r1 -> posY) == (_yMax/2))) { // Bottom Right Quadrant 4

        delRocket(r1);

        r1 -> posX = r1 -> posX - (r1 -> gravity);
        
        printRocket(r1, r2, _yMax, _xMax, bh);
    }
}

/////////////////////////
/////* GAME *//////////
/////////////////////////


// Collision detector for general purposes
bool check_collisions (int x1, int y1, int x2, int y2) { 
    return (x1 == x2 && y1 == y2);
}

// Prints the winner at the end of the game
void printWinner (struct Rocket * r1, struct Rocket * r2) {
    char loser[10];
    char winner[10];

    if ((r1 -> score) < (r2 -> score)) {
        strcpy(loser, r1 -> name);
        strcpy(winner, r2 -> name);
    }
        
    if ((r1 -> score) > (r2 -> score)) {
        strcpy(loser, r2 -> name);
        strcpy(winner, r1 -> name);
    }
        
    printf("%s = %d points\n", r1 -> name, r1 -> score);
    printf("%s = %d points\n", r2 -> name, r2 -> score);
    printf("The WINNER is %s. Sorry %s, maybe next time buddy.\n", winner, loser);
}

// Gets the keys pressed by the players, and return the value of the key pressed
int getKeys(struct Rocket * r1, struct Rocket * r2, WINDOW * screen, int _yMax, int _xMax, struct BlackHole * bh) {

        int KeyPress = wgetch(screen); // waits for input
    
        switch (KeyPress)
        {
            case 119: // W == SHOOT
                break; // Just going to use this to detect the key

            case 115: // S == THRUST
                rocketThrust(r1, r2, _yMax, _xMax, bh);
                break;

            case 97: // A == ROTATE LEFT
                rotateLeft(r1, r2, _yMax, _xMax, bh);
                break;

            case 100: // D == ROTATE RIGHT
                rotateRight(r1, r2, _yMax, _xMax, bh);
                break;

            case 105: // I == SHOOT
                break; // Just going to use this to detect the key

            case 107:// K == TRHUST
                rocketThrust(r2, r1, _yMax, _xMax, bh);
                break;

            case 106: // J == ROTATE LEFT
                rotateLeft(r2, r1, _yMax, _xMax, bh);
                break;

            case 108: // L == ROTATE RIGHT
                rotateRight(r2, r1, _yMax, _xMax, bh);
                break;

            case 113: // Q = HYPERSPACE
                hyperSpace(r1, r2, _yMax, _xMax, bh);
                break;

            case 117: // U = HYPERSPACE
                hyperSpace(r2, r1, _yMax, _xMax, bh);
                break;
            
            default:
                break;
        }

    refresh();
    wrefresh(screen); // This refresh in necesary to use the arrow keys, so DO NOT DELETE
    return KeyPress; 
}

// Prints the starting screen
void startScreen(int _yMax, int _xMax, WINDOW * screen) {
    move(_yMax/2, (_xMax/2) - 24);
    printw("Welcome to the Epic Space Wars!!!! (really epic)");
    move(_yMax/2 + 1, (_xMax/2) - 15);
    printw("Press any key to start the game");
    refresh();
    halfdelay(100); // Gives 10 seconds to wait for any inpuy. Even if no key is pressed, the game will start after 5secs
    getch();
    erase();
    halfdelay(1); // Go back to the lowest value of input waiting time`
    box(screen, 0, 0);
}

// Prints a screen between every round
void waitingScreen(struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, WINDOW * screen) {
    
    box(screen, 0, 0);
    move(_yMax/2, (_xMax/2) - 7);
    printw("Nice Round!!!!!");
    move(_yMax/2 + 1, (_xMax/2) - 20);
    printw("Get ready for the next battle, WarLord!");
    move(_yMax/2 + 2, (_xMax/2) - 6);
    printw("CURRENT SCORE");
    move(_yMax/2 + 3, (_xMax/2) - 10);
    printw("%s = %d points", r1 -> name, r1 -> score);
    move(_yMax/2 + 4, (_xMax/2) - 10);
    printw("%s = %d points", r2 -> name, r2 -> score);
    wrefresh(screen);
    refresh();
    napms(3000); // Stops the program for 3 seconds
    
}

// Prints the score of Player 1
void printScore1 (struct Rocket * r) { // Print score of rocket 1
    int y = r -> startY + 3 ;
    int x = r -> startX + 1;
    move(y, x);
    printw("%s = %d", r -> name, r -> score);
}

// Prints the score of Player 2
void printScore2 (struct Rocket * r) { // Print score of rocket 2
    int y = r -> startY - 3 ;
    int x = r -> startX - 10;
    move(y, x);
    printw("%s = %d", r -> name, r -> score);
}

// Prints the starting fuel of Player 1
void printFuel1 (struct Rocket * r) { // Print score of rocket 1
    int y = r -> startY + 3 ;
    int x = r -> startX + 1 + 18;
    move(y, x);
    printw("Fuel = %d", r -> fuel);
}

// Prints the starting fuel of Player 2
void printFuel2 (struct Rocket * r) { // Print score of rocket 2
    int y = r -> startY - 3 ;
    int x = r -> startX - 10 - 18;
    move(y, x);
    printw("Fuel = %d", r -> fuel);
}

// Adds 1 to the score, if both rockets crashed together, both rockets receive 1 point
void addScore (struct Rocket * r1, struct Rocket * r2) {
    if (r2 -> crashed == true && r1 -> crashed == true) {
        r1 -> score = r1 -> score + 1;
        r2 -> score = r2 -> score + 1;
        return;
    }

    if (r1 -> crashed == true)
        r2 -> score = r2 -> score + 1;
    if (r2 -> crashed == true)
        r1 -> score = r1 -> score + 1;
  
}

// Creates the game scenario
void createScenario (struct Rocket * r1, struct Rocket * r2, WINDOW * screen) {

    // Clears screen and creates the box it again
    erase();
    box(screen, 0, 0);

    // MOVE ROCKET 1
    move(r1->startY, r1->startX ); // moves the cursor 
    printRocketStart(r1); // prints the rocket

    // MOVE ROCKET 2
    move(r2->startY, r2->startX ); // moves the cursor 
    printRocketStart(r2); // prints the rocket

    // Print score, when score = 5, stop the game.
    printScore1(r1);
    printScore2(r2);
    printFuel1(r1);
    printFuel2(r2);


}

/*MOST IMPORTANT FUNCTION, THIS IS WHAT RUNS THE GAME*/
void gameRunner (struct Rocket * r1, struct Rocket * r2, int _yMax, int _xMax, struct BlackHole * bh, WINDOW * screen) {
    
    int bulletCounter1 = 0;
    int bulletCounter2 = 0;
    struct Bullet * bullet1[MAX_BULLETS];
    struct Bullet * bullet2[MAX_BULLETS];

    clock_t lastTS1 = clock (); // Last time shoot of rocket 1
    clock_t lastTS2 = clock (); // Last time shoot of rocket 2

    while ((r1 -> crashed != true) && (r2 -> crashed != true)) // game loop
    {    
        clock_t nowTime = clock();

        changeBlackHole(screen, bh);  // Gives an animation to the black blackhole

        gravityRocket(r1, r2, _yMax, _xMax, bh);
        gravityRocket(r2, r1, _yMax, _xMax, bh);

        int input = getKeys(r1, r2, screen, _yMax, _xMax, bh); // Ask for an input

        if (input == 119 && bulletCounter1 < MAX_BULLETS) { // Check if the key for shooting P1 has been pressed

            double lapse = (double)(nowTime - lastTS1); 

            if (lapse >= MIN_FIRE_RATE) { // ONLY CREATE THE BULLET IF THE TIME RATE IS APPROVED
                bullet1[bulletCounter1] = newBullet(r1, _yMax, _xMax);
                bullet1[bulletCounter1]->active = true;
                bulletCounter1++;
                lastTS1 = nowTime;  // Update the last fired time for rocket 1
            }
        }

        if (input == 105 && bulletCounter2 < MAX_BULLETS) { // Check if the key for shooting P2 has been pressed

            double lapse = (double)(nowTime - lastTS2);

            if (lapse >= MIN_FIRE_RATE) { // ONLY CREATE THE BULLET IF THE TIME RATE IS APPROVED
                bullet2[bulletCounter2] = newBullet(r2, _yMax, _xMax);
                bullet2[bulletCounter2]->active = true;
                bulletCounter2++;
                lastTS2 = nowTime;  // Update the last fired time for rocket 2
            }
        }

        for (int i = 0; i < bulletCounter1; i++) { // Collisions between bullets
            for (int j = 0; j < bulletCounter2; j++) {
                if (colBulxBul(bullet1[i], bullet2[j])) {
                    bullet1[i] -> active = false;
                    bullet2[j] -> active = false;
                    delBullet(bullet1[i]);
                    delBullet(bullet2[j]);

                    // No deallocation of memory, as you cant do two deallocations in the same time
                    // Error: free(): double free detected in tcache 2 -> aborted
                }
            }
        }

        // Checking for collisions between bullets and rocket 1
        for (int i = 0; i < bulletCounter2; i++) {
            if (bullet2[i]->active) {
                if (check_collisions(r1->posX, r1->posY,bullet2[i]->posX, bullet2[i]->posY))
                    r1->crashed = true;
            }
        }

        // Checking for collisions between bullets and rocket 2
        for (int i = 0; i < bulletCounter1; i++) {
            if (bullet1[i]->active) {
                if (check_collisions(r2->posX, r2->posY, bullet1[i]->posX, bullet1[i]->posY))
                    r2->crashed = true;
            }
        }

        /* After checking all collisions, we can print move the bullets */

        for (int i = 0; i < bulletCounter1; i++) { // Update the bullets already created by rocket 1
            if (bullet1[i] -> active) 
                bulletThrust(bullet1[i], _yMax, _xMax);
        }

        for (int i = 0; i < bulletCounter2; i++) { // Update the bullets already created by rocket 2
            if (bullet2[i] -> active) 
                bulletThrust(bullet2[i], _yMax, _xMax);
        }

    }
}

// Resets the stats of both rockets
void resetRockets (struct Rocket * r1, struct Rocket * r2) {
    // Reset crash state of rockets
    r1 -> crashed = false;
    r2 -> crashed = false;

    // Reset fuel of rockets
    r1 -> fuel = 100;
    r2 -> fuel = 100;

    // Clear previous rockets
    delRocket(r1); 
    delRocket(r2); 

    // Reset rockets positions
    r1 -> posX = r1 -> startX;
    r1 -> posY = r1 -> startY;

    r2 -> posX = r2 -> startX;
    r2 -> posY = r2 -> startY;

    // Reset hyperSpace of rockets
    r1 -> hyperSpace = false;
    r2 -> hyperSpace = false;

    
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
///////////////////////*                                    *///////////////////////
///////////////////////*                MAIN                *///////////////////////     
///////////////////////*                                    *///////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


int main()
{
    srand(time(NULL)); // Initialization for random, only called once.

    /* CREATES THE WINDOW, THE BOX AND SOME VARIABLES*/

    initscr(); // initializates the screen
    refresh(); // this refresh is needed to refresh the parent window
    noecho(); // stops the user input when using getch()
    cbreak(); // alllows the use of all keys, specially control C to stop the program

    int yMax, xMax; // creates the parameters of the size of the window
    getmaxyx(stdscr, yMax, xMax); // gets the max amount that y and x can be in the terminal and saves the values inside of y and x->

    WINDOW  *  win = newwin(BOARD_COLS, BOARD_ROWS, (yMax/2) - (BOARD_COLS/2), (xMax/2) - (BOARD_ROWS/2)); 
    // starts a new window (height, width, starting Y of cursor, starting X of cursor)
    // (yMax/2) - halfHeight->->->->-> This will make it centered on the screen-> Same applies to the width

    box(win, 0, 0); // creates a box inside of window-> arg2 and arg3 are the type of borders that the box has
    wrefresh(win); // refreshes the visuals of the window
    keypad(win, TRUE); // allows the use of arrow keys and F keys
    // nodelay(win , true);

    startScreen(yMax, xMax, win); // We start the screen

    /* INITIALISATION OF ROCKET 1 and 2 */

    struct Rocket * rocket1 = newRocket("Player 1", (yMax - BOARD_COLS)/2 + BOARD_COLS - 3, (xMax/2) - (BOARD_ROWS/2) + 2, 90, 1);
    struct Rocket * rocket2 = newRocket("Player 2", (yMax - BOARD_COLS)/2 + 2, (xMax/2) - (BOARD_ROWS/2) + BOARD_ROWS - 3, 270, 2);

    /* INITIALISATION OF BLACK HOLE */

    struct BlackHole * hole = newBlackHole(yMax/2, xMax/2);
    refresh();


    /* MAIN WHILE LOOP */

    while ((rocket1 -> score != MAX_POINTS) && (rocket2 -> score != MAX_POINTS)) {
        createScenario(rocket1, rocket2, win); // create screen with rockets, score, and fuel
        gameRunner(rocket1, rocket2, yMax, xMax, hole, win); 
        addScore(rocket1, rocket2);
        waitingScreen(rocket1, rocket2, yMax, xMax, win);
        resetRockets(rocket1, rocket2);
    }
    
    /* END OF THE GAME */

    endwin(); // stops the window, terminates the program
    printWinner(rocket1, rocket2);
    return 0;
}