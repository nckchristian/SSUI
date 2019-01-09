#
#    All contents of this file were written by Brandon Mord 
#    bdrmord001@gmail.com
#    
#    Original owner of git code Bmord01
#
# change application name here (executable output name)
TARGET=SSUI

# compiler
CC=gcc
# debug
#DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread
#$(DEBUG)
CCFLAGS= $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS=	main.o

compile: $(OBJS)
	cd ShortcutFiles && make -f makefile
	pwd > Env.txt
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS) -lm -lpthread -lX11
	rm -f *.o
	
main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o

run: $(OBJS)
	./SSUI & ./traffic.out & ./TPO.out & python3 heartBeats.py

bg:
	gcc -g Source/main.c -o Nav.exe -lwiringPi -lm
	chmod 777 heartBeats.py
	gcc -g Source/PerlOut.c -o TPO.exe
	g++ -std=c++11 -o traffic.exe cpp/*.cpp -pthread -lX11 
	chmod 777 GPS.pl
	rm -f *.o
clean:
	rm -f *.o $(TARGET) 
	rm -f *.out
install:
	gcc -g Source/install.c -o Source/install.exe
	./Source/install.exe
	rm Source/install.exe
