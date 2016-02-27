CC=gcc

all: SDC SDC_ELS SDC_PRF

SDC: SDC.o
	$(LINK.c) SDC.o -o SDC

SDC.o: SDC.c SDC.h
	$(COMPILE.c) SDC.c -o SDC.o

SDC_ELS: SDC_ELS.o
	$(LINK.c) SDC_ELS.o -o SDC_ELS

SDC_ELS.o: SDC_ELS.c SDC_ELS.h
	$(COMPILE.c) SDC_ELS.c -o SDC_ELS.o

SDC_PRF: SDC_PRF.o
	$(LINK.c) SDC_PRF.o -o SDC_PRF

SDC_PRF.o: SDC_PRF.c SDC_PRF.h
	$(COMPILE.c) SDC_PRF.c -o SDC_PRF.o

clean:
	rm -f *.o

mrproper: clean
	rm -f SDC SDC_ELS SDC_PRF