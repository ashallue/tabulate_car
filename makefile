paths = -I/usr/local/include -L/usr/local/lib
tags = -lntl -lm -lgmp -O3  
debugtags = -lntl -lm -lgmp -ggdb
objects = bigint.o Pseudosquare.o Pinch.o Construct_car.o Factgen.o functions.o int.o LargeP_Odometer.o Odometer.o primetest.o postprocess.o 

all: main test int_testing timings

%.o:	%.cpp
	g++ $(paths) -c $<

main.o:	main.cpp
	g++ $(paths) -c main.cpp Stack.h

main:	main.o $(objects)
	g++ $(paths) main.o $(objects) -o tabulate $(tags)

test.o:	test.cpp
	g++ $(paths) -c test.cpp Stack.h

test:	test.o $(objects)
	g++ $(paths) test.o $(objects) -o test $(tags)

int_testing.o:	int_testing.cpp
	g++ $(paths) -c int_testing.cpp

int_testing:	int_testing.o $(objects)
	g++ $(paths) int_testing.o $(objects) -o int_testing $(tags)

timings.o:	timings.cpp 
	g++ $(paths) -c timings.cpp

timings:	timings.o $(objects)
	g++ $(paths) timings.o $(objects) -o timings $(tags) 

clean:
	rm *.o
