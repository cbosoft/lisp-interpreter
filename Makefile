CXX = g++
CFLAGS = -g -Wall -Wextra -std=c++17 -O0
OBJ = \
			obj/singletons.o \
			obj/pointer.o \
			obj/tokenise.o \
			obj/parse.o \
			obj/debug.o \
			obj/object.o \
			obj/environment.o \
			obj/exception_check.o \
			obj/builtins.o \
			obj/traceable.o \
			obj/list.o \
			obj/eval.o \
			obj/search.o \
			obj/atom.o \
			obj/help.o

HDRS = \
  		src/types.hpp \
			src/exception.hpp

LINK = -ledit -lncurses -lgc -lpcre

.PHONY: default

default: crisp

obj/builtins.o: src/builtins.cpp $(shell ls src/builtins/*.hpp)
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $< -c -o $@

obj/%.o: src/%.cpp $(HDRS)
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $< -c -o $@

crisp: obj/main.o $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $@ $(LINK)

#tests: obj/tests.o $(OBJ)
#	$(CXX) $(CFLAGS) obj/tests.o $(OBJ) -o $@ $(LINK) && ./tests

install:
	cp crisp /usr/bin/.
	mkdir -p /usr/lib/crisp
	cp lib/* /usr/lib/crisp/.

uninstall:
	rm -rf /usr/bin/crisp

clean:
	rm -rf obj crisp
