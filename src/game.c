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
    // Create rand seed with time func
    srand(time(NULL));

    // Initialize all the players at random postions within the grid
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        int unique = 0;
        while (!unique) {
            // Position
            players[i].pos.x = rand() % GRID_SIZE;
            players[i].pos.y = rand() % GRID_SIZE;

            unique = 1;
            for (int j = 0; j < i; ++j) {
                if (players[j].pos.x == players[i].pos.x &&
                    players[j].pos.y == players[i].pos.y) {
                    unique = 0;
                    break;
                }
            }
        }
        // Health
        players[i].health = 10;
        
        // id
        players[i].id = i;
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
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        if(players[i].id == p->id) continue;

        // Simulate player interaction (decrease health)
        if (players[i].pos.x == p->pos.x && players[i].pos.y == p->pos.y) {
            int damage = 1 + (rand() % 3);
            p->health -= damage;
            players[i].health -= damage;

            printf("Player %d & %d interacted. Health: %d & %d\n",
                   p->id, players[i].id, p->health, players[i].health);

            if (p->health <= 0 || players[i].health <= 0) {
                END_GAME = 1;
            }
        }
    }

    // Interation with PowerUps
    for (int j = 0; j < NUM_POWERUPS; ++j) {
        // Simulate powerups interaction (increase health)
        if (powerUps[j].x == p->pos.x && powerUps[j].y == p->pos.y) {
            int heal = 1 + (rand() % 3);
            p->health += heal;
            printf("Player %d powerUp. Health: %d\n", p->id, p->health);
        }
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

    int id = *((int*)player_id);

    while (!END_GAME) {
        // Generate random movement in the x-axis
        int dx = (rand() % 3) - 1;

        // Generate random movement in the y-axis
        int dy = (rand() % 3) - 1;

        int new_x = players[id].pos.x + dx;
        int new_y = players[id].pos.y + dy;

        if (new_x >= 0 && new_x < GRID_SIZE && new_y >= 0 && new_y < GRID_SIZE) {
            players[id].pos.x = new_x;
            players[id].pos.y = new_y;
        }

        // Lock the mutex before displaying and checking interactions
        pthread_mutex_lock(&mutex);

        displayGame(players, powerUps);
        checkInteraction(&players[id]);

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);

        usleep(500000);  // Sleep for 0.5 seconds
    }

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
        if (END_GAME) break;

        sleep(3);

        pthread_mutex_lock(&mutex);
        powerUps[i].x = rand() % GRID_SIZE;
        powerUps[i].y = rand() % GRID_SIZE;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}


int main() {

    // Initialize Players 
    initializePlayers();

    // Create one thread for power-up generation
    int player_ids[NUM_PLAYERS];
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        player_ids[i] = i;
    }
    // Create seperate threads for each player movement simulation
    pthread_create(&threads[NUM_PLAYERS], NULL, generatePowerUps, NULL);
    
    // Wait for all threads to finish
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        pthread_create(&threads[i], NULL, simulateMovement, &player_ids[i]);
    }

    for (int i = 0; i < NUM_PLAYERS + 1; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Display Statistics at the end of the game.
    pthread_mutex_lock(&mutex);
    displayGame(players, powerUps);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_destroy(&mutex);

    return 0;
}
