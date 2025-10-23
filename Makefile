SRC = src/*
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
