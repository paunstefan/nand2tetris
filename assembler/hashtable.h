/*
	The header file for the hash table implementation.
*/

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef HASHTABLE_H
#define HASHTABLE_H

struct HashTable{
	struct ListNode** buckets;
	size_t size;
};

typedef struct Label{
	char* name;
	uint16_t address;
}Label;

struct ListNode{
	Label* info;
	struct ListNode* next;
};

void init_hash_table(struct HashTable* ht, size_t dim);
void insert_HT(struct HashTable ht, char* name, uint16_t addr);
void print_HT(struct HashTable ht);
bool is_item(struct HashTable ht, char* name);
int32_t get_item(struct HashTable ht, char* name);

#endif