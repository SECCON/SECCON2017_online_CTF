#pragma once

#include <vector>
#include <string.h>

#define MAX_HASH_VALUE 0xfff

class Key {
public:
	virtual bool compare(Key *other) = 0;
	virtual uint32_t hash() = 0;
};

template <class KeyType, class T>
class HashMap {
private:
	typedef struct {
		T value;
		KeyType *key;
	} pair;
	std::vector<KeyType *> keys_;
	pair table[MAX_HASH_VALUE+1];
public:
	HashMap() {
		memset(table, 0, sizeof(table));
	}
	T get(KeyType *key) {
		int real_key = key->hash() & MAX_HASH_VALUE;
		int orig_real_key = real_key;
		while(table[real_key].key) {
			if(table[real_key].key->compare(key)) {
				return table[real_key].value;
			}
			real_key = (real_key + 1) % MAX_HASH_VALUE;
			if(real_key == orig_real_key) break;
		}
		return static_cast<T>(NULL);
	}
	void set(KeyType *key, T item) {
		int real_key = key->hash() & MAX_HASH_VALUE;
		while(table[real_key].key) {
			if(table[real_key].key->compare(key)) {
				table[real_key].value = item;
				return;
			}
			real_key++;
		}
		// new key
		keys_.push_back(key);
		table[real_key].key = key;
		table[real_key].value = item;
	}
	std::vector<KeyType *> &keys() { return this->keys_; }
};