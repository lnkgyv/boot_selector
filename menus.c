#include "menus.h"

static inline int initialize_curses() {
	/* init screen */
	initscr();
	
	/* disable buffering */
	cbreak();

	/* disable echoing of inputed symbols */
	noecho();

	return 0;
}

static inline int end_curses() {
	/* disable screen */
	endwin();

	return 0;
}

static inline WINDOW *create_newwin(int height, int width, int starty, int startx) {
	WINDOW *window;

	window = newwin(height, width, starty, startx);
	box(window, 0, 0);
	wrefresh(window);

	return window;
}

static inline void destroy_win(WINDOW *window) {
	wborder(window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(window);
	delwin(window);
}

static inline int _free_menu(MENU *menu, ITEM **menu_items) {
	free_item(menu_items[0]);
	free_item(menu_items[1]);
	free_menu(menu);

	return 0;
}

static inline int middle(int width, const char *str) {
	int middle;

	middle = (width - strlen(str)) / 2;

	return middle;
}

int node_menu(node current_node) {
	ITEM **menu_items;
	MENU *menu;
	WINDOW *node_window;

	int i, c;

	/* Filling menu items */
	/* 1. Lock
	 * 2. Unlock
	 * 3. Set Boot Device
	 * 4. PXE Options (if PXE choosen as bootdev)
	 * 5. Reboot Node
	 * 6. ------ ------
	 * 7. Close */
	/* PXE didn't implemented for now, so
	 * so menu entries is less */
	menu_items = (ITEM **)calloc(7, sizeof(ITEM *));

	menu_items[0] = new_item("Lock", "(Mark node as busy)");
	menu_items[1] = new_item("Unlock", "(Mark node as free)");
	menu_items[2] = new_item("Set Boot Device", "HDD/PXE");
	menu_items[3] = new_item("Reboot", "(Reboot node)");
	menu_items[4] = new_item("------", "------");
	menu_items[5] = new_item("Close", "(Return to Main menu)");
	menu_items[6] = (ITEM *)NULL;

	/* Forming menu itself */
	menu = new_menu((ITEM **)menu_items);
	set_menu_mark(menu, " * ");

	/* Creating menu window */
	node_window = newwin(7 + 6, 50, 2, 2);
	keypad(node_window, true);

	/* Binding menu to window */
	set_menu_win(menu, node_window);
	set_menu_sub(menu, derwin(node_window, 7 + 2, 0, 2, 1));

	/* Displaying menu window with title  */
	box(node_window, 0, 0);
	mvwprintw(node_window, 0, middle(50, current_node.name), "%s", current_node.name);
	refresh(); /* stdscr */
	wrefresh(node_window);

	/* Put menu */
	post_menu(menu);

	while ((c = wgetch(node_window)))
	{
		switch(c)
		{
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
			case KEY_NPAGE: /* scrolling */
				menu_driver(menu, REQ_SCR_DPAGE);
				break;
			case KEY_PPAGE:
				menu_driver(menu, REQ_SCR_UPAGE);
				break;
			case 10: /* Enter */
				{
					int cur_item_index = item_index(current_item(menu));

					if (cur_item_index == 5) {
						_free_menu(menu, menu_items);
						delwin(node_window);

						return 0;
					} else {
						;
					}
				}
				break;
		}
		wrefresh(node_window);
	}

	_free_menu(menu, menu_items);
	delwin(node_window);

	return 0;
}

int main_menu(const node *NODES, const unsigned int nodes_count) {
	ITEM **menu_items;
	MENU *menu;
	WINDOW *menu_window;

	int i, c;

	initialize_curses();

	/* Filling menu items */
	menu_items = (ITEM **)calloc(nodes_count + 3, sizeof(ITEM *));

	for (i = 0; i < nodes_count; i++) {
		if (strcmp(NODES[i].name, "host3") == 0) { /* occupy test */
			menu_items[i] = new_item(NODES[i].name, "Occupied by me");
		} else {
			menu_items[i] = new_item(NODES[i].name, "");
		}
	}
	menu_items[nodes_count] = new_item("------", "------");
	menu_items[nodes_count + 1] = new_item("Quit", "");
	menu_items[nodes_count + 2] = (ITEM *)NULL;

	/* Forming menu itself */
	menu = new_menu((ITEM **)menu_items);
	set_menu_mark(menu, " * ");

	/* Creating menu window */
	menu_window = newwin(nodes_count + 6, 40, 0, 0);
	keypad(menu_window, true);

	/* Binding menu to window */
	set_menu_win(menu, menu_window);
	set_menu_sub(menu, derwin(menu_window, nodes_count + 2, 0, 2, 1));

	/* Displaying menu window with title  */
	box(menu_window, 0, 0);
	mvwprintw(menu_window, 0, middle(40, PROG_NAME), "%s", PROG_NAME);
	refresh(); /* stdscr */
	wrefresh(menu_window);

	/* Put menu */
	post_menu(menu);
	
	while ((c = wgetch(menu_window)))
	{
		switch (c)
		{
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
			case KEY_NPAGE: /* scrolling */
				menu_driver(menu, REQ_SCR_DPAGE);
				break;
			case KEY_PPAGE:
				menu_driver(menu, REQ_SCR_UPAGE);
				break;
			case 10: /* Enter */
				{
					int cur_item_index = item_index(current_item(menu));

					if (cur_item_index == nodes_count + 1) {
						_free_menu(menu, menu_items);
						delwin(menu_window);

						end_curses();

						exit(0);
					} else {
						node_menu(NODES[cur_item_index]);
					}
				}
				break;
		}
		wrefresh(menu_window);
	}

	_free_menu(menu, menu_items);
	delwin(menu_window);
	
	end_curses();

	return 0;
}
