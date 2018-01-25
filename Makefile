COMMON_SRC = $(wildcard common/base/*.cpp common/message/*.cpp)
COMMON_SRC_NOTDIR = $(notdir $(COMMON_SRC))
COMMON_OBJS = $(patsubst %.cpp, %.o, $(COMMON_SRC_NOTDIR))
COMMON_INC = common

CORE_SRC = $(wildcard core/*.cpp)
CORE_SRC_NOTDIR = $(notdir $(CORE_SRC))
CORE_OBJS = $(patsubst %.cpp, %.o, $(CORE_SRC_NOTDIR))
CORE_INC = common

all: boardchecker

$(COMMON_OBJS): $(COMMON_SRC)
	gcc -g -std=c++11 `pkg-config --cflags glib-2.0` -I$(COMMON_INC) -c $^

$(CORE_OBJS): $(CORE_SRC)
	gcc -g -std=c++11 `pkg-config --cflags libevent` -I$(CORE_INC) -c $^

boardchecker: $(CORE_OBJS) $(COMMON_OBJS)
	g++ -g $^ -o $@ `pkg-config --libs libevent glib-2.0` -pthread

clean:
	rm -rf boardchecker $(COMMON_OBJS) $(CORE_OBJS)
