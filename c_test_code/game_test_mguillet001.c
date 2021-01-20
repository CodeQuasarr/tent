#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../h_code/game.h"
#include "../h_code/game_aux.h"

/* ********** Tests ********** */

/**
 * @brief Test of game_new_empty
 * @return bool
 */
bool test_game_new_empty() {
    game gtest = game_new_empty();
    bool res = true;

    if (game_get_expected_nb_tents_all(gtest) != 0) {
        res = false;
    }
    // Improvement : || game_get_square_size(g) != DEFAULT_SIZE ; We don't have
    // something to return "squares" size
    for (uint i = 0; i < (DEFAULT_SIZE * DEFAULT_SIZE); i++) {
        if (game_get_square(gtest, i % DEFAULT_SIZE, i / DEFAULT_SIZE) !=
            EMPTY) {
            res = false;
        }
    }
    game_delete(gtest);
    return res;
}

/**
 * @brief Test of game_set_square
 *
 * @return true
 * @return false
 */
bool test_game_set_square() {
    game gtest = game_new_empty();
    game_set_square(gtest, 0, 0, TREE);

    if (game_get_square(gtest, 0, 0) != TREE) {
        game_delete(gtest);
        return false;
    } else {
        game_delete(gtest);
        return true;
    }
}

/**
 * @brief Test of game_set_expected_nb_tents_row
 *
 * @return true
 * @return false
 */
bool test_game_set_expected_nb_tents_row() {
    game gtest = game_new_empty();
    game_set_expected_nb_tents_row(gtest, 0, 42);

    if (game_get_expected_nb_tents_row(gtest, 0) != 42) {
        game_delete(gtest);
        return false;
    } else {
        game_delete(gtest);
        return true;
    }
}

/**
 * @brief Test of game_get_current_nb_tents_row
 *
 * @return true
 * @return false
 */
bool test_game_get_current_nb_tents_row() {
    game gtest = game_default();
    uint count = 0;
    bool res;
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        if (game_get_square(gtest, i % DEFAULT_SIZE, i / DEFAULT_SIZE) ==
            TENT) {
            count++;
        }
    }
    res = count == game_get_current_nb_tents_row(gtest, 0);
    game_delete(gtest);
    return res;
}

/**
 * @brief Test of game_fill_grass_row
 *
 * @return bool
 */
bool test_game_fill_grass_row() {
    game gtest = game_default();
    bool res = true;
    game_fill_grass_row(gtest, 0);

    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        if (game_get_square(gtest, 0, i) == EMPTY) {
            res = false;
        }
    }
    game_delete(gtest);
    return res;
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
    fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
    exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {
    if (argc == 1) usage(argc, argv);

    // start test
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok;

    if (strcmp("game_new_empty", argv[1]) == 0)
        ok = test_game_new_empty();
    else if (strcmp("game_set_square", argv[1]) == 0)
        ok = test_game_set_square();
    else if (strcmp("game_set_expected_nb_tents_row", argv[1]) == 0)
        ok = test_game_set_expected_nb_tents_row();
    else if (strcmp("game_get_current_nb_tents_row", argv[1]) == 0)
        ok = test_game_get_current_nb_tents_row();
    else if (strcmp("game_fill_grass_row", argv[1]) == 0)
        ok = test_game_fill_grass_row();
    else if (strcmp("all", argv[1]) == 0) {
        if (test_game_new_empty() == false || test_game_set_square() == false ||
            test_game_set_expected_nb_tents_row() == false ||
            test_game_get_current_nb_tents_row() == false ||
            test_game_fill_grass_row() == false) {
            ok = false;
        } else {
            ok = true;
        }
    } else {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // print test result
    if (ok) {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}