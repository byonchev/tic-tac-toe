#ifndef GAME_H
#define GAME_H

typedef enum {X, O, EMPTY} cell;

typedef enum {X_PLAYER, Y_PLAYER} player;

struct board
{
    cell cells[9];
    player turn;
};

#endif
