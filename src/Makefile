CC = gcc
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS = -Wall -Wextra -lm `sdl2-config --cflags`
LDFLAGS = -lSDL2_image -lSDL2_gfx `sdl2-config --libs` -lm
EXEC = circles

$(EXEC): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $(EXEC)

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(EXEC)
