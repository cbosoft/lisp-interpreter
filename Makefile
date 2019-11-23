CXX = gcc
CFLAGS = -g -Wall -Wextra
OBJ = \
			obj/main.o \
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
			obj/list.o \
			obj/util.o \
			obj/singletons.o \
			obj/help.o
LINK = -ledit -lncurses -lgc

obj/%.o: src/%.c
	mkdir -p obj
	$(CXX) $(CFLAGS) $< -c -o $@

chrisp: $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o $@ $(LINK)

clean:
	rm -rf obj LISP
