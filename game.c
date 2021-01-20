#include "h_code/game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h_code/game_aux.h"
#include "h_code/game_ext.h"
#include "h_code/queue.h"
#include "h_code/struct.h"

// ------------- DEBUG FUNCTIONS -------------

void treat_error(char* error_message) {
    fprintf(stderr, "ERROR : %s \n", error_message);
    exit(EXIT_FAILURE);
}

void errorGrid(square* squares, uint* nb_tents_row, uint* nb_tents_col) {
    if (squares == NULL || nb_tents_row == NULL || nb_tents_col == NULL) {
        treat_error("Unvalid grid parameters!\n");
    }
}

void errorGame(cgame g) {
    if (g == NULL || g->squareArray == NULL || g->nbTentsRowArray == NULL ||
        g->nbTentsColArray == NULL || g->undoStack == NULL ||
        g->redoStack == NULL) {
        treat_error("Unvalid game pointer!\n");
    }
}

void insidePoint(uint point, uint size) {
    if (point >= size)
        treat_error("Unvalid parameter: one index is outside the game grid!\n");
}

// ------------- GAME FUNCTIONS -------------

game game_new_ext(uint nb_rows, uint nb_cols, square* squares,
                  uint* nb_tents_row, uint* nb_tents_col, bool wrapping,
                  bool diagadj) {
    errorGrid(squares, nb_tents_row, nb_tents_col);
    game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, diagadj);
    errorGame(g);

    // Populating the game board and expected tents row/col arrays.
    for (int i = 0; i < g->grid.size; i++) g->squareArray[i] = squares[i];
    for (int i = 0; i < g->grid.nb_cols; i++)
        g->nbTentsColArray[i] = nb_tents_col[i];
    for (int i = 0; i < g->grid.nb_rows; i++)
        g->nbTentsRowArray[i] = nb_tents_row[i];
    return g;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        bool diagadj) {
    struct game_s* myGame;
    // Memory allocation for the game and its arrays
    myGame = malloc(sizeof(struct game_s));
    myGame->squareArray = (uint*)calloc((nb_cols * nb_cols), sizeof(uint));
    myGame->nbTentsColArray = (uint*)calloc(nb_cols, sizeof(uint));
    myGame->nbTentsRowArray = (uint*)calloc(nb_rows, sizeof(uint));

    // Initializing options, grid and stacks
    myGame->option.diagadj = diagadj;
    myGame->option.wrapping = wrapping;

    myGame->grid.nb_cols = nb_cols;
    myGame->grid.nb_rows = nb_rows;
    myGame->grid.size = myGame->grid.nb_cols * myGame->grid.nb_rows;

    myGame->undoStack = queue_new();
    myGame->redoStack = queue_new();

    errorGame(myGame);
    return myGame;
}

uint game_nb_rows(cgame g) {
    errorGame(g);
    return g->grid.nb_rows;
}

uint game_nb_cols(cgame g) {
    errorGame(g);
    return g->grid.nb_cols;
}

bool game_is_wrapping(cgame g) {
    errorGame(g);
    return g->option.wrapping;
}

bool game_is_diagadj(cgame g) {
    errorGame(g);
    return g->option.diagadj;
}

void game_undo(game g) {
    // Checking for errors
    errorGame(g);
    if (queue_is_empty(g->undoStack))
        treat_error("undo called, but undo stack is empty.\n");

    // Retrieving the previous state of last modified square
    move* moveToPlay = queue_pop_tail(g->undoStack);

    // Populating the "Redo" queue with the move that is getting erased
    move* erasedMove = malloc(sizeof(move));
    erasedMove->row = moveToPlay->row;
    erasedMove->col = moveToPlay->col;
    erasedMove->square = game_get_square(g, erasedMove->row, erasedMove->col);
    queue_push_tail(g->redoStack, erasedMove);

    // Undoing move
    game_set_square(g, moveToPlay->row, moveToPlay->col, moveToPlay->square);
    free(moveToPlay);
    return;
}

void game_redo(game g) {
    // Checking for errors
    errorGame(g);
    if (queue_is_empty(g->redoStack))
        treat_error("redo called, but redo stack is empty.\n");

    // Retrieving the previous state of the last undone move
    move* moveToPlay = queue_pop_tail(g->redoStack);

    // Populating the "Undo" queue with the move that is getting erased
    move* erasedMove = malloc(sizeof(move));
    erasedMove->row = moveToPlay->row;
    erasedMove->col = moveToPlay->col;
    erasedMove->square = game_get_square(g, erasedMove->row, erasedMove->col);
    queue_push_tail(g->undoStack, erasedMove);

    // Redoing move
    game_set_square(g, moveToPlay->row, moveToPlay->col, moveToPlay->square);
    free(moveToPlay);
    return;
}

//---------- DEFINITION OF GAME.H FUNCTIONS ----------

game game_new(square* squares, uint* nb_tents_row, uint* nb_tents_col) {
    return game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares, nb_tents_row,
                        nb_tents_col, false, false);
}

game game_new_empty(void) {
    return game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false, false);
}

game game_copy(cgame g) {
    errorGame(g);

    // Initialization copyGame
    game copyGame = game_new_empty();

    // Copying all struct elements in copyGame
    memcpy(&(copyGame->grid), &(g->grid), sizeof(struct Grid));
    memcpy(copyGame->squareArray, g->squareArray,
           sizeof(uint) * (g->grid.nb_cols * g->grid.nb_rows));
    memcpy(copyGame->nbTentsRowArray, g->nbTentsRowArray,
           sizeof(uint) * g->grid.nb_rows);
    memcpy(copyGame->nbTentsColArray, g->nbTentsColArray,
           sizeof(uint) * g->grid.nb_cols);
    memcpy(&(copyGame->option), &(g->option), sizeof(struct Option));

    return copyGame;
}

bool game_equal(cgame g1, cgame g2) {
    errorGame(g1);
    errorGame(g2);

    // Testing if the size of the grids are equals
    if (game_nb_cols(g1) != game_nb_cols(g2) ||
        game_nb_rows(g1) != game_nb_rows(g2)) {
        return false;
    }

    // Checking if each square, row and col are equal
    for (uint i = 0; i < game_nb_rows(g1); i++) {
        if (game_get_expected_nb_tents_row(g1, i) !=
            game_get_expected_nb_tents_row(g2, i))
            return false;
        for (uint j = 0; j < game_nb_cols(g1); j++) {
            if (game_get_square(g1, i, j) != game_get_square(g2, i, j))
                return false;
            if (game_get_expected_nb_tents_col(g1, j) !=
                game_get_expected_nb_tents_col(g2, j))
                return false;
        }
    }

    // Checking if options are the same
    if (game_is_diagadj(g1) != game_is_diagadj(g2) ||
        game_is_wrapping(g1) != game_is_wrapping(g2))
        return false;

    return true;
}

void game_delete(game g) {
    // Checking prerequisite
    errorGame(g);

    // Freeing all of the pointers inside of g
    free(g->squareArray);
    free(g->nbTentsRowArray);
    free(g->nbTentsColArray);
    queue_clear_full(g->undoStack, &free);
    queue_free(g->undoStack);
    queue_clear_full(g->redoStack, &free);
    queue_free(g->redoStack);

    // Freeing the pointer g itself
    free(g);

    return;
}

void game_set_square(game g, uint i, uint j, square s) {
    // Checking prerequisites
    errorGame(g);
    insidePoint(i, game_nb_rows(g));
    insidePoint(j, game_nb_cols(g));
    if (s > 3) treat_error("Invalid square\n");

    g->squareArray[(game_nb_cols(g) * i) + j] = s;
    return;
}

square game_get_square(cgame g, uint i, uint j) {
    errorGame(g);
    insidePoint(i, game_nb_rows(g));
    insidePoint(j, game_nb_cols(g));

    return g->squareArray[(game_nb_cols(g) * i + j)];
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) {
    // Checking prerequisites
    errorGame(g);
    insidePoint(i, game_nb_rows(g));

    // Changing the expected number of tents in row i
    g->nbTentsRowArray[i] = nb_tents;
    return;
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents) {
    // Checking prerequisites
    errorGame(g);
    insidePoint(j, game_nb_cols(g));

    // Changing the expected number of tents in col j
    g->nbTentsColArray[j] = nb_tents;
    return;
}

uint game_get_expected_nb_tents_row(cgame g, uint i) {
    // Checking prerequisites
    errorGame(g);
    insidePoint(i, game_nb_rows(g));

    return g->nbTentsRowArray[i];
}

uint game_get_expected_nb_tents_col(cgame g, uint j) {
    // Checking prerequisites
    errorGame(g);
    insidePoint(j, game_nb_cols(g));

    return g->nbTentsColArray[j];
}

uint game_get_expected_nb_tents_all(cgame g) {
    // Checking prerequisites
    errorGame(g);

    uint allExpectedTent = 0;
    for (uint counter = 0; counter < game_nb_rows(g); counter++)
        allExpectedTent += game_get_expected_nb_tents_row(g, counter);

    return allExpectedTent;
}

uint game_get_current_nb_tents_row(cgame g, uint i) {
    // Checking prerequisites
    errorGame(g);
    insidePoint(i, game_nb_rows(g));

    uint nbtents = 0;
    for (uint j = 0; j < game_nb_cols(g); j++) {
        if (game_get_square(g, i, j) == TENT) {
            nbtents++;
        }
    }
    return nbtents;
}

uint game_get_current_nb_tents_col(cgame g, uint j) {
    // Checking prerequisites
    errorGame(g);
    insidePoint(j, game_nb_cols(g));

    uint nbtents = 0;
    for (uint i = 0; i < game_nb_rows(g); i++) {
        if (game_get_square(g, i, j) == TENT) {
            nbtents++;
        }
    }
    return nbtents;
}

uint game_get_current_nb_tents_all(cgame g) {
    // Checking prerequisites
    errorGame(g);

    uint nbtents = 0;
    for (uint i = 0; i < game_nb_rows(g); i++) {
        nbtents += game_get_current_nb_tents_row(g, i);
    }
    return nbtents;
}

void game_play_move(game g, uint i, uint j, square s) {
    // Checking prerequisites
    errorGame(g);
    insidePoint(i, game_nb_rows(g));
    insidePoint(j, game_nb_cols(g));
    if (s > 3) treat_error("Invalid square\n");

    // Exiting function if trying to play on a tree
    if (game_get_square(g, i, j) == TREE) {
        printf("Warning: Tried to play a move on a tree!\n");
        return;
    }

    // Saving the previous move for future undo
    move* movePlayed = malloc(sizeof(move));
    movePlayed->square = game_get_square(g, i, j);
    movePlayed->row = i;
    movePlayed->col = j;
    queue_push_tail(g->undoStack, movePlayed);
    queue_clear_full(g->redoStack, &free);

    // Playing the move
    game_set_square(g, i, j, s);
    return;
}

int game_check_move(cgame g, uint i, uint j, square s) {
    errorGame(g);

    if (game_get_square(g, i, j) == TREE || s == TREE) return ILLEGAL;

    // Checking if putting a grass square makes it impossible to win or not
    if (s == GRASS) {
        int emptySquaresLeft = 0;

        // Checking row i
        for (int k = 0; k < DEFAULT_SIZE; k++) {
            if (k != i && game_get_square(g, i, k) == EMPTY) emptySquaresLeft++;
        }
        if (game_get_expected_nb_tents_row(g, i) > emptySquaresLeft)
            return LOSING;

        // Checking col j
        emptySquaresLeft = 0;
        for (int k = 0; k < DEFAULT_SIZE; k++) {
            if (k != j && game_get_square(g, k, j) == EMPTY) emptySquaresLeft++;
        }
        if (game_get_expected_nb_tents_col(g, j) > emptySquaresLeft)
            return LOSING;
    }

    // Checking for tent rules
    if (s == TENT) {
        // Rule 1 + 4

        bool hasTree = false;
        // Checking the row before and after row i for diagonal + orthogonal
        // check
        int i_test = i - 1;
        for (int i_count = 0; i_count < 3; i_test++, i_count++) {
            // Checking if the row doesn't exist
            bool rowExists = (i_test >= 0 && i_test < game_nb_rows(g));
            if (rowExists || game_is_wrapping(g)) {
                // Checking the col before and after row i for diagonal +
                // orthogonal check
                if (!rowExists) {
                    i_test = i_test % game_nb_rows(g);
                }
                int j_test = j - 1;
                for (int j_count = 0; j_count < 3; j_count++, j_test++) {
                    // Checking if the col doesn't exist
                    bool colExists = j_test >= 0 && j_test < game_nb_cols(g);
                    if (colExists || game_is_wrapping(g)) {
                        if (!colExists) {
                            j_test = j_test % game_nb_cols(g);
                        }
                        // Do not do the check the square of the move
                        if (i_test != i || j_test != j) {
                            // Rule 1
                            if (game_get_square(g, i_test, j_test) == TENT)
                                return LOSING;
                            // Rule 4
                            if (game_is_diagadj(g)) {
                                if (game_get_square(g, i_test, j_test) == TREE)
                                    hasTree = true;
                            } else {
                                if ((i_test == i || j_test == j) &&
                                    game_get_square(g, i_test, j_test) == TREE)
                                    hasTree = true;
                            }
                        }
                    }
                }
            }
        }
        // No tree orthogonally
        if (hasTree == false) return LOSING;

        // The 2 following checks assume the player isn't playing a tent on a
        // tent, however if it is the case, then the checks would count 1 tent
        // when it shouldn't (which may result in false positives). The int
        // tentNotPlaced counteracts this issue
        uint tentNotPlaced = 1;
        if (game_get_square(g, i, j) == TENT) {
            tentNotPlaced = 0;
        }

        // Rule 2
        if ((game_get_current_nb_tents_row(g, i) + tentNotPlaced >
             game_get_expected_nb_tents_row(g, i)) ||
            (game_get_current_nb_tents_col(g, j) + tentNotPlaced >
             game_get_expected_nb_tents_col(g, j)))
            return LOSING;

        // Rule 3
        if (game_get_current_nb_tents_all(g) + tentNotPlaced >
            game_get_expected_nb_tents_all(g))
            return LOSING;
    }

    return REGULAR;
}

bool game_is_over(cgame g) {
    // These 2 variables are used to make sure every tree has at least a tent
    // next to it (rule 4 bis) and that there are as many trees as tents They
    // are used and incremented in the big loop down under.
    uint treeCount = 0;
    bool hasTent;

    for (int i = 0; i < game_nb_rows(g); i++) {
        for (int j = 0; j < game_nb_cols(g); j++) {
            // Checking if every tent placed was not a LOSING move. If at least
            // one LOSING move was done, it means the game is not over.
            if (game_get_square(g, i, j) == TENT) {
                if (game_check_move(g, i, j, TENT) == LOSING) {
                    return false;
                }
            }

            // Checking every single tree for rule 4 bis, and counting them for
            // rule 3.
            if (game_get_square(g, i, j) == TREE) {
                treeCount++;
                hasTent = false;
                int i_test = i - 1;
                for (int i_count = 0; i_count < 3; i_test++, i_count++) {
                    // Checking if the row doesn't exist
                    bool rowExists = (i_test >= 0 && i_test < game_nb_rows(g));
                    if (rowExists || game_is_wrapping(g)) {
                        // Checking the col before and after row i for diagonal
                        // + orthogonal check
                        if (!rowExists) {
                            i_test = i_test % game_nb_rows(g);
                        }
                        int j_test = j - 1;
                        for (int j_count = 0; j_count < 3;
                             j_count++, j_test++) {
                            // Checking if the col doesn't exist
                            bool colExists =
                                j_test >= 0 && j_test < game_nb_cols(g);
                            if (colExists || game_is_wrapping(g)) {
                                if (!colExists) {
                                    j_test = j_test % game_nb_cols(g);
                                }
                                // Do not do the check the square of the move
                                if (i_test != i || j_test != j) {
                                    if (game_is_diagadj(g)) {
                                        if (game_get_square(g, i_test,
                                                            j_test) == TENT)
                                            hasTent = true;
                                    } else {
                                        if ((i_test == i || j_test == j) &&
                                            game_get_square(g, i_test,
                                                            j_test) == TENT)
                                            hasTent = true;
                                    }
                                }
                            }
                        }
                    }
                }
                if (hasTent == false) {
                    return false;
                }
            }
        }
    }

    // 2nd && 3rd rules
    for (uint c = 0; c < DEFAULT_SIZE; c++) {
        if (game_get_expected_nb_tents_row(g, c) !=
                game_get_current_nb_tents_row(g, c) ||
            game_get_expected_nb_tents_col(g, c) !=
                game_get_current_nb_tents_col(g, c) ||
            game_get_expected_nb_tents_all(g) !=
                game_get_current_nb_tents_all(g) ||
            game_get_current_nb_tents_all(g) != treeCount) {
            return false;
        }
    }
    return true;
}

void game_fill_grass_row(game g, uint i) {
    // Checking prerequisites
    errorGame(g);
    insidePoint(i, game_nb_rows(g));

    for (uint j = 0; j < game_nb_cols(g); j++) {
        if (game_get_square(g, i, j) == EMPTY) {
            game_play_move(g, i, j, GRASS);
        }
    }
    return;
}

void game_fill_grass_col(game g, uint j) {
    // Checking prerequisites
    errorGame(g);
    insidePoint(j, game_nb_cols(g));

    for (uint i = 0; i < game_nb_rows(g); i++) {
        if (game_get_square(g, i, j) == EMPTY) {
            game_play_move(g, i, j, GRASS);
        }
    }
    return;
}

void game_restart(game g) {
    // Checking prerequisites
    errorGame(g);

    // Undoing every move
    while (!queue_is_empty(g->undoStack)) {
        game_undo(g);
    }
    queue_clear_full(g->redoStack, &free);

    // Resetting every square that isn't a tree
    for (int i = 0; i < game_nb_rows(g); i++) {
        for (int j = 0; j < game_nb_cols(g); j++) {
            if (game_get_square(g, i, j) != TREE) {
                game_set_square(g, i, j, EMPTY);
            }
        }
    }
}
