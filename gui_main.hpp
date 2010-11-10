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

string itos(int a);

class loqhness : public QDialog {
  Q_OBJECT
  public:
    loqhness();
    ~loqhness();
  signals:
  private slots:
    void updateBoard();
    void updateCards();
    void calcBoard_clicked();
    void calcDraws_clicked();
    void calcEquity_clicked();
  private:
    QVBoxLayout * mainLayout;

    QGridLayout * linesLayout;
    QLabel * labels[5];
    QLabel * lblActors[ACTORS];
    QLineEdit * linCards[ACTORS];
    QProgressBar * barEquity[ACTORS];
    QProgressBar * barBoardStr[ACTORS];
    QProgressBar * barClassStr[ACTORS];
    QLabel * lblBoard;
    QLineEdit * linBoard;

    void barEquities_clear();
    void barEquities_update(float* equities, int players);

    QHBoxLayout * typesLayout;
    loq_draws * wgtDraws;
    loq_board * wgtBoard;

    QHBoxLayout * calcLayout;
    QPushButton * calcBoard;
    QPushButton * calcDraws;
    QPushButton * calcEquity;

    StdDeck_CardMask** player;
    int* hands;
    int players;
    StdDeck_CardMask* board;
    int boards;
    int** board_all_hand; //[boards][1326]
  //
};

#endif // LOQHNESS_HPP
