#ifndef GAME_H
#define GAME_H

typedef enum {X, O, EMPTY} cell_t;

typedef enum {X_PLAYER, O_PLAYER} player_t;

typedef struct
{
    cell_t cells[9];
    player_t turn;
} board_t;

void start_game(int sockfd, player_t player);

#endif
