CC=g++
CFLAGS=-Wall -I.
DEPS = SMatrix.h
OBJ = smatrix_test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

smatrix_test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

check: smatrix_test
	./smatrix_test

clean:
	rm *.o smatrix_test