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

#include <poker-eval/inlines/eval.h>
#include <poker-eval/inlines/eval_type.h>
#include <poker-eval/poker_defs.h>
#include <poker-eval/deck_std.h>

#include "eval_single.h"

#include "misc_helpers.h"

int evaluate_hand(StdDeck_CardMask * hand, StdDeck_CardMask * board) {
  StdDeck_CardMask tmp;
  StdDeck_CardMask_OR(tmp, *hand, *board);
  return StdDeck_StdRules_EVAL_N(tmp, StdDeck_numCards(tmp));
}

float * evaluate_hand_draws(StdDeck_CardMask * board, StdDeck_CardMask * hand, int flags) {
  float * results = (float*) malloc(sizeof(float) * 9);

  StdDeck_CardMask gen_board, tmp_cards;

  int board_cards = StdDeck_numCards(*board);
  int gen_cards = (flags & STREETS) - board_cards;
  StdDeck_CardMask used_cards;
  StdDeck_CardMask_OR(used_cards, *hand, *board);

  int iterations = ITERATIONS_GET(flags);

  int count = 0;

  if (iterations) {
    DECK_MONTECARLO_N_CARDS_D(
      StdDeck,
      gen_board,
      used_cards,
      gen_cards,
      iterations,
      {
        StdDeck_CardMask_OR(tmp_cards, *board, gen_board);
        results[HandVal_HANDTYPE(evaluate_hand(hand, &tmp_cards))]++;
        count++;
      }
    );
  } else {
    switch(gen_cards) {
      case 3: DECK_ENUMERATE_3_CARDS_D(StdDeck, gen_board, used_cards, {
        results[HandVal_HANDTYPE(evaluate_hand(hand, board))]++;
        count++;
      } ); break;
      case 4: DECK_ENUMERATE_4_CARDS_D(StdDeck, gen_board, used_cards, {
        results[HandVal_HANDTYPE(evaluate_hand(hand, board))]++;
        count++;
      } ); break;
      case 5: DECK_ENUMERATE_5_CARDS_D(StdDeck, gen_board, used_cards, {
        results[HandVal_HANDTYPE(evaluate_hand(hand, board))]++;
        count++;
      } ); break;
      case 0:
        results[evaluate_hand(hand, board)]++;
        count++;
      break;
      default:
        printf("eval_draws: incorrect gen_cards: %d\n", gen_cards); exit(0);
    }
  }

  for (int i = 0; i < 9; i++) {
    results[i] /= count;
  }
  return results;
}

float * evaluate_hands_draws(StdDeck_CardMask * board, StdDeck_CardMask * hand, int hands, int flags) {
  float ** results = (float**) malloc(hands * sizeof(float*));
  for (int i = 0; i < hands; i++) {
    results[i] = evaluate_hand_draws(board, &(hand[i]), flags);
  }
  float* values = (float*) malloc(sizeof(float) * 9);
  values = array2_mean_array_f(results, hands, 9);
  array2_free_f(results, hands);
  return values;
}

float * evaluate_hands_draws_boards(StdDeck_CardMask * board, int boards, StdDeck_CardMask * hand, int hands, int flags) {
  float ** results = (float**) malloc(boards * sizeof(float*));
  for (int i = 0; i < boards; i++) {
    results[i] = evaluate_hands_draws(&(board[i]), hand, hands, flags);
  }
  float* values = (float*) malloc(sizeof(float) * 9);
  values = array2_mean_array_f(results, boards, 9);
  array2_free_f(results, boards);
  return values;
}
