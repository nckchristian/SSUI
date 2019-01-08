#
#    All contents of this file were written by Brandon Mord 
#    bdrmord001@gmail.com
# 
#    Original owner of git code Bmord01
#
# change application name here (executable output name)
TARGET=SmartSealz

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall
 
PTHREAD=-pthread
 
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe
 
GTKLIB=`pkg-config --cflags --libs gtk+-3.0`
 
# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic
 
OBJS=	main.o
 
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
	mv SmartSealz /home/pi/Desktop
	rm -f *.o
    
main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o
    
clean:
	rm -f *.o $(TARGET)
