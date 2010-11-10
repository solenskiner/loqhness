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


#ifndef LOQ_TYPES_HPP
#define LOQ_TYPES_HPP

#include <QLabel>
#include <QGridLayout>
#include <QProgressBar>

extern "C" {
  #include <poker-eval/poker_defs.h>
}

static QString strTypes[9] = {"High card:", "Pair:", "Two pair:", "Three of a kind:", "Straight:", "Flush:", "Full house:", "Four of a Kind:", "Straightflush:"};

class loq_types {
  public:
    QLayout* getLayout();
    loq_types(QString name, QWidget * parent, bool showLabels);
    void clear();
    void update(float * item);
    ~loq_types();
  private:
    QGridLayout * layout;
    QLabel * label;
    QLabel * lblTypes[9];
    QProgressBar * barTypes[9];
  //
};

class loq_draws : public QWidget {
  Q_OBJECT
  public:
    QLayout* getLayout();
    loq_draws(QWidget * parnt, bool showLabels);
    void update(StdDeck_CardMask* board, int boards, StdDeck_CardMask * player, int hands, int flags);
    ~loq_draws();
     void clear();
  signals:
  private slots:

  private:
    loq_types * streets[3];
    QHBoxLayout * layout;
  //
};

class loq_board : public QWidget {
  Q_OBJECT
  public:
    loq_board(QWidget * parnt, bool showLabels);
    QLayout* getLayout();
    void update(int ** board_enumed_hand, int boards, int * board_enumed_hands);
    void clear();
  signals:
  private slots:

  private:
    loq_types * street;
  //
};

#endif // LOQ_TYPES_HPP
