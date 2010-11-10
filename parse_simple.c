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

#include "parse_simple.h"
#include <poker-eval/poker_defs.h>
#include <assert.h>
#include <string.h>
#include "misc_toksplit.h"
#include "misc_helpers.h"

int to_suit(char chr) {
  if      (chr == 's') return StdDeck_Suit_SPADES;
  else if (chr == 'h') return StdDeck_Suit_HEARTS;
  else if (chr == 'd') return StdDeck_Suit_DIAMONDS;
  else if (chr == 'c') return StdDeck_Suit_CLUBS;
  else                 return 'E';
}
int StdDeck_Rank_from_INT(int num){
  return (num - (2 - StdDeck_Rank_FIRST));
}
int to_rank(char chr) {
       if ( (((int) (chr)) - 48 <= 9)
         && (((int) (chr)) - 48 >= 2) ) return StdDeck_Rank_from_INT(((int) chr) - 48);
  else if (chr == 'A')                  return StdDeck_Rank_ACE;
  else if (chr == 'K')                  return StdDeck_Rank_KING;
  else if (chr == 'Q')                  return StdDeck_Rank_QUEEN;
  else if (chr == 'J')                  return StdDeck_Rank_JACK;
  else if (chr == 'T')                  return StdDeck_Rank_TEN;
  else                                  return 'E';
}


void array2_char_alloc_range(char** arr, int size, int begin, int end) {
  for (int i = begin; i < end; i++) {
    arr[i] = (char*) malloc(sizeof(char) * size);
  }
}
void array2_char_free_range(char** arr, int begin, int end) {
  for (int i = begin; i < end; i++) {
    free(arr[i]);
  }
}

char** parse_string_array_strings(char * str, int *res_x) {
  int buf_inner = 11;
  int step_size = 10;
  int buf_size = step_size;
  char** res = (char**) malloc(sizeof(char*) * buf_size);
  array2_char_alloc_range(res, buf_inner, 0, buf_size);
  int i = 0;
  char* str_ptr = str;
  while (*str_ptr != '\0') {
    if (i == buf_size) {
      buf_size += step_size;
      res = (char**) realloc(res, sizeof(char*) * buf_size);
      array2_char_alloc_range(res, buf_inner, i, buf_size);
    }
    str_ptr = toksplit(str_ptr, ',', res[i], buf_inner - 1);
    i++;
  }
  array2_char_free_range(res, i, buf_size);
  res = (char**) realloc(res, sizeof(char*) * i);
  *res_x = i;
  return res;
}

StdDeck_CardMask parse_string_cardmask(char* str) {
  StdDeck_CardMask hand;
  StdDeck_CardMask_RESET(hand);
  int card_index;
  assert(even(strlen(str)));
  for (int i = 0; i < (int) strlen(str); i+=2) {
    card_index = StdDeck_MAKE_CARD(to_rank(str[i]), to_suit(str[i+1]));
    hand = cardmask_OR(hand, StdDeck_MASK(card_index));
  }
  assert( (int) (strlen(str) / 2) == StdDeck_numCards(hand));
  return hand;
}

StdDeck_CardMask* parse(char* str, int* hands) {
  char** str_hand = parse_string_array_strings(str, hands);
  StdDeck_CardMask* hand = (StdDeck_CardMask*) malloc(sizeof(StdDeck_CardMask) * *hands);
  if (*hands > 0) {
    for (int i = 0; i < *hands; i++) {
      hand[i] = parse_string_cardmask(str_hand[i]);
    }
  }
  return hand;
}
