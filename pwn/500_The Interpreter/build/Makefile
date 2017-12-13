CXX:=em++
CXXFLAGS:= -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -std=c++14 -I. -g -s ASSERTIONS=2

all: interpreter.js

HANDLERS=handlers/arithmetic.cc\
handlers/stack.cc\
handlers/proc.cc\
handlers/array.cc\
handlers/string.cc

SRC=main.cc script.cc registered_handlers.cc value.cc error.cc $(HANDLERS)
HEADERS=handler.hpp config.hpp value.hpp error.hpp script.hpp

interpreter.js: $(SRC) $(HEADERS)
	$(CXX) $(SRC) $(CXXFLAGS) -o $@

clean:
	rm interpreter.js interpreter.wasm interpreter.wast -f