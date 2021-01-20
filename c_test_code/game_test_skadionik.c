#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../h_code/game.h"
#include "../h_code/game_aux.h"

void usage(char* name) { fprintf(stderr, "Usage: %s test_name\n", name); }

//*------------------TEST GAME_PRINT------------------*

// We just call the function to check for segmentation faults/memory leaks
bool test_game_print() {
    game g = game_new_empty();
    game_print(g);
    game_delete(g);
    return true;
}

//*------------------TEST GAME_NEW------------------*

/*This test function test for the following:
- invalid pointer returned
- incorrect initialization of squares
- incorrect initialization of expected tents
*/
bool test_game_new() {
    unsigned int squares_size = DEFAULT_SIZE * DEFAULT_SIZE;
    square squares[squares_size];

    // Fill array with EMPTY
    for (int i = 0; i < squares_size; i++) {
        squares[i] = EMPTY;
    }
    // Fill first row with TREE
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        squares[i] = TREE;
    }

    // Creating both 'nb_tents_***' array
    unsigned int tents_row[DEFAULT_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7};
    unsigned int tents_col[DEFAULT_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7};

    game g = game_new(squares, tents_row, tents_col);

    // Testing returned pointer
    if (g == NULL) {
        fprintf(stderr,
                "Error: Invalid game pointer returned by \'game_new\'!\n");
        return false;
    }

    // Testing initialization of squares
    for (int j = 0; j < DEFAULT_SIZE; j++) {
        if (game_get_square(g, 0, j) != TREE) {
            fprintf(stderr,
                    "Error: \'game_new\' didn't initialize TREE squares "
                    "correctly!\n");
            return false;
        }
    }

    for (int i = 1; i < DEFAULT_SIZE; i++) {
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            if (game_get_square(g, i, j) != EMPTY) {
                fprintf(stderr,
                        "Error: \'game_new\' didn't initialize EMPTY squares "
                        "correctly!\n");
                return false;
            }
        }
    }

    // Testing initialization of expected tents

    for (int i = 0; i < DEFAULT_SIZE; i++) {
        if (game_get_expected_nb_tents_row(g, i) != i) {
            fprintf(stderr,
                    "Error: \'game_new\' didn't initialize expected_tents_row "
                    "correctly!\n");
            return false;
        }
        if (game_get_expected_nb_tents_col(g, i) != i) {
            fprintf(stderr,
                    "Error: \'game_new\' didn't initialize expected_tents_col "
                    "correctly!\n");
            return false;
        }
    }

    game_delete(g);

    // Tests passed!

    return true;
}

//*------------------TEST GAME_SET_EXPECTED_NB_TENTS_COL------------------*

/*This test function tests for the following:
- Row/column index inversion
- Incorrect column set
- Incorrect number set
*/
bool test_game_set_expected_nb_tents_col() {
    unsigned int squares_size = DEFAULT_SIZE * DEFAULT_SIZE;
    square squares[squares_size];

    // Fill array with EMPTY
    for (int i = 0; i < squares_size; i++) {
        squares[i] = EMPTY;
    }

    unsigned int tents_row[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int tents_col[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};

    game g = game_new(squares, tents_row, tents_col);

    // Check for invalid game pointer
    if (g == NULL) {
        fprintf(stderr,
                "Error: Invalid game pointer in "
                "\'test_game_set_expected_nb_tents_col\'!\n");
        return false;
    }

    // Test for row/column inversion
    game_set_expected_nb_tents_col(g, 0, 5);

    if (game_get_expected_nb_tents_row(g, 0) != 0) {
        fprintf(stderr,
                "Error: \'game_set_expected_nb_tents_col\' modified a row "
                "instead of a column!\n");
        return false;
    }

    if (game_get_expected_nb_tents_col(g, 0) != 5) {
        fprintf(stderr,
                "Error: \'game_set_expected_nb_tents_col\' didn't change "
                "correctly the number of expected tents!\n");
        return false;
    }

    // Test for incorrect value/column checked

    for (int j = 0; j < DEFAULT_SIZE; j++) {
        game_set_expected_nb_tents_col(g, j, j);
        if (game_get_expected_nb_tents_col(g, j) != j) {
            fprintf(stderr,
                    "Error: \'game_set_expected_nb_tents_col\' didn't change "
                    "correctly the number of expected tents!\n");
            fprintf(stderr, "Expected value: %i; returned value: %i\n", j,
                    game_get_expected_nb_tents_col(g, j));
            return false;
        }
    }

    // Tests passed!
    game_delete(g);
    return true;
}

//*------------------TEST GAME_GET_CURRENT_NB_TENTS_COL------------------*

/*This test function tests for the following:
- Row/column index inversion
- Incorrect column check
- Incorrect expected number returned
- Counting squares that aren't tents
*/
bool test_game_get_current_nb_tents_col() {
    unsigned int squares_size = DEFAULT_SIZE * DEFAULT_SIZE;
    square squares[squares_size];

    // Fill array with EMPTY
    for (int i = 0; i < squares_size; i++) {
        squares[i] = EMPTY;
    }

    // Adding tents
    // Each column has j tent, the Nth tent of each column is always on the Nth
    // row
    for (int j = 0; j < DEFAULT_SIZE; j++) {
        for (int i = 0; i < j; i++) {
            squares[j + DEFAULT_SIZE * i] = TENT;
        }
    }

    unsigned int tents_row[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int tents_col[DEFAULT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};

    game g = game_new(squares, tents_row, tents_col);

    // Check for invalid game pointer
    if (g == NULL) {
        fprintf(stderr,
                "Error: Invalid game pointer in "
                "\'test_game_get_current_nb_tents_col\'!\n");
        return false;
    }

    // Adding TREE and GRASS
    game_set_square(g, 1, 0, TREE);
    game_set_square(g, 2, 1, GRASS);

    // test for row/column inversion
    if (game_get_current_nb_tents_col(g, 0) == 7) {
        fprintf(stderr,
                "Error: \'game_get_current_nb_tents_col\' checked a row "
                "instead of a column!\n");
        return false;
    }

    // test for incorrect value/column checked
    for (int j = 0; j < DEFAULT_SIZE; j++) {
        if (game_get_current_nb_tents_col(g, j) != j) {
            fprintf(stderr,
                    "Error: \'game_get_current_nb_tents_col\' didn't return "
                    "the expected value!\n");
            fprintf(stderr, "Expected value: %i; returned value: %i\n", j,
                    game_get_current_nb_tents_col(g, j));
            return false;
        }
    }

    // tests passed!
    game_delete(g);
    return true;
}

//*------------------TEST GAME_CHECK_MOVE------------------*

/*This test function tests for the following:
- Undetected ILLEGAL moves
- Undetected LOSING moves (broken rule + move making it impossible to solve the
puzzle)
- False positive ILLEGAL/LOSING move
*/
bool test_game_check_move() {
    game g = game_new_empty();

    if (g == NULL) {
        fprintf(stderr,
                "Error: Invalid game pointer in \'test_game_check_move\'!\n");
        return false;
    }

    // Testing ILLEGAL moves:
    // Attempting to place a tree

    if (game_check_move(g, 0, 0, TREE) != ILLEGAL) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'ILLEGAL' when "
                "trying to place a TREE!\n");
        return false;
    }

    // Attempting to override a tree

    game_set_square(g, 1, 0, TREE);
    if (game_check_move(g, 1, 0, TENT) != ILLEGAL) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'ILLEGAL' when "
                "trying to override a TREE with a TENT!\n");
        return false;
    }

    // Testing LOSING moves:
    // Rule 1 test: No two tents are adjacent, even diagonally.

    // Preparing the board to make sure only rule 1 is broken

    for (int i = 0; i < 3; i++) {
        game_set_square(g, 0, 1 + i, TREE);
        game_set_square(g, 4, 1 + i, TREE);
        game_set_square(g, 1 + i, 0, TREE);
        game_set_square(g, 1 + i, 4, TREE);
        game_set_expected_nb_tents_row(g, 1 + i, 3);
        game_set_expected_nb_tents_col(g, 1 + i, 3);
    }
    game_set_square(g, 2, 2, TENT);

    // Testing
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j != 1 && i != 1) {
                if (game_check_move(g, 1 + i, 1 + j, TENT) != LOSING) {
                    fprintf(stderr,
                            "Error: \'game_check_move\' didn't return 'LOSING' "
                            "when breaking Rule 1)!\n");
                    return false;
                }
            }
        }
    }

    game_delete(g);

    game g2 = game_default();

    if (g2 == NULL) {
        fprintf(stderr,
                "Error: Invalid game pointer in \'test_game_check_move\'!\n");
        return false;
    }

    // Rule 2 test: The number of tents in each row, and in each column, matches
    // the expected numbers given around the sides of the grid.

    if (game_check_move(g2, 0, 1, TENT) != LOSING) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'LOSING' when "
                "breaking Rule 2)!\n");
        return false;
    }

    if (game_check_move(g2, 7, 7, TENT) != LOSING) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'LOSING' when "
                "breaking Rule 2)!\n");
        return false;
    }

    if (game_check_move(g2, 7, 1, TENT) != LOSING) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'LOSING' when "
                "breaking Rule 2)!\n");
        return false;
    }

    // Rule 3 test: There are exactly as many tents as trees.

    for (int j = 2; j < 8; j++) {
        game_set_square(g2, 6, j, TENT);
        game_set_square(g2, 7, j, TENT);
    }

    if (game_check_move(g2, 0, 0, TENT) != LOSING) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'LOSING' when "
                "breaking Rule 3)!\n");
        return false;
    }

    game_delete(g2);
    game g3 = game_default();

    // Rule 4 test: Each tent must be orthogonally adjacent (horizontally or
    // vertically, but not diagonally) to at least one tree and vice versa.

    if (game_check_move(g3, 0, 2, TENT) != LOSING) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'LOSING' when "
                "breaking Rule 4)!\n");
        return false;
    }

    if (game_check_move(g3, 7, 2, TENT) != LOSING) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'LOSING' when "
                "breaking Rule 4)!\n");
        return false;
    }

    // Extended rule test: if you place a grass square that prevents you from
    // placing enough tent in a row/column, it is a LOSING move

    if (game_check_move(g3, 0, 0, GRASS) != LOSING) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'LOSING' when "
                "breaking the Extended rule!\n");
        return false;
    }

    // Testing for false positive: Checking the moves required to win the
    // default game

    if (game_check_move(g3, 0, 0, TENT) != REGULAR) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'REGULAR' when "
                "playing a correct move!\n");
        return false;
    }

    if (game_check_move(g3, 0, 6, TENT) != REGULAR) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'REGULAR' when "
                "playing a correct move!\n");
        return false;
    }

    if (game_check_move(g3, 2, 3, TENT) != REGULAR) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'REGULAR' when "
                "playing a correct move!\n");
        return false;
    }

    if (game_check_move(g3, 4, 2, TENT) != REGULAR) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'REGULAR' when "
                "playing a correct move!\n");
        return false;
    }

    if (game_check_move(g3, 6, 0, TENT) != REGULAR) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'REGULAR' when "
                "playing a correct move!\n");
        return false;
    }

    // Testing for false positives: Checking a few REGULAR grass moves

    if (game_check_move(g3, 0, 1, GRASS) != REGULAR) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'REGULAR' when "
                "playing a correct move!\n");
        return false;
    }

    if (game_check_move(g3, 1, 1, GRASS) != REGULAR) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'REGULAR' when "
                "playing a correct move!\n");
        return false;
    }

    if (game_check_move(g3, 6, 7, GRASS) != REGULAR) {
        fprintf(stderr,
                "Error: \'game_check_move\' didn't return 'REGULAR' when "
                "playing a correct move!\n");
        return false;
    }

    // Tests passed!
    game_delete(g3);
    return true;
}

//*------------------TEST GAME_FILL_GRASS_COL------------------*

/*This test function test for the following:
- Row/column inversion
- Didn't fill/Filled with wrong square
- Tree and Tent overrides
- Empty square not filled
*/
bool test_game_fill_grass_col() {
    // Initializing game default
    game g = game_default();
    if (g == NULL) {
        fprintf(
            stderr,
            "Error: Invalid game pointer in \'test_game_fill_grass_col\'!\n");
        return false;
    }

    // We put a Tent in square (0,0) to check if tents are not erased by
    // function
    game_set_square(g, 0, 0, TENT);

    // Testing for inversion of columns and rows indexes
    game_fill_grass_col(g, 0);
    if (game_get_square(g, 0, 1) == GRASS) {
        fprintf(stderr,
                "Error: \'game_fill_grass_col\' filled a row instead of a "
                "column!\n");
        return false;
    }

    // Testing for wrong square filled
    if (game_get_square(g, 2, 0) != GRASS) {
        fprintf(stderr,
                "Error: \'game_fill_grass_col\' didn't fill a column or put "
                "the wrong square!\n");
        fprintf(stderr, "Error: Index of square: %i\n",
                game_get_square(g, 2, 0));
        return false;
    }

    // Filling the entire board
    for (int j = 0; j < DEFAULT_SIZE; j++) {
        game_fill_grass_col(g, j);
    }

    // Square override test
    if (game_get_square(g, 0, 0) == GRASS) {
        fprintf(stderr,
                "Error: \'game_fill_grass_col\' overrided TENT square!\n");
        return false;
    }

    if (game_get_square(g, 0, 4) == GRASS) {
        fprintf(stderr,
                "Error: \'game_fill_grass_col\' overrided TREE square!\n");
        return false;
    }

    // Empty square remaining test

    for (int i = 0; i < DEFAULT_SIZE; i++) {
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            if (game_get_square(g, i, j) == EMPTY) {
                fprintf(
                    stderr,
                    "Error: \'game_fill_grass_col\' left an empty square!\n");
                return false;
            }
        }
    }

    game_delete(g);

    // Tests passed!

    return true;
}

//*------------------TEST GAME_EQUAL------------------*

/*This test function test for the following:
- False negative: function says equal games are not equal
- False positive: function says different games are equal
In this case, the checked differences are:
- Different expected tents
- One square is removed, but all the other squares are otherwise fine
- One square has been added (all the different square types are tested)
*/

bool test_game_equal() {
    // Testing for false negatives
    game g_default = game_default();
    game g_empty = game_new_empty();

    if (game_equal(g_default, g_default) == false) {
        fprintf(stderr,
                "Error: \'game_equal\' returned false after testing two equal "
                "games!\n");
        return false;
    }

    if (game_equal(g_empty, g_empty) == false) {
        fprintf(stderr,
                "Error: \'game_equal\' returned false after testing two equal "
                "games!\n");
        return false;
    }

    // Testing for false positives
    // Different expected tents (row then column)

    game g = game_default();
    game_set_expected_nb_tents_row(g, 0, 0);

    if (game_equal(g, g_default) == true) {
        fprintf(stderr,
                "Error: \'game_equal\' returned true after testing two "
                "different games (different expected tents)!\n");
        return false;
    }

    game_set_expected_nb_tents_row(g, 0, 3);
    game_set_expected_nb_tents_col(g, 0, 0);

    if (game_equal(g, g_default) == true) {
        fprintf(stderr,
                "Error: \'game_equal\' returned true after testing two "
                "different games (different expected tents)!\n");
        return false;
    }

    game_set_expected_nb_tents_col(g, 0, 4);

    // Square removed

    game_set_square(g, 1, 0, EMPTY);

    if (game_equal(g, g_default) == true) {
        fprintf(stderr,
                "Error: \'game_equal\' returned true after testing two "
                "different games (1 square removed)!\n");
        return false;
    }

    game_set_square(g, 1, 0, TREE);

    // Square added

    game_set_square(g, 0, 0, TREE);

    if (game_equal(g, g_default) == true) {
        fprintf(stderr,
                "Error: \'game_equal\' returned true after testing two "
                "different games (TREE added)!\n");
        return false;
    }

    game_set_square(g, 0, 0, TENT);

    if (game_equal(g, g_default) == true) {
        fprintf(stderr,
                "Error: \'game_equal\' returned true after testing two "
                "different games (TENT added)!\n");
        return false;
    }

    game_set_square(g, 0, 0, GRASS);

    if (game_equal(g, g_default) == true) {
        fprintf(stderr,
                "Error: \'game_equal\' returned true after testing two "
                "different games (GRASS added)!\n");
        return false;
    }

    // Tests passed!
    game_delete(g);
    game_delete(g_default);
    game_delete(g_empty);

    return true;
}

//*------------------MAIN------------------*

int main(int argc, char* argv[]) {
    if (argc != 2) {
        usage(argv[0]);
        return (EXIT_FAILURE);
        ;
    }

    bool test_success;

    printf("%s\n", argv[1]);
    // start test
    if (strcmp(argv[1], "dummy") == 0) {
        test_success = true;
    } else if (strcmp(argv[1], "game_print") == 0) {
        test_success = test_game_print();
    } else if (strcmp(argv[1], "game_new") == 0) {
        test_success = test_game_new();
    } else if (strcmp(argv[1], "game_set_expected_nb_tents_col") == 0) {
        test_success = test_game_set_expected_nb_tents_col();
    } else if (strcmp(argv[1], "game_get_current_nb_tents_col") == 0) {
        test_success = test_game_get_current_nb_tents_col();
    } else if (strcmp(argv[1], "game_check_move") == 0) {
        test_success = test_game_check_move();
    } else if (strcmp(argv[1], "game_fill_grass_col") == 0) {
        test_success = test_game_fill_grass_col();
    } else if (strcmp(argv[1], "game_equal") == 0) {
        test_success = test_game_equal();
    } else {
        fprintf(stderr, "Test %s doesn't exist!\n", argv[1]);
        return EXIT_FAILURE;
    }

    // results

    if (test_success) {
        fprintf(stderr, "Test %s: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Test %s: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}
