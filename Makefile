COMMON_SRC = $(wildcard common/base/*.cpp common/message/*.cpp)
COMMON_SRC_NOTDIR = $(notdir $(COMMON_SRC))
COMMON_OBJS = $(patsubst %.cpp, %.o, $(COMMON_SRC_NOTDIR))
COMMON_INC = common

CORE_SRC = $(wildcard core/*.cpp)
CORE_SRC_NOTDIR = $(notdir $(CORE_SRC))
CORE_OBJS = $(patsubst %.cpp, %.o, $(CORE_SRC_NOTDIR))
CORE_INC = common

.PHONY: clean-objs clean

all: boardchecker clean-objs

$(COMMON_OBJS): $(COMMON_SRC)
	gcc -Wall -g -std=c++11 `pkg-config --cflags libevent glib-2.0` -I$(COMMON_INC) -c $^

$(CORE_OBJS): $(CORE_SRC)
	gcc -Wall -g -std=c++11 `pkg-config --cflags libevent` -I$(CORE_INC) -c $^

boardchecker: $(CORE_OBJS) $(COMMON_OBJS)
	gcc -g $^ -o $@ `pkg-config --libs libevent glib-2.0` -pthread

clean-objs:
	rm -rf $(COMMON_OBJS) $(CORE_OBJS)

clean: clean-objs
	rm -rf boardchecker
