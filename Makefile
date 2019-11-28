CXX = g++
CFLAGS = -g -Wall -Wextra -std=c++17
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
			obj/atom.o
LINK = -ledit -lncurses -lgc -lpcre

obj/%.o: src/%.cpp
	mkdir -p obj
	$(CXX) $(CFLAGS) $< -c -o $@

crisp: obj/main.o $(OBJ)
	$(CXX) $(CFLAGS) obj/main.o $(OBJ) -o $@ $(LINK)

#tests: obj/tests.o $(OBJ)
#	$(CXX) $(CFLAGS) obj/tests.o $(OBJ) -o $@ $(LINK) && ./tests

#install:
#	cp crisp /usr/bin/.

clean:
	rm -rf obj crisp
