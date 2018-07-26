
CC = g++
CFLAGS = -O3 -Wall
LDFLAGS =

#all: Cgnuplot

#%: main_%.cpp $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

Cgnuplot.o:
	$(CC) $(CFLAGS) -c Cgnuplot.c

clean:
	rm -f *.o *~ *.swp Cgnuplot

