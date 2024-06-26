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
UNAME_P := $(shell uname -p)

CPPS = $(wildcard *.cc)
HEADERS = $(wildcard *.h)
OBJS = $(CPPS:%.cc=%.o)
LIBSOSX = $(wildcard libOSX/*.a)
LIBSOSXARM = $(wildcard libOSX/arm64/*.a)
#there is an isue with linux lib order
LIBSUNIX = libUNIX/libclingo.a libUNIX/libgringo.a libUNIX/libreify.a libUNIX/libclasp.a libUNIX/libpotassco.a libUNIX/libBooleanFunction.a
#LIBSUNIX = $(wildcard libUNIX/*.a)
TARGET = modrev


default: all

.PHONY: all clean depend

all: cleanconfig config depend $(TARGET)


$(TARGET): $(OBJS)
ifeq ($(OS),Linux)
	$(CXX) $(LFLAGSLINUX) -o $@ $^ $(LIBSUNIX) -lpthread
else
ifeq ($(UNAME_P),arm)
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBSOSXARM)
else
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBSOSX)
endif
endif

%.o: %.cc
ifeq ($(OS),Linux)
	$(CXX) $(CXXFLAGSLINUX) $(IHEADERS) -c $<
else
	$(CXX) $(CXXFLAGSOSX) $(IHEADERS) -c $<
endif

clean:
	rm -f .makedeps *.o $(TARGET) config.txt

depend: $(CPPS) $(HEADERS)
ifeq ($(OS),Linux)
	$(CXX) $(CXXFLAGSLINUX) $(IHEADERS) -MM $(CPPS) > .makedeps
else
	$(CXX) $(CXXFLAGSOSX) $(IHEADERS) -MM $(CPPS) > .makedeps
endif

cleanconfig:
	rm -f config.txt

config: cleanconfig
	printf "check_ASP=true\n" >> config.txt
	printf "compareLevelFunction=true\n" >> config.txt
	printf "exactMiddleFunctionDetermination=true\n" >> config.txt
	printf "forceOptimum=false\n" >> config.txt
	printf "ASP_dir=$(CURDIR)/ASP/\n" >> config.txt
	printf "ASP_CC_BASE=$(CURDIR)/ASP/base.lp\n" >> config.txt
	printf "ASP_CC_SS=$(CURDIR)/ASP/StableState/core-ss.lp\n" >> config.txt
	printf "ASP_CC_D=$(CURDIR)/ASP/Dynamic/core-ts.lp\n" >> config.txt
	printf "ASP_CC_D_A=$(CURDIR)/ASP/Dynamic/a-update.lp\n" >> config.txt
	printf "ASP_CC_D_S=$(CURDIR)/ASP/Dynamic/s-update.lp\n" >> config.txt
	printf "ASP_CC_D_MA=$(CURDIR)/ASP/Dynamic/ma-update.lp\n" >> config.txt

dev: config depend $(TARGET)
	printf "debug=true\n" >> config.txt
