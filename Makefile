CXX = g++
CFLAGS = -g -Wall -Wextra -std=c++17 -O0
OBJ = \
			obj/singletons.o \
			obj/builtins/quote.o \
			obj/builtins/defun.o \
			obj/builtins/defmacro.o \
			obj/builtins/defvar.o \
			obj/builtins/add.o \
			obj/builtins/subtract.o \
			obj/builtins/multiply.o \
			obj/builtins/divide.o \
			obj/builtins/modulo.o \
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
			obj/builtins/print.o \
			obj/builtins/rest.o \
			obj/builtins/pop.o \
			obj/builtins/eval.o \
			obj/builtins/eval_file.o \
			obj/builtins/import.o \
			obj/builtins/with_open.o \
			obj/builtins/write.o \
			obj/builtins/read.o \
			obj/builtins/list.o \
			obj/builtins/append.o \
			obj/builtins/random.o \
			obj/builtins/types.o \
			obj/builtins/exit.o \
			obj/pointer.o \
			obj/tokenise.o \
			obj/parse.o \
			obj/debug.o \
			obj/object.o \
			obj/environment.o \
			obj/exception_check.o \
			obj/builtins.o \
			obj/list.o \
			obj/eval.o \
			obj/search.o \
			obj/atom.o \
			obj/help.o

HDRS = \
  		src/types.hpp \
			src/exception.hpp

LINK = -ledit -lncurses -lgc -lpcre

obj/%.o: src/%.cpp $(HDRS)
	mkdir -p `dirname $@`
	$(CXX) $(CFLAGS) $< -c -o $@

crisp: obj/main.o $(OBJ) $(HDRS)
	$(CXX) $(CFLAGS) obj/main.o $(OBJ) -o $@ $(LINK)

#tests: obj/tests.o $(OBJ)
#	$(CXX) $(CFLAGS) obj/tests.o $(OBJ) -o $@ $(LINK) && ./tests

install:
	cp crisp /usr/bin/.

uninstall:
	rm -rf /usr/bin/crisp

clean:
	rm -rf obj crisp
