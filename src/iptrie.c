#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "iptrie.h"

struct iptrie {
	struct iptrie	*left;
	struct iptrie	*right;
	uintptr_t	 value;
	bool		 is_terminator;
};

struct iptrie *iptrie_new(void)
{
	return calloc(1, sizeof(struct iptrie));
}

void iptrie_free(struct iptrie *t)
{
	if (t == NULL) {
		return;
	}

	iptrie_free(t->left);
	iptrie_free(t->right);
	free(t);
}

void iptrie_set(struct iptrie *root, struct in_addr a, unsigned mask, uintptr_t value)
{
	struct iptrie *t = root;
	struct iptrie **next;

	a.s_addr = ntohl(a.s_addr);

	while (mask--) {
		if (a.s_addr & 0x80000000u) {
			next = &t->right;
		} else {
			next = &t->left;
		}
		if (*next == NULL) {
			*next = calloc(1, sizeof(struct iptrie));
		}
		t = *next;
		a.s_addr <<= 1;
	}

	t->value = value;
	t->is_terminator = true;
}

size_t iptrie_lookup(struct iptrie *root, struct in_addr a, uintptr_t *values, size_t values_len)
{
	struct iptrie *t = root;
	size_t num_values = 0;
	unsigned mask = 0x80000000u;

	a.s_addr = ntohl(a.s_addr);

	while (t != NULL && (num_values < values_len || (values_len == 0 && values == NULL))) {
		if (t->is_terminator) {
			if (values != NULL) {
				values[num_values++] = t->value;
			} else {
				num_values++;
			}
		}
		if (mask == 0) {
			break;
		}
		if (a.s_addr & mask) {
			t = t->right;
		} else {
			t = t->left;
		}
		mask >>= 1;
	}

	return num_values;
}

bool iptrie_has_terminators(struct iptrie *t)
{
	if (t == NULL) {
		return false;
	} else if (t->is_terminator) {
		return true;
	}

	if (iptrie_has_terminators(t->left)) {
		return true;
	}
	if (iptrie_has_terminators(t->right)) {
		return true;
	}

	return false;
}

void iptree_del(struct iptrie *root, struct in_addr a, unsigned mask)
{
	struct iptrie *t = root;
	struct iptrie **prev = NULL;
	struct iptrie **next = NULL;

	a.s_addr = ntohl(a.s_addr);

	while (mask-- && t != NULL) {
		prev = next;
		if (a.s_addr & 0x80000000u) {
			next = &t->right;
			t = t->right;
		} else {
			next = &t->left;
			t = t->left;
		}
		a.s_addr <<= 1;
	}

	if (t != NULL) {
		t->is_terminator = false;
		if (t != root && !iptrie_has_terminators(t)) {
			iptrie_free(t);
			*prev = NULL;
		}
	}
}
