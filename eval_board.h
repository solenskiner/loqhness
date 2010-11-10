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

int * enumerate_all_hands( StdDeck_CardMask * board, int * out_size);

int** enumerate_all_hands_boards(StdDeck_CardMask * board, int boards, int* out_size);

float * eval_distribution(int * board_enumed_hand, int board_enumed_hands);

float* eval_distribution_boards(int ** board_enumed_hand, int boards, int * board_enumed_hands);

#endif
