paths = -I/usr/local/include -L/usr/local/lib
tags = -lntl -lm -lgmp -O3  
objects = bigint.o Pinch.o Construct_car.o Factgen.o functions.o int.o Odometer.o primetest.o postprocess.o 

all: main test

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

clean:
	rm *.o
