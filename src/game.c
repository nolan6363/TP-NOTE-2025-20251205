#include "game.h"

// Game Variables
Player players[NUM_PLAYERS];                       // Players
Position powerUps[NUM_POWERUPS];                   // PowerUps
pthread_t threads[NUM_PLAYERS + 1];     // Threads to hands Players and PowerUps
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex to handle thread contention while displaying the grid.
char END_GAME = 0;                                 // Game status flag. 0 = Run, 1 = End.


void initializePlayers() {
    srand(time(NULL));

    for (int i = 0; i < NUM_PLAYERS; ++i) {
        int unique = 0;
        while (!unique) {
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

        players[i].health = 10;
        players[i].id = i;
    }
}

void checkInteraction(Player *p) {

    for (int i = 0; i < NUM_PLAYERS; ++i) {
        if(players[i].id == p->id) continue;

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

    for (int j = 0; j < NUM_POWERUPS; ++j) {
        if (powerUps[j].x == p->pos.x && powerUps[j].y == p->pos.y) {
            int heal = 1 + (rand() % 3);
            p->health += heal;
            printf("Player %d powerUp. Health: %d\n", p->id, p->health);
        }
    }
}


void* simulateMovement(void* player_id) {

    int id = *((int*)player_id);

    while (!END_GAME) {
        int dx = (rand() % 3) - 1;
        int dy = (rand() % 3) - 1;

        int new_x = players[id].pos.x + dx;
        int new_y = players[id].pos.y + dy;

        if (new_x >= 0 && new_x < GRID_SIZE && new_y >= 0 && new_y < GRID_SIZE) {
            players[id].pos.x = new_x;
            players[id].pos.y = new_y;
        }

        pthread_mutex_lock(&mutex);

        displayGame(players, powerUps);
        checkInteraction(&players[id]);

        pthread_mutex_unlock(&mutex);

        usleep(500000);
    }

    pthread_exit(NULL);
}

void* generatePowerUps(void* unused_arg) {

    for (int i = 0; i < NUM_POWERUPS; ++i) {
        powerUps[i].x = -1;
        powerUps[i].y = -1;
    }

    for (int i = 0; i < NUM_POWERUPS; ++i) {
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

    initializePlayers();

    int player_ids[NUM_PLAYERS];
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        player_ids[i] = i;
    }

    pthread_create(&threads[NUM_PLAYERS], NULL, generatePowerUps, NULL);

    for (int i = 0; i < NUM_PLAYERS; ++i) {
        pthread_create(&threads[i], NULL, simulateMovement, &player_ids[i]);
    }

    for (int i = 0; i < NUM_PLAYERS + 1; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_lock(&mutex);
    displayGame(players, powerUps);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_destroy(&mutex);

    return 0;
}
