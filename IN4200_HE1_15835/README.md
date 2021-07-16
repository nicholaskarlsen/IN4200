# Instructions

The program is compiled using the Makefile by running make in the root directory. A binary then produced in the bin/ subdirectory.
You may also compile and directly run the program by invoking
```
make run
```
This compiles the program and immediately runs the main function, which performs all of the unit tests and benchmarks, writing the results to the terminal. Running the program should take ~10s depending on the specs of your computer.

You may also check for memory leaks in the program with Valgrind by running
```
make debug
```
which writes its output to the file valgrind-out.txt. This will take quite a lot of time due to the benchmarking. Consider commenting out the benchmarks from the main() function in main.c if you wish to run valgrind.


NOTE: On the offchance that i forget to change the CC variable in the makefile from the one that i use on my personal computer to the standard gcc command, please uncomment the 2nd line in the Makefile, and comment out the 1st line.
