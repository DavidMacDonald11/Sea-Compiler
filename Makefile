.DEFAULT_GOAL := build

.PHONY: build
build: 
	./gradlew build
	@unzip -oq app/build/distributions/app.zip -d app/build/distributions

.PHONY: test
test: build
	-$(RM) test/bin/*
	./sea.bash -d -o=test/bin test/src

.PHONY: clean
clean:
	./gradlew clean
