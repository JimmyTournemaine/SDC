CC=gcc
EXEC=SRC_ELS

all: $(EXEC)

$(EXEC): socket.o student.o SRC_ELS.o
	$(LINK.c) socket.o student.o SRC_ELS.o -o $(EXEC)

socket.o: socket.c socket.h
	$(COMPILE.c) socket.c -o socket.o

student.o: student.c student.h
	$(COMPILE.c) student.c -o student.o

SRC_ELS.o:
	$(COMPILE.c) SRC_ELS.c -o SRC_ELS.o

clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)