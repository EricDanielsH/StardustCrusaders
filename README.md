# Stardust Crusaders

## A game that takes inspiration from the classic game "SpaceWar!".

This project is a small game that I have built as an assesment for my first year of university at Lancaster. The program is written in C and runs in the terminal. This game uses **"SpaceWar!"** as an inspiration, which was the first classic ‘shoot’em’up’ arcade game. 

This game consists on two player spaceships that have a duel in the space. There is also a 'singularity' that pulls both players to the spaceships in due to the force of gravity. The ships have a limited number of torpedoes (not affected by gravity), which can be fired at a maximum rate. The ships also have limited fuel to "thrust" to escape the singularity or to cleverly use the singularity in their favour.

This implementation includes a hyperspace jump, that allows the spaceship to jump one time each round to a random location on the map, making this choice a salvation or a downfall for the player.

The **controls** are really basic:<br><br>
    Player 1: W, A, S, D, Q <br>
    Player 2: I, J, K, L, U

* To **rotate**, use:<br>
    Player 1: A, D<br>
    Player 2: J, L

* To **thrust**, use:<br>
    Player 1: S<br>
    Player 2: K

* To **shoot**, use:<br>
    Player 1: W<br>
    Player 2: I

* To **Hyperspace**, use:<br>
    Player 1: Q<br>
    Player 2: U

## How to start the game
You only have to do some simple steps:
* Pull the repository from github into your machine (Use this in your terminal: "*git clone https://github.com/EricDanielsH/EpicSpaceWar*") and enter your account parameters if asked.
* Make sure that you have the ncurses library installed.
* Compile the program (Do: "*gcc -o main main.c -lncurses -g*").
* Start the program (Do: "*./main*").

## Variables that you can change!
This program allows the user to change some variables to customize the game as the player wants. All of these variables are found at the beggining of the program file. All variables have to be positive.

* **BOARD_DIMENSIONS** -> This variable controls the dimensions of the board. If you like to make the board of the game smaller, decrease this value. It is set by **default to 50.** 

    *Bonus tip:* You can increase the size of the board while decreasing the size of the terminal. In my experience, for each time you decrease your terminal size, you can add 10 to the BOARD_DIMENSIONS variable. I **REALLY** recommend doing this.

* **MAX_BULLETS** -> This variable refers to the maximum amount of bulletes that each spaceship can fire. The **default value of this variable is 25.** Reduce this variable if you feel your aim on point!

* **MAX_FIRE_RATE** -> This is the time that you have to wait before shooting your next bullet. The **default value is 1500**, which is approximately the same as 0.5 seconds. To make the game more chaotic, reduce this value to shoot faster.

* **GRAVITY_STRENGTH** -> This variable regulates the speed to which the spaceship is driven into the black hole. The **default** and minimum value that this variable has **is 1.** I personally would not change this variable, but ff you feel a bit heavier today, try to increase the it!

* **MAX_POINTS** -> This variable controls the highest amount of points that each spaceship can achieve. In other words, the number of rounds that you have to win before ending the game and acquire the victory against your opponent. The **default value is set to 5**, but feel free to increase or decrease this value depending on your time needs.  

This is not recommended, but you can also change the name of each ship in lines: **1053 & 1054** (replace "Player 1" & "Player 2"), but it should not be longer than 10 characters.

## Considerations that didn't make the game
* Increased gravity after each round. The gravity pull force is already good enough at the minimum value, and I think that if this value is increased, each round would only last 1 second.
* Adding gravity to bullets. One reason is that the original game does not have gravity force for the bullets. The other reason is that it would be really difficult to aim and hit a spaceship when the bullets do not go straight.
* Adding sounds. Sounds could not be added due to the absence of sound libraries installed in the computer labs (and the and the restriction of downloading libraries into the lab machines).
