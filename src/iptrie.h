#ifndef __IPTRIE_H__
#define __IPTRIE_H__

#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdint.h>

struct iptrie;

struct iptrie *	iptrie_new(void);
void		iptrie_free(struct iptrie *t);

void		iptrie_set(struct iptrie *root, struct in_addr a, unsigned mask, uintptr_t value);
void		iptree_del(struct iptrie *root, struct in_addr a, unsigned mask);

size_t		iptrie_lookup(struct iptrie *root, struct in_addr a, uintptr_t *values, size_t values_len);

#endif
