#pragma once

#include "types.hpp"
#include "error.hpp"
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <iomanip>

enum ValueType {
	NIL,
	IMM,
	ARRAY,
	STRING
};

class Script;

class Value {
protected:
	byte type_;
public:
	Value(): type_(0) {}
	byte type() { return type_; }
	void set_type(byte type) { type_ = type; }

	virtual Value *subscr(Script *s, Value *offset) {
		fatal_error(s, "NotImplemented: operation subscr");
		return NULL;
	};
	virtual Value *add(Script *s, Value *other) {
		fatal_error(s, "NotImplemented: operation add");
		return NULL;
	};
	virtual Value *sub(Script *s, Value *other) {
		fatal_error(s, "NotImplemented: operation sub");
		return NULL;
	};
	virtual Value *mul(Script *s, Value *other) {
		fatal_error(s, "NotImplemented: operation mul");
		return NULL;
	};
	virtual bool nonzero() {
		return false;
	}
	virtual std::string *repr(Script *sc) {
		return new std::string("<invalid>");
	};
};

class NilValue: public Value {
public:
	static const int type = NIL;
	NilValue() {
		set_type(NilValue::type);
	}
	virtual std::string *repr(Script *sc) {
		return new std::string("None");
	}
};

extern NilValue nil;

class ImmValue: public Value {
protected:
	address value_;
public:
	static const byte type = IMM;
	ImmValue(ImmValue *other): value_(other->value_) {
		set_type(ImmValue::type);
	}
	ImmValue(address value): value_(value) {
		set_type(ImmValue::type);
	}
	address value() { return value_; }

	#define ARITH(name, op) \
	virtual Value *name(Script *sc, Value *other) override { \
		std::stringstream ss;\
		if(other->type() == ImmValue::type) { \
			return new ImmValue(value_ op static_cast<ImmValue *>(other)->value_); \
		} \
		ss << *repr(sc) << " "#op" " << *other->repr(sc) << ": imm + imm is only allowed for addition";\
		fatal_error(sc, ss.str().c_str());\
		return NULL; \
	};
	virtual bool nonzero() override {
		return !!value_;
	}
	ARITH(add, +)
	ARITH(sub, -)
	ARITH(mul, *)
	#undef ARITH
	virtual std::string *repr(Script *sc) override {
		return new std::string(std::to_string(value_));
	}
};

class ArrayValue: public Value {
protected:
	std::vector<Value *> s;
public:
	static const int type = ARRAY;
	ArrayValue(size array_size) {
		Init(array_size);
	}

	void Init(size array_size) {
		s.reserve(array_size);
	}

	void push_front(Value *v) {
		s.insert(s.begin(), 1, v);
	}
	
	void push_back(Value *v) {
		s.push_back(v);
	}

	virtual Value *subscr(Script *sc, Value *offset) override;
	virtual Value *add(Script *sc, Value *other) override {
		std::stringstream ss;
		if(other->type() == ArrayValue::type) {
			auto *other_imm = static_cast<ArrayValue *>(other);
			address old_size = s.size();
			address old_size2 = other_imm->s.size();
			address new_size = old_size + old_size2;
			if(new_size < old_size || new_size < old_size2) {
				fatal_error(sc, "Array size overflow");
				return NULL;
			}
			ArrayValue *newone = new ArrayValue(new_size);
			int j = 0;
			for(int i = 0; i < old_size; i++, j++) {
				newone->s[j] = s[i];
			}
			for(int i = 0; i < old_size2; i++, j++) {
				newone->s[j] = other_imm->s[i];
			}
			return newone;
		}
		ss << *repr(sc) << " + " << *other->repr(sc) << ": array + array is only allowed for concatenating";
		fatal_error(sc, ss.str().c_str());
		return NULL;
	}
	virtual Value *mul(Script *sc, Value *other) override {
		std::stringstream ss;
		if(other->type() == ImmValue::type) {
			ImmValue *other_imm = static_cast<ImmValue *>(other);
			address old_size = s.size();
			address new_size = other_imm->value() * old_size;
			if(new_size < old_size) {
				ss << *repr(sc) << " * " << *other_imm->repr(sc) << ": " << "Array size overflow";
				fatal_error(sc, ss.str().c_str());
				return NULL;
			}
			ArrayValue *newone = new ArrayValue(new_size);
			for(int i = 0; i < other_imm->value(); i++) {
				for(int j = 0; j < old_size; j++) {
					newone->s[i * old_size + j] = s[j];
				};
			}
			return newone;
		}
		ss << *repr(sc) << " * " << *other->repr(sc) << ": array * imm is only allowed for multiplication";
		fatal_error(sc, ss.str().c_str());
		return NULL;
	}
	virtual bool nonzero() override{
		return !!s.size();
	}
	virtual std::string *repr(Script *sc) override {
		std::stringstream r("[");
		if(s.size()) {
			r << s[0]->repr(sc);
			for(int i = 1; i < s.size(); i++) {
				r << ", ";
				r << s[i]->repr(sc);
			}
		}
		r << "]";
		return new std::string(r.str());
	}
};

class StringValue: public Value {
protected:
	byte *start_, *end_;
public:
	static const int type = STRING;
	StringValue(byte *start, size size): start_(start), end_(start + size) {
		set_type(StringValue::type);
		if(end_ < start_) {
			start_ = end_ = NULL;
		}
	}
	virtual Value *add(Script *sc, Value *other) override {
		std::stringstream ss;
		if(other->type() == StringValue::type) {
			StringValue *str = static_cast<StringValue *>(other);
			size old_size = end_ - start_;
			size old_size2 = str->end_ - str->start_;
			size new_size = old_size + old_size2;
			if(new_size < old_size || new_size < old_size2) {
				fatal_error(sc, "string overflow");
				return NULL;
			}
			byte *newchar = new byte[new_size];
			memcpy(newchar, start_, old_size);
			memcpy(newchar + old_size, str->start_, old_size2);
			auto *newone = new StringValue(newchar, new_size);
			return newone;
		}
		ss << *repr(sc) << " + " << *other->repr(sc) << ": str + str is only allowed for concatenating";
		fatal_error(sc, ss.str().c_str());
		return NULL;
	}
	virtual Value *mul(Script *sc, Value *other) override {
		std::stringstream ss;
		if(other->type() == ImmValue::type) {
			ImmValue *other_imm = static_cast<ImmValue *>(other);
			address old_size = (address)(end_ - start_);
			address new_size = other_imm->value() * old_size;
			byte *newchar;
			if(new_size < old_size) {
				ss << *repr(sc) << " * " << *other_imm->repr(sc) << ": " << "String size overflow";
				fatal_error(sc, ss.str().c_str());
				return NULL;
			}
			newchar = new byte[new_size];
			StringValue *newone = new StringValue(newchar, new_size);
			for(int i = 0; i < other_imm->value(); i++) {
				for(int j = 0; j < old_size; j++) {
					newchar[i * old_size + j] = start_[j];
				};
			}
			return newone;
		}
		ss << *repr(sc) << " * " << *other->repr(sc) << ": str * imm is only allowed for string multiplication";
		fatal_error(sc, ss.str().c_str());
		return NULL;
	}
	virtual bool nonzero() override {
		return !!length();
	}
	virtual std::string *repr(Script *sc) override {
		std::stringstream r;
		r << "'";
		for(byte *cur = start_; cur < end_; cur++) {
			if(!isprint(*cur) && !isspace(*cur)) {
				r << "\\x";
				r << std::hex << std::setfill('0') << std::setw(2);
				r << *cur;
			} else if(*cur == '\x27') {
				r << "\\'";
			} else {
				r << (char)*cur;
			}
		}
		r << "'";
		return new std::string(r.str());
	}
	size length() {return end_ - start_;}
};

class AddrValue: public ImmValue {
public:
	AddrValue(AddrValue *other): ImmValue(static_cast<ImmValue *>(other)) {
	}
	AddrValue(void *value): ImmValue((address)value) {
	}
	virtual std::string *repr(Script *sc) override {
		return new std::string("<internal_address>");
	}
	void *addr() {
		return reinterpret_cast<void *>(value_);
	}

};