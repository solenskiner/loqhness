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

#include "misc_helpers.h"
#include <stdlib.h>
#include <assert.h>
#include "misc_bool.h"

/////////////////////////////////////////////////////////////////////////////////////////////

int max_i(int a, int b) {
  if (a > b) return a;
  else return b;
}

int min_i(int a, int b) {
  if (a > b) return b;
  else return a;
}

float max_f(float a, float b) {
  if (a > b) return a;
  else return b;
}

float min_f(float a, float b) {
  if (a > b) return b;
  else return a;
}

/////////////////////////////////////////////////////////////////////////////////////////////

int even(int n) {
  if ( ( (int) ( ((float) n) / 2.0 + 0.5)) == ( (int) ( ((float) n) / 2.0))) return true;
  else return false;
}
int odd(int n) {
  if (even(n)) return false;
  else return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////

int compare_desc_f( const void* a, const void* b ) {
  if      ( *( (const float*) a) < *( (const float*) b) ) return  1;
  else if ( *( (const float*) a) > *( (const float*) b) ) return -1;
  else                                                    return  0;
}

int compare_desc_i( const void* a, const void* b ) {
  if      ( *( (const int*) a) < *( (const int*) b) ) return  1;
  else if ( *( (const int*) a) > *( (const int*) b) ) return -1;
  else                                                return  0;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void array_init_i(int elements, int value, int* array) {
  for (int i = 0; i < elements; i++) {
    array[i] = value;
  }
}
void array_init_f(int elements, float value, float* array) {
  for (int i = 0; i < elements; i++) {
    array[i] = value;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

void array_shift_left_f(int elements, float* array) {
  float temp = array[0];
  for (int i = 0; i < elements - 1; i++) {
    array[i] = array[i+1];
  }
  array[elements-1] = temp;
}

float array_sum_f(int elements, float* array) {
  float sum = 0;
  for (int i = 0; i < elements; i++) {
    sum += array[i];
  }
  return sum;
}

float array_sum_i(int elements, int* array) {
  int sum = 0;
  for (int i = 0; i < elements; i++) {
    sum += array[i];
  }
  return sum;
}

float array_mean_f(float * in, int x) {
  float out = 0;
  for (int i = 0; i < x; i++) {
    out += in[i] / x;
  }
  return out;
}

float* array2_mean_array_f(float ** in, int x, int y) {
  float* out = (float*) malloc(sizeof(float) * y);
  array_init_f(y, 0, out);
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      out[j] += in[i][j] / x;
    }
  }
  return out;
}

/////////////////////////////////////////////////////////////////////////////////////////////

//start is inclusive, end is exclusive
float* array_slice_f(int start, int end, float* array) {
  assert(start <= end);
  float* slice = (float*) malloc(sizeof(float) * end-start);
  for (int i = 0; i < end - start; i++) {
    slice[i] = array[i + start];
  }
  return slice;
}

// destructive!
float* array_append_f(int num1, float* array1, int num2, float* array2) {
  assert(num1 >= 0);
  assert(num2 >= 0);
  float* array = (float*) realloc(array1, sizeof(float) * (num1 + num2));
  for(int i = 0; i < num2; i++) {
    array[i+num1] = array2[i];
  }
  free(array2);
  return array;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void array_array_cartesian_array2_cardmask_recurse(int x, int xi, int* y, StdDeck_CardMask** A, int* newx, StdDeck_CardMask* temp_inner, StdDeck_CardMask** B) {
  if (xi < x) {
    for (int i = 0; i < y[xi]; i++) {
      temp_inner[xi] = A[xi][i];
      array_array_cartesian_array2_cardmask_recurse(x, xi + 1, y, A, newx, temp_inner, B);
    }
  } else {
    for (int i = 0; i < x; i++) {
      B[*newx][i] = temp_inner[i];
    }
    (*newx)++;
  }
}

StdDeck_CardMask** array_array_cartesian_array2_cardmask (StdDeck_CardMask** A, int x, int* y, int* newx) {
  *newx = 1;
  for (int i = 0; i < x; i++)
    *newx *= y[i];
  StdDeck_CardMask** B = (StdDeck_CardMask**) malloc(sizeof(StdDeck_CardMask*) * *newx);
  for (int i = 0 ; i < *newx; i++) {
     B[i] = (StdDeck_CardMask*) malloc(sizeof(StdDeck_CardMask) * x);
  }

  int xi = 0;
  StdDeck_CardMask* tmp_prod = (StdDeck_CardMask*) malloc(sizeof(StdDeck_CardMask) * x);
  array_array_cartesian_array2_cardmask_recurse(x, 0, y, A, &xi, tmp_prod, B);
  free(tmp_prod);

  return B;
}

/////////////////////////////////////////////////////////////////////////////////////////////

StdDeck_CardMask** array2_filter_array2_cardmask(StdDeck_CardMask** array, int x, int y, int* newx) {
  StdDeck_CardMask temp;
  int mark[x];
  array_init_i(x, false, mark);
  for (int i = 0; i < x; i++) {
    StdDeck_CardMask_RESET(temp);
    for (int j = 0; j < y; j++) {
      StdDeck_CardMask_OR(temp, temp, array[i][j]);
    }
    if (StdDeck_numCards(temp) != 2*y) mark[x] = true;
  }
  *newx = x - array_sum_i(x, mark);
  StdDeck_CardMask** results = (StdDeck_CardMask**) malloc(sizeof(StdDeck_CardMask*) * *newx);
  for (int i = 0; i < x; i++) {
    results[i] = (StdDeck_CardMask*) malloc(sizeof(StdDeck_CardMask) * y);
  }
  for(int i = 0, j = 0; i < x; i++) {
    if (mark[i] == false) {
      for (int k = 0; k < y; k++) {
        results[j][k] = array[i][k];
      }
      j++;
    }
  }
  return results;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void array2_free_f(float ** arr, int x) {
  for (int i = 0; i < x; i++) {
    free(arr[i]);
    arr[i] = NULL;
  }
  free(arr);
  arr = NULL;
}
void array2_free_i(int ** arr, int x) {
  for (int i = 0; i < x; i++) {
    free(arr[i]);
    arr[i] = NULL;
  }
  free(arr);
  arr = NULL;
}
void array2_free_cardmask(StdDeck_CardMask ** arr, int x) {
  for (int i = 0; i < x; i++) {
    free(arr[i]);
    arr[i] = NULL;
  }
  free(arr);
  arr = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////

StdDeck_CardMask cardmask_OR(StdDeck_CardMask a, StdDeck_CardMask b) {
  StdDeck_CardMask r;
  StdDeck_CardMask_OR(r, a, b);
  return r;
}
