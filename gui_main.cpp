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

string itos(int a) {
  string sign = a<0?"-":"";
  string result = a>0?string(1,(a%10+'0')):string(1,((a=-a)%10+'0'));
  (a/=10)>0?result=itos(a)+result:result;
  return sign+result;
}

loqhness::loqhness() {
  mainLayout = new QVBoxLayout(this);
  mainLayout->setSpacing(2);

  linesLayout = new QGridLayout();
  linesLayout->setSpacing(2);
  string text[5] = {"", "Cards:", "Equity:", "On-Board %:", "In-Class %:"};
  for (int i = 0; i < 5; i++) {
    labels[i] = new QLabel(QString::fromStdString(text[i]), this);
    linesLayout->addWidget(labels[i], 0, i);
  }
  for (int i = 0; i < ACTORS; i++) {
    lblActors[i] = new QLabel(this);
    lblActors[i]->setText(QString::fromStdString("Player &" + itos(i+1) + ":"));
    linCards[i] = new QLineEdit(this);
    //linCards[i]->setValidator(&validateCards);
    lblActors[i]->setBuddy(linCards[i]);
    barEquity[i] = new QProgressBar(this);
    barBoardStr[i] = new QProgressBar(this);
    barClassStr[i] = new QProgressBar(this);
    linesLayout->addWidget(lblActors[i], i+1, 0);
    linesLayout->addWidget(linCards[i], i+1, 1);
    linesLayout->addWidget(barEquity[i], i+1, 2);
    linesLayout->addWidget(barBoardStr[i], i+1, 3);
    linesLayout->addWidget(barClassStr[i], i+1, 4);
  }
  lblBoard = new QLabel("&Board:", this);
  linBoard = new QLineEdit(this);
  lblBoard->setBuddy(linBoard);
  linesLayout->addWidget(lblBoard, ACTORS+1, 0);
  linesLayout->addWidget(linBoard, ACTORS+1, 1, 1, 4);
  mainLayout->addLayout(linesLayout);

  typesLayout = new QHBoxLayout();
  wgtDraws = new loq_draws(this, true);
  wgtBoard = new loq_board(this, false);
  typesLayout->addLayout(wgtDraws->getLayout());
  typesLayout->addLayout(wgtBoard->getLayout());
  mainLayout->addLayout(typesLayout);

  calcLayout= new QHBoxLayout();
  calcLayout->setSpacing(2);
  calcDraws = new QPushButton("Calculate player 1 &draws", this);
  calcBoard = new QPushButton("Calculate d&istribution", this);
  calcEquity = new QPushButton("Calculate &equity", this);
  calcLayout->addWidget(calcDraws);
  calcLayout->addWidget(calcBoard);
  calcLayout->addWidget(calcEquity);
  mainLayout->addLayout(calcLayout);

  connect(linBoard, SIGNAL(editingFinished()), this, SLOT(updateBoard()));
  for (int i = 0; i < ACTORS; i++) {
    connect(linCards[i], SIGNAL(editingFinished()), this, SLOT(updateCards()));
  }

  connect(calcBoard,  SIGNAL(clicked()), this, SLOT(calcBoard_clicked()));
  connect(calcDraws,  SIGNAL(clicked()), this, SLOT(calcDraws_clicked()));
  connect(calcEquity, SIGNAL(clicked()), this, SLOT(calcEquity_clicked()));

  hands = (int*) malloc(sizeof(int) * ACTORS);
  player = (StdDeck_CardMask**) malloc(sizeof(StdDeck_CardMask*) * ACTORS);
  for (int i = 0; i < ACTORS; i++) {
    player[i] = (StdDeck_CardMask*) malloc(sizeof(StdDeck_CardMask));
  }
  board = (StdDeck_CardMask*) malloc(sizeof(StdDeck_CardMask));
}


void loqhness::calcBoard_clicked() {
  board_all_hand = enumerate_all_hands_boards(boards, board_all_hands);

  float* players_board_worth = find_hands_worth_boards(player[i], hands[i], board, boards, boards_all_hand, board_all_hands);

  filter

  int i = 0;
  for (; i < players; i++) {
    barBoardStr[i]->setValue((int) players_board_worth[i] * 100 + 0.5);
    barBoardStr[i]->setEnabled(true);

    barBoardStr[i]->setValue((int) players_board_worth_class[i] * 100 + 0.5);
    barBoardStr[i]->setEnabled(true);
  }
  for (; i < ACTORS; i++) {
    barBoardStr[i]->setEnabled(false);
  }
  wgtBoard->update(board, boards);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void loqhness::calcDraws_clicked() {
  wgtDraws->update(board, boards, player[0], hands[0], EVALFLAGS);
}

void loqhness::barEquities_clear() {
  for (int i = 0; i < ACTORS; i++) {
    barEquity[i]->setEnabled(false);
  }
}
void loqhness::barEquities_update(float* equities, int plyers) {
  int i;
  for (i = 0; i < plyers; i++) {
    barEquity[i]->setEnabled(true);
    barEquity[i]->setValue((int)(equities[i] * 100 + 0.5));
  }
  for (i; i < ACTORS; i++) {
    barEquity[i]->setEnabled(false);
  }
}

void loqhness::calcEquity_clicked() {
  if (boards == 0) {
    boards = 1;
    StdDeck_CardMask_RESET(board[0]);
  }
  float * equities = evaluate_equity_players_hands_boards(player, hands, players, board, boards, EVALFLAGS | RIVER);
  barEquities_update(equities, players);
  free(equities);
}

void loqhness::updateBoard() {
  free(board);
  board = parse(const_cast<char*>(linBoard->text().toStdString().c_str()), &boards);
  wgtBoard->clear();
}
void loqhness::updateCards() {
  for (int i = 0; i < ACTORS; i++) {
    free(player[i]);
    player[i] = parse(const_cast<char*>(linCards[i]->text().toStdString().c_str()), &(hands[i]));
  }
  for (players = 0; players < ACTORS; players++) {
    if (hands[players] == 0) break;
  }
  wgtDraws->clear();
}

loqhness::~loqhness() {
  free(hands);
  array2_free_cardmask(player, ACTORS);
  free(board);
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  loqhness * dlgMain = new loqhness;
  dlgMain->show();
  int r = app.exec();
  delete dlgMain;
  return r;
}
