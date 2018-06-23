#include <stdio.h>
#include <stdlib.h>

#include <libconfig.h>

#include <ncurses.h>
#include <menu.h>

#include "config_reader.h"
#include "menus.h"

int main(int argc, char *argv[])
{
	config_t cfg;
	const char *PATHS[ALL_PATHS];
	node *NODES;
	unsigned int nodes_count;

	int ret = 0;

	int i;

	config_init(&cfg);
	if (!config_read_file(&cfg, "boot_selector.cfg")) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
			config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);

		return(EXIT_FAILURE);
	}

	ret = get_string_param(&cfg, NODES_LOCK_CFG, &PATHS[NODES_LOCK_ID]);
	CHECK_PARAM_SUCCESS(ret, "NodesLock");

	ret = get_string_param(&cfg, PXEOS_CFG, &PATHS[PXEOS_ID]);
	CHECK_PARAM_SUCCESS(ret, "PXEOS");

	ret = get_string_param(&cfg, SNAPSHOTS_SUFFIX_CFG, &PATHS[SNAPSHOTS_SUFFIX_ID]);
	CHECK_PARAM_SUCCESS(ret, "SnapshotsSuffix");

	ret = get_nodes_params(&cfg, NODES_CFG, &NODES, &nodes_count);
	CHECK_PARAM_SUCCESS(ret, NODES_CFG);

	main_menu(NODES, nodes_count);

	free(NODES);

	config_destroy(&cfg);

	return(EXIT_SUCCESS);
}
