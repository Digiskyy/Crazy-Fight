#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the game (events and game code) and the scores of each player
 */



/**
 * @struct Scores
 * @brief Stores the scores for each player
 */
typedef struct Scores
{
    int killsPlayer[NB_PLAYERS];    /**< Number of kills for each player */
    int deathsPlayer[NB_PLAYERS];   /**< Number of deaths (a death is only when a player kill another player) for each player */
    int suicidesPlayer[NB_PLAYERS]; /**< Number of suicides (a suicide is when a player leaps into the void) for each player */
    int wonRounds[NB_PLAYERS];      /**< Number of won rounds (a round is won when the left player is the only one who remains alive) for each player */
    SDL_bool alive[NB_PLAYERS];     /**< If each player is alive or not */
    int remainingPlayers;           /**< Number of remaining players */
    int winnerRound;                /**< The player who wins the current round */
    int winnerGame;                 /**< The player who wins the current game */
}Scores;


/* ================================================== HANDLING THE GAME ================================================== */

/**
 * @brief Runs the game code each turn of the game loop
 *
 * @param < *map > Structure which stands for the map
 * @param < players[] > Array which contains the players
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *scores > Structure which retains the score of each player
 * @param < arrayKill[] > Contains the last player who fired and the last player who got hit by another, it is useful in order to update the scores
 * @param < lastTime[] > Array which contains the lastTime of each player
 *                       lastTime handles the change of the sprites of the animations by saving the last time when the sprite changed
 * @param < lastFireTime[] > Array which contains the lastFireTime of each player
 *                           lastFireTime handles the speed to fire for each player by retaining the last time they fire
 * @param < *choice > Handles if the players get back to the menu or if they keep playing
 * @param < tableSimilarKeys[][] > Table used for the relation between the different keys of each player
 */
void launch_game(Map* map, Character* players[NB_PLAYERS], Input *in, Scores *scores, int arrayKill[2], unsigned int lastTime[NB_PLAYERS], unsigned int lastFireTime[NB_PLAYERS], int *choice, const int tableSimilarKeys[2][5]);

/**
 * @brief Runs the proper code in function of the events triggered by the players
 *
 * @param < *map > Structure which stands for the map
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < players[] > Array which contains all the players
 * @param < lastTime[] > Array which contains the lastTime of each player
 *                       lastTime handles the change of the sprites of the animations by saving the last time when the sprite changed
 * @param < *choice > Handles if the players get back to the menu or if they keep playing
 * @param < tableSimilarKeys[][] > Table used for the relation between the different keys of each player
 */
void game_event(Map* map, Input *in, Character* players[NB_PLAYERS], unsigned int lastTime[NB_PLAYERS], int *choice, const int tableSimilarKeys[NB_PLAYERS][5]);

/**
 * @brief Displays the sprites on the renderer in function of the state of the player
 *
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @param < players[] > Array which contains all the players
 */
void display_sprite(SDL_Renderer *screen, Character* players[NB_PLAYERS]);

/**
 * @brief Displays the health bars showing the remaining health points of each player
 *
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @param < players[] > Array which contains all the players
 */
void display_health_bar(SDL_Renderer *screen, Character* players[NB_PLAYERS]);


/* ================================================== HANDLING SCORES ================================================== */

/**
 * @brief Initialises the structure Scores
 *
 * @param < *scores > Structure which retains the score of each player
 */
void init_scores(Scores *scores); //before each game

/**
 * @brief Resets the structure Scores
 *
 * @param < *scores > Structure which retains the score of each player
 */
void reset_scores(Scores *scores); //between each round

/**
 * @brief Updates the structure Scores
 *
 * @param < *scores > Structure which retains the score of each player
 * @param < players[] > Contains all the players
 */
void update_scores(Scores *scores, Character* players[NB_PLAYERS]);

void scores_save(const char *pathScoresFile, Scores *scores);


/* ================================================== HANDLING TEXT ================================================== */

typedef struct Text Text;

/**
 * @brief Initialises the text which have to be displayed at the end of a round
 *
 * @param < *screen > Represents the renderer on which the texts will be displayed
 * @param < *text > Structure which stands for the text with several parameters needed
 * @param < *scores > Structure which retains the score of each player
 */
void init_text_end_round(SDL_Renderer *screen, Text *text, Scores *scores);

/**
 * @brief Initialises the text which have to be displayed at the end of a game to state the winner
 *
 * @param < *screen > Represents the renderer on which the texts will be displayed
 * @param < *text > Structure which stands for the text with several parameters needed
 * @param < indexWinner > Index of the player who has won the current game
 */
void init_text_end_game(SDL_Renderer *screen, Text *text, const int indexWinner);

/**
 * @brief Frees the memory used by the structures Text which are displayed in game
 *
 * @param < *text > Structure which stands for the text with several parameters needed
 */
void free_text_in_game(Text *text);

/**
 * @brief Displays the text on the screen
 *
 * @param < *screen > Represents the renderer on which the texts will be displayed
 * @param < *text > Structure which stands for the text with several parameters needed
 */
void display_text_game(SDL_Renderer *screen, Text *text);


#endif // GAME_H_INCLUDED
