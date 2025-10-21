SRC = src/main.c src/player.c src/window.c src/camera.c src/tilemap.c src/csvparser.c src/world.c src/game.c src/config.c src/debug.c
TARGET = target
LIB = -lraylib -lm -lpthread -ldl -lrt -lX11
all: $(TARGET)

$(TARGET):
	gcc $(SRC) -o $(TARGET) $(LIB)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	rm -f $(TARGET)
	gcc $(SRC) -o $(TARGET) $(LIB)
	./$(TARGET)
