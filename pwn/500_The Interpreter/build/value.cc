#include "value.hpp"

NilValue nil;

Value *ArrayValue::subscr(Script *sc, Value *offset) {
	if(offset->type() != ImmValue::type) {
		fatal_error(sc, "offset must be immediate value");
		return NULL;
	}
	auto offset_value = static_cast<ImmValue *>(offset)->value();
	if(offset_value >= s.size()) {
		fatal_error(sc, "array out of bound access");
	}
	return s[offset_value];
}