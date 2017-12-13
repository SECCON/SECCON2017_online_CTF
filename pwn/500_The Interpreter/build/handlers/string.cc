#include "handler.hpp"

void LOADSTRHandler::Run() {
	if(!available(5)) return;
	size s = u32(1);
	if(s >= 10000) {
		char tmp[100];
		snprintf(tmp, 100, "string literal too large (%d)", s);
		fatal_error(tmp);
		return;
	}
	if(available(s + 5)) {
		byte *newone = new byte[s];
		memcpy(newone, at(5), s);
		regs_[DST] = new StringValue(newone, s);
	}
	ctx_->pc += 6 + s;
}

void PRINTHandler::Run() {
	if(!available(1)) return;
	auto val = regs_[SRC]->repr(script_);
	std::cout << *val << std::endl;
	delete val;
	ctx_->pc += 2;
}