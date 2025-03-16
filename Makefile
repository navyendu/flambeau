# Author: Navyendu V. Surendranadhan

# Tools -------

CPP = cpp
CXX = g++
AR  = ar
LD  = g++
MD  = mkdir
RM  = rm
RD  = rmdir

CPPFLAGS = 
CXXFLAGS = -g -O0 -pedantic -std=c++0x -fstrict-enums -Wabi -Wall -Wextra -Wshadow
ARFLAGS  = rcs
LDFLAGS  = -g
MDFLAGS  = 
RMFLAGS  = --interactive=never
RDFLAGS  = 

# Files -------

INCPATHS = include

SRCS = \
src/flambeau/logic.cpp \
src/flambeau/module.cpp \
src/flambeau/signal.cpp \
src/flambeau/listener.cpp \
src/flambeau/ctrl.cpp \
src/flambeau/dump.cpp \
src/flambeau/logger.cpp \
src/flambeau/expression.cpp
# \
#src/test.cpp
#src/flambeau/library/func/cells/not.cpp \
#src/flambeau/library/func/cells/and.cpp \
#src/flambeau/library/func/cells/xor.cpp \
#src/flambeau/library/func/cells/nand.cpp \
#src/flambeau/library/func/cells/nor.cpp \
#src/flambeau/library/func/cells/or.cpp \
#src/flambeau/library/cmos/cells/not.cpp \
#src/flambeau/library/func/cells/xnor.cpp \
#src/flambeau/library/func/cells/dff.cpp \

OBJS = $(patsubst src/%, obj/%, $(patsubst %.cpp, %.o, $(SRCS)))
OBJDIRS = $(sort $(dir $(OBJS))) obj/

LIB = lib/libflambeau.a
LIBDIR = $(sort $(dir $(LIB)))

EXE = bin/test.out
EXEDIR = $(sort $(dir $(EXE)))

# Targets -----

.PHONY: all run wc clean test examples

all: $(LIB) examples

$(LIB): $(OBJS) | $(LIBDIR)
	@echo [ AR ] $@
	@$(AR) $(ARFLAGS) $@ $^

$(OBJS) obj/test.o: | $(OBJDIRS)

obj/%.o: src/%.cpp
	@echo [ CC ] $<
	@$(CXX) $(CXXFLAGS) -o $@ -c $< -I$(INCPATHS) 

%/:
	@echo [ MKDIR ] $@
	@mkdir -p $@

wc:
	@wc `tree -if --noreport | grep -e'\.h' -e'\.cpp'`

#clean:
#	rm $(OBJS)

examples:
	@for i in `ls -p examples | grep -e'/'`; do\
		$(MAKE) -C examples/$$i; \
	done

clean:
	@for i in obj/test.o $(OBJS); do \
		if [ -f $$i ]; then \
			echo [ RM ] $$i; \
			$(RM) $(RMFLAGS) $$i; \
		fi \
	done
	@for i in `ls -p examples | grep -e'/'`; do\
		$(MAKE) -C examples/$$i clean; \
	done

distclean: clean
	@for i in $(LIB); do \
		if [ -f $$i ]; then \
			echo [ RM ] $$i; \
			$(RM) $(RMFLAGS) $$i; \
		fi \
	done
	@for i in $(OBJDIRS); do \
		if [ -d $$i ]; then \
			echo [ RMDIR ] $$i; \
			$(RD) $(RDFLAGS) $$i; \
		fi \
	done
	@for i in $(LIBDIR); do \
		if [ -d $$i ]; then \
			echo [ RMDIR ] $$i; \
			$(RD) $(RDFLAGS) $$i; \
		fi \
	done
	@for i in `ls -p examples | grep -e'/'`; do\
		$(MAKE) -C examples/$$i distclean; \
	done

test:
	@echo $(INCPATHS)
	@echo $(SRCS)
	@echo $(LIB)
	@echo $(LIBDIR) 
	@echo $(OBJS)
	@echo $(OBJDIRS)
