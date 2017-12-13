#include <iostream>
#include "hashmap.hpp"
#include "stringkey.hpp"
#include <stdio.h>
#include <openssl/sha.h>

#include <string>

typedef HashMap<StringKey, const char *> BurgerMap;

BurgerMap *map;
std::string _pow;

#define LINER "-----------------------------\n"
#define ORDER_MENU_STRING LINER "1. Shrimp burger\n2. Hashbrown burger\n3. Double cheese burger\n0: Exit\n" LINER
#define MENU_STRING LINER "1. Order\n2. Show order\n3. See orders\n0: Exit\n" LINER

void show_menu() {
	std::cout << MENU_STRING;
}

void show_order_menu() {
	std::cout << ORDER_MENU_STRING;
}

void order() {
	StringKey *key;
	int input;
	std::string name, tmp;
	while(1) {
		show_order_menu();
		std::cout << "What burger do you want to order?\n";
		std::getline(std::cin, tmp);
		input = atoi(tmp.c_str());
		if(!(input >= 0 && input <= 3)) {
			std::cout << "Incorrect choice\n";
			continue;
		}
		if(input == 0) return;
		std::cout << "What's your name?\n";
		std::getline(std::cin, name);
		key = new StringKey(name);
		switch(input) {
			case 1: {
				map->set(key, "shrimp");
				break;
			}
			case 2: {
				map->set(key, "hashbrown");
				break;
			}
			case 3: {
				map->set(key, "double cheese");
				break;
			}
		}
		break;
	}
}

void show_order() {
	std::string name;
	StringKey *key;
	std::cout << "What's your name?\n";
	std::getline(std::cin, name);
	key = new StringKey(name);
	auto item = map->get(key);
	if(item == NULL) {
		std::cout << "No order for the name.\n";
	} else {
		std::cout << "You ordered " << item << "\n";
	}
	delete key;
}

void proof_of_work() {
	int count = 10;
	unsigned char hash[20];
	char prefix[8];
	srand(time(0));
	sprintf(prefix, "%08x", rand());
	std::cout << "Typical POW for with-ASLR problem:\n";
	std::cout << "  1. string starts with '" << prefix << "'\n";
	std::cout << "  2. sha1 hash starts with '\\x00'\n";
	while(count--) {
		std::getline(std::cin, _pow);
		if(memcmp(_pow.c_str(), prefix, 8)) {
			std::cout << "Invalid prefix.\n";
			continue;
		}
		SHA1((const unsigned char *)_pow.c_str(), _pow.size(), hash);
		if(hash[0] == 0) {
            std::cout << ("Now you have enough POW for access this binary\n");
			return;
		} else {
			std::cout << "Invalid POW. " << count - 1 << " chances left.\n";
		}
	}
	std::cout << "Too many invalid POW! Bye!\n";
	exit(0);
}

int main() {
	int input;
	std::string tmp;
	setvbuf(stdout, 0, 2, 0);
	std::cout << "Welcome to the hash burger!\n";
	proof_of_work();
	map = new BurgerMap;
	while(1) {
		show_menu();
		std::cout << "May I check your order?\n";
		input = 0;
		std::getline(std::cin, tmp);
		input = atoi(tmp.c_str());
		if(input == 0) return 0;
		else if(input == 1) {
			order();
		} else if(input == 2) {
			show_order();
		} else if(input == 3) {
			for(auto i = map->keys().begin(); i != map->keys().end(); i++) {
				auto key = *i;
				std::cout << "| " << key->str() << ": " << map->get(key) << std::endl;
			}
		}
	}
}
