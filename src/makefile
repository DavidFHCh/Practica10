OBJ = list.o strmap.o
OBJ2 = p0.o list.o strmap.o

default: P10 p9

P10: P10.o 
	gcc -o P10 P10.o strmap.o -lm

P10.o : P10.c $(OBJ)
	gcc -c P10.c $(OBJ)


p9: p9.o
	gcc p9.o -o p9  $(OBJ)

p9.o: p9.c $(OBJ)
	gcc -c p9.c -o p9.o

list.o : list.c list.h
	gcc -c list.c

strmap.o : strmap.c strmap.h
	gcc -c strmap.c

clean:
	-rm -f *.o
	-rm -f p9
	-rm -f P10

run:
	./p9