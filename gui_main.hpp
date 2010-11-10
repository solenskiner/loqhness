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

#ifndef LOQHNESS_HPP
#define LOQHNESS_HPP

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include "gui_types.hpp"

extern "C" {
  #include <poker-eval/poker_defs.h>
  #include "eval_defines.h"
}

#define EVALFLAGS ITERATIONS_SET(10000)
#define ACTORS 9

std::string itos(int a);

class loqhness : public QDialog {
  Q_OBJECT
  public:
    loqhness();
    ~loqhness();
  signals:
  private slots:
    void calc_board_clicked();
    void calc_draws_clicked();
    void calc_equity_clicked();
    void update_board();
    void update_cards();
  private:
    void update_equities(float* equities, int players);
    void clear_equities();

    QVBoxLayout * main;

    QGridLayout * lines;
    QLabel * labels[5];

    QHBoxLayout * types;
    loq_draws * wgtDraws;
    loq_board * wgtBoard;

    QHBoxLayout * calc;
    QPushButton * calc_board;
    QPushButton * calc_draws;
    QPushButton * calc_equity;

    QProgressBar * equity[ACTORS];
    QProgressBar * str_on_board[ACTORS];
    QProgressBar * str_in_class[ACTORS];
    QLabel * player_label[ACTORS];
    QLineEdit * hands_edit[ACTORS];
    StdDeck_CardMask** player;
    int* hands;
    int players;

    QLabel * board_label;
    QLineEdit * board_edit;
    StdDeck_CardMask* board;
    int boards;
    int* board_enumed_hands;
    int** board_enumed_hand; // evaluated values
  //
};

#endif // LOQHNESS_HPP
