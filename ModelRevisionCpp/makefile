mkfile_path := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
CXX = g++
CXXFLAGSLINUX = $(WARNINGS) -std=c++11 -ggdb3 -static
CXXFLAGSOSX = $(WARNINGS) -std=c++11 -ggdb3
WARNINGS = -Wall -Wextra -pedantic
LFLAGS = -std=c++11 -Wl,-search_paths_first -Wl,-headerpad_max_install_names
#LFLAGSLINUX = -std=c++11 -static -Wl,-search_paths_first -Wl,-headerpad_max_install_names
LFLAGSLINUX = -std=c++11 -static
#IHEADERS = -I$(mkfile_path)headers/libclingo -I$(mkfile_path)headers/libBooleanFunction
IHEADERS = -Iheaders/libclingo -Iheaders/libBooleanFunction

# Operating System
OS := $(shell uname)

CPPS = $(wildcard *.cc)
HEADERS = $(wildcard *.h)
OBJS = $(CPPS:%.cc=%.o)
LIBSOSX = $(wildcard libOSX/*.a)
#there is an isue with linux lib order
LIBSUNIX = libUNIX/libclingo.a libUNIX/libgringo.a libUNIX/libreify.a libUNIX/libclasp.a libUNIX/libpotassco.a libUNIX/libBooleanFunction.a
#LIBSUNIX = $(wildcard libUNIX/*.a)
TARGET = modrev


default: all

.PHONY: all clean depend

all: depend $(TARGET)

$(TARGET): $(OBJS)
ifeq ($(OS),Linux)
	$(CXX) $(LFLAGSLINUX) -o $@ $^ $(LIBSUNIX) -lpthread
else
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBSOSX)
endif

%.o: %.cc
ifeq ($(OS),Linux)
	$(CXX) $(CXXFLAGSLINUX) $(IHEADERS) -c $<
else
	$(CXX) $(CXXFLAGSOSX) $(IHEADERS) -c $<
endif

clean:
	rm -f .makedeps *.o $(TARGET)

depend: $(CPPS) $(HEADERS)
ifeq ($(OS),Linux)
	$(CXX) $(CXXFLAGSLINUX) $(IHEADERS) -MM $(CPPS) > .makedeps
else
	$(CXX) $(CXXFLAGSOSX) $(IHEADERS) -MM $(CPPS) > .makedeps
endif
