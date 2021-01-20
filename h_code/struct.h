
/**
 * @file struct.h
 * @brief All structs used within the program.
 * @details See @ref index for further details.
 *
 **/

#ifndef __STRUCT_H__
#define __STRUCT_H__
#include <stdbool.h>
#include "queue.h"
/**
 * @brief  Different kinds of option wrapping and diagadj used in the game.
 */
struct Option {
    bool wrapping;
    bool diagadj;
};
typedef struct Option* Option;

/**
 * @brief  Different kinds of grid used in the game.
 */
struct Grid {
    uint nb_rows;
    uint nb_cols;
    uint size;
};
typedef struct Grid* Grid;

/**
 * @brief  Structure used to store the game state.
 */
struct game_s {
    square* squareArray;  // DEFAULT_SIZE**
    uint* nbTentsRowArray;
    uint* nbTentsColArray;
    struct Grid grid;
    struct Option option;
    queue* undoStack;
    queue* redoStack;
};

/**
 * @brief  Structure used to store the state of a square for undo/redo.
 */
typedef struct move_s {
    square square;
    uint row;
    uint col;
} move;

#endif  // __STRUCT_H__
