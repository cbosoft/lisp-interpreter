CXX = gcc
CFLAGS = -Wall -Wextra
OBJ = obj/main.o obj/tokenise.o obj/parse.o obj/eval.o obj/object.o obj/environment.o obj/exception.o obj/builtins.o

obj/%.o: src/%.c
	mkdir -p obj
	$(CXX) $(CFLAGS) $< -c -o $@

LISP: $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o $@

clean:
	rm -rf obj LISP
