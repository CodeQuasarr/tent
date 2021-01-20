cmake_minimum_required(VERSION 2.6)

project(tentsA22e C)

enable_testing()
include(CTest)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c99 -g --coverage")

add_executable(game_text c_code/game_text.c c_code/game_aux.c game.c c_code/queue.c)
add_executable(game_test_dsanyaronke c_test_code/game_test_dsanyaronke.c c_code/game_aux.c game.c c_code/queue.c)
add_executable(game_test_skadionik c_test_code/game_test_skadionik.c c_code/game_aux.c game.c c_code/queue.c)
add_executable(game_test_mguillet001 c_test_code/game_test_mguillet001.c c_code/game_aux.c game.c c_code/queue.c)
add_executable(game_test_loreilly c_test_code/game_test_loreilly.c c_code/game_aux.c game.c c_code/queue.c)

add_executable(test_gameExt c_test_code/test_gameExt.c c_code/game_aux.c game.c c_code/queue.c)

add_library(game c_code/game_text.c c_code/game_aux.c game.c c_code/queue.c)
target_link_libraries(game_text m)


add_test(test_gameExt_new_ext ./test_gameExt game_new_ext)
add_test(test_gameExt_new_empty_ext ./test_gameExt game_new_empty_ext)
add_test(test_gameExt_nb_rows ./test_gameExt game_nb_rows)
add_test(test_gameExt_nb_cols ./test_gameExt game_nb_cols)
add_test(test_gameExt_is_wrapping ./test_gameExt game_is_wrapping)
add_test(test_gameExt_is_diagadj ./test_gameExt game_is_diagadj)
add_test(test_gameExt_undo ./test_gameExt game_undo)
add_test(test_gameExt_redo ./test_gameExt game_redo)
add_test(test_gameExt_restart ./test_gameExt restart)

add_test(test_loreilly_game_default_solution ./game_test_loreilly game_default_solution)
add_test(test_loreilly_game_copy ./game_test_loreilly game_copy)
add_test(test_loreilly_game_get_expected_nb_tents_all ./game_test_loreilly game_get_expected_nb_tents_all)
add_test(test_loreilly_game_get_current_nb_tents_all ./game_test_loreilly game_get_current_nb_tents_all)
add_test(test_loreilly_game_is_over ./game_test_loreilly game_is_over)
add_test(test_loreilly_game_restart ./game_test_loreilly game_restart)

add_test(test_dsanyaronke_game_default ./game_test_dsanyaronke game_default)
add_test(test_dsanyaronke_game_delete ./game_test_dsanyaronke game_delete)
add_test(test_dsanyaronke_get_square ./game_test_dsanyaronke get_square)
add_test(test_dsanyaronke_game_equal ./game_test_dsanyaronke game_equal)
add_test(test_dsanyaronke_get_expected_nb_tents_row ./game_test_dsanyaronke get_expected_nb_tents_row)
add_test(test_dsanyaronke_get_expected_nb_tents_col ./game_test_dsanyaronke get_expected_nb_tents_col)
add_test(test_dsanyaronke_game_get_current_nb_tents_row ./game_test_dsanyaronke game_get_current_nb_tents_row)
add_test(test_dsanyaronke_game_get_current_nb_tents_col ./game_test_dsanyaronke game_get_current_nb_tents_col)
add_test(test_dsanyaronke_game_play_move ./game_test_dsanyaronke play_move)
add_test(test_dsanyaronke_game_is_over ./game_test_dsanyaronke game_is_over)


add_test(test_skadionik_game_print ./game_test_skadionik game_print)
add_test(test_skadionik_game_new ./game_test_skadionik game_new)
add_test(test_skadionik_game_set_expected_nb_tents_col ./game_test_skadionik game_set_expected_nb_tents_col)
add_test(test_skadionik_game_get_current_nb_tents_col ./game_test_skadionik game_get_current_nb_tents_col)
add_test(test_skadionik_game_check_move ./game_test_skadionik game_check_move)
add_test(test_skadionik_game_fill_grass_col ./game_test_skadionik game_fill_grass_col)
add_test(test_skadionik_game_equal ./game_test_skadionik game_equal)

add_test(test_mguillet001_game_new_empty ./game_test_mguillet001 game_new_empty)
add_test(test_mguillet001_game_set_square ./game_test_mguillet001 game_set_square)
add_test(test_mguillet001_game_set_expected_nb_tents_row ./game_test_mguillet001 game_set_expected_nb_tents_row)
add_test(test_mguillet001_game_get_current_nb_tents_row ./game_test_mguillet001 game_get_current_nb_tents_row)
add_test(test_mguillet001_game_fill_grass_row ./game_test_mguillet001 game_fill_grass_row)