TARGET=spd_tool

PREFIX = 


CC = $(PREFIX)gcc

CFLAGS = -Wall -DNDEBUG 
LFLAGS = -s 

C_SRC := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(notdir $(C_SRC)))

.PHONY: all clean

all : clean $(TARGET) 

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f *.o *.d *~ core tags $(TARGET)
