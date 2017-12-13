#include "handler.hpp"

Handler *g_handler_map[256] = {reinterpret_cast<Handler *>(0), };

class HandlerInitializer {
public:
#define H(code, name) g_handler_map[code] = new name##Handler();

HandlerInitializer() {
#include "registered_handlers.hpp"
}

#undef H
};

HandlerInitializer h;