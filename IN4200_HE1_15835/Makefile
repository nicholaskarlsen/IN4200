CC=/opt/homebrew/bin/gcc-10
#CC=gcc
FLAGS=-Wall -O3 -fopenmp

main:
	$(CC) $(FLAGS) src/*.c -o bin/main.o

clean:
	rm bin/main.o

run: main
	./bin/main.o

debug: clean
	$(CC) $(FLAGS) -ggdb3 src/*.c -o bin/main.o
	valgrind \
		--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=valgrind-out.txt \
		./bin/main.o

