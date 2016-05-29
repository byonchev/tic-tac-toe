#include "game.h"

#include <stdio.h>

static cell_t get_row_winner(cell_t *row)
{
    if ((row[0] == row[1]) && (row[1] == row[2]) && (row[0] != EMPTY))
    {
        return row[0];
    }

    return EMPTY;
}

static cell_t get_col_winner(cell_t *col)
{
    if ((*col == *(col+3)) && (*(col+3) == *(col+6)) && (*col != EMPTY))
    {
        return *col;
    }

    return EMPTY;
}

static cell_t get_diag_winner(cell_t *board)
{
    if ((board[0] == board[4]) && (board[4] == board[8]) && (board[0] != EMPTY))
    {
        return board[0];
    }
    else if ((board[2] == board[4]) && (board[4] == board[6]) && (board[2] != EMPTY))
    {
        return board[2];
    }

    return EMPTY;
}

static cell_t get_winner(board_t board)
{
    int row, col, i, j;
    cell_t winner = EMPTY;

    for (i = 0; i < 3; i += 1)
    {
        cell_t row_winner = get_row_winner(board.cells + i*3);

        if (row_winner != EMPTY)
        {
            winner = row_winner;
            break;
        }

        cell_t col_winner = get_col_winner(board.cells + i);

        if (col_winner != EMPTY)
        {
            winner = col_winner;
            break;
        }
    }

    if (winner == EMPTY)
    {
        winner = get_diag_winner(board.cells);
    }

    return winner;
}

static char cell(cell_t cell)
{
    if (cell == X) return 'X';
    if (cell == O) return 'O';

    return '_';
}

static void render_board(board_t board)
{
    int i;
    char row_letters[] = {'A', 'B', 'C'};

    printf("    1   2   3\n\n");

    for (i = 0; i < 3; i += 1)
    {
        cell_t *row = board.cells + i*3;
        printf("%c   %c   %c   %c\n\n", row_letters[i], cell(row[0]), cell(row[1]), cell(row[2]));
    }
}

static int get_cell_number(char *input)
{
    int row, col;

    row = input[0] - 'A';
    col = input[1] - '1';

    if ((row > 3) || (row < 0) || (col > 3) || (col < 0))
    {
        return -1;
    }

    return row*3 + col;
}

static int board_full(board_t board)
{
    int i;

    for (i = 0; i < 9; i += 1)
    {
        if (board.cells[i] == EMPTY)
        {
            return 0;
        }
    }

    return 1;
}

void start_game(int sockfd, player_t player)
{
    int i;
    board_t board;
    cell_t winner;

    board.turn = X_PLAYER;

    for (i = 0; i < 9; i += 1)
    {
        board.cells[i] = EMPTY;
    }

    char buffer[128];

    while (((winner = get_winner(board)) == EMPTY) && !board_full(board))
    {
        render_board(board);

        if (board.turn == player)
        {
            int cell_number;

            do {
                printf("Your turn! Enter cell id (A1, B3, etc...): \n");
                fgets(buffer, 128, stdin);
                cell_number = get_cell_number(buffer);
            } while ((cell_number < 0) || (board.cells[cell_number] != EMPTY));

            board.cells[cell_number] = (board.turn == X_PLAYER) ? X : O;

            board.turn = (board.turn == X_PLAYER) ? O_PLAYER : X_PLAYER;

            write(sockfd, &board, sizeof(board));
        }
        else
        {
            printf("Wait for your opponent's turn.\n");

            if (read(sockfd, &board, sizeof(board)) <= 0)
            {
                printf("Connection error. Quitting...");
                return;
            }
        }
    }

    render_board(board);

    if (winner == EMPTY)
    {
        printf("DRAW\n");
    }
    else
    {
        printf("The winner is player %s!\n", (winner == X) ? "X" : "O");
    }
}
