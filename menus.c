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

static WINDOW *create_newwin(int height, int width, int starty, int startx) {
	WINDOW *window;

	window = newwin(height, width, starty, startx);
	box(window, 0, 0);
	wrefresh(window);

	return window;
}

static void destroy_win(WINDOW *window) {
	wborder(window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(window);
	delwin(window);
}

int node_menu(node current_node) {
	WINDOW *node_win;
	ITEM **menu_items;
	MENU *menu;
	int c, i;
	int startx, starty, width, height;

	char *choices[] = {
				"item0",
				"Exit",
	};

	int n_choices = sizeof(choices) / sizeof(choices[0]);
	menu_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
	

	for (i = 0; i < n_choices; i++) {
		menu_items[i] = new_item(choices[i], "");
	}
	menu_items[n_choices] = (ITEM *)NULL;
	
	height = 10;
	width = 20;
	starty = 1;
	startx = 5;

	node_win = create_newwin(height, width, starty, startx);
	
	menu = new_menu((ITEM **)menu_items);

	set_menu_win(menu, node_win);
	set_menu_format(menu, 5, 1);

	refresh();

	while((c = getch()) != KEY_F(2))
	{       switch(c)
	        {	case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
			case 10: /* Enter */
				if (strcmp(item_name(current_item(menu)), "Exit") == 0) {
					destroy_win(node_win);

					return 0;
				}

				break;
		}
	}
	

	return 0;
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

						end_curses();

						exit(0);
					} else {
						;
					}
				}
				break;
		}
		wrefresh(menu_window);
	}

	_free_menu(menu, menu_items);
	
	end_curses();

	return 0;
}
