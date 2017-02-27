ver = release
PROGS = casefactory ac
CC = g++

ifeq ($(ver), debug)
CFLAGS = -g -O2 -Wall -std=c++11 -Ddebug
else
CFLAGS = -g -O2 -Wall -std=c++11
endif

<<<<<<< HEAD
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
=======
OBJS    = wrap.o
TARGETS = casefactory ac 

.PHONY: all 
all: $(TARGETS)

casefactory: case.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

ac: main.o ac.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)	
	
%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

%.d: %.cpp
	@set -e; rm -f $@; $(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(OBJS:.o=.d)
>>>>>>> parent of fb2957b... 修复了自动生成头文件依赖的bug

clean:
	rm -f ${PROGS} *.o

.PHONY: clean
