omega : main.c player.c player.h
	$(CC) main.c player.c $(CFLAGS) -o omega -lSDL -lSDL_mixer -lm
