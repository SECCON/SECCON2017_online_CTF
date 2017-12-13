CXXFLAGS = -s -std=c++11 -pie -fPIC

dist: bin/inception distclean

bin/inception: obj/main.o obj/IO.o obj/VM3.o obj/VM2.o obj/VM1.o
	$(CXX) $^ $(CXXFLAGS) -o $@

obj/main.o: main.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

obj/IO.o: IO.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

obj/VM1.o: VM1.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

obj/VM2.o: VM2.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

obj/VM3.o: VM3.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

clean:
	rm bin/*
	rm obj/*

distclean:
	rm obj/* -f
