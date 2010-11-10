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

#ifndef _eval_simple_h
#define _eval_simple_h

#include <poker-eval/poker_defs.h>
#include <poker-eval/deck_std.h>

#include "eval_defines.h"

int evaluate_hand(StdDeck_CardMask * hand, StdDeck_CardMask * board);

int evaluate_hand_type(StdDeck_CardMask * hand, StdDeck_CardMask * board);

float * evaluate_hand_draws(StdDeck_CardMask * board, StdDeck_CardMask * hand, int flags);

float * evaluate_hands_draws(StdDeck_CardMask * board, StdDeck_CardMask * hand, int hands, int flags);

float * evaluate_hands_draws_boards(StdDeck_CardMask * board, int boards, StdDeck_CardMask * hand, int hands, int flags);

#endif
