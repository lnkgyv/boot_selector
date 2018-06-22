#ifndef MENUS_H
#define MENUS_H

#include <curses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>

#include "config_reader.h"

static int initialize_curses();

static int end_curses();

int main_menu(const node *NODES, const unsigned int nodes_count);

#endif /* MENUS_H */
