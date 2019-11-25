CXX = gcc
CFLAGS = -g -Wall -Wextra
OBJ = \
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
LINK = -ledit -lncurses -lgc -lpcre

obj/%.o: src/%.c
	mkdir -p obj
	$(CXX) $(CFLAGS) $< -c -o $@

chrisp: obj/main.o $(OBJ)
	$(CXX) $(CFLAGS) obj/main.o $(OBJ) -o $@ $(LINK)


tests: obj/tests.o $(OBJ)
	$(CXX) $(CFLAGS) obj/tests.o $(OBJ) -o $@ $(LINK) && ./tests

clean:
	rm -rf obj LISP
