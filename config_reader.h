#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>

#define ALL_PATHS	     3
#define NODES_LOCK_ID        0
#define PXEOS_ID             1
#define SNAPSHOTS_SUFFIX_ID  2
#define NODES_LOCK_CFG       "paths.NodesLock"
#define PXEOS_CFG            "paths.PXEOS"
#define SNAPSHOTS_SUFFIX_CFG "paths.SnapshotsSuffix"

#define NODES_CFG            "nodes"
#define NODE_NAME_LEN        256
#define NODE_IP_LEN          16
#define NODE_IPMI_LEN        16
#define NODE_PXECFGS_LEN     256
#define NODE_PXECURCFG_LEN   256

#define CHECK_PARAM_SUCCESS(x, param)                                      \
	if ( (x) == -1 ) {                                                 \
		fprintf(stderr, "Can't get param \"%s\"!\n", param);       \
		return(EXIT_FAILURE);                                      \
	}                                                                  \

/* Every node will be described by this structure */
typedef struct {
	char name[NODE_NAME_LEN];
	char ip[NODE_IP_LEN];
	char ipmi[NODE_IPMI_LEN];
	char pxecfgs[NODE_PXECFGS_LEN];
	char pxecurcfg[NODE_PXECURCFG_LEN];
} node;


int get_string_param(const config_t *config,
			const char *param, const char **value);

int get_nodes_params(const config_t *config,
			const char *param, node **NODES, unsigned int *count);

#endif /* CONFIG_READER_H */
