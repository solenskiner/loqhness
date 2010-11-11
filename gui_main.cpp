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

#include <QApplication>
#include "gui_main.hpp"
//#include "gui_types.hpp"

extern "C" {
  #include <poker-eval/poker_defs.h>

  #include "eval_board.h"
  #include "eval_equity.h"
  #include "eval_single.h"
  #include "eval_defines.h"

  #include "parse_simple.h"
}

static QString classes_names[9] = {"High card:", "Pair:", "Two pair:", "Three of a kind:", "Straight:", "Flush:", "Full house:", "Four of a Kind:", "Straightflush:"};

std::string itos(int a);

/// int -> Std::string
std::string itos(int a) {
  std::string sign = a<0?"-":"";
  std::string result = a>0?std::string(1,(a%10+'0')):std::string(1,((a=-a)%10+'0'));
  (a/=10)>0?result=itos(a)+result:result;
  return sign+result;
}

//// PLAYER TAB //////////////////////////////////////////////////

tab_player_t::tab_player_t(QWidget* _parent) {
  layout         = new QGridLayout(this);
  layout->setSpacing(2);

  equity_label   = new QLabel("Equity: ", this);
  layout->addWidget(equity_label, 0, 0);

  equity         = new QProgressBar(this);
  layout->addWidget(equity, 0, 1);

  strenght_board_label   = new QLabel("Strength: ", this);
  layout->addWidget(strenght_board_label, 1, 0);

  strenght_board = new QProgressBar(this);
  layout->addWidget(strenght_board, 1, 1);

  draws          = new draws_bars(this, true);
  layout->addLayout(draws->getLayout(), 3, 0, 1, 2);

  spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  layout->addItem(spacer, 4, 0);
}
void tab_player_t::clear() {
  equity->setEnabled(false);
  strenght_board->setEnabled(false);
  draws->clear();
}
void tab_player_t::update(float _equity, int* _boards, int _boards_num) {
  equity->setValue(_equity);
  equity->setEnabled(true);
  //strenght_board->setValue(_board);
  //strenght_board->setEnabled(true);
  //_draws->update();
}

void tab_player_t::update_hands() {
  if (((QLineEdit*) (QObject::sender()))->text() == old_hands)
    goto t_p_upd_end;

  old_hands = ((QLineEdit*) QObject::sender())->text();
  free(hands);
  hands =  parse(const_cast<char*>(old_hands.toStdString().c_str()), &hands_num);

  clear();
  emit hands_changed(hands_num);

  t_p_upd_end:
  return;
}



tab_overview_t::tab_overview_t(QWidget *_parent, tab_player_t ** tab_player, int players) {
  layout = new QGridLayout(this);
  layout->setSpacing(2);

  equity_label = new QLabel("Equity: ", this);
  layout->addWidget(equity_label, 0, 1);
  strength_label = new QLabel("Strength: ", this);
  layout->addWidget(strength_label, 0, 2);
  strength_label = new QLabel("Draws: ", this);
  layout->addWidget(strength_label, 0, 3);
  hands_num_label = new QLabel("Hands: ", this);
  layout->addWidget(hands_num_label, 0, 4);

  player_label = new QLabel*[players];
  equity = new QProgressBar*[players];
  strength = new QProgressBar*[players];
  draws = new QProgressBar*[players];
  hands_num = new QLabel*[players];

  for (int i = 0; i < players; i++) {
    player_label[i] = new QLabel(QString::fromStdString("Player " + itos(i + 1) + ": "), this);
    layout->addWidget(player_label[i], i + 1, 0);

    equity[i] = new QProgressBar(this);
    layout->addWidget(equity[i], i + 1, 1);

    strength[i] = new QProgressBar(this);
    layout->addWidget(strength[i], i + 1, 2);

    draws[i] = new QProgressBar(this);
    layout->addWidget(draws[i], i + 1, 3);

    hands_num[i] = new QLabel(this);
    layout->addWidget(hands_num[i], i + 1, 4);

  }

  board_label = new QLabel("Boards: ", this);
  layout->addWidget(board_label, players + 2, 0);
  boards_num_label = new QLabel(this);
  layout->addWidget(boards_num_label, players + 2, 4);

  combs_label = new QLabel("Combinations: ", this);
  layout->addWidget(combs_label, players + 3, 0);
  combs_num_label = new QLabel(this);
  layout->addWidget(combs_num_label, players + 3, 4);

  spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  layout->addItem(spacer, players + 4, 0);
}
tab_overview_t::~tab_overview_t() {
  delete[] player_label;
  delete[] equity;
  delete[] strength;
  delete[] draws;
  delete[] hands_num;
}

//////////////////////////////////////////////////////////////////

/// sets up main gui and signal handlers.
loqhness::loqhness() {
  main = new QVBoxLayout(this);
  main->setSpacing(2);

  // set up UI line edits
  lines = new QGridLayout();
  lines->setSpacing(2);
  cards_label = new QLabel("Cards:", this);
  lines->addWidget(cards_label, 0, 1);
  for (int i = 0; i < ACTORS; i++) {
    player_label[i] = new QLabel(this);
    player_label[i]->setText(QString::fromStdString("Player &" + itos(i+1) + ":"));
    hands_edit[i] = new QLineEdit(this);
    player_label[i]->setBuddy(hands_edit[i]);
    lines->addWidget(player_label[i], i+1, 0);
    lines->addWidget(hands_edit[i], i+1, 1);
  }
  board_label = new QLabel("&Board:", this);
  board_edit = new QLineEdit(this);
  board_label->setBuddy(board_edit);
  lines->addWidget(board_label, ACTORS+1, 0);
  lines->addWidget(board_edit, ACTORS+1, 1);
  main->addLayout(lines);

  // set up UI displays
  tabs = new QTabWidget(this);
  for (int i = 0; i < ACTORS; i++) {
    tab_player[i] = new tab_player_t(this);
  }
  tab_overview = new tab_overview_t(this, tab_player, ACTORS);
  tabs->addTab(tab_overview, "Overview");
  for (int i = 0; i < ACTORS; i++) {
    tabs->addTab(tab_player[i], QString::fromStdString("Player " + itos(i + 1)));
  }
  main->addWidget(tabs);

  // set up UI buttons
  calc = new QHBoxLayout();
  calc->setSpacing(2);
  calc_draws = new QPushButton("Calculate &draws", this);
  calc_board = new QPushButton("Calculate d&istribution", this);
  calc_equity = new QPushButton("Calculate e&quity", this);
  calc->addWidget(calc_draws);
  calc->addWidget(calc_board);
  calc->addWidget(calc_equity);
  main->addLayout(calc);

  // connect signals and slots
  connect(board_edit, SIGNAL(editingFinished()), this, SLOT(update_board()));
  for (int i = 0; i < ACTORS; i++) {
    connect(hands_edit[i], SIGNAL(editingFinished()), this, SLOT(update_cards()));
  }
  connect(calc_board,  SIGNAL(clicked()), this, SLOT(calc_board_clicked()));
  connect(calc_draws,  SIGNAL(clicked()), this, SLOT(calc_draws_clicked()));
  connect(calc_equity, SIGNAL(clicked()), this, SLOT(calc_equity_clicked()));

  // init private data
  hands = (int*) malloc(sizeof(int) * ACTORS);
  player = (StdDeck_CardMask**) malloc(sizeof(StdDeck_CardMask*) * ACTORS);
  for (int i = 0; i < ACTORS; i++) {
    player[i] = (StdDeck_CardMask*) malloc(sizeof(StdDeck_CardMask));
  }
  board = (StdDeck_CardMask*) malloc(sizeof(StdDeck_CardMask));
}

/////////////////////////////////////////////////////////////////////////////////////////////

void loqhness::calc_board_clicked() {
  /*
  float* player_str_board = find_hands_worth_boards(player[i], hands[i], board, boards, boards_all_hand, board_all_hands);

  for (; i < players; i++) {
    str_on_board[i]->setValue((int) player_str_board[i] * 100 + 0.5);
    str_on_board[i]->setEnabled(true);

    str_on_board[i]->setValue((int) players_board_worth_class[i] * 100 + 0.5);
    str_on_board[i]->setEnabled(true);
  }
  for (; i < ACTORS; i++) {
    str_on_board[i]->setEnabled(false);
  }
*/
}

void loqhness::calc_draws_clicked() {/*
  draws->update(board, boards, player[0], hands[0], EVALFLAGS); */
}
void loqhness::update_equities(float* equities, int plyers) {/*
  int i;
  for (i = 0; i < plyers; i++) {
    equity[i]->setEnabled(true);
    equity[i]->setValue((int)(equities[i] * 100 + 0.5));
  }
  for (; i < ACTORS; i++) {
    equity[i]->setEnabled(false);
  }*/
}

void loqhness::calc_equity_clicked() {
  if (boards == 0) {
    boards = 1;
    StdDeck_CardMask_RESET(board[0]);
  }
  float * equities = evaluate_equity_players_hands_boards(player, hands, players, board, boards, EVALFLAGS | RIVER);
  update_equities(equities, players);
  free(equities);
}

void loqhness::update_board() {
  free(board);
  board = parse(const_cast<char*>(board_edit->text().toStdString().c_str()), &boards);
  array2_free_i(board_enumed_hand, boards);
  board_enumed_hand = enumerate_all_hands_boards(board, boards, board_enumed_hands);
  //distribution->update(board_enumed_hand, boards, board_enumed_hands);
}
void loqhness::update_cards() {
  for (int i = 0; i < ACTORS; i++) {
    free(player[i]);
    player[i] = parse(const_cast<char*>(hands_edit[i]->text().toStdString().c_str()), &(hands[i]));
  }
  for (players = 0; players < ACTORS; players++) {
    if (hands[players] == 0) break;
  }
}

///////    TYPES    ///////////////////////////////////////////////

hand_classes_bars::hand_classes_bars (QString name, QWidget * parnt, bool showLabels) {
  layout = new QGridLayout();
  label = new QLabel(name, parnt);
  if (showLabels) {
    layout->addWidget(label, 1, 2);
    for (int i = 0; i < 9; i++) {
      labels[i] = new QLabel(classes_names[i], parnt);
      layout->addWidget(labels[i], i+2, 1);
      bars[i] = new QProgressBar(parnt);
      layout->addWidget(bars[i], i+2, 2);
    }
  } else {
    for (int i = 0; i < 9; i++) {
      layout->addWidget(label, 1, 1);
      bars[i] = new QProgressBar(parnt);
      layout->addWidget(bars[i], i+2, 1);
    }
  }
  layout->setSpacing(2);
  hand_classes_bars::clear();
}
void hand_classes_bars::clear() {
  for (int i = 0; i < 9; i++) {
    bars[i]->setEnabled(false);
  }
}
void hand_classes_bars::update(float * item) {
  for (int i = 0; i < 9; i++) {
    bars[i]->setEnabled(true);
    bars[i]->setValue(int(item[i] * 100 + 0.5));
  }
}
QLayout* hand_classes_bars::getLayout () { return layout; }
hand_classes_bars::~hand_classes_bars() { delete layout; }

///////    DRAWS    ///////////////////////////////////////////////

draws_bars::draws_bars(QWidget * parent, bool showLabels) {
  QString name[3] = {"Flop:", "Turn:", "River:"};
  layout = new QHBoxLayout();
  if (showLabels) {
    streets[0] = new hand_classes_bars(name[0], parent, true);
    streets[1] = new hand_classes_bars(name[1], parent, false);
    streets[2] = new hand_classes_bars(name[2], parent, false);
  } else {
    for (int i = 0; i < 3; i++) {
      streets[i] = new hand_classes_bars(name[i], parent, false);
    }
  }
  for (int i = 0; i < 3; i++) {
    layout->addLayout(streets[i]->getLayout());
  }
  layout->setSpacing(2);
}

void draws_bars::update(StdDeck_CardMask* board, int boards, StdDeck_CardMask * player, int hands, int flags) {
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

void draws_bars::clear() {
  for (int i = 0; i < 3; i++) {
    streets[i]->clear();
  }
}
QLayout* draws_bars::getLayout() { return layout; }
draws_bars::~draws_bars() { delete layout; }

///////////////////////////////////////////////////////////////////

loqhness::~loqhness() {
  free(hands);
  array2_free_cardmask(player, ACTORS);
  free(board);
  array2_free_i(board_enumed_hand, boards);
  free(board_enumed_hands);
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  loqhness * dlgMain = new loqhness;
  dlgMain->show();
  return app.exec();
}
