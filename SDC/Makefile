CC=gcc
SRV=SDC
EXEC=SDC

all: $(EXEC)

$(EXEC): SDC.o
	$(LINK.c) SDC.o -o $(EXEC)

SDC.o: SDC.c SDC.h
	$(COMPILE.c) SDC.c -o SDC.o

clean:
	rm -f SDC.o

mrproper: clean
	rm -rf $(EXEC)