# Tools -------

CPP = cpp
CXX = g++
AR  = ar
LD  = g++
MD  = mkdir
RM  = rm
RD  = rmdir

CPPFLAGS = 
CXXFLAGS = -std=c++0x -pedantic -Wall -Wextra -Wshadow -fno-rtti
ARFLAGS  = rcs
LDFLAGS  =
MDFLAGS  = 
RMFLAGS  = -f
RDFLAGS  = 

# Files -------

INCPATHS = \
. \
../../include/

SRCS = eg.inverter.cpp
OBJS = $(SRCS:.cpp=.o)

LIB = ../../lib/libflambeau.a

EXE = eg.inverter.out

# Targets -----

.PHONY: all test

all: $(EXE)

$(EXE): $(OBJS) $(LIB)
	@echo [ LD ] $@
	@$(LD) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	@echo [ CC ] $<
	@$(CXX) $(CXXFLAGS) -o $@ -c $< $(addprefix -I, $(INCPATHS))

clean:
	@for i in $(OBJS); do \
		if [ -f $$i ]; then \
			echo [ RM ] $$i; \
			$(RM) $(RMFLAGS) $$i; \
		fi \
	done

distclean: clean
	@for i in $(EXE); do \
		if [ -f $$i ]; then \
			echo [ RM ] $$i; \
			$(RM) $(RMFLAGS) $$i; \
		fi \
	done

test:
	@echo SRCS = $(SRCS)
	@echo OBJS = $(OBJS)
	@echo LIB = $(LIB)
	@echo EXE = $(EXE)
