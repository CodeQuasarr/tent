#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../h_code/game.h"
#include "../h_code/game_aux.h"
#include "../h_code/game_ext.h"
#include "../h_code/struct.h"

/* ********** TEST GAME DEFAULT ********** */

bool test_game_default(void) {
    game myGame = game_default();
    game testGame = game_new_empty();

    if (myGame == NULL || myGame == testGame) {
        fprintf(stderr, "Error: invalid delete!\n");
        return false;
    }
    //  we test if the default set contains only trees
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        for (uint j = 0; j < DEFAULT_SIZE; j++) {
            if (game_get_square(myGame, i, j) == TENT ||
                game_get_square(myGame, i, j) == GRASS) {
                fprintf(stderr, "invalide content game default \n");
                game_delete(myGame);
                game_delete(testGame);
                return false;
            }
        }
        //  we test whether the number of tents per column and per row in the
        //  grid is properly initialized
        uint nb_col = game_get_expected_nb_tents_col(myGame, i);
        uint nb_row = game_get_expected_nb_tents_row(myGame, i);
        if (nb_row < 0 || nb_col < 0 || nb_row > DEFAULT_SIZE ||
            nb_col > DEFAULT_SIZE) {
            fprintf(stderr, "invalide content game default col row \n");
            printf("nb_row = %u   nb_col = %u \n", nb_row, nb_col);
            game_delete(myGame);
            game_delete(testGame);
            return false;
        }
    }
    game_delete(myGame);
    game_delete(testGame);
    return true;
}

/* ********** TEST GAME EQUAL ********** */

bool test_game_equal() {
    game g1 = game_new_empty_ext(4, 4, true, false);
    game g2 = game_copy(g1);

    if (game_nb_cols(g1) != game_nb_cols(g2) ||
        game_nb_rows(g2) != game_nb_rows(g1)) {
        return false;
    }

    if (game_is_wrapping(g1) != game_is_wrapping(g2) ||
        game_is_diagadj(g2) != game_is_diagadj(g1)) {
        return false;
    }

    for (uint i = 0; i < game_nb_rows(g1); i++) {
        if (game_get_expected_nb_tents_row(g1, i) !=
            game_get_expected_nb_tents_row(g2, i)) {
            return false;
        }

        for (uint j = 0; j < game_nb_cols(g1); j++) {
            if (game_get_square(g1, i, j) != game_get_square(g1, i, j)) {
                return false;
            }
            if (game_get_expected_nb_tents_col(g1, j) !=
                game_get_expected_nb_tents_col(g2, j)) {
                return false;
            }
        }
    }

    game_set_square(g1, 0, 0, TENT);
    game_set_square(g1, 2, 0, TENT);
    if (game_equal(g1, g2)) {
        fprintf(stderr, "g1 and g2 can't be equals\n");
        return false;
    }

    if (g1 == NULL || g2 == NULL) {
        fprintf(stderr, "Error: invalid delete!\n");
        return false;
    }
    game_set_square(g2, 0, 0, TENT);
    game_set_square(g2, 2, 0, TENT);
    if (!game_equal(g1, g2)) {
        fprintf(stderr, "Error: Games are not equals!\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }
    game_delete(g1);
    game_delete(g2);
    return true;
}

/* ********** TEST GAME DELETE ********** */

bool test_game_delete() {
    // Initialisation de game
    game g = game_default_solution();
    if (g != NULL) {
        game_delete(g);
    }
    return true;
}

/* ********** TEST GAME GET SQUARE ********** */

bool test_game_get_square(uint i, uint j) {
    //  we test if the values ​​can be contained in the grid
    if (i > DEFAULT_SIZE - 1 || j > DEFAULT_SIZE - 1 || i < 0 ||
        j < 0)  // test if
    {
        fprintf(stderr, "Error: coordinates are not in the grid -> (%u, %u)\n",
                i, j);
        return false;
    }
    // we create a new game
    uint row_tents[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
    uint col_tents[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
    square squares[DEFAULT_SIZE * DEFAULT_SIZE];

    for (uint x = 0; x < (DEFAULT_SIZE * DEFAULT_SIZE); x++) squares[x] = EMPTY;

    game g = game_new(squares, row_tents, col_tents);
    // we want to test if the game_get_square returns exactly the same value of
    // a cell with a given coordinate
    game_set_square(g, i, j, TENT);
    square s = game_get_square(g, i, j);
    if (s != 2)  // we hope that the value of a box is not outside our imposed
                 // framework
    {
        fprintf(stderr,
                "Error: there is a foreign value compared to what is heard\n");
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

/* ********** TEST GET EXPECTED NB TENTS ROW ********** */

bool test_get_expected_nb_tents_row(uint i) {
    game g = game_default();
    if (g == NULL) {
        fprintf(stderr, "Error: invalid delete!\n");
        return false;
    }

    //  at a certain line we test to see if the number of attempts that can
    //  contain this line is reasonable
    uint s = game_get_expected_nb_tents_row(g, i);
    if (s != 3 || s > DEFAULT_SIZE - 1) {
        fprintf(stderr,
                "Error: the expected number is not what you are showing! .\n");
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

/* ********** TEST GET EXPECTED NB TENTS ROW ********** */

bool test_get_expected_nb_tents_col(uint j) {
    game g = game_default();
    if (g == NULL) {
        fprintf(stderr, "Error: invalid delete!\n");
        return false;
    }
    //  at a certain colum we test to see if the number of attempts that can
    //  contain this line is reasonable
    uint s = game_get_expected_nb_tents_col(g, j);
    if (s != 4 || s > DEFAULT_SIZE - 1) {
        fprintf(stderr,
                "Error: the expected number is not what you are showing! .\n");
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}
/* **************************** TEST_OTHER_GAMES *****************************
 */
bool test_game_get_current_nb_tents_row(uint i) {
    if (i < 0 || i > DEFAULT_SIZE - 1) {
        fprintf(stderr, "ERROR: INVALID ROW \n");
        return false;
    }
    square square_array[64] = {
        TENT,  GRASS, GRASS, TENT,  TREE,  TREE,  TENT,  GRASS, TREE,  GRASS,
        GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  TENT,  GRASS, GRASS, TENT,
        TREE,  TENT,  GRASS, TENT,  TREE,  GRASS, GRASS, GRASS, GRASS, TREE,
        GRASS, GRASS, TENT,  TREE,  TENT,  GRASS, TENT,  GRASS, TENT,  GRASS,
        TREE,  GRASS, GRASS, GRASS, TREE,  GRASS, TREE,  GRASS, TENT,  GRASS,
        GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS,
        GRASS, GRASS, GRASS, GRASS};
    uint column_array[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
    uint row_array[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
    game g = game_new(square_array, column_array, row_array);

    uint count = 0;
    for (uint x = (DEFAULT_SIZE * (i + 1));
         x > ((DEFAULT_SIZE * (i + 1)) - DEFAULT_SIZE); x--)
        if (square_array[x - 1] == TENT)  //|| square_array[x-1] == GRASS
            count++;
    if (count != game_get_current_nb_tents_row(g, i)) {
        fprintf(stderr, "ERROR: INVALIDE TENT IN THE ROW (%u)", i);
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

bool test_game_get_current_nb_tents_col(uint j) {
    if (j < 0 || j > DEFAULT_SIZE - 1) {
        fprintf(stderr, "ERROR: INVALID ROW \n");
        return false;
    }
    square square_array[64] = {
        TENT,  GRASS, GRASS, TENT,  TREE,  TREE,  TENT,  GRASS, TREE,  GRASS,
        GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  TENT,  GRASS, GRASS, TENT,
        TREE,  TENT,  GRASS, TENT,  TREE,  GRASS, GRASS, GRASS, GRASS, TREE,
        GRASS, GRASS, TENT,  TREE,  TENT,  GRASS, TENT,  GRASS, TENT,  GRASS,
        TREE,  GRASS, GRASS, GRASS, TREE,  GRASS, TREE,  GRASS, TENT,  GRASS,
        GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS,
        GRASS, GRASS, GRASS, GRASS};
    uint column_array[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
    uint row_array[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
    game g = game_new(square_array, column_array, row_array);
    uint count = 0;
    for (uint y = j + 1; y < ((DEFAULT_SIZE * DEFAULT_SIZE));
         y = y + DEFAULT_SIZE)
        if (square_array[y - 1] == TENT)  // || square_array[y-1] == GRASS
            count++;

    if (count != game_get_current_nb_tents_col(g, j)) {
        fprintf(stderr, "ERROR: INVALIDE TENT IN THE ROW (%u)", j);
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

bool test_play_move() {
    game g = game_default();
    bool valide_1 = true;
    bool valide_2 = true;
    bool valide_3 = true;

    game_play_move(g, 0, 0, TENT);
    if (game_get_square(g, 0, 0) == EMPTY || game_get_square(g, 0, 0) == TREE ||
        game_get_square(g, 0, 0) == GRASS)
        valide_1 = false;
    game_play_move(g, 0, 1, GRASS);
    if (game_get_square(g, 0, 1) == EMPTY || game_get_square(g, 0, 1) == TREE ||
        game_get_square(g, 0, 1) == TENT)
        valide_2 = false;
    game_play_move(g, 0, 1, EMPTY);
    if (game_get_square(g, 0, 1) == TENT || game_get_square(g, 0, 1) == TREE ||
        game_get_square(g, 0, 1) == GRASS)
        valide_3 = false;

    if (!(valide_1) || (!(valide_2)) || (!(valide_3))) {
        fprintf(stderr, "ERROR: PLAY MOVE FAILED.\n");
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

bool test_game_is_over() {
    // Correction d'une fuite mémoire
    game game_solution = game_default_solution();

    /* creation of the game grid already resolved, an 8x8 board */
    square square_array[64] = {
        TENT,  GRASS, GRASS, TENT,  TREE,  TREE,  TENT,  GRASS, TREE,  GRASS,
        GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  TENT,  GRASS, GRASS, TENT,
        TREE,  TENT,  GRASS, TENT,  TREE,  GRASS, GRASS, GRASS, GRASS, TREE,
        GRASS, GRASS, TENT,  TREE,  TENT,  GRASS, TENT,  GRASS, TENT,  GRASS,
        TREE,  GRASS, GRASS, GRASS, TREE,  GRASS, TREE,  GRASS, TENT,  GRASS,
        GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS,
        GRASS, GRASS, GRASS, GRASS};
    uint column_array[DEFAULT_SIZE] = {3, 0, 4, 0, 4, 0, 1, 0};
    uint row_array[DEFAULT_SIZE] = {4, 0, 1, 2, 1, 1, 2, 1};
    game g = game_new(square_array, column_array, row_array);
    uint tree_counter = 0;
    uint empty_counter = 0;
    if (g == NULL) {
        fprintf(stderr, "Error: pointeur!\n");
        return false;
    }
    if (game_is_over(game_solution) != game_is_over(g)) {
        fprintf(stderr, "Error: GAME CAN'T BE OVER!\n");
        game_delete(g);
        return false;
    }
    for (int i = 0; i < (DEFAULT_SIZE * DEFAULT_SIZE); i++) {
        if (square_array[i] == TREE) tree_counter++;
        if (square_array[i] == EMPTY) empty_counter++;
    }

    if (tree_counter != game_get_expected_nb_tents_all(g)) {
        fprintf(stderr,
                "Error: tree_counter must be equal to "
                "game_get_expected_nb_tents_all\n");
        game_delete(g);
        return false;
    }

    if ((game_get_square(g, 0, 3) == game_get_square(g, 1, 3)) ||
        (game_get_square(g, 0, 0) == game_get_square(g, 0, 1))) {
        fprintf(stderr, "Error: position adjacent\n");
        game_delete(g);
        return false;
    }
    game_delete(g);
    game_delete(game_solution);
    return true;
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
    fprintf(stderr, "Usage: %s game_test_dsnyaronke_dummy failed\n", argv[0]);
    exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {
    if (argc != 2) usage(argc, argv);

    // start test
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool valide = false;

    if (strcmp(argv[1], "dummy") == 0)
        valide = true;

    else if (strcmp(argv[1], "game_default") == 0)
        valide = test_game_default();

    else if (strcmp(argv[1], "game_delete") == 0)
        valide = test_game_delete();

    else if (strcmp(argv[1], "get_square") == 0)
        valide = test_game_get_square(2, 1);

    else if (strcmp(argv[1], "game_equal") == 0)
        valide = test_game_equal();

    else if (strcmp(argv[1], "get_expected_nb_tents_row") == 0)
        valide = test_get_expected_nb_tents_row(0);

    else if (strcmp(argv[1], "get_expected_nb_tents_col") == 0)
        valide = test_get_expected_nb_tents_col(0);

    /******************otherthes **********************/

    else if (strcmp(argv[1], "game_get_current_nb_tents_row") == 0)
        valide = test_game_get_current_nb_tents_row(5);

    else if (strcmp(argv[1], "game_get_current_nb_tents_col") == 0)
        valide = test_game_get_current_nb_tents_col(5);

    else if (strcmp(argv[1], "play_move") == 0)
        valide = test_play_move();

    else if (strcmp(argv[1], "game_is_over") == 0)
        valide = test_game_is_over();

    else {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // print test result
    if (valide) {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}