CC = gcc
CFLAGS = -Wall -pthread -Iinclude

all: visitor receptionist monitor

visitor: src/visitor.o src/common.o
	$(CC) $(CFLAGS) -o visitor src/visitor.o src/common.o

receptionist: src/receptionist.o src/common.o
	$(CC) $(CFLAGS) -o receptionist src/receptionist.o src/common.o

monitor: src/monitor.o src/common.o
	$(CC) $(CFLAGS) -o monitor src/monitor.o src/common.o

src/visitor.o: src/visitor.c include/common.h
	$(CC) $(CFLAGS) -c src/visitor.c -o src/visitor.o

src/receptionist.o: src/receptionist.c include/common.h
	$(CC) $(CFLAGS) -c src/receptionist.c -o src/receptionist.o

src/monitor.o: src/monitor.c include/common.h
	$(CC) $(CFLAGS) -c src/monitor.c -o src/monitor.o

src/common.o: src/common.c include/common.h
	$(CC) $(CFLAGS) -c src/common.c -o src/common.o

clean:
	rm -f src/*.o visitor receptionist monitor