COMMON_SRC = $(wildcard common/base/*.cpp common/message/*.cpp)
COMMON_SRC_NOTDIR = $(notdir $(COMMON_SRC))
COMMON_OBJS = $(patsubst %.cpp, %.o, $(COMMON_SRC_NOTDIR))
COMMON_INC = common

all: boardchecker

$(COMMON_OBJS): $(COMMON_SRC)
	gcc -g -std=c++11 `pkg-config --cflags glib-2.0` -pthread -I$(COMMON_INC) -c $^

netcore.o: core/netcore.cpp
	gcc -g -std=c++11 `pkg-config --cflags libevent` -c $< -o $@

netlist.o: core/netlist.cpp
	gcc -g -std=c++11 -c $< -o $@

main.o: core/main.cpp
	gcc -g -std=c++11 -c $< -o $@

boardchecker: netlist.o netcore.o main.o $(COMMON_OBJS)
	g++ -g $^ -o $@ `pkg-config --libs libevent glib-2.0`

clean:
	rm -rf netlist.o netcore.o main.o boardchecker $(COMMON_OBJS)
