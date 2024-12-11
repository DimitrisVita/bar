CC = gcc
CFLAGS = -Wall -pthread
LDFLAGS = -lrt

all: receptionist visitor monitor

receptionist: receptionist.o shared_memory.o
	$(CC) $(CFLAGS) -o receptionist receptionist.o shared_memory.o $(LDFLAGS)

visitor: visitor.o shared_memory.o
	$(CC) $(CFLAGS) -o visitor visitor.o shared_memory.o $(LDFLAGS)

monitor: monitor.o shared_memory.o
	$(CC) $(CFLAGS) -o monitor monitor.o shared_memory.o $(LDFLAGS)

receptionist.o: receptionist.c shared_memory.h
	$(CC) $(CFLAGS) -c receptionist.c

visitor.o: visitor.c shared_memory.h
	$(CC) $(CFLAGS) -c visitor.c

monitor.o: monitor.c shared_memory.h
	$(CC) $(CFLAGS) -c monitor.c

shared_memory.o: shared_memory.c shared_memory.h
	$(CC) $(CFLAGS) -c shared_memory.c

clean:
	rm -f *.o receptionist visitor monitor