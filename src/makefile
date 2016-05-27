OBJ = list.o strmap.o

default: p9

p9.o: p9.c $(OBJ)
	gcc -c p9.c -o p9.o

p9: p9.o
	gcc p9.o -o p9  $(OBJ)

list.o : list.c list.h
	gcc -c list.c

strmap.o : strmap.c strmap.h
	gcc -c strmap.c

clean:
	-rm -f *.o
	-rm -f p9

run:
	./p9