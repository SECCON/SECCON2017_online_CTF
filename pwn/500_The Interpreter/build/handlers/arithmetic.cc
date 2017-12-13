#include "handler.hpp"

void MOVHandler::Run() {
	if(!available(1)) return;
	regs_[DST] = regs_[SRC];
	ctx_->pc += 2;
}

#define ARITH(op, handler) \
void op##Handler::Run() { \
	if(!available(2)) return; \
	regs_[DST] = regs_[SRC]->handler(script_, regs_[SRC2]); \
	ctx_->pc += 3; \
}

ARITH(ADD, add);
ARITH(SUB, sub);
ARITH(MUL, mul);

void JMPHandler::Run() {
	if(!available(4)) return;
	ctx_->pc = u32(0);
}

#define COND(op, cond) \
void op##Handler::Run() { \
	if(!available(5)) return; \
	if(cond) ctx_->pc = u32(1); \
	else ctx_->pc += 6; \
}

COND(JZ, !regs_[SRC]->nonzero());
COND(JNZ, regs_[SRC]->nonzero());

void LOADIMMHandler::Run() {
	if(!available(5)) return;
	regs_[DST] = new ImmValue(u32(1));
	ctx_->pc += 6;
}