#include "config.hpp"
#include "script.hpp"
#include "handler.hpp"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static void r(FILE *fp, void *buf, int size) {
	uint32_t res;
	if ((res=fread(buf, 1, size, fp)) != size) {
		fatal_error(NULL, "ParserError: unexpected EOF at reading the script file");
	}
}

Script *Script::loadScriptFromFd(FILE *fp) {
	byte *buf;
	char tmp[9];
	uint32_t file_size;

	if(!fp) {
		fatal_error(NULL, "FileError: script file not found");
		return NULL;
	}
	r(fp, tmp, 4);
	if(memcmp(tmp, "GRRR", 4)) {
		fatal_error(NULL, "ParserError: magic does not match: expected 'GRRR'.");
		return NULL;
	}
	r(fp, tmp, 8);
	tmp[8] = 0;
	file_size = strtoul(tmp, NULL, 16);
	if(file_size > MAX_FILESIZE) {
		fatal_error(NULL, "ParserError: Too big");
		return NULL;
	}
	buf = new byte[file_size * 2];
	r(fp, buf, file_size * 2);
	tmp[2] = 0;
	for(int i = 0; i < file_size * 2; i+=2) {
		memcpy(tmp, buf + i, 2);
		buf[i/2] = strtoul(tmp, NULL, 16);
	}
	return new Script(buf, file_size);
}

int Script::Execute() {
	std::stringstream err;
	error_ = NULL;
	while(true) {
		if(this->ctx_.pc >= this->code_size_) {
			err << ("Execution fault at 0x") << std::hex << ctx_.pc;
			set_error(err.str().c_str());
			return ERROR;
		}
		Handler *h = g_handler_map[this->code_[this->ctx_.pc]];
		// printf("%05x %d\n", this->ctx_.pc, this->code_[this->ctx_.pc]);
		if(!h) {
			set_error("Illegal opcode");
			return ERROR;
		}
		h->Bind(this);
		h->Run();
		if(this->ctx_.cycle++ >= MAX_CYCLE) {
			set_error("Program runs too long");
			return ERROR;
		}
		if(this->ctx_.keep_execution == false) {
			return SUCCESS;
		}
		if(this->error_) {
			return ERROR;
		}
	}
}

void Script::set_error(const char *message) {
	this->error_ = strdup(message);
}