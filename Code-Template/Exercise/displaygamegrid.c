#include "game.h"

/* 
    A function to display the game grid.
    Feel free to modify this function or use it as it is !
*/
void displayGame(Player players[], Position powerUps[]) {
    // Display the grid with the player's position
    
    system("clear"); // Clear the terminal screen. (Remove this line if you do not see any output on the terminal screen !)

    for (int i = 0; i < GRID_SIZE; i++) { // Print Grid Row
        for (int j = 0; j < GRID_SIZE; j++) { // Print Grid Column
            
            char sym = '.'; // . represents empty cell
            
            // Check if the current cell has a power-up
            int idx = 0;
            while(idx < NUM_POWERUPS) { 
                if(i == powerUps[idx].x && j == powerUps[idx].y) {sym='+'; break;} // + represents the powerUp
                ++idx;
            } 

            // Check if the current cell has a player
            idx = 0;
            while(idx < NUM_PLAYERS) { 
                if(i == players[idx].pos.x && j == players[idx].pos.y) {
                    // If the cell is empty or has a power-up, display the player's ID
                    if (sym == '.' || sym == '+') { 
                        sym='0'+players[idx].id; 
                    } else { 
                        sym = '*'; // * represents the player interaction (multiple players on the same cell)
                    }
                }
                ++idx;
            }

            // Print the cell symbol (empty cell, PowerUp, or Player id) within square brackets
            printf("[%c]", sym);
        }
        printf("\n");
    }

     // Display player health information
    for(int i = 0; i < NUM_PLAYERS; ++i) {
            printf("Player %d Health: %d\n", players[i].id, players[i].health);
    }
}
