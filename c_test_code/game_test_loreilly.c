#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../h_code/game.h"
#include "../h_code/game_aux.h"

/* ********** TEST game_default_solution ********** */

bool test_game_default_solution(void) {
    game myGame = game_default_solution();
    bool test = true;
    if (myGame == NULL) {
        fprintf(stderr, "Error: invalid game space!\n");
        test = false;
    }
    if (game_get_current_nb_tents_all(myGame) !=
        game_get_expected_nb_tents_all(myGame)) {
        fprintf(stderr, "Error: invalid tent value!\n");
        test = false;
    }
    game_delete(myGame);
    return test;
}

/* ********** TEST game_copy ********** */

bool test_game_copy() {
    game g1 = game_default();
    game g2 = game_copy(g1);
    bool test = true;
    if (g1 == NULL || g2 == NULL) {
        fprintf(stderr, "Error: invalid game space!\n");
        test = false;
    }
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        for (uint j = 0; j < DEFAULT_SIZE; j++) {
            if (game_get_square(g1, i, j) != game_get_square(g2, i, j) &&
                test == true) {
                fprintf(stderr, "Error: invalid square value!\n");
                test = false;
            }
        }
        if (game_get_expected_nb_tents_col(g1, i) !=
                game_get_expected_nb_tents_col(g2, i) &&
            test == true) {
            fprintf(stderr, "Error: invalid expected colum value!\n");
            test = false;
        }
        if (game_get_expected_nb_tents_row(g1, i) !=
                game_get_expected_nb_tents_row(g2, i) &&
            test == true) {
            fprintf(stderr, "Error: invalid expected row value!\n");
            test = false;
        }
    }
    game_delete(g1);
    game_delete(g2);
    return test;
}

/* ********** TEST game_get_expected_nb_tents_all ********** */

bool test_game_get_expected_nb_tents_all() {
    game g = game_default();
    bool test = true;
    int numb_row = 0;
    int numb_col = 0;
    if (g == NULL) {
        fprintf(stderr, "Error: invalid game space!\n");
        test = false;
    }
    int num_all = game_get_expected_nb_tents_all(g);
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        numb_row += game_get_expected_nb_tents_row(g, i);
        numb_col += game_get_expected_nb_tents_col(g, i);
        if ((numb_col < 0 || numb_row < 0 || num_all < 0) && test == true) {
            fprintf(stderr, "Error: invalid tent row or tent col value\n");
            test = false;
        }
    }
    if (numb_row != numb_col || numb_col != num_all || numb_row != num_all) {
        fprintf(stderr, "Error: nb of tents doesn't match\n");
        test = false;
    }
    game_delete(g);
    return test;
}

/* ********** TEST game_get_current_nb_tents_all ********** */

bool test_game_get_current_nb_tents_all() {
    game g = game_default();
    bool test = true;
    int numb_row = 0;
    int numb_col = 0;
    if (g == NULL) {
        fprintf(stderr, "Error: invalid game space!\n");
        test = false;
    }
    int num_all = game_get_current_nb_tents_all(g);
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        numb_row += game_get_current_nb_tents_row(g, i);
        numb_col += game_get_current_nb_tents_col(g, i);
        if (num_all < 0 && test == true) {
            fprintf(stderr, "Error: invalide tent value\n");
            test = false;
        }
    }
    if ((numb_col != num_all || numb_row != num_all) && test == true) {
        fprintf(stderr, "Error: nb of tents doesn't match\n");
        test = false;
    }
    game_delete(g);
    return test;
}

/* ********** TEST game_is_over ********** */

bool test_game_is_over() {
    game game_s = game_default_solution();
    bool test = true;
    if (!(game_is_over(game_s))) {
        fprintf(stderr, "Error: game should be over\n");
        test = false;
    }
    game_delete(game_s);
    return test;
}

/* ********** TEST game_restart ********** */

bool test_game_restart() {
    game g1 = game_default();
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        game_fill_grass_row(g1, i);
    }
    game g2 = game_default();
    game_restart(g1);
    if (g1 == NULL) {
        fprintf(stderr, "Error: invalid game space!\n");
        game_delete(g2);
        game_delete(g1);
        return false;
    }
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        for (uint j = 0; j < DEFAULT_SIZE; j++) {
            if (game_get_square(g1, i, j) != game_get_square(g2, i, j)) {
                fprintf(stderr, "Error: incomplete restart\n");
                game_delete(g1);
                game_delete(g2);
                return false;
            }
        }
    }
    game_delete(g2);
    game_delete(g1);
    return true;
}

/* ********** Usage ********** */

void usage(char* name) { fprintf(stderr, "Usage: %s test_name\n", name); }

/* ********** main routine ********** */

int main(int argc, char* argv[]) {
    if (argc != 2) {
        usage(argv[0]);
    }
    bool result;

    // test
    if (strcmp(argv[1], "dummy") == 0) {
        result = true;
    } else if (strcmp(argv[1], "game_default_solution") == 0) {
        result = test_game_default_solution();
    } else if (strcmp(argv[1], "game_copy") == 0) {
        result = test_game_copy();
    } else if (strcmp(argv[1], "game_get_expected_nb_tents_all") == 0) {
        result = test_game_get_expected_nb_tents_all();
    } else if (strcmp(argv[1], "game_get_current_nb_tents_all") == 0) {
        result = test_game_get_current_nb_tents_all();
    } else if (strcmp(argv[1], "game_is_over") == 0) {
        result = test_game_is_over();
    } else if (strcmp(argv[1], "game_restart") == 0) {
        result = test_game_restart();
    } else {
        fprintf(stderr, "Test %s does not exist", argv[1]);
        return EXIT_FAILURE;
    }

    // result
    if (result) {
        fprintf(stderr, "Test %s is successful", argv[1]);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Test %s failed", argv[1]);
        return EXIT_FAILURE;
    }
}
