ver = release
PROGS = casefactory ac
CC = g++

ifeq ($(ver), debug)
CFLAGS = -g -O2 -Wall -std=c++11 -Ddebug
else
CFLAGS = -g -O2 -Wall -std=c++11
endif

all: ${PROGS}
#	rm -f *.csv

casefactory: case.o wrap.o
	${CC} -o casefactory case.o wrap.o ${CFLAGS}
	
ac: main.o wrap.o ac.o
	${CC} -o ac ac.o wrap.o main.o ${CFLAGS}
	
main.o: main.cpp wrap.h ac.h
	${CC} -o main.o -c main.cpp ${CFLAGS}
	
ac.o: ac.cpp wrap.h ac.h
	${CC} -o ac.o -c ac.cpp ${CFLAGS}
	
case.o: case.cpp wrap.h
	${CC} -o case.o -c case.cpp ${CFLAGS}
	
wrap.o: wrap.cpp wrap.h
	${CC} -o wrap.o -c wrap.cpp ${CFLAGS}

clean:
	rm -f ${PROGS} *.o

.PHONY: clean