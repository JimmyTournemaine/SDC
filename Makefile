CC=gcc

all: SDC SDC_ELS SDC_PR SA

SDC: SDC.o
	$(LINK.c) SDC.o -o SDC

SDC.o: SDC.c SDC.h
	$(COMPILE.c) SDC.c -o SDC.o

SA: SA.o
	$(LINK.c) SA.o -o SA

SA.o: SA.c SA.h
	$(COMPILE.c) SA.c -o SA.o

SDC_ELS: SDC_ELS.o
	$(LINK.c) SDC_ELS.o -o SDC_ELS

SDC_ELS.o: SDC_ELS.c SDC_ELS.h
	$(COMPILE.c) SDC_ELS.c -o SDC_ELS.o

SDC_PR: SDC_PR.o
	$(LINK.c) SDC_PR.o -o SDC_PR

SDC_PR.o: SDC_PR.c SDC_PR.h
	$(COMPILE.c) SDC_PR.c -o SDC_PR.o

clean:
	rm -f *.o

mrproper: clean
	rm -f SDC SDC_ELS SDC_PR SA