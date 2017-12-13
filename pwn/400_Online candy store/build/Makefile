CC=gcc
CCFLAGS=-g3 -O0 -Wall -w -fstack-protector-all -m64

O_FILES = Global.o Order.o UserInput.o Title.o Candy.o  Stock.o Purchase.o Account.o Charge.o main.o Board.o

all : $(O_FILES)
	$(CC) $(CCFLAGS) -o Lazenca.0x0 $(O_FILES)
	strip -g -S -d Lazenca.0x0
	strip -s Lazenca.0x0
	strip -x Lazenca.0x0
	strip -X Lazenca.0x0

-include $(O_FILES:.o=.d)

.SUFFIXES : .o .c

.c.o :
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f Lazenca.0x0 *.o *.d *.c~ *.h~
