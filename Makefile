CC = g++
SRC = src/*.cpp src/*.c src/imgui/*.cpp
INCLUDES = -I include/
OUTPUT = -o main
LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl 

main: $(SRC)
	$(CC) $(SRC) $(OUTPUT) $(INCLUDES) $(LIBS) 
	
