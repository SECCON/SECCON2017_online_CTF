# vim: set sw=4 ts=4 ai :
all: fsmachine default.fs

fsmachine: fsmachine.o
	$(CC) -o $@ $<

default.fs: generate_fs.py eq.json
	./generate_fs.py

eq.json: generate_equation.py
	./generate_equation.py

clean::
	rm -f fsmachine.o fsmachine default.fs
