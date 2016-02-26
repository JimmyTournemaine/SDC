CC=gcc
SRV=SDC
EXEC=SDC_ELS

all: $(EXEC)

$(EXEC): SDC_ELS.o
	$(LINK.c) SDC_ELS.o -o $(EXEC)

SDC_ELS.o: SDC_ELS.c SDC_ELS.h
	$(COMPILE.c) SDC_ELS.c -o SDC_ELS.o

clean:
	rm -f SDC_ELS.o

mrproper: clean
	rm -rf $(EXEC)