#include "config_reader.h"

/* get specific string parameter  */
int get_string_param(const config_t *config,
			const char *param, const char **value) {

	/* get paths group of parameters */
	if (config_lookup_string(config, param, value)) {
		return 0;
	} else {
		return -1;
	}
}

/* get configuration of nodes */
int get_nodes_params(const config_t *config,
			const char *param, node **NODES, unsigned int *count) {

	config_setting_t *setting;

	setting = config_lookup(config, param);
	if (setting != NULL) {
		*count = config_setting_length(setting);
		*NODES = (node*)malloc(sizeof(node) * (*count));
		int i;

		for (i = 0; i < (*count); i++) {
			config_setting_t *host = config_setting_get_elem(setting, i);
			const char *node, *ip, *ipmi_ip, *pxecfgs, *pxecurcfg;

			if (!(config_setting_lookup_string(host, "name", &node)
				&& config_setting_lookup_string(host, "ip", &ip)
				&& config_setting_lookup_string(host, "ipmi", &ipmi_ip)
				&& config_setting_lookup_string(host, "pxecfgs", &pxecfgs)
				&& config_setting_lookup_string(host, "pxecurcfg", &pxecurcfg))) {
				continue;
			}

			/* name */
			strncpy((*NODES)[i].name, node, NODE_NAME_LEN);
			(*NODES)[i].name[NODE_NAME_LEN - 1] = '\0';

			/* ip */
			strncpy((*NODES)[i].ip, ip, NODE_IP_LEN);
			(*NODES)[i].ip[NODE_IP_LEN - 1] = '\0';

			/* ipmi */
			strncpy((*NODES)[i].ipmi, ipmi_ip, NODE_IPMI_LEN);
			(*NODES)[i].ipmi[NODE_IPMI_LEN - 1] = '\0';

			/* pxecfgs */
			strncpy((*NODES)[i].pxecfgs, pxecfgs, NODE_PXECFGS_LEN);
			(*NODES)[i].pxecfgs[NODE_PXECFGS_LEN - 1] = '\0';

			/* pxecurcfg */
			strncpy((*NODES)[i].pxecurcfg, pxecurcfg, NODE_PXECURCFG_LEN);
			(*NODES)[i].pxecurcfg[NODE_PXECURCFG_LEN - 1] = '\0';
		}
	} else {
		return -1;
	}

	return 0;

}
