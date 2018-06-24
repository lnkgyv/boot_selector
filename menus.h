#ifndef MENUS_H
#define MENUS_H

#include <curses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config_reader.h"

#define PROG_NAME "Boot selector"

#define FREE_MENU(menu)                 \
		free_item(menu_items[0]); \
		free_item(menu_items[1]); \
		free_menu(menu);          \

static inline int initialize_curses();

static inline int end_curses();

static inline int _free_menu(MENU *menu, ITEM **menu_items);

static inline int middle(int width, const char *str);

static inline WINDOW *create_newwin(int height, int width, int starty, int startx);

static inline void destroy_win(WINDOW *window);

int node_menu(node current_node);

int main_menu(const node *NODES, const unsigned int nodes_count);

#endif /* MENUS_H */
