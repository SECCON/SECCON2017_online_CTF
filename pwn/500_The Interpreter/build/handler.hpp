#pragma once

#include "types.hpp"
#include "script.hpp"
#include <iostream>

class Handler {
protected:
	Script *script_;
	bool available(size bytes) {
		if(script_->ctx().pc + bytes + 1 > script_->code_size()) {
			script_->set_error("Execution error");
			return false;
		} else {
			return true;
		}
	};
	address u8(address offset) {
		byte r = script_->code()[script_->ctx().pc + offset + 1];
		return r;
	}
	address u16(address offset) {
		return (u8(offset + 1) * 256) | u8(offset);
	}
	address u32(address offset) {
		return (u16(offset + 2)  * 65536) | u16(offset);
	}
	byte *at(address offset) {
		return script_->code() + script_->ctx().pc + offset + 1;
	}
	void fatal_error(const char *message) {
		::fatal_error(script_, message);
	}
	Value **regs_;
	vm_context *ctx_;
public:
	virtual void Run() = 0;
	void Bind(Script *script) {
		script_ = script;
		regs_ = script_->ctx().regs;
		ctx_ = &script->ctx();
	}

	std::vector<Value *> &stack() { return script_->stack(); }
};

#define DST (u8(0) >> 4)
#define SRC (u8(0)) & 0xf
#define SRC2 (u8(1) & 0xf)

#define WITH_CODE(code) \
	public: \
	static constexpr byte handler_code() { return code; }

extern Handler *g_handler_map[];

#define H(code, name) class name##Handler: public Handler { \
	virtual void Run(); \
};

#include "registered_handlers.hpp"

#undef H