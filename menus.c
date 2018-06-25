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
	CDKSCREEN *cdk_window;
	CDKSCROLL *cdk_menu;
	WINDOW *node_window;
	unsigned int number_of_menu_items = 6;
	char *menu_items[number_of_menu_items];

	int current_menu_item;

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

	menu_items[0] = "Lock (Mark node as busy)";
	menu_items[1] = "Unlock (Mark node as free)";
	menu_items[2] = "Set Boot Device  (HDD/PXE)";
	menu_items[3] = "Reboot (Reboot node)";
	menu_items[4] = "";
	menu_items[5] = "Close (Return to Main menu)";

	/* Creating menu window */
	node_window = newwin(7 + 6, 50, 2, 2);
	keypad(node_window, true);

	/* Create and bind CDK screen to window */
	cdk_window = initCDKScreen(node_window);

	/* Displaying menu window with title  */
	box(node_window, 0, 0);
	mvwprintw(node_window, 0, middle(50, current_node.name), "%s", current_node.name);
	//refresh(); /* stdscr */
	wrefresh(node_window);

	/* Forming menu */
	cdk_menu = newCDKScroll(cdk_window,
			CENTER, /* x position */
			CENTER, /* y position */
			RIGHT,  /* scrol position */
			number_of_menu_items + 4, /* height */
			40, /* width */
			"Choose an anction:", /* title */ 
			menu_items, /* items */
			number_of_menu_items, /* number of elements in list */
			false, /* show numbers ?  */
			A_REVERSE, /* how selected item will be higlighted ? */
			false, /* show box ?  */
			false /* show shadow ? */
	);

	while ((current_menu_item = activateCDKScroll(cdk_menu, 0)) != -1) {
		if (cdk_menu->exitType == vNORMAL) {
			if (current_menu_item == number_of_menu_items - 1) {
				destroyCDKScroll(cdk_menu);
				destroyCDKScreen(cdk_window);
				delwin(node_window);
	
				return 0;
			} else if (current_menu_item == number_of_menu_items - 2) {
				continue ;
			} else {
				char temp[256], *mesg[10];
				sprintf (temp, "%s", chtype2Char(cdk_menu->item[current_menu_item]));
				mesg[0] = temp;
				popupLabel (cdk_window, mesg, 1);
			}
		}
	}

	destroyCDKScroll(cdk_menu);
	destroyCDKScreen(cdk_window);
	delwin(node_window);

	return 0;
}

int main_menu(const node *NODES, const unsigned int nodes_count) {
	CDKSCREEN *cdk_window;
	CDKSCROLL *cdk_menu;
	WINDOW *menu_window;
	unsigned int number_of_menu_items = nodes_count + 2;
	char *menu_items[number_of_menu_items];

	int i, current_menu_item;

	initialize_curses();

	/* Filling menu items */
	for (i = 0; i < nodes_count; i++) {
		if (strcmp(NODES[i].name, "host3") == 0) { /* occupy test */
			menu_items[i] = strcat((char *)NODES[i].name, " (Occupied by me)");
		} else {
			menu_items[i] = (char *)NODES[i].name;
		}
	}
	menu_items[nodes_count] = "";
	menu_items[nodes_count + 1] = "Quit";

	/* Creating menu window */
	menu_window = newwin(nodes_count + 6, 40, 0, 0);
	keypad(menu_window, true);

	/* Create and bind CDK screen to window */
	cdk_window = initCDKScreen(menu_window);

	/* Displaying menu window with title  */
	box(menu_window, 0, 0);
	mvwprintw(menu_window, 0, middle(40, PROG_NAME), "%s", PROG_NAME);
	//refresh(); /* stdscr */
	wrefresh(menu_window);

	cdk_menu = newCDKScroll(cdk_window,
			CENTER, /* x position */
			CENTER, /* y position */
			RIGHT,  /* scrol position */
			nodes_count + 4, /* height */
			30, /* width */
			"Choose node:", /* title */ 
			menu_items, /* items */
			number_of_menu_items, /* number of elements in list */
			false, /* show numbers ?  */
			A_REVERSE, /* how selected item will be higlighted ? */
			false, /* show box ?  */
			false /* show shadow ? */
	);

	while ((current_menu_item = activateCDKScroll(cdk_menu, 0)) != -1) {
		if (cdk_menu->exitType == vNORMAL) {
			if (current_menu_item == number_of_menu_items - 1) {
				destroyCDKScroll(cdk_menu);
				destroyCDKScreen(cdk_window);
				delwin(menu_window);
	
				end_curses();

				return 0;
			} else if (current_menu_item == number_of_menu_items - 2) {
				continue ;
			} else {
				node_menu(NODES[current_menu_item]);
			}
		}
	}
	
	destroyCDKScroll(cdk_menu);
	destroyCDKScreen(cdk_window);
	delwin(menu_window);
	
	end_curses();

	return 0;
}
