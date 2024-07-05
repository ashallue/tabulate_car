paths = -I/usr/local/include -L/usr/local/lib
tags = -lntl -lm -lgmp -O3 
#-ggdb 
debugtags = -lntl -lm -lgmp 
objects = gmpprint.o bigint.o Preproduct.o Pseudosquare.o Pinch.o Construct_car.o SmallP_Carmichael.o LargePreproduct.o Factgen.o functions.o int.o Odometer.o primetest.o postprocess.o 

all: main tab_serial tab_parallel test int_testing timings

%.o:	%.cpp
	g++ $(paths) -c $< $(tags)

#main.o:	main.cpp
#	g++ $(paths) -c main.cpp Stack.h

main:	main.o $(objects)
	g++ $(paths) main.o $(objects) -o tabulate $(tags)

#tab_serial.o: tab_serial.cpp
#	g++ $(paths) -c tab_serial.cpp Stack.h

tab_serial: tab_serial.o $(objects)
	g++ $(paths) tab_serial.o $(objects) -o tab_serial $(tags)

tab_parallel.o: tab_parallel.cpp
	mpic++ $(paths) -c tab_parallel.cpp Stack.h $(tags)

tab_parallel: tab_parallel.o $(objects)
	mpic++ $(paths) tab_parallel.o $(objects) -o tab_parallel $(tags)

#test.o:	test.cpp
#	g++ $(paths) -c test.cpp Stack.h

test:	test.o $(objects)
	g++ $(paths) test.o $(objects) -o test $(tags)

#int_testing.o:	int_testing.cpp
#	g++ $(paths) -c int_testing.cpp

int_testing:	int_testing.o $(objects)
	g++ $(paths) int_testing.o $(objects) -o int_testing $(tags)

#timings.o:	timings.cpp 
#	g++ $(paths) -c timings.cpp

timings:	timings.o $(objects)
	g++ $(paths) timings.o $(objects) -o timings $(tags) 

clean:
	rm *.o
