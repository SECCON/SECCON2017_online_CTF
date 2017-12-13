#include "handler.hpp"

void PUSHHandler::Run() {
	if(!available(1)) return;
	script_->stack().push_back(regs_[SRC]);
	ctx_->pc += 2;
}

void POPHandler::Run() {
	if(!available(1)) return;
	regs_[DST] = script_->stack().back();
	script_->stack().pop_back();
	ctx_->pc += 2;
}

