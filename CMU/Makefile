all: temperature.out

temperature.out: temperature.cpp chain.c chain.h repeat.h mem.h
	g++ temperature.cpp chain.c -lstdc++ -o temperature.out

clean:
	rm -rf temperature.out