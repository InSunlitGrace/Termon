.PHONY: gac lines

game: main.c ui.c ./h/ui.h ./h/termon.h termon.c ./h/overworld.h overworld.c ./h/saves.h saves.c ./h/player.h player.c ./h/vcanvas.h vcanvas.c ./h/battle.h battle.c ./h/utilib.h utilib.c ./h/constants.h constants.c
	gcc main.c ui.c termon.c overworld.c saves.c player.c vcanvas.c battle.c utilib.c constants.c -lncurses -o ./bin/game
gac:
	./genAllCode.sh
lines:
	./lines