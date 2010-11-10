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

#ifndef BITOPERATIONS_H
#define BITOPERATIONS_H

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) | (m))
#define bit_clear(p,m) ((p) & ~(m))
#define bit_flip(p,m) ((p) ^ (m))
#define bit_invert(p) (~(p))
//#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m)) //lolwut?
#define BIT(x) ((unsigned int)0x01 << (x))
#define BIT_LONG(x) ((unsigned long)0x00000001 << (x))

int bit_count(int flag);
int BITS(int start, int end);

#endif // BITOPERATIONS_H
