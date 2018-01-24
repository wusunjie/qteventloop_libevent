all: boardchecker

netcore.o: core/netcore.c
	gcc -g `pkg-config --cflags libevent` -c $< -o $@

netlist.o: core/netlist.cpp
	gcc -g -std=c++11 -c $< -o $@

main.o: core/main.cpp
	gcc -g -std=c++11 -c $< -o $@

boardchecker: netlist.o netcore.o main.o
	g++ -g $^ -o $@ `pkg-config --libs libevent`

clean:
	rm -rf netlist.o netcore.o main.o boardchecker
