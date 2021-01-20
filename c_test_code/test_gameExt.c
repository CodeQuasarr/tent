#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../h_code/game.h"
#include "../h_code/game_aux.h"
#include "../h_code/game_ext.h"
#include "../h_code/struct.h"

/**
 * @brief  test the default game
 * @return bool
 */
#define NB_ROWS 3
#define NB_COLS 3
bool test_game_new_ext() {
    //  we estimate the number of tents in a given rows.
    game testGame = game_new_empty_ext(NB_ROWS, NB_COLS, false, false);
    game_set_expected_nb_tents_row(testGame, 0, 2);
    game_set_expected_nb_tents_row(testGame, 2, 2);
    //  we will determine if the desired number of tents is true.
    if (game_get_expected_nb_tents_row(testGame, 0) != 2 ||
        game_get_expected_nb_tents_row(testGame, 2) != 2 ||
        (game_get_expected_nb_tents_row(testGame, 0) +
         game_get_expected_nb_tents_row(testGame, 0)) !=
            game_get_expected_nb_tents_all(testGame)) {
        fprintf(stderr, "expected number(s) of tents is false\n");
        game_delete(testGame);
        return false;
    }
    //  we play moves in our grid.
    game_play_move(testGame, 0, 0, TENT);
    game_play_move(testGame, 2, 0, TENT);
    //  we determine if the number of tents in our grid is true.
    uint count = 0;
    for (uint i = 0; i < game_nb_rows(testGame); i++)
        for (uint j = 0; j < game_nb_cols(testGame); j++)
            if (game_get_square(testGame, i, j) == TENT) count++;
    if (count != 2 || game_get_square(testGame, 0, 0) != TENT ||
        game_get_square(testGame, 2, 0) != TENT) {
        fprintf(stderr, "nuber(s) of square is false !\n");
        game_delete(testGame);
        return false;
    }
    //  we test wrapping and diagadj options.
    if (game_is_wrapping(testGame) || game_is_diagadj(testGame)) {
        fprintf(stderr, "wrapping or diagadj option doesn't seem correct\n");
        game_delete(testGame);
        return false;
    }
    game_delete(testGame);
    return true;
}

bool test_game_new_empty_ext() {
    game testGame = game_new_empty_ext(NB_ROWS, NB_COLS, false, true);
    //  we test wrapping and diagadj options.
    if (game_is_wrapping(testGame) || !game_is_diagadj(testGame)) {
        fprintf(stderr, "wrapping or diagadj option doesn't seem correct\n");
        game_delete(testGame);
        return false;
    }

    //  we determine if the dimension of grid is true.
    if (game_nb_rows(testGame) != NB_ROWS ||
        game_nb_cols(testGame) != NB_COLS) {
        printf("grid isn't print correctly\n");
        game_delete(testGame);
        return false;
    }
    //  we will determine if the desired number of tents is true.
    if (game_get_expected_nb_tents_all(testGame) != 0) {
        printf("numbers of tents for each row/column will be 0\n");
        game_delete(testGame);
        return false;
    }
    // Improvement : || game_get_square_size(g) != 3 ; We don't have
    // something to return "squares" size
    for (uint i = 0; i < (NB_ROWS * NB_COLS); i++) {
        if (game_get_square(testGame, i % NB_ROWS, i / NB_COLS) != EMPTY) {
            printf("error\n");
            game_delete(testGame);
            return false;
        }
    }
    game_delete(testGame);
    return true;
}

/**
 * @brief  test if the entered dimension of the number of rows is the same as at
 * the exit
 * @return bool
 */
bool test_game_nb_rows() {
    // Test 1: the function works correctly for the base case
    game testGame = game_new_empty_ext(8, 8, true, true);
    if (game_nb_cols(testGame) != 8) {
        fprintf(stderr, "erreur au niveaux des dimension\n");
        return false;
    }
    game_delete(testGame);

    // Test 2: the function uses the parameter and not a value by
    // default
    game testGameValeurDefaut = game_new_empty_ext(4, 4, true, true);
    if (game_nb_rows(testGameValeurDefaut) != 4) {
        fprintf(stderr, "erreur au niveaux des dimension\n");
        return false;
    }
    game_delete(testGameValeurDefaut);

    // Test 3: the function does not reverse the parameters of columns and rows
    game testGameInversion = game_new_empty_ext(4, 8, true, true);
    if (game_nb_rows(testGameInversion) != 4) {
        fprintf(stderr, "erreur au niveaux des dimension\n");
        return false;
    }
    game_delete(testGameInversion);

    // Test 4: the function has no limit on the extremes
    game testGameLimite = game_new_empty_ext(100, 1, true, true);
    if (game_nb_rows(testGameLimite) != 100) {
        fprintf(stderr, "erreur au niveaux des dimension\n");
        return false;
    }
    game_delete(testGameLimite);

    // Tests réussis!
    return true;
}

/**
 * @brief  test if the entered dimension of the number of colomns is the same as
 * at the exit
 * @return bool
 */
bool test_game_nb_cols() {
    // Test 1: the function works correctly for the base case
    game testGame = game_new_empty_ext(8, 8, true, true);
    if (game_nb_cols(testGame) != 8) {
        fprintf(stderr, "erreur au niveaux des dimension\n");
        return false;
    }
    game_delete(testGame);

    // Test 2: la fonction utilise bien le paramètre et pas une valeur par
    // défaut
    game testGameValeurDefaut = game_new_empty_ext(4, 4, true, true);
    if (game_nb_cols(testGameValeurDefaut) != 4) {
        fprintf(stderr, "erreur au niveaux des dimension\n");
        return false;
    }
    game_delete(testGameValeurDefaut);

    // Test 3: la fonction n'inverse pas les paramètres de colonnes et de lignes
    game testGameInversion = game_new_empty_ext(8, 4, true, true);
    if (game_nb_cols(testGameInversion) != 4) {
        fprintf(stderr, "erreur au niveaux des dimension\n");
        return false;
    }
    game_delete(testGameInversion);

    // Test 4: la fonction n'a pas de limite sur les extrêmes
    game testGameLimite = game_new_empty_ext(1, 100, true, true);
    if (game_nb_cols(testGameLimite) != 100) {
        fprintf(stderr, "erreur au niveaux des dimension\n");
        return false;
    }
    game_delete(testGameLimite);

    // Tests réussis!
    return true;
}

/**
 * @brief test if the wrapping option is checked
 * @return bool
 */
bool test_game_is_wrapping() {
    // Cas 1: Wrapping et diagadj = true
    game testGame = game_new_empty_ext(8, 8, true, true);
    if (!game_is_wrapping(testGame)) {
        game_delete(testGame);
        fprintf(
            stderr,
            "Error: game_is_wrapping returned false while true was expected\n");
        return false;
    }
    game_delete(testGame);

    // Cas 2: Wrapping et diagadj = false
    game testGame2 = game_new_empty_ext(8, 8, false, false);
    if (game_is_wrapping(testGame2)) {
        game_delete(testGame2);
        fprintf(
            stderr,
            "Error: game_is_wrapping returned true while false was expected\n");
        return false;
    }
    game_delete(testGame2);

    // Cas 3: Wrapping = true et diagadj = false
    game testGame3 = game_new_empty_ext(8, 8, true, false);
    if (!game_is_wrapping(testGame3)) {
        game_delete(testGame3);
        fprintf(
            stderr,
            "Error: game_is_wrapping returned false while true was expected\n");
        return false;
    }
    game_delete(testGame3);

    // Cas 4: Wrapping = false et diagadj = true
    game testGame4 = game_new_empty_ext(8, 8, false, true);
    if (game_is_wrapping(testGame4)) {
        game_delete(testGame4);
        fprintf(
            stderr,
            "Error: game_is_wrapping returned true while false was expected\n");
        return false;
    }
    game_delete(testGame4);

    return true;
}

/**
 * @brief test if the wrapping option is checked
 * @return bool
 */
bool test_game_is_diagadj() {
    // Cas 1: Wrapping et diagadj = true
    game testGame = game_new_empty_ext(8, 8, true, true);
    if (!game_is_diagadj(testGame)) {
        game_delete(testGame);
        fprintf(
            stderr,
            "Error: game_is_diagadj returned false while true was expected\n");
        return false;
    }
    game_delete(testGame);

    // Cas 2: Wrapping et diagadj = false
    game testGame2 = game_new_empty_ext(8, 8, false, false);
    if (game_is_diagadj(testGame2)) {
        game_delete(testGame2);
        fprintf(
            stderr,
            "Error: game_is_diagadj returned true while false was expected\n");
        return false;
    }
    game_delete(testGame2);

    // Cas 3: Wrapping = true et diagadj = false
    game testGame3 = game_new_empty_ext(8, 8, true, false);
    if (game_is_diagadj(testGame3)) {
        game_delete(testGame3);
        fprintf(
            stderr,
            "Error: game_is_diagadj returned true while false was expected\n");
        return false;
    }
    game_delete(testGame3);

    // Cas 4: Wrapping = false et diagadj = true
    game testGame4 = game_new_empty_ext(8, 8, false, true);
    if (!game_is_diagadj(testGame4)) {
        game_delete(testGame4);
        fprintf(
            stderr,
            "Error: game_is_diagadj returned false while true was expected\n");
        return false;
    }
    game_delete(testGame4);

    return true;
}

/**
 * @brief  testing if undo option works
 * @return boolean
 */
bool test_game_undo() {
    // we create a game in which we play a move and we make a copy of the game
    return true;
}

/**
 * @brief  testing if undo option works
 * @return boolean
 */
bool test_game_redo() {
    // we create a game in which we play a move and we make a copy of the game
    game testGame = game_new_empty_ext(8, 8, true, false);
    game_play_move(testGame, 2, 2, TENT);

    // Testing the redoo
    move* movePlayed = malloc(sizeof(move));
    movePlayed->square = game_get_square(testGame, 2, 2);
    movePlayed->row = 2;
    movePlayed->col = 2;
    queue_push_tail(testGame->undoStack, movePlayed);
    game_play_move(testGame, 2, 2, GRASS);
    game testGameCopy = game_copy(testGame);
    game_undo(testGame);
    game_redo((testGame));

    if (game_get_square(testGameCopy, 2, 2) != GRASS) {
        fprintf(stderr, "erreur game 1 et game 2 devrai etres ls meme\n");
        return false;
    }
    game_delete(testGame);
    game_delete(testGameCopy);
    return true;
}

bool test_game_retart() {
    game g1 = game_new_empty_ext(NB_ROWS, NB_COLS, true, false);
    game g2 = game_copy(g1);

    game_play_move(g1, 2, 0, TENT);
    game_play_move(g1, 0, 2, GRASS);
    game_play_move(g1, 2, 2, TENT);
    game_restart(g1);

    if (game_nb_cols(g1) != game_nb_cols(g2) ||
        game_nb_rows(g2) != game_nb_rows(g1)) {
        fprintf(stderr, "Error: invalid delete!\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }
    if (!game_is_wrapping(g1) && !game_is_wrapping(g2)) return false;
    if (game_is_wrapping(g1) != game_is_wrapping(g2) ||
        game_is_diagadj(g2) != game_is_diagadj(g1)) {
        fprintf(stderr, "Error: invalid delete!\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }

    for (uint i = 0; i < game_nb_rows(g1); i++) {
        if (game_get_expected_nb_tents_row(g1, i) !=
            game_get_expected_nb_tents_row(g2, i)) {
            fprintf(stderr, "Error: invalid delete!\n");
            game_delete(g1);
            game_delete(g2);
            return false;
        }

        for (uint j = 0; j < game_nb_cols(g1); j++) {
            if (game_get_square(g1, i, j) != game_get_square(g1, i, j)) {
                fprintf(stderr, "Error: invalid delete!\n");
                game_delete(g1);
                game_delete(g2);
                return false;
            }
            if (game_get_expected_nb_tents_col(g1, j) !=
                game_get_expected_nb_tents_col(g2, j)) {
                fprintf(stderr, "Error: invalid delete!\n");
                game_delete(g1);
                game_delete(g2);
                return false;
            }
        }
    }
    if (g1 == NULL || g2 == NULL) {
        fprintf(stderr, "Error: invalid delete!\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }
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

void usage(int argc, char* argv[]) {
    fprintf(stderr, "Usage: %s game_test_dsnyaronke_dummy failed\n", argv[0]);
    exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char* argv[]) {
    if (argc != 2) usage(argc, argv);

    // start test
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool valide = false;

    if (strcmp(argv[1], "game_new_ext") == 0)
        valide = test_game_new_ext();

    else if (strcmp(argv[1], "game_new_empty_ext") == 0)
        valide = test_game_new_empty_ext();

    else if (strcmp(argv[1], "game_nb_rows") == 0)
        valide = test_game_nb_rows();

    else if (strcmp(argv[1], "game_nb_cols") == 0)
        valide = test_game_nb_cols();

    else if (strcmp(argv[1], "game_is_wrapping") == 0)
        valide = test_game_is_wrapping();

    else if (strcmp(argv[1], "game_is_diagadj") == 0)
        valide = test_game_is_diagadj();

    else if (strcmp(argv[1], "game_undo") == 0)
        valide = test_game_undo();

    else if (strcmp(argv[1], "game_redo") == 0)
        valide = test_game_redo();
    else if (strcmp(argv[1], "restart") == 0)
        valide = test_game_retart();

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