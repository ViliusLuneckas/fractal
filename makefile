all: clean fractal

fractal: fractal.o
	g++ fractal.o -o fractal -lGL -lglfw

fractal.o: fractal.cpp
	g++ -c fractal.cpp

clean:
	rm -f *.o fractal

format:
	astyle *.cpp --style=k/r --indent=spaces=2
