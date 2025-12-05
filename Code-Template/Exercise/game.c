#include "game.h"

// Game Variables
Player players[NUM_PLAYERS];                       // Players
Position powerUps[NUM_POWERUPS];                   // PowerUps
pthread_t threads[NUM_PLAYERS + 1];     // Threads to hands Players and PowerUps
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex to handle thread contention while displaying the grid.
char END_GAME = 0;                                 // Game status flag. 0 = Run, 1 = End.


/*
    TODO: 
    This function should initialise the players
    - At random positions within the grid
    - set their health level (example 10)
    - set their id (it must be unique for each player)
*/
void initializePlayers() {
    // Initialize all the players at random postions within the grid
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        // Position
        // ...
        
        // Health
        // ...
        
        // id
        // ...
    }
}

/*
    TODO: 
    This function should check for interaction between the player 'p' and the other players.
    - On interaction : Decreace both players health by a random value.
    - Otherwise : de nothing.
    It should also check for the interaction between the player 'p' and the PowerUps
    - On interaction : Increase player 'p' health by a random value.
    - Otherwise : de nothing.
*/
void checkInteraction(Player *p) {
    
    // Interation with other players
    for (int i = 0; i < NUM_PLAYERS-1; ++i) {
        if(players[i].id == p->id) continue;

        // Simulate player interaction (decrease health)
        // ...
    }

    // Interation with PowerUps
    for (int j = 0; j < NUM_POWERUPS; ++j) {

        // Simulate powerups interaction (increase health)
        // ...
    }

}


/*
    TODO:
    This function should simulate the movement of the player having id == player_id.

    // NOTE: This function MUST be called in seperate thread for each player.

    A player can move :
     - up, down, left, right, or diagonally
     - within the grid space
     - only one step at a time (HINT : use a loop and in each loop move only one step.)
*/
void* simulateMovement(void* player_id) {
    
    int id = *((int*)player_id); // Player ID

    // Loop :
        
        // Generate random movement in the x-axis
        // ...

        // Generate random movement in the y-axis
        // ...

        // Lock the mutex before displaying and checking interactions
        // ...

        displayGame(players, powerUps);
        checkInteraction(&players[id]);
        
        // Unlock the mutex
        // ...

        usleep(500000); // Sleep for 0.5 seconds
    
    // END LOOP

    pthread_exit(NULL);
}

/*
    TODO:
    This function generate PowerUps that appear on the game grid
    // - at random postion
    // - at an interval of 3 seconds (Hint: Use sleep()) 
    // - and stays there forever (it can be used by the players infinitely).
    Maximum number of PowerUps = NUM_POWERUPS

    // NOTE: This function MUST be called in one seperate thread.
*/
void* generatePowerUps(void* unused_arg) {
    
    // Remove all PowerUps from the grid
    for (int i = 0; i < NUM_POWERUPS; ++i) {
        powerUps[i].x = -1;
        powerUps[i].y = -1;
    }
    
    for (int i = 0; i < NUM_POWERUPS; ++i) {
        // Randomly assign positions for each power-up
        // ...

    }

    pthread_exit(NULL);
}


int main() {

    // Initialize Players    
    // ...

    // Create one thread for power-up generation
    // ...

    // Create seperate threads for each player movement simulation
    // ...

    // Wait for all threads to finish
    // ...

    // Display Statistics at the end of the game.
    displayGame(players, powerUps);

    return 0;
}
