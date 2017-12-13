#include "script.hpp"
#include <iostream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

int main(int argc, char **argv) {
	std::cout.setf(std::ios::unitbuf);
	if(argc < 2) {
		std::cout << ("Usage: ./interpreter [script file]") << std::endl;
		return 1;
	}
	#ifdef __EMSCRIPTEN__
	  EM_ASM(
		FS.mkdir('/working');
		FS.mount(NODEFS, { root: '/' }, '/working');
	  );
	#endif
	Script *script = Script::loadScriptFromFd(fopen(argv[1],"r"));
	if(script->Execute())
		fatal_error(NULL, script->error());
	return 0;
}