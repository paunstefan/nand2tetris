/*
	The header file for the hash table implementation.
*/

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

#pragma once

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

/**
	Creates the HashTable structure.

	@param ht: the hastable pointer
	@param dim: the wanted dimension
*/
void init_hash_table(struct HashTable* ht, size_t dim);

/**
	Inserts a new element into the HashTable struct. 
	It uses chaining for collision avoidance (using a circular linked list).

	@param ht: the hashtable structure
	@param name: the label name
	@param addr: the label address
*/
void insert_HT(struct HashTable ht, char* name, uint16_t addr);

/*
	Prints all elements in a list.
*/
void print_HT(struct HashTable ht);

/**
	Checks if an item is present in the table.

	@param ht: the hashtable structure
	@param name: the label name
	@return true if it is present, else false
*/
bool is_item(struct HashTable ht, char* name);

/**
	Returns the value from the HashTable associated with a given key. Returns -1 if the item does not exist.

	@param ht: the hashtable structure
	@param name: the label name
	@return value of the label, -1 if it does not exist
*/
int32_t get_item(struct HashTable ht, char* name);
