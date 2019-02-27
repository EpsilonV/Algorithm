#include "hash-table.h"

struct _HashTable {
	HashTableEntry **table;
	HashTableHashFunc hash_func;
	HashTableEqualFunc equal_func;
	HashTableKeyFreeFunc key_free_func;
	HashTableValueFreeFunc value_free_func;
	unsigned int table_size;
	unsigned int entries;
	unsigned int prime_index;
}

struct _HashTableEntry {
	HashTablePair pair;
	HashTableEntry *next;
}

//http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
static const unsigned int hash_table_primes[] = {
	193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
	196613, 393241, 786433, 1572869, 3145739, 6291469,
	12582917, 25165843, 50331653, 100663319, 201326611,
	402653189, 805306457, 1610612741,
};

static const unsigned int hash_table_primes_num = sizeof(hash_table_primes)/sizeof(unsigned int);

static int hash_table_alloc(HashTable *hash_table) {
	unsigned int size;

	if (hash_table->prime_index < hash_table_primes_num){
		size = hash_table_primes[hash_table->prime_index];
	}else {
		size = hash_table->entries * 10;
	}

	hash_table->table_size = size;

	hash_table->table = calloc(hash_table->table_size, sizeof(HashTableEntry*));

	return hash_table->table != NULL;
}

static int hash_table_enlarge(HashTable *hash_table)
{
	int i, ret, index;
	HashTableEntry **old_table;
	HashTableEntry *entry, *next;
	HashTablePair *pair;
 	int old_size;

 	old_size = hash_table->table_size;
	old_table = hash_table->table;
	hash_table->prime_index += 1;
	ret = hash_table_alloc(hash_table);
	if (!ret){
		hash_table->table = old_table;
		hash_table->table_size = old_size;
		hash_table->prime_index -= 1;
		return ret; 
	}

	for (i = 0; i < old_size; ++i)
	{
		entry = old_table[i];
		while (entry) {
			next = entry->next;

			pair = &(entry->pair);
			index = hash_table->hash_func(pair->key)%hash_table->table_size;
			entry->next = hash_table->table[index];
			hash_table->table[index] = entry;

			entry = next;
		}
	}

	free(old_table);

	return 1;

}

static void hash_table_free_entry(HashTable *hash_table, HashTableEntry *entry)
{
	HashTablePair *pair;

	pair = &entry->pair;
	if (hash_table->key_free_func) {
		hash_table->key_free_func(pair->key);
	}

	if (hash_table->value_free_func) {
		hash_table->value_free_func(pair->value);
	}

	free(entry);
}

HashTable *hash_table_new(HashTableHashFunc hash_func, HashTableEqualFunc equal_func)
{
	HashTable *hash_table;

	hash_table = (HashTable *)malloc(sizeof(HashTable));
	if (hash_table == NULL)
		return NULL;

	hash_table->hash_func = hash_func;
	hash_table->equal_func = equal_func;
	hash_table->key_free_func = NULL;
	hash_table->value_free_func = NULL;
	hash_table->entries = 0;
	hash_table->prime_index = 0;

	if (!hash_table_alloc(hash_table)) {
		free(hash_table);
		return NULL;
	}

	return hash_table;
}

void hash_table_regiter_free_funcs(HashTable *hash_table, HashTableKeyFreeFunc key_free_func, HashTableValueFreeFunc value_free_func)
{
	hash_table->key_free_func = key_free_func;
	hash_table->value_free_func = value_free_func;
}

void hash_table_free(HashTable *hash_table)
{
	int i;
	HashTableEntry *entry, *next;

	for (i = 0; i < hash_table->table_size; i++) {
		entry = hash_table->table[i];

		while(entry) {
			next = entry->next;
			hash_table_free_entry(hash_table, entry);
			entry = next;
		}
	}

	free(hash_table->table);
	free(hash_table);
}

int hash_table_insert(HashTable *hash_table, HashTableKey key, HashTableValue value)
{
	int index;
	HashTableEntry *entry;

	if ((hash_table->entries *3)/hash_table->table_size > 0) {
		if (!hash_table_enlarge(hash_table)){

			return 0;
		}
	}

	index = hash_table->hash_func(key) % hash_table->table_size;
	entry = hash_table->table[index];

	while(entry) {
		pair = &(entry->pair);
		if (hash_table->equal_func(key, pair->key)) {
			if (hash_table->key_free_func) {
				hash_table->key_free_func(pair->key);
			}

			if (hash_table->value_free_func) {
				hash_table->value_free_func(pair->value);
			}

			pair->key = key;
			pair->value = value;

			return 1;
		}

		entry = entry->next;
	}

	entry = (HashTableEntry *)malloc(sizeof(HashTableEntry));
	if (entry == NULL)
		return 0;

	entry->pair.key = key;
	entry->pair.value = value;

	entry->next = hash_table->table[index];
	hash_table->table[index] = entry;

	hash_table->entries += 1;

	return 1;
}

HashTableValue hash_table_lookup(HashTable hash_table, HashTableKey key)
{
	int index;
	HashTableEntry *entry;
	HashTablePair *pair;

	index = hash_table->hash_func(key) % hash_table->table_size;

	entry = hash_table->table[index];

	while (entry) {
		pair = &(entry->pair);
		if (hash_table->equal_func(key, pair->key)) {
			return pair->value;
		}

		entry = entry->next;
	}

	return HASH_TABLE_NULL;
}

int hash_table_remove(HashTable *hash_table, HashTableKey key)
{
	int index;
	HashTableEntry **entry;
	HashTableEntry *delEntry;
	HashTablePair *pair;

	index = hash_table->hash_func(key) % hash_table->table_size;

	entry = &(hash_table->table[index]);

	while(entry) {
		pair = (*entry)->pair;
		if (hash_table->equal_func(key, pair->key)) {
			delEntry = *entry;
			*entry = delEntry->next;
			hash_table_free_entry(hash_table, delEntry);
			hash_table->entries -= 1;
			return 1;
		}

		entry = &((*entry)->next);
	}

	return 0;
}