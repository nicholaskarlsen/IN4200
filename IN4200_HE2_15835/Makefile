CC=mpicc
MPIRUN=mpirun
#CC=/usr/lib64/openmpi/bin/mpicc
#MPIRUn=/usr/lib64/openmpi/bin/mpirun
FLAGS=-Wall -O3
NUMTHREADS=4

M=1000
N=1000
K1=5
K2=5

main:
	$(CC) $(FLAGS) *.c -o main.o

clean:
	rm main.o

run: main
	$(MPIRUN) -n $(NUMTHREADS) main.o $(M) $(N) $(K1) $(K2)
