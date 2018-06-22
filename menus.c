#include "menus.h"

static int initialize_curses() {
	/* init screen */
	initscr();
	
	/* disable buffering */
	cbreak();

	/* disable echoing of inputed symbols */
	noecho();

	return 0;
}

static int end_curses() {
	/* disable screen */
	endwin();

	return 0;
}

int main_menu(const node *NODES, const unsigned int nodes_count) {
	ITEM **menu_items;
	MENU *menu;

	int i, c;

	initialize_curses();

	/* accept FN keys */
	keypad(stdscr, true);

	menu_items = (ITEM **)calloc(nodes_count + 1, sizeof(ITEM *));

	for (i = 0; i < nodes_count; i++) {
		if (strcmp(NODES[i].name, "host3") == 0) { /* occupy test */
			menu_items[i] = new_item(NODES[i].name, "Occupied by me");
		} else {
			menu_items[i] = new_item(NODES[i].name, "");
		}
	}
	menu_items[nodes_count] = (ITEM *)NULL;
	
	menu = new_menu((ITEM **)menu_items);
	mvprintw(LINES - 2, 0, "F1 to Exit");
	post_menu(menu);
	refresh();
	
	while ((c = getch()) != KEY_F(1))
	{
		switch (c)
		{
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
		}
	}

	free_item(menu_items[0]);
	free_item(menu_items[1]);
	free_menu(menu);
	
	end_curses();

	return 0;
}
