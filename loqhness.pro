# fixa find_hand_worth
# 4. fixa montecarlo för bord och spelare. undersök lib för montecarlo, combinationer
#    auto enum->montecarlo för stora beräkningar.
# 5. fixa kort-generator ( [13][13]+filter? )
# 6. GUI
# version 0.1 rel?
# 7. CLI
# lägg till weighting när räknar ut medelvärden. för punkt 1
# x. bryt ut generera kompletta bord från evaluera kompletta bord
# weighting för punkt x
# y. optimeringar
# semi-LUT semi-heurestik för PF evalueringar
# räkna ej på irrelevanta bord, beror på punkt x
# undersök isomorfismer
#valgrind is quite useful in combination with kcachegrind - the text output could be formatted better.
#gprof is good and easy to use. However it does not work well for multiple processes and threads.
#I've also found that oprofile is quite good, once you've learned how to use it.
#https://secure.wikimedia.org/wikibooks/en/wiki/Optimizing_C%2B%2B ?
# ???
# många uträkningar kastas nu bort, finn slöseriet och fixa det.
# tråda gui-evaluatorn
# version 0.2 rel? undersök möjlighet att sälja
# tråda evaluatorn
# börja med riktig AI, finn beyasiskt perfekta lösningar för turn-river spel
# efter 0.2 av gui-eval
# skriv program att undersöka situationer, använd databaskoppling från i²cm
# låt samma program skriva winholdem specifikationer för situationer
# solve pushfold för 3 spelare, i²cm för 3 spelare
# efter punkt y. solve push-fold för >3 spelare, i²cm för >3 spelare
# alternativt, för mer än 3 genom att kolla realworldresultat i databas ist för simulera
# undersök möjlighet att sälja i²cm
# rent i²cm program för realtime användning, hud, databas koppling (för att finna stackar etc)
TARGET = ./loqhness
TEMPLATE = app
qt = \
    core \
    gui
CONFIG = \
    qt \
#    gcc \
    debug
 #   release

DEPENDPATH += .
INCLUDEPATH += . /usr/include/poker-eval/
LIBRARY_PATH = /usr/lib
OBJECTS_DIR = /tmp/
MOC_DIR = /tmp/

LIBS += \
    -lpoker-eval
HEADERS += \
    eval_board.h \
    eval_defines.h \
    eval_equity.h \
    eval_single.h \
    gui_main.hpp \
    gui_types.hpp \
    misc_bitoperations.h \
    misc_helpers.h \
    misc_bool.h \
    parse_simple.h \
    misc_toksplit.h
SOURCES += \
    eval_board.c \
    eval_equity.c \
    eval_single.c \
    gui_types.cpp \
    gui_main.cpp \
    misc_bitoperations.c \
    misc_helpers.c \
    parse_simple.c \
    misc_toksplit.c

RELEASEFLAGS += \
#    -O4 \
    -O3 \
    -fomit-frame-pointer \
    -march=i686 \
    -ffast-math \
#    -flto \
    -fwhole-program
DEBUGFLAGS += \
    -O0 \
    -ggdb \
    -pedantic \
    -W \
    -Wall \
    -Wextra \
    -Wuninitialized \
    -Wshadow \
    -Wsign-compare \
    -Wwrite-strings \
    -Wunused \
    -Wfloat-equal \
    -Wmissing-declarations \
    -Wunreachable-code \
    -Wcast-qual

QMAKE_LFLAGS_RELEASE = $$RELEASEFLAGS
QMAKE_LFLAGS_DEBUG = $$DEBUGFLAGS
QMAKE_CFLAGS_GENERIC = \
    -std=c99 \
    -Wc++-compat \
    -pipe
QMAKE_CFLAGS_RELEASE = \
    $$QMAKE_CFLAGS_GENERIC \
    $$RELEASEFLAGS
QMAKE_CFLAGS_DEBUG = \
    $$QMAKE_CFLAGS_GENERIC \
    $$DEBUGFLAGS
QMAKE_CXXFLAGS_GENERIC = \
   -std=c++98 \
    -pipe
QMAKE_CXXFLAGS_RELEASE = \
    $$QMAKE_CXXFLAGS_GENERIC \
    $$RELEASEFLAGS
QMAKE_CXXFLAGS_DEBUG = \
    $$QMAKE_CXXFLAGS_GENERIC \
    $$DEBUGFLAGS

QMAKE_CC = clang
QMAKE_CXX = clang
CONFIG(gcc) {
    QMAKE_CC = gcc
    QMAKE_CXX = g++
}

