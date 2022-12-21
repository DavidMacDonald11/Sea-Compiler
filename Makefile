CC			:= g++
CFLAGS		:= -std=c++20 -Wall -Wextra -g

CINCLUDES	:= -Iinclude

SOURCES		:= $(wildcard $(patsubst %, %/*.cpp, src))
OBJECTS		:= $(patsubst src%, bin%, $(SOURCES:.cpp=.o))

all: clean bin/main

bin/main: $(OBJECTS)
	$(CC) $(CFLAGS) $(CINCLUDES) $^ -o $@
	-$(RM) $(OBJECTS)

$(OBJECTS): bin clean
	$(eval OFILE := $@)
	$(eval SFILE := $(patsubst bin/%.o, src/%.cpp, $(OFILE)))
	$(CC) -c -o $(OFILE) $(SFILE)

bin:
	mkdir bin

.PHONY: clean
clean:
	-$(RM) bin/main

run: all
	./bin/main

mem: all
	valgrind ./bin/main
