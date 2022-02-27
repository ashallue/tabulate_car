paths = -I/usr/local/include -L/usr/local/lib
tags = -lntl -lm -lgmp -O3 -pg 
objects = bigint.o Pinch.o Construct_car.o Factgen.o functions.o int.o Odometer.o primetest.o 

all: main

%.o:	%.cpp
	g++ $(paths) -c $<

main.o:	main.cpp
	g++ $(paths) -c main.cpp Stack.h

main:	main.o $(objects)
	g++ $(paths) main.o $(objects) -o tabulate $(tags)

clean:
	rm *.o
