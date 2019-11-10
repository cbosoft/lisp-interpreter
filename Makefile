CXX = gcc
CFLAGS = -Wall -Wextra
OBJ = obj/main.o obj/tokenise.o obj/parse.o obj/eval.c

obj/%.o: src/%.c
	mkdir -p obj
	$(CXX) $(CFLAGS) $< -c -o $@

int: $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o $@

clean:
	rm -rf src/*.o
