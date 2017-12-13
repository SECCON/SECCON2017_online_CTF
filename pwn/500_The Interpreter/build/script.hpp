#pragma once

#include "types.hpp"
#include "value.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <dlfcn.h>
#include <stdio.h>
#include <vector>

enum FrameValue {
	FRAME_RETADDR,
	FRAME_ARGS,
	FRAME_NEXT
};

struct vm_context {
	address pc, cycle;
	Value *regs[16];
	bool keep_execution;
};

class Script {
	vm_context ctx_;
	const char *error_;
	const char *banner_;
	byte *code_;
	size code_size_;
	std::vector<Value *> stack_;
	Value **frame_;

public:
	Script(byte *code, size length): code_(code), code_size_(length), error_(NULL) {
		ctx_.keep_execution = true;
		banner_ = "Interpreter Version 1.0";
		print_banner();
		for(int i = 0; i < 16; i++)
			ctx_.regs[i] = &nil;
		ctx_.pc = 0;
		ctx_.cycle = 0;
		// push initial frame with nextframe NULL
		stack_.reserve(0x1000);
		frame_ = NULL;
		push_frame(0, -1);
	};
	int print_banner() {
		std::stringstream str;
		#ifdef __EMSCRIPTEN__
		str << "console.log('" << banner_ << "');";
		emscripten_run_script(str.str().c_str());
		#endif
		return true;
	}
	void set_error(const char *message);
	void pop_frame() {
		frame_ = reinterpret_cast<Value **>(
			static_cast<AddrValue *>(stack_.back())->addr());
		stack_.pop_back();
		stack_.pop_back();
		stack_.pop_back();
	}
	void push_frame(uint16_t argc, address retaddr) {
		if(argc > stack_.size()) {
			fatal_error(this, "too many arguments");
			return;
		}
		ArrayValue *args = new ArrayValue(argc);
		for(int i = 0; i < argc; i++) {
			args->push_back(stack_[-argc+i]);
		}
		stack_.erase(stack_.begin()+stack_.size()-argc,stack_.end());
		Value **frame = &*stack_.end();
		stack_.push_back(new ImmValue(retaddr));
		stack_.push_back(args);
		stack_.push_back(new AddrValue(frame_));
		frame_ = frame;
		if(frame != &*stack_.end() - 3) puts("triggered\n");
	}
	int Execute();
	size code_size() {return code_size_;}
	byte *code() {return code_;}
	vm_context &ctx() {return ctx_;}
	const char *error() { return error_; }
	static Script *loadScriptFromFd(FILE *fp);
	std::vector<Value *> &stack() { return stack_; }
	Value **frame() { return frame_; }

	enum Result {
		SUCCESS,
		ERROR
	};
};