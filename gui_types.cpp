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

#include "gui_types.hpp"

extern "C" {
  #include "eval_single.h"
  #include "eval_board.h"
  #include "misc_helpers.h"
}

///////    TYPES    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

loq_types::loq_types (QString name, QWidget * parnt, bool showLabels) {
  layout = new QGridLayout();
  label = new QLabel(name, parnt);
  if (showLabels) {
    layout->addWidget(label, 1, 2);
    for (int i = 0; i < 9; i++) {
      lblTypes[i] = new QLabel(strTypes[i], parnt);
      layout->addWidget(lblTypes[i], i+2, 1);
      barTypes[i] = new QProgressBar(parnt);
      layout->addWidget(barTypes[i], i+2, 2);
    }
  } else {
    for (int i = 0; i < 9; i++) {
      layout->addWidget(label, 1, 1);
      barTypes[i] = new QProgressBar(parnt);
      layout->addWidget(barTypes[i], i+2, 1);
    }
  }
  layout->setSpacing(2);
  loq_types::clear();
}
void loq_types::clear() {
  for (int i = 0; i < 9; i++) {
    barTypes[i]->setEnabled(false);
  }
}
void loq_types::update(float * item) {
  for (int i = 0; i < 9; i++) {
    barTypes[i]->setEnabled(true);
    barTypes[i]->setValue(int(item[i] * 100 + 0.5));
  }
}
QLayout* loq_types::getLayout () { return layout; }
loq_types::~loq_types() { delete layout; }

///////    DRAWS    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

loq_draws::loq_draws(QWidget * parnt, bool showLabels) {
  QString name[3] = {"To hit flop:", "To hit turn:", "To hit river:"};
  layout = new QHBoxLayout();
  if (showLabels) {
    streets[0] = new loq_types(name[0], parnt, true);
    streets[1] = new loq_types(name[1], parnt, false);
    streets[2] = new loq_types(name[2], parnt, false);
  } else {
    for (int i = 0; i < 3; i++) {
      streets[i] = new loq_types(name[i], parnt, false);
    }
  }
  for (int i = 0; i < 3; i++) {
    layout->addLayout(streets[i]->getLayout());
  }
  layout->setSpacing(2);
}

void loq_draws::update(StdDeck_CardMask* board, int boards, StdDeck_CardMask * player, int hands, int flags) {
  if (boards == 0) {
    board = (StdDeck_CardMask*) realloc(board, sizeof(StdDeck_CardMask));
    boards = 1;
    StdDeck_CardMask_RESET(*board);
  }

  int evalargs[3] = {FLOP | flags, TURN | flags, RIVER | flags};

  float ** results = (float**) malloc(sizeof(float*) * 3);
  for (int i = 0; i < 3; i++) {
    results[i] = evaluate_hands_draws_boards(board, boards, player, hands, evalargs[i] );
  }

  for (int i = 0; i < 3; i++) {
    streets[i]->clear();
    streets[i]->update(results[i]);
  }

  array2_free_f(results, 3);
}

void loq_draws::clear() {
  for (int i = 0; i < 3; i++) {
    streets[i]->clear();
  }
}
QLayout* loq_draws::getLayout() { return layout; }
loq_draws::~loq_draws() { delete layout; }

///////    BOARD    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

loq_board::loq_board(QWidget * parnt, bool showLabels) {
    street = new loq_types("Board distribution:", parnt, showLabels);
}

void loq_board::update(int ** board_enumed_hand, int boards, int * board_enumed_hands) {
  float * results = eval_distribution_boards(board_enumed_hand,boards, board_enumed_hands);
  street->update(results);
  free(results);
}

void loq_board::clear() { street->clear(); }
QLayout* loq_board::getLayout() { return street->getLayout(); }
