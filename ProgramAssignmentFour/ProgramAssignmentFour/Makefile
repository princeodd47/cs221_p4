OUTPUT = test_Hashing

.PHONY: all
all: test

.PHONY: compile
compile: $(OUTPUT)

$(OUTPUT): *.cpp
	@g++ -g -o $(OUTPUT) *.cpp

.PHONY: test
test: compile
	@./$(OUTPUT)

.PHONY: clean
clean:
	@rm $(OUTPUT)
