CXX = g++
CXXFLAGSLINUX = $(WARNINGS) -std=c++11 -ggdb3 -static
CXXFLAGSOSX = $(WARNINGS) -std=c++11 -ggdb3
WARNINGS = -Wall -Wextra -pedantic
LFLAGS = -std=c++11
LFLAGSLINUX = -std=c++11 -static

# Operating System
OS := $(shell uname)

CPPS = $(wildcard *.cc)
HEADERS = $(wildcard *.h)
OBJS = $(CPPS:%.cc=%.o)
TARGET = modrev


default: all

.PHONY: all clean depend

all: depend $(TARGET)

$(TARGET): $(OBJS)
ifeq ($(OS),Linux)
	$(CXX) $(LFLAGSLINUX) -o $@ $^
else
	$(CXX) $(LFLAGS) -o $@ $^
endif

%.o: %.cc
ifeq ($(OS),Linux)
	$(CXX) $(CXXFLAGSLINUX) -c $<
else
	$(CXX) $(CXXFLAGSOSX) -c $<
endif

clean:
	rm -f .makedeps *.o $(TARGET)

depend: $(CPPS) $(HEADERS)
ifeq ($(OS),Linux)
	$(CXX) $(CXXFLAGSLINUX) -MM $(CPPS) > .makedeps
else
	$(CXX) $(CXXFLAGSOSX) -MM $(CPPS) > .makedeps
endif
