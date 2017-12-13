#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "script.hpp"

void fatal_error(Script *s, const char *message) {
	char tmp[100];
	std::cerr << std::hex << "fatal error: " << message << std::endl;
	if(!s) exit(0);
	std::cerr << std::hex << "===REGISTERS===" << std::endl;
	for(int i = 0; i < 16; i++) {
		if(s->ctx().regs[i] != &nil) {
			snprintf(tmp, 100, "r%02d: %p, type: %d\n", i, s->ctx().regs[i], s->ctx().regs[i]->type());
			std::cerr << tmp;
		}
	}
	std::cerr << std::hex << "===FRAMES===" << std::endl;
	Value **cur = s->frame();
	while(cur) {
		std::cerr << "#0 (0x" << cur << "): ";
		address addr = static_cast<ImmValue *>(cur[0])->value();
		if(addr == -1)
			std::cerr << "<root>\n";
		else
			std::cerr << "sub_" << addr << std::endl;
		cur = reinterpret_cast<Value **>(
			static_cast<AddrValue *>(cur[2])->addr());
	}
	exit(0);
}