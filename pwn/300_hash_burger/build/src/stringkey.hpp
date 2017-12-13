#pragma once

#include "crc32_tab.hpp"
#include "hashmap.hpp"
#include <string>

class StringKey : public Key {
private:
	std::string key;
public:
	StringKey(std::string key) {
		this->key = key;
	}
	virtual bool compare(Key *other) {
		StringKey *_other = static_cast<StringKey *>(other);
		return this->key.compare(_other->key) == 0;
	}
	virtual uint32_t hash() {
		uint32_t crc = ~0U;
		for(int i = 0; i < this->key.size(); i++)
			crc = crc32_tab[(crc ^ this->key[i]) & 0xFF] ^ (crc >> 8);
		crc = crc ^ ~0U;
		return crc;
	}
	std::string &str() { return this->key; }
};