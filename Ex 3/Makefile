all: NumericalAnalyzer

NumericalAnalyzer: NumericalAnalyzer.o libinfi.a 
	gcc -Wextra -Wvla -Wall -g -lm -std=c99 NumericalAnalyzer.o -L . -linfi -o NumericalAnalyzer

NumericalAnalyzer.o: NumericalAnalyzer.c infi.h
	gcc -Wextra -Wvla -Wall -g -lm -std=c99 -c NumericalAnalyzer.c

infi.o:  infi.h infi.c
	gcc -Wextra -Wvla -Wall -g -lm -std=c99 -c infi.c

libinfi.a: infi.o infi.h
	ar rcs libinfi.a infi.o

libs: libinfi.a

clean:
	rm -f NumericalAnalyzer *.o *.a 

.PHONY: clean
