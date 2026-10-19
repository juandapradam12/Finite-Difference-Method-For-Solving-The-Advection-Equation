CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -Iinclude
LDFLAGS  =
LDLIBS   = -lm

SRC = src/main.c src/params.c src/field.c src/solver.c src/io.c
OBJ = $(SRC:src/%.c=build/%.o)
BIN = build/advection

.PHONY: all run plot clean dirs

all: $(BIN)

dirs:
	@mkdir -p build output figures

$(BIN): $(OBJ) | dirs
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

build/%.o: src/%.c | dirs
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

plot: run
	python3 scripts/plot_solution.py

clean:
	rm -rf build output figures
