
//
// Implementation of a HashTable that stores void *
//
#include "HashTableVoid.h"

// Obtain the hash code of a key
int HashTableVoid::hash(const char * key) {
	int h = 0;
	const char * p = key;
	while (*p != '\0') {
		h += *p;
		p++;
	}
	return h % TableSize;
}

// Constructor for hash table. Initializes hash table
HashTableVoid::HashTableVoid() {
	_buckets = (HashTableVoidEntry **) malloc(TableSize*sizeof(HashTableVoidEntry*));
	int i;
	for (i = 0; i < TableSize; i++)
		_buckets[i] = NULL;
}

// Add a record to the hash table. Returns true if key already exists.
// Substitute content if key already exists.
bool HashTableVoid::insertItem( const char * key, void * data) {
	int h = hash(key);
	
	HashTableVoidEntry * e = _buckets[h];
	while (e != NULL) {
		if (strcmp(e -> _key, key) == 0) {
			e -> _data = data;
			return true;
		}
		e  = e-> _next;
	}
	
	e = new HashTableVoidEntry;
	e -> _key = strdup(key);
	e -> _data = data;
	e -> _next = _buckets[h];
	_buckets[h] = e;
	
	return false;
}

// Find a key in the dictionary and place in "data" the corresponding record
// Returns false if key is does not exist
bool HashTableVoid::find( const char * key, void ** data) {
	int h = hash(key);

	HashTableVoidEntry * e = _buckets[h];
	while (e != NULL) {
		if (strcmp(e -> _key, key) == 0) {
			*data = e -> _data;
			return true;
		}
		e = e -> _next;
	}
	return false;
}

// Find a key in the dictionary 
// Returns false if key is does not exist
bool HashTableVoid::exists( const char * key) {
	int h = hash(key);

	HashTableVoidEntry * e = _buckets[h];
	while (e != NULL) {
		if (strcmp(e -> _key, key) == 0) {
			return true;
		}
		e = e -> _next;
	}
	return false;
}


// Removes an element in the hash table. Return false if key does not exist.
bool HashTableVoid::removeElement(const char * key) {
	int h = hash(key);

	HashTableVoidEntry * e = _buckets[h];
	HashTableVoidEntry * p = NULL;

	while (e != NULL) {
		if (strcmp(e -> _key, key) ==0) {
			if (p != NULL) {
				p -> _next = e -> _next;
			} else {
				_buckets[h] = e -> _next; 
			}
			free((char *) e -> _key);
			delete e;
			return true;
		}
		p = e;
		e = e -> _next;
	}

	 return false;
}

// Creates an iterator object for this hash table
HashTableVoidIterator::HashTableVoidIterator(HashTableVoid * hashTable) {
	_hashTable = hashTable;
	_currentBucket = -1;
	_currentEntry = NULL;
}

// Returns true if there is a next element. Stores data value in data.
bool HashTableVoidIterator::next(const char * & key, void * & data) {
	if ((_currentEntry != NULL) && (_currentEntry -> _next != NULL)) {
		_currentEntry = _currentEntry -> _next;
		
		key = _currentEntry -> _key;
		data = _currentEntry -> _data;
		
		return true;
	}

	int i = _currentBucket + 1;

	while (i < 2039 && _hashTable -> _buckets[i] == NULL) i++;
	
	if (i < 2039 && _hashTable -> _buckets[i] != NULL) {
		key =  _hashTable -> _buckets[i] -> _key;
		data = _hashTable -> _buckets[i] -> _data;

		_currentBucket = i;
		_currentEntry = _hashTable -> _buckets[i];
		
		return true;
	}
	
	return false;
}

