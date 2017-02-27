ver = release
CC = g++

ifeq ($(ver), debug)
CFLAGS = -g -std=c++11 -Ddebug
else
CFLAGS = -g -std=c++11
endif

OBJS1    = wrap.o case.o
OBJS2    = wrap.o main.o ac.o
TARGETS = casefactory ac 

.PHONY: all 
all: $(TARGETS)

casefactory: $(OBJS1)
	$(CC) -o $@ $^ $(CFLAGS)

ac: $(OBJS2)
	$(CC) -o $@ $^ $(CFLAGS)	
	
%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

%.d: %.cpp
	@set -e; rm -f $@; $(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(OBJS1:.o=.d)
-include $(OBJS2:.o=.d)

.PHONY:clean 
clean:
	rm -f $(TARGETS) *.o *.d *.d.*