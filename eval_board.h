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

#ifndef _eval_board_h
  #define _eval_board_h

#include <poker-eval/poker_defs.h>
#include "eval_data.h"

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

#include "eval_single.h"

#include "misc_helpers.h"

int * enumerate_all_hands( StdDeck_CardMask * board, int * out_size);
int** enumerate_all_hands_boards(int ** board, int * boards, int** out_size);

float find_hand_worth_i(int spec_value, int* board_hand_value, int board_hand_values);
float find_hand_worth(StdDeck_CardMask * hand, StdDeck_CardMask * board, int* board_hand_value, int board_hand_values);
float find_hands_worth(StdDeck_CardMask * hand, int hands, StdDeck_CardMask * board, int* board_hand_value, int board_hand_values);
float find_hands_worth_boards(StdDeck_CardMask * hand, int hands, StdDeck_CardMask** board, int boards, int** board_hand_value, int* board_hand_values);
// SUPER IMPORTANT board is the same board used to calculate all_hand
float* find_hands_worth_boards_players(StdDeck_CardMask ** hand, int* hands, int players, StdDeck_CardMask** board, int boards, int** board_hand_value, int* board_hand_values);

int* filter_hands_type(int type, int* hand, int hands, int * out_size);

float * eval_distribution(int * board_hand_value);
float* eval_distribution_boards(int ** board_hand_value, int * boards);

#endif
