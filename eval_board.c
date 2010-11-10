/***************************************************************************
 *   Copyright (C) 2010 by Henrik Asp                                      *
 *   solenskiner@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdlib.h>

#include <poker-eval/inlines/eval.h>
#include <poker-eval/inlines/eval_type.h>
#include <poker-eval/poker_defs.h>
#include <poker-eval/deck_std.h>
#include <poker-eval/enumerate.h>

#include "eval_board.h"
#include "eval_single.h"

#include "misc_helpers.h"

/////////////////////////////////////////////////////////////////////////////////////////////

/// board -> array of evaluated_values for every possible hand
int * enumerate_all_hands( StdDeck_CardMask * board, int * out_size) {
  int * results = (int*) malloc(sizeof(int) * 1326);

  StdDeck_CardMask tmp_hand;

  *out_size = 0;
  DECK_ENUMERATE_2_CARDS_D(StdDeck, tmp_hand, *board, {
    results[*out_size] = evaluate_hand(&tmp_hand, board);
    (*out_size)++;
  } );

  results = (int*) realloc(results, sizeof(int) * *out_size);

  qsort(results, *out_size, sizeof(int), &compare_desc_i);

  return results;
}

/// array of boards -> array of array of evaluated_values for every possible hand
int** enumerate_all_hands_boards(StdDeck_CardMask * board, int boards, int* out_size) {
  int** array_result = (int**) malloc(sizeof(int*) * boards);
  for (int i = 0; i < boards; i++) {
    array_result[i] = enumerate_all_hands(&(board[i]), &(out_size[i]));
  }
  return array_result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/*
float find_hand_worth_i(int spec_value, int* board_hand_value, int board_hand_values) {
  for (int i = 0; i < board_hand_values; i++)
    if (spec_value == board_hand_value[i]) {
      for (int j = i;; j--) {
        if (j == 0) return 1;
        else if (board_hand_value[i] != board_hand_value[j])
          return 1.0 - (float) (j + 2) / (float) board_hand_values; //+1 för one-past räkning, +1 för 0indexering
      }
    }
  return (float) -1;
}


float find_hand_worth(StdDeck_CardMask * hand, StdDeck_CardMask * board, int* board_hand_value, int board_hand_values) {
  find_hand_worth_i(evaluate_hand(hand, board), board_hand_value, board_hand_values);
}

float find_hands_worth(StdDeck_CardMask * hand, int hands, StdDeck_CardMask * board, int* board_hand_value, int board_hand_values) {
  float* array_result = (float*) malloc(sizeof(float) * hands);
  for (int i = 0; i < hands; i++) {
    array_result[i] = find_hand_worth(&(hand[i]), board, board_hand_value, board_hand_values);
  }
  float result = array_mean_f(array_result, hands);
  free(array_result);
  return result;
}

float find_hands_worth_boards(StdDeck_CardMask * hand, int hands, StdDeck_CardMask** board, int boards, int** board_hand_value, int* board_hand_values) {
  float* array_result = (float*) malloc(sizeof(float) * boards);
  for (int i = 0; i < boards; i++) {
    array_result[i] = find_hands_worth(hand, hands, &(board[i]), board_hand_value[i], board_hand_values[i]);
  }
  float result = array_mean_f(array_result, boards);
  free(array_result);
  return result;
}

// SUPER IMPORTANT board is the same board used to calculate all_hand
float* find_hands_worth_boards_players(StdDeck_CardMask ** hand, int* hands, int players, StdDeck_CardMask** board, int boards, int** board_hand_value, int* board_hand_values) {
  float* array_result = (float*) malloc(sizeof(float) * players);
  for (int i = 0; i < players; i++) {
    array_result[i] = find_hands_worth_boards(hand[i], hands[i], board, boards, board_hand_value[i], board_hand_values);
  }
  return array_result;
}

/////////////////////////////////////////////////////////////////////////////////////////////

int* filter_hand_type(int type, int* board_hand_value, int board_hand_values, int * out_size) {
  int * results = (int*) malloc(sizeof(int) * board_hand_values);
  *out_size = 0;
  for (int i = 0; i < board_hand_values; i++) {
    if (type == HandVal_HANDTYPE_VALUE(board_hand_value[i])) {
      results[*out_size] = board_hand_value[i];
      (*out_size)++;
    }
    else if (*out_size != 0) break;
  }
  results = (int*) realloc(results, *out_size);
  return results;
}

int* filter_hand_type_boards(int type, int* hand, int board_hand_values, int ** out_size) {
  int * results = (int*) malloc(sizeof(int) * hands);
  *out_size = 0;
  for (int i = 0; i < hands; i++) {
    if (type == HandVal_HANDTYPE_VALUE(hand[i])) {
      results[*out_size] = hand[i];
      (*out_size)++;
    }
    else if (*out_size != 0) break;
  }
  results = (int*) realloc(results, *out_size);
  return results;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////

float * eval_distribution(int * board_enumed_hand, int board_enumed_hands) {
  float * results = (float*) malloc(sizeof(float) * 9);
  array_init_f(9, 0, results);
  for (int i = 0; i < board_enumed_hands; i++)
    results[HandVal_HANDTYPE(board_enumed_hand[i])]++;
  for (int i = 0; i < 9; i++)
    results[i] /= (float) board_enumed_hands;
  return results;
}

float* eval_distribution_boards(int ** board_enumed_hand, int boards, int * board_enumed_hands) {
  float** array_result = (float**) malloc(sizeof(float*) * boards);
  for (int i = 0; i < boards; i++) {
    array_result[i] = eval_distribution(board_enumed_hand[i], board_enumed_hands[i]);
  }
  float* result = array2_mean_array_f(array_result, boards, 9);
  array2_free_f(array_result, boards);
  return result;
}

