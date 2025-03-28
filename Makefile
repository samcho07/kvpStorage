CC ?= gcc
CXX ?= g++
LD ?= g++

CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
LDFLAGS = -pthread

SRCS = src/main.cpp src/storage.cpp src/command_processor.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

KVPStorage: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) KVPStorage

-include $(DEPS)
