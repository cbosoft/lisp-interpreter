CXX = g++
CFLAGS = -g -Wall -Wextra -std=c++17 -O0
C_OBJ = \
			obj/atom.o \
			obj/tokenise.o \
			obj/parse.o \
			obj/eval.o \
			obj/object.o \
			obj/environment.o \
			obj/exception.o \
			obj/builtins.o \
			obj/debug.o \
			obj/function.o \
			obj/import.o \
			obj/math.o \
			obj/list.o \
			obj/util.o \
			obj/singletons.o \
			obj/help.o
OBJ = \
			obj/tokenise.o \
			obj/parse.o \
			obj/debug.o \
			obj/object.o \
			obj/environment.o \
			obj/builtins.o \
			obj/list.o \
			obj/eval.o \
			obj/builtins/quote.o \
			obj/builtins/defun.o \
			obj/builtins/defvar.o \
			obj/builtins/defmacro.o \
			obj/builtins/add.o \
			obj/builtins/subtract.o \
			obj/builtins/multiply.o \
			obj/builtins/divide.o \
			obj/builtins/and.o \
			obj/builtins/or.o \
			obj/builtins/not.o \
			obj/builtins/gt.o \
			obj/builtins/ge.o \
			obj/builtins/lt.o \
			obj/builtins/le.o \
			obj/builtins/eq.o \
			obj/builtins/if.o \
			obj/builtins/cond.o \
			obj/singletons.o \
			obj/atom.o \
			obj/help.o \
			obj/builtins/exit.o
LINK = -ledit -lncurses -lgc -lpcre

obj/%.o: src/%.cpp src/types.hpp
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $< -c -o $@

crisp: obj/main.o $(OBJ) src/types.hpp
	$(CXX) $(CFLAGS) obj/main.o $(OBJ) -o $@ $(LINK)

#tests: obj/tests.o $(OBJ)
#	$(CXX) $(CFLAGS) obj/tests.o $(OBJ) -o $@ $(LINK) && ./tests

#install:
#	cp crisp /usr/bin/.

clean:
	rm -rf obj crisp
