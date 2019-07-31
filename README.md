# libiptrie
Simple [Trie](https://en.wikipedia.org/wiki/Trie) data structure for looking
up IP addresses.

A lookup on a Trie data structure takes a maximum of O(k) steps, where k is
the maximum length of the key. So for an IPv4 address, it will take a maximum
of 32 steps to lookup an IP address, regardless of how many IP
addresses/prefixes you already have in the Trie.

```c
// Assign ptr1 to IP prefix "0.0.0.0/0"
inet_pton(AF_INET, "0.0.0.0", &addr);
iptrie_set(t, addr, 0, ptr1);

// Assign ptr2 to IP prefix "192.168.1.0/24"
inet_pton(AF_INET, "192.168.1.0", &addr);
iptrie_set(t, addr, 24, ptr2);

// Assign ptr3 to IP address "192.168.1.42"
inet_pton(AF_INET, "192.168.1.42", &addr);
iptrie_set(t, addr, 32, ptr3);

// Lookup IP address "192.168.1.42"
iptrie_lookup(t, addr, values, 32);

// `values` now contains [ ptr1, ptr2, ptr3 ]
```
