CFLAGS		:= -std=c++20 -Wall -Wextra -g -lfmt
CINCLUDES	:= -Iinclude
CC			:= g++ $(CFLAGS) $(CINCLUDES)

BUILD		:= bin/build
SOURCES 	:= $(shell find src -type f -name '*.cpp')
OBJECTS		:= $(patsubst src/%.cpp, $(BUILD)/%.o, $(SOURCES))
DEPENDS		:= $(patsubst %.o, %.d, $(OBJECTS))

.DEFAULT_GOAL := test

.PHONY: build
build: bin/main

bin/main: $(OBJECTS) | infoLink
	$(info $@)
	@$(CC) $(CFLAGS) $(CINCLUDES) $^ -o $@

$(OBJECTS): $@ | infoBuild

-include $(DEPENDS)

$(BUILD)/%.o: src/%.cpp
	$(info $<)
	@mkdir -p $(@D)
	@$(CC) -MMD -c $< -o $@

.PHONY: infoBuild
infoBuild:
	$(info ******BUILDING******)

.PHONY: infoLink
infoLink:
	$(info ******LINKING*******)

.PHONY: infoRun
infoRun:
	$(info ******RUNNING*******)

.PHONY: clean
clean:
	-$(RM) -r bin/*

.PHONY: test
test: bin/main | infoRun
	@./sea.bash -d -o=test/bin test/src

.PHONY: mem
mem: bin/main | infoRun
	@valgrind --leak-check=full --track-origins=yes -s ./bin/main -d test/bin test/src/file1.sea

.git:
	git init
	git add .
	git commit -m "Create C++ Project"
