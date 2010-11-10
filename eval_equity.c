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
#include <poker-eval/poker_defs.h>
#include <poker-eval/deck_std.h>
#include <poker-eval/enumerate.h>

#include "eval_defines.h"
#include "eval_equity.h"
#include "misc_helpers.h"
#include "misc_bool.h"

void evaluate_equity_single(StdDeck_CardMask* player, int players, StdDeck_CardMask board, int cards, float* values) {
  int handvalue[players];
  for (int i = 0; i < players; i++) {
    handvalue[i] = StdDeck_StdRules_EVAL_N(cardmask_OR(player[i], board), cards + 2);
  }
  int winner[players];
  array_init_i(players, true, winner);
  for (int i = 0; i < players; i++) {
    for (int j =  0; j < players; j++) {
           if (handvalue[i] < handvalue[j]) winner[i] = false;
      else if (handvalue[i] > handvalue[j]) winner[j] = false;
    }
  }
  int winners = 0;
  for (int i = 0; i < players; i++)
    if (winner[i]) winners++;
  for (int i = 0; i < players; i++) if (winner[i]) values[i] += 1.0/winners;
}

float* evaluate_equity_players(StdDeck_CardMask* player, int players, StdDeck_CardMask board, int flags) {
  StdDeck_CardMask used;
  StdDeck_CardMask_RESET(used);
  for (int i = 0; i < players; i++) {
    used = cardmask_OR(used, player[i]);
  }
  StdDeck_CardMask_OR(used, used, board);

  int cards = (flags & STREETS) - StdDeck_numCards(board);

  float* values = (float*) malloc(sizeof(float)*players);

  int iterations = ITERATIONS_GET(flags);
  int cards2iter[6]={1, 46, 1081, 0, 0, 2118760};

  if (iterations) {
    iterations = min_i(iterations, cards2iter[cards]);
    DECK_MONTECARLO_N_CARDS_D(StdDeck, board, used, cards, iterations, evaluate_equity_single(player, players, board, cards, values); );
  } else {
    switch(cards) {
      case 3: DECK_ENUMERATE_3_CARDS_D(StdDeck, board, used, evaluate_equity_single(player, players, board, cards, values); ); break;
      case 4: DECK_ENUMERATE_4_CARDS_D(StdDeck, board, used, evaluate_equity_single(player, players, board, cards, values); ); break;
      case 5: DECK_ENUMERATE_5_CARDS_D(StdDeck, board, used, evaluate_equity_single(player, players, board, cards, values); ); break;
      case 0: evaluate_equity_single(player, players, board, cards, values); break;
    }
  }

  if (iterations == 0) iterations = cards2iter[cards];
  for (int i = 0; i < players; i++) values[i] /= iterations;

  return values;
}



float* evaluate_equity_players_hands(StdDeck_CardMask** player, int* hands, int players, StdDeck_CardMask board, int flags) {
  int combs;
  StdDeck_CardMask** comb_uf = array_array_cartesian_array2_cardmask (player, players, hands, &combs);
  StdDeck_CardMask** comb = array2_filter_array2_cardmask(comb_uf, combs, players, &combs);
  array2_free_cardmask(comb_uf, combs);
  float** value = (float**) malloc(sizeof(float*) * combs);
  for (int i = 0; i < combs; i++) {
    value[i] = evaluate_equity_players(comb[i], players, board, ITERATIONS_DIVIDE(flags, combs));
  }
  array2_free_cardmask(comb, combs);
  float* value_mean = array2_mean_array_f(value, combs, players);
  array2_free_f(value, combs);
  return value_mean;
}


float* evaluate_equity_players_hands_boards(StdDeck_CardMask** player, int* hands, int players, StdDeck_CardMask* board, int boards, int flags) {
  float** value = (float**) malloc(sizeof(float*) * boards);
  for (int i = 0; i < boards; i++) {
    value[i] = evaluate_equity_players_hands(player, hands, players, board[i], ITERATIONS_DIVIDE(flags, boards));
  }
  float* value_mean = array2_mean_array_f(value, boards, players);
  array2_free_f(value, boards);
  return value_mean;
}
