# How to compile & run
Compile the program using the included Makefile, optionally setting the CC variable to the MPI C
compiler on your system, by running:
```bash
make
```
The makefile can also be used to compile and run the program with a set of sane defaults directly like:
```bash
make run
```
alternatively changing the defaults like.
```bash
make M=$M N=$N NUMTHREADS=$NTHREADS run
```

You may compile manually without using the Makefile by running:
```bash
mpicc -O3 *.c -o main.o
```
and the program is subsequently executed like
```bash
mpirun -n $NUMTHREADS ./main.o $M $N $K1 $K2
```

The program has tested to run successfully on the IFI linux server on with 24 threads and M=N=1000, K1=K2=5, and
on my own laptop with 4 threads yielding no errors in the "test" towards the end of MPI_main.c.

# Note
In addition to MPI_double_layer_convolution.c which implements:

- single_layer_convolution(...)
- MPI_double_layer_convolution(...)

I have also included the header-only library float2d.h, which implements

- init_float2d(...)
- free_float2d(...)
- print_float2d(...)

which handles allocation, freeing and printing contiguous 2D float arrays.
