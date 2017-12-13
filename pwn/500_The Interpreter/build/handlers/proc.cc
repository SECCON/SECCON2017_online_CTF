#include "handler.hpp"

void RETHandler::Run() {
	ImmValue *retaddr = static_cast<ImmValue *>(script_->frame()[FRAME_RETADDR]);
	ctx_->pc = retaddr->value();
	script_->pop_frame();
}

void CALLHandler::Run() {
	if(!available(6)) return;
	script_->push_frame(u16(4), ctx_->pc + 7);
	ctx_->pc = u32(0);
}

void LOADARGHandler::Run() {
	if(!available(3)) return;
	ImmValue *tmp = new ImmValue(u16(1));
	auto args = static_cast<ArrayValue *>(
		script_->frame()[FRAME_ARGS]);
	regs_[DST] = args->subscr(script_, tmp);
	delete tmp;
	ctx_->pc += 4;
}

void EXITHandler::Run() {
	ctx_->keep_execution = false;
}