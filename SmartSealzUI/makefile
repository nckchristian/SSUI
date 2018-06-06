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
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS) -lm
	rm -f *.o
	
main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o

run: $(OBJS)
	./SSUI & ./traffic.out & ./TPO.out & python3 heartBeats.py

bg:
	#gcc -g main.c -o Nav.exe -lwiringPi -lm
	chmod 777 heartBeats.py
	gcc -g Source/PerlOut.c -o TPO.exe
	g++ -std=c++11 -o cpp/traffic.exe cpp/easywsclient.cpp cpp/jsoncpp.cpp cpp/traffic.cpp cpp/main.cpp
	chmod 777 GPS.pl
	gcc RunSSUI.c -o RunSSUI.exe
	rm -f *.o
clean:
	rm -f *.o $(TARGET) 
	rm -f *.out
FirstSetup:
	gcc -g install.c -o install.exe
	./install
