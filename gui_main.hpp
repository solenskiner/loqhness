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
#include <QTabWidget>
#include <QLabel>
#include <QGridLayout>
#include <QProgressBar>
#include <QSpacerItem>

extern "C" {
  #include <poker-eval/poker_defs.h>
  #include "eval_defines.h"
}

#define EVALFLAGS ITERATIONS_SET(10000)
#define ACTORS 9

/// subclass of QLineEdit for focusIn(int) signal, for player cards, matches tab index in tabs
class line_edit : public QLineEdit {
  Q_OBJECT
  public:
    line_edit(QWidget* _parent = 0) { _n++; _i = _n; }
  signals:
  void focusIn(int _i);
  public slots:
  protected:
    void focusInEvent(QFocusEvent * _event) { emit focusIn(_i); }
  private:
  int _i;
  static int _n;
};

/// Widget for displaying hand classes as progressbars.
class hand_classes_bars {
  public:
    QLayout* getLayout();
    hand_classes_bars(QString name, QWidget * parent, bool showLabels);
    void clear();
    void update(float * item);
    ~hand_classes_bars();
  private:
    QGridLayout * layout;
    QLabel * label;
    QLabel * labels[9];
    QProgressBar * bars[9];
  //
};

/// Widget for displaying draws.
class draws_bars : public QWidget {
  Q_OBJECT
  public:
    QLayout* getLayout();
    draws_bars(QWidget * parent, bool showLabels);
    void update(StdDeck_CardMask* board, int boards, StdDeck_CardMask * player, int hands, int flags);
    ~draws_bars();
     void clear();
  signals:
  private slots:

  private:
    hand_classes_bars * streets[3];
    QHBoxLayout * layout;
  //
};

/// Widget which contains and shows player data
class tab_player_t : public QWidget {
  Q_OBJECT
  public:
    tab_player_t(QWidget* _parent);
    void update(float _equity, int* _boards, int _boards_num);
    void clear();
  signals:
    void hands_changed(int _hands_num);
    void hands_changed(QString _hands_num);
  public slots:
    void update_hands();
  private:
    QString          old_hands;
    StdDeck_CardMask * hands;
    int hands_num;

    QGridLayout * layout;

    QLabel      * equity_label;
    QProgressBar* equity;

    QLabel      * strenght_board_label;
    QProgressBar* strenght_board;

    QLabel      * draws_bar_label;
    QProgressBar* draws_bar;

    draws_bars  * draws;

    QSpacerItem * spacer;
  //
  friend class tab_overview_t;
};

/// Widget which contains and shows board data
class tab_board_t : public QWidget {
  Q_OBJECT
  public:
    tab_board_t(QWidget* _parent);
    void clear();
  signals:
    void cards_changed(int _hands_num);
    void cards_changed(QString _hands_num);
  public slots:
    void update_cards();
  private:
    QString          old_cards;
    StdDeck_CardMask * cards;
    int cards_num;

    QGridLayout * layout;

    QSpacerItem * spacer;
  //
  friend class tab_overview_t;
};

/// Widget which shows an overview of several tab_player_t widgets.
class tab_overview_t : public QWidget {
  Q_OBJECT
  public:
    tab_overview_t(QWidget* _parent, tab_player_t** tab_player, int players);
    ~tab_overview_t();
  signals:
  private slots:
  private:
    QGridLayout * layout;

    QLabel      * equity_label;
    QLabel      * strength_label;
    QLabel      * draws_label;
    QLabel      * hands_num_label;

    QLabel      ** player_label;
    QProgressBar** equity;
    QProgressBar** strength;
    QProgressBar** draws;
    QLabel      ** hands_num;

    QLabel      * board_label;
    QLabel      * boards_num_label;
    QLabel      * combs_label;
    QLabel      * combs_num_label;

    QSpacerItem * spacer;
  //
};

/// Application main dialog
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

    QGridLayout * layout;
    QLabel * cards_label;
    QLabel ** player_label;
    line_edit ** hands_edit;

    QLabel * board_label;
    line_edit * board_edit;

    QTabWidget* tabs;
    tab_overview_t* tab_overview;
    tab_player_t** tab_player;
    tab_board_t* tab_board;

    QHBoxLayout * calc;
    QPushButton * calc_board;
    QPushButton * calc_draws;
    QPushButton * calc_equity;

  //
};

#endif // LOQHNESS_HPP
