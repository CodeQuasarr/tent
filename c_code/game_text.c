#include <stdio.h>
#include <stdlib.h>
#include "../h_code/game.h"
#include "../h_code/game_aux.h"
#include "../h_code/game_ext.h"
#include "../h_code/queue.h"
#include "../h_code/struct.h"

/*********************************************************************************************************/

void perso_print_help() {
    printf("Action: help\n");
    printf("press 't <i> <j>' to put a tent at sqaure (i,j) \n");
    printf("press 'g <i> <j>' to put grass at sqaure (i,j) \n");
    printf("press 'e <i> <j>' to erase sqaure (i,j) \n");
    printf("press 'z' to undo one move \n");
    printf("press 'y' to redo one move \n");
    printf("press 'r' to restart \n");
    printf("press 'q' to quit \n");
}

/*********************************************************************************************************/

void perso_test(game g, char command_1, uint command_2, uint command_3,
                square s) {
    /*
     *this function tests the playability of the game
     *game_check_move will allow you to work on a movement
     *in a box, whether it is illeage or not
     */

    if (command_2 >= DEFAULT_SIZE) {
        printf(
            "> Action: Can't perform this action out of bounds. Try again with "
            "a lower row index.\n");
        return;
    }

    if (command_3 >= DEFAULT_SIZE) {
        printf(
            "> Action: Can't perform this action out of bounds. Try again with "
            "a lower column index.\n");
        return;
    }

    if (game_check_move(g, command_2, command_3, s) == ILLEGAL) {
        printf("> Action: Illegal movement ! Cut the tree if you can.\n");
    } else if (game_check_move(g, command_2, command_3, s) == REGULAR) {
        printf("> Action: Playing move '%c' into square (%d,%d).\n", command_1,
               command_2, command_3);

        game_play_move(g, command_2, command_3, s);
    } else {
        printf("> Action: Playing move '%c' into square (%d,%d).\n", command_1,
               command_2, command_3);
        printf("> Warning: Losing move on square (%d,%d).\n", command_2,
               command_3);

        game_play_move(g, command_2, command_3, s);
    }
}
/*********************************************************************************************************/

void play_game(game g) {
    /*this fonction is used to already play the game */
    char command_1;
    uint command_2, command_3;

    while (!game_is_over(g)) {
        fflush(stdin);
        scanf("%c", &command_1);

        if (command_1 == 't' || command_1 == 'g' || command_1 == 'e') {
            scanf("%d %d", &command_2, &command_3);
            switch (command_1) {
                case 't':
                    perso_test(g, command_1, command_2, command_3, TENT);
                    break;
                case 'e':
                    perso_test(g, command_1, command_2, command_3, EMPTY);
                    break;
                case 'g':
                    perso_test(g, command_1, command_2, command_3, GRASS);
                    break;
            }
            game_print(g);
        }

        if (command_1 == 'h' || command_1 == 'r' || command_1 == 'q' ||
            command_1 == 'z' || command_1 == 'y') {
            switch (command_1) {
                case 'h':
                    printf("> Action: help\n");
                    perso_print_help();
                    break;
                case 'r':
                    printf("> Action: restart\n");
                    game_restart(g);
                    break;
                case 'q':
                    printf("> Action: quit\n");
                    printf("Are you already giving up ? What a shame!!\n");
                    game_delete(g);

                    exit(EXIT_SUCCESS);
                case 'z':
                    if (queue_is_empty(g->undoStack)) {
                        printf("No move to undo!\n");
                        break;
                    }
                    move* undoneMove = queue_peek_tail(g->undoStack);
                    printf("Undone move in square (%d, %d)\n", undoneMove->row,
                           undoneMove->col);
                    game_undo(g);
                    break;
                case 'y':
                    if (queue_is_empty(g->redoStack)) {
                        printf("No move to redo!\n");
                        break;
                    }
                    move* redoneMove = queue_peek_tail(g->redoStack);
                    printf("Redone move in square (%d, %d)\n", redoneMove->row,
                           redoneMove->col);
                    game_redo(g);
                    break;
            }
            game_print(g);
        }
    }
}
/*********************************************************************************************************/
int main(void) {
    game g = game_default();
    game_print(g);
    play_game(g);

    game_delete(g);
    printf("Congratulations, you have won!\n");
    return EXIT_SUCCESS;
}
