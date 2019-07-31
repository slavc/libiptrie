#include <stdio.h>
#include <stdlib.h>

#include "iptrie.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	int exit_code = EXIT_SUCCESS;

	struct in_addr addr;

	struct iptrie *iptrie = iptrie_new();

	const struct {
		const char *addr;
		unsigned mask;
		uintptr_t store_value;
		uintptr_t retrieve_value;
		unsigned value_count;
	} tab[] = {
		{ "192.168.0.1", 24, 1, 2, 1 },
		{ "192.168.0.2", 24, 2, 2, 1 },
		{ "192.168.0.111", 32, 111, 111, 2 },
		{ "192.168.42.1", 24, 3, 4, 1 },
		{ "192.168.42.2", 24, 4, 4, 1 },
		{ "192.168.42.42", 32, 5, 5, 2 },
		{ NULL },
	}, *tab_p;

	for (tab_p = tab; tab_p->addr != NULL; tab_p++) {
		inet_pton(AF_INET, tab_p->addr, &addr);
		iptrie_set(iptrie, addr, tab_p->mask, tab_p->store_value);
	}

	unsigned i = 0;
	for (tab_p = tab; tab_p->addr != NULL; tab_p++) {
		inet_pton(AF_INET, tab_p->addr, &addr);
		uintptr_t values[32];
		unsigned count = iptrie_lookup(iptrie, addr, values, 32);
		if (count != tab_p->value_count) {
			exit_code = EXIT_FAILURE;
			printf("entry %u (%s/%u): returned value count %u doesn't match expected %u\n",
				i, tab_p->addr, tab_p->mask,
				count, tab_p->value_count);
		}
		if (count > 0 && values[count-1] != tab_p->retrieve_value) {
			exit_code = EXIT_FAILURE;
			printf("entry %u (%s/%u): retrieved value %u didn't match %u\n",
				i, tab_p->addr, tab_p->mask,
				(unsigned)values[count-1], (unsigned)tab_p->retrieve_value);
		}
		i++;
	}

	iptrie_free(iptrie);

	return exit_code;
}
