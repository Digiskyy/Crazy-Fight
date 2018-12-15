#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


/**
 * @author Aurélien BLAISE
 * @date november 2018
 * @version 1.0
 * @brief Handles the game (events and game code)
 */



typedef struct Scores
{
    int killsPlayer[NB_PLAYERS];
    int deathsPlayer[NB_PLAYERS];
    int suicidesPlayer[NB_PLAYERS];
    int wonRounds[NB_PLAYERS];
    SDL_bool alive[NB_PLAYERS];
    int remainingPlayers;
    int winnerRound;
    int winnerGame;
}Scores;


/**
 * @brief Runs the game code each turn of the game loop
 *
 * @param < *map > Structure which stands for the map
 * @param < *player > Structure which stands for the player 1
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *lastTime > Handles the change of the sprites of the animations by saving the last time when the sprite changed
 * @param < *lastFireTime > Handles the rate of fire
 */
void launch_game(Map* map, Character* players[NB_PLAYERS], Input *in, Scores *scores, int arrayKill[2], unsigned int lastTime[NB_PLAYERS], unsigned int lastFireTime[NB_PLAYERS], int *choice, const int tableSimilarKeys[2][5]);

/**
 * @brief Runs the proper code in function of the events triggered by the players
 *
 * @param < *map > Structure which stands for the map
 * @param < *in > Structure which points the states of the keys, buttons and so on related to the events
 * @param < *player > Structure which stands for the player 1
 * @param < *lastTime > Handles the change of the sprites of the several animations by saving the last time when the sprite changed
 * @param < *lastFireTime > Handles the rate of fire
 */
void game_event(Map* map, Input *in, Character* players[NB_PLAYERS], unsigned int lastTime[NB_PLAYERS], int *choice, const int tableSimilarKeys[NB_PLAYERS][5]);

 /**
 * @brief Displays the sprites on the renderer in function of the state of the player
 *
 * @param < *screen > Represents the renderer on which sprites will be displayed
 * @param < *player > Represents the player whose sprites will be displayed
 */
void display_sprite(SDL_Renderer *screen, Character* players[NB_PLAYERS]);

void init_scores(Scores *scores); //before each game

void reset_scores(Scores *scores); //between each round

void update_scores(Scores *scores, Character* players[NB_PLAYERS]);

void scores_save(const char *pathScoresFile, Scores *scores);

typedef struct Text Text;

void init_text_end_round(SDL_Renderer *screen, Text *text, Scores *scores);

void free_text_in_game(Text *text);

void display_text_game(SDL_Renderer *screen, Text *text);


#endif // GAME_H_INCLUDED
