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

extern "C" {
  #include <math.h>

  #include <poker-eval/poker_defs.h>
  #include <poker-eval/deck_std.h>

  #include "eval_board.h"
  #include "eval_equity.h"
  #include "eval_single.h"
  #include "eval_defines.h"

  #include "parse_simple.h"
}

/// int -> Std::string
std::string itos(int a) {
  std::string sign = a<0?"-":"";
  std::string result = a>0?std::string(1,(a%10+'0')):std::string(1,((a=-a)%10+'0'));
  (a/=10)>0?result=itos(a)+result:result;
  return sign+result;
}

/// sets up main gui and signal handlers.
loqhness::loqhness() {
  main = new QVBoxLayout(this);
  main->setSpacing(2);

  lines = new QGridLayout();
  lines->setSpacing(2);
  std::string text[5] = {"", "Cards:", "Equity:", "On-Board %:", "In-Class %:"};
  for (int i = 0; i < 5; i++) {
    labels[i] = new QLabel(QString::fromStdString(text[i]), this);
    lines->addWidget(labels[i], 0, i);
  }
  for (int i = 0; i < ACTORS; i++) {
    player_label[i] = new QLabel(this);
    player_label[i]->setText(QString::fromStdString("Player &" + itos(i+1) + ":"));
    hands_edit[i] = new QLineEdit(this);
    //hands_edit[i]->setValidator(&validateCards);
    player_label[i]->setBuddy(hands_edit[i]);
    equity[i] = new QProgressBar(this);
    str_on_board[i] = new QProgressBar(this);
    str_in_class[i] = new QProgressBar(this);
    lines->addWidget(player_label[i], i+1, 0);
    lines->addWidget(hands_edit[i], i+1, 1);
    lines->addWidget(equity[i], i+1, 2);
    lines->addWidget(str_on_board[i], i+1, 3);
    lines->addWidget(str_in_class[i], i+1, 4);
  }
  board_label = new QLabel("&Board:", this);
  board_edit = new QLineEdit(this);
  board_label->setBuddy(board_edit);
  lines->addWidget(board_label, ACTORS+1, 0);
  lines->addWidget(board_edit, ACTORS+1, 1, 1, 4);
  main->addLayout(lines);

  types = new QHBoxLayout();
  wgtDraws = new loq_draws(this, true);
  wgtBoard = new loq_board(this, false);
  types->addLayout(wgtDraws->getLayout());
  types->addLayout(wgtBoard->getLayout());
  main->addLayout(types);

  calc= new QHBoxLayout();
  calc->setSpacing(2);
  calc_draws = new QPushButton("Calculate player 1 &draws", this);
  calc_board = new QPushButton("Calculate d&istribution", this);
  calc_equity = new QPushButton("Calculate &equity", this);
  calc->addWidget(calc_draws);
  calc->addWidget(calc_board);
  calc->addWidget(calc_equity);
  main->addLayout(calc);

  connect(board_edit, SIGNAL(editingFinished()), this, SLOT(update_board()));
  for (int i = 0; i < ACTORS; i++) {
    connect(hands_edit[i], SIGNAL(editingFinished()), this, SLOT(update_cards()));
  }

  connect(calc_board,  SIGNAL(clicked()), this, SLOT(calc_board_clicked()));
  connect(calc_draws,  SIGNAL(clicked()), this, SLOT(calc_draws_clicked()));
  connect(calc_equity, SIGNAL(clicked()), this, SLOT(calc_equity_clicked()));

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

void loqhness::calc_draws_clicked() {
  wgtDraws->update(board, boards, player[0], hands[0], EVALFLAGS);
}
void loqhness::update_equities(float* equities, int plyers) {
  int i;
  for (i = 0; i < plyers; i++) {
    equity[i]->setEnabled(true);
    equity[i]->setValue((int)(equities[i] * 100 + 0.5));
  }
  for (; i < ACTORS; i++) {
    equity[i]->setEnabled(false);
  }
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
  wgtBoard->update(board_enumed_hand, boards, board_enumed_hands);
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
