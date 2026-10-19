CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -Iinclude
LDFLAGS  =
LDLIBS   = -lm

LIB_SRC = src/params.c src/field.c src/solver.c src/io.c src/error.c
LIB_OBJ = $(LIB_SRC:src/%.c=build/%.o)

BIN      = build/advection
CONV_BIN = build/convergence

.PHONY: all run convergence plot study clean dirs

all: $(BIN) $(CONV_BIN)

dirs:
	@mkdir -p build output figures

$(BIN): build/main.o $(LIB_OBJ) | dirs
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(CONV_BIN): build/convergence.o build/params.o build/field.o build/solver.o build/error.o | dirs
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

build/%.o: src/%.c | dirs
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

convergence: $(CONV_BIN)
	./$(CONV_BIN)

plot: run convergence
	python3 scripts/plot_solution.py

study: plot

clean:
	rm -rf build output
	# Keep tracked figures under figures/ for the README.
