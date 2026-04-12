SRC = main.c ui.c termon.c overworld.c saves.c player.c vcanvas.c battle.c utilib.c constants.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)
BIN = bin/game

.PHONY: all clean

all: $(BIN)

-include $(DEP)

$(BIN): $(OBJ)
	gcc $(OBJ) -lncurses -o $(BIN)

%.o: %.c
	gcc -Ih -MMD -MP -c $< -o $@

clean:
	rm -f $(OBJ) $(DEP) $(BIN)