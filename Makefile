ver = release
CC = g++

ifeq ($(ver), debug)
CFLAGS = -g -std=c++11 -Ddebug
else
CFLAGS = -g -std=c++11
endif

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

.PHONY:clean 
clean:
	rm -f $(TARGETS) *.o *.d *.d.*