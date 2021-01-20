# TENTS A22E

## General information
This project is a group work by DARE Anyaronke, GUILLET Mathieu, KADIONIK Sacha and O'REILLY Liam.
It aims to recreate the game Tents (c.f. the game documentation [HERE](https://pt2.gitlabpages.inria.fr/tents/v1/))

In its current state, the project should contain the following files:

* CMakeLists.txt
* game.c
* game.aux.c
* game_text.c
* queue.c
* game_test_dsanyaronke.c
* game_test_loreilly.c
* game_test_mguillet001.c
* game_test_skadionik.c
* test_gameExt.c
* game_aux.h
* game_ext.h
* game.h
* queue.h
* struct.h



## Implementation

Nothing to implement yet

## Known bugs

game_check_move does not pass some of the tests (2 and 4). This may results in some problem regarding wrapping/diagadj.

## Version and changelog

CURRENT VERSION : 1.6.1


Changelog: 1.6.1
-Added debugging functions to factorize tests and make error management easier

1.6.0 - TD6
-Replaced game.o with game.c
-Defined the 'game' struct
-Created the empty shell for game.c functions
-Updated CMakeLists.txt and .gitignore to reflect that change
-Added .clang-format file


TD - 5:

1.5.2
-Added most of the other tests (game_play_move missing)
-Fixed an oversight in game_text.c : player could crash the game by attempting to play a move out of bounds


1.5.1
-Added 6 tests in game_test_skadionik.c
-Replaced game.o with a fixed version
-Fixed a bug in game_text.c : game started with the default solution instead of the default game
-Fixed a bug in game_text.c : game didn't print "Losing move" if it was because of a misplaced grass
-Fixed a typo in game_text.c : game didn't line break after printing the "Quit" message