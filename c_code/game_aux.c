#include "../h_code/game_aux.h"
#include <stdio.h>
#include <stdlib.h>
#include "../h_code/game_ext.h"
#include "../h_code/struct.h"

void game_print(cgame g) {
    if (g == NULL) {
        fprintf(stderr,
                "ERROR: game pointer in \"game_print\" function is invalid!\n "
                "Exiting program.");
        exit(EXIT_FAILURE);
    }
    printf("   ");
    for (int i = 0; i < g->grid.nb_cols; i++) {
        printf("%d", i);
    }
    printf("\n   ");
    for (int i = 0; i < g->grid.nb_cols; i++) {
        printf("-");
    }

    char charArray[] = {' ', 'X', '*', '-'};
    for (int i = 0; i < g->grid.nb_rows; i++) {
        printf("\n%d |", i);
        for (int j = 0; j < g->grid.nb_cols; j++) {
            printf("%c", charArray[game_get_square(g, i, j)]);
        }
        printf("| %d", game_get_expected_nb_tents_row(g, i));
    }
    printf("\n   ");
    for (int i = 0; i < g->grid.nb_cols; i++) {
        printf("-");
    }
    printf("\n   ");
    for (int i = 0; i < g->grid.nb_cols; i++) {
        printf("%d", game_get_expected_nb_tents_col(g, i));
    }
    printf("\n");
}

game game_v2_test(void) {
    uint row = 4;
    uint col = 4;

    uint row_tents[] = {2, 0, 1, 1};
    uint col_tents[] = {1, 1, 1, 1};

    // Initialisation de la grille
    square squares[row * col];
    int default_trees[] = {1, 3, 5, 7, 12, 14};
    int last_tree_index = 0;
    for (int i = 0; i < (row * col); i++) {
        if (i == default_trees[last_tree_index]) {
            squares[i] = TREE;
            last_tree_index++;
        } else {
            squares[i] = EMPTY;
        }
    }

    // Initialisation de g
    return game_new_ext(row, col, squares, row_tents, col_tents, true, false);
}

game game_default(void) {
    // Initialisation du nombre de tentes attendues
    uint row_tents[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
    uint col_tents[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};

    // Initialisation de la grille
    square squares[DEFAULT_SIZE * DEFAULT_SIZE];
    int default_trees[12] = {4, 5, 8, 15, 20, 24, 29, 33, 40, 44, 46, 56};
    int last_tree_index = 0;
    for (int i = 0; i < (DEFAULT_SIZE * DEFAULT_SIZE); i++) {
        if (i == default_trees[last_tree_index]) {
            squares[i] = TREE;
            last_tree_index++;
        } else {
            squares[i] = EMPTY;
        }
    }

    // Initialisation de game
    game g = game_new(squares, row_tents, col_tents);
    return g;
}

game game_default_solution(void) {
    // Initialisation du nombre de tentes attendues
    uint row_tents[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
    uint col_tents[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};

    // Initialisation de la grille
    square squares[DEFAULT_SIZE * DEFAULT_SIZE];
    int default_trees[12] = {4, 5, 8, 15, 20, 24, 29, 33, 40, 44, 46, 56};
    int last_tree_index = 0;
    int default_tents[12] = {0, 3, 6, 16, 19, 21, 23, 32, 34, 36, 38, 48};
    int last_tent_index = 0;
    for (int i = 0; i < (DEFAULT_SIZE * DEFAULT_SIZE); i++) {
        if (i == default_trees[last_tree_index]) {
            squares[i] = TREE;
            last_tree_index++;
        } else if (i == default_tents[last_tent_index]) {
            squares[i] = TENT;
            last_tent_index++;
        } else {
            squares[i] = GRASS;
        }
    }

    // Initialisation de game
    return game_new(squares, row_tents, col_tents);
}
