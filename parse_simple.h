/***************************************************************************
 *   Copyright (C) 2010 by Henrik Asp					   *
 *   solenskiner@gmail.com						   *
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

#ifndef PARSE_SIMPLE_H
#define PARSE_SIMPLE_H

#include <poker-eval/poker_defs.h>
#include <assert.h>
#include <string.h>
#include "misc_toksplit.h"
#include "misc_helpers.h"

int to_suit(char chr);
int StdDeck_Rank_from_INT(int num);
int to_rank(char chr);

void array2_char_alloc_range(char** arr, int size, int begin, int end);
void array2_char_free_range(char** arr, int begin, int end);

char** parse_string_array_strings(char * str, int *res_x);
StdDeck_CardMask parse_string_cardmask(char* str);
StdDeck_CardMask* parse(char* str, int* hands);

#endif // PARSE_SIMPLE_H
