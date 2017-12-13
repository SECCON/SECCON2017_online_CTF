CXXFLAGS = -std=c++11
LDFLAGS = -lssl -lcrypto

dist: build/challenge distclean

build/challenge: build/main.o
	$(CXX) $^ $(LDFLAGS) $(CXXFLAGS) -o $@

build/main.o: src/main.cc src/hashmap.hpp src/crc32_tab.hpp src/stringkey.hpp
	mkdir -p build
	$(CXX) src/main.cc $(CXXFLAGS) -c -o $@

clean:
	rm build -rf

distclean:
	rm build/main.o -f
