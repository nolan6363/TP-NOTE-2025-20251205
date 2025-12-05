#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


// Define Game Environment
#define NUM_PLAYERS 3   // Maximum number of players
#define NUM_POWERUPS 2  // Maximum number of PowerUps
#define GRID_SIZE 5     // Game Grid Dimension

typedef struct {        // Player Position inside Grid
    int x;
    int y;
} Position;

typedef struct {        // Player Structure
    Position pos;
    int health;
    int id;
} Player;


void displayGame(Player players[], Position powerUps[]);


#endif // GAME_H
