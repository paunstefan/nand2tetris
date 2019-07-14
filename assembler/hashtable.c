#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"




/*
	Creates the HashTable structure.
*/
void init_hash_table(struct HashTable* ht, size_t dim){
	ht->size = dim;
	ht->buckets = (struct ListNode**)malloc(sizeof(struct ListNode) * dim);
	memset(ht->buckets, 0, dim*sizeof(struct ListNode*));
}


/*
	Creates the Label structure which holds the actual information.
*/
Label* create_label(char* name, uint16_t addr){
	Label* temp = (Label*)malloc(sizeof(Label));
	temp->name = (char*)malloc(strlen(name)+1);
	strcpy(temp->name, name);
	temp->address = addr;
	return temp;
}


/*
	Simple hashing function for the name. (can be improved)
*/
uint16_t hash(char* key, size_t size){
	uint16_t h = 0;
	size_t len = strlen(key);
	for(size_t i=0; i<len; i++){
		h += key[i];
	}

	return h % size;
}


/*
	Creates a list node.
*/
struct ListNode* create_node(char* name, uint16_t addr){
	Label *label = create_label(name, addr);
	struct ListNode *node = (struct ListNode*)malloc(sizeof(struct ListNode));
	node->info = label;
	node->next = NULL;
	return node;
}


/*
	Inserts a new node into the list.
*/
struct ListNode* insert_node(struct ListNode* list, char* name, uint16_t addr){
	struct ListNode *node = create_node(name, addr);

	struct ListNode *temp = list;

	if(!list){
		node->next = node;
	}
	else{
		node->next = list;
		while(temp->next != list){
			temp = temp->next;
		}
		temp->next = node;
	}

	return node;
}


/*
	Inserts a new element into the HashTable struct. 
	It uses chaining for collision avoidance (using a circular linked list).
*/
void insert_HT(struct HashTable ht, char* name, uint16_t addr){
	uint16_t position = hash(name, ht.size);
	struct ListNode *list = ht.buckets[position];

	list = insert_node(list, name, addr);

	ht.buckets[position] = list; 
}


/*
	Prints all elements in a list.
*/
void print_list(struct ListNode *ls){
	struct ListNode *temp = ls;
	do{
		printf("Name: %s, address: %d\n", temp->info->name, temp->info->address);
		temp = temp->next;
	}while(temp != ls);
}


/*
	Prints all elements in a table.
*/
void print_HT(struct HashTable ht){
	if(ht.size > 0){
		for(uint16_t i = 0; i < ht.size; i++){
			if(ht.buckets[i]){
				print_list(ht.buckets[i]);
				printf("\n");
			}
		}
	}
}


/*
	Checks if an item is present in the table.
*/
bool is_item(struct HashTable ht, char* name){
	uint16_t position = hash(name, ht.size);
	struct ListNode *list = ht.buckets[position];

	if(!list){
		return false;
	}
	else{
		struct ListNode *temp = list;
		do{
			if(strcmp(temp->info->name, name) == 0){
				return true;
			}
			temp = temp->next;
		}while(temp != list);

	}
	return false;
}


/*
	Returns the value from the HashTable associated with a given key. Returns -1 if the item does not exist.
*/
int32_t get_item(struct HashTable ht, char* name){
	int32_t addr = -1;
	uint16_t position = hash(name, ht.size);
	struct ListNode *list = ht.buckets[position];

	if(!list){
		return -1;
	}
	else{
		struct ListNode *temp = list;
		do{
			if(strcmp(temp->info->name, name) == 0){
				addr = temp->info->address;
			}
			temp = temp->next;
		}while(temp != list);

	}
	return addr;
}