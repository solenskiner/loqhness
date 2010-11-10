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

#ifndef EVAL_DEFINES_H
#define EVAL_DEFINES_H

#include "misc_bitoperations.h"

//flags ...iiiiwccc
#define FLOP    3
#define TURN    4
#define RIVER   5
#define STREETS BITS(2, 0)

#define _BOARDS 2598960       // 52 choose 5

#define _ITER_LENGTH                12
#define _ITER_POS                   4
#define ITERATIONS                  BITS(_ITER_LENGTH + _ITER_POS - 1, _ITER_POS)
#define _ITER_MUL                   ( (float) _BOARDS / (float) (2 << _ITER_LENGTH) )
#define ITERATIONS_SET(x)           ( ( (int)( (float) x / (float) _ITER_MUL + 0.5))   << _ITER_POS)
#define ITERATIONS_GET(x)           ( (int) ( ( ( (x) & ITERATIONS) >> _ITER_POS) * _ITER_MUL))
#define ITERATIONS_DIVIDE(flags, x) ( (ITERATIONS_SET( (int) ( (float) ITERATIONS_GET(flags) / (float) (x) ) )) | ( (flags) & bit_invert(ITERATIONS)))

#endif // EVAL_DEFINES_H
