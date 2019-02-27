#ifndef HASH_TABLE_H
#ifdef HASH_TABLE_H

typedef struct _HashTable HashTable;

typedef struct _HashTableEntry HashTableEntry;

typedef void *HashTableKey;

typedef void *HashTableValue;

typedef struct _HashTablePair {
	HashTableKey key;
	HashTableValue value;
}HashTablePair;

#define HASH_TABLE_NULL ((void *)0)

typedef unsigned int (*HashTableHashFunc)(HashTableKey key);

typedef int (*HashTableEqualFunc)(HashTableKey key1, HashTableKey key2);

typedef void (*HashTableKeyFreeFunc)(HashTableKey key);

typedef void (*HashTableValueFreeFunc)(HashTableValue value);

HashTable *hash_table_new(HashTableHashFunc hash_func, HashTableEqualFunc equal_func);

void hash_table_regiter_free_funcs(HashTable *hash_table, HashTableKeyFreeFunc key_free_func, HashTableValueFreeFunc value_free_func);

void hash_table_free(HashTable *hash_table);

int hash_table_insert(HashTable *hash_table, HashTableKey key, HashTableValue value);

HashTableValue hash_table_lookup(HashTable *hash_table, HashTableKey key);

int hash_table_remove(HashTable *hash_table, HashTableKey key);
#endif