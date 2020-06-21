CXX = g++
CFLAGS = -g -Wall -Wextra -std=c++17 -O0

ATOM = \
			 obj/atom/atom.o

BUILTINS = \
					 obj/builtins/builtins.o

ENV = \
			obj/env/env.o

FUNCTION = \
					 obj/function/eval.o

LIST = \
			 obj/list/eval_each.o \
			 obj/list/list.o

OBJECT = \
				 obj/object/object.o \
				 obj/object/eval.o \
				 obj/object/object.o \
				 obj/object/singletons.o

PARSER = \
				 obj/parser/module_search.o \
				 obj/parser/parse.o

TOKEN = \
				obj/token/tokenise.o

TRACEABLE = \
						obj/traceable/traceable.o

UTIL = \
			 obj/util/debug.o \
			 obj/util/exception_check.o \
			 obj/util/help.o

OBJ = $(ATOM) $(BUILTINS) $(ENV) $(FUNCTION) $(LIST) $(OBJECT) $(PARSER) $(TOKEN) $(TRACEABLE) $(UTIL)

HDRS = $(shell ls src/**/*.hpp)

LINK = -ledit -lncurses -lgc -lpcre -lpthread

.PHONY: default

default: crisp

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
