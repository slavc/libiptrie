# libiptrie
Simple [Trie](https://en.wikipedia.org/wiki/Trie) data structure for looking
up IP addresses.

A lookup on a Trie data structure takes a maximum of O(k) steps, where k is
the maximum length of the key. So for an IPv4 address, it will take a maximum
of 32 steps to lookup an IP address, regardless of how many IP
addresses/prefixes you already have in the Trie.

