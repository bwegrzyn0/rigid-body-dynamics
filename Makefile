CC = g++
SRC = src/main.cpp src/shader.cpp src/glad.c 
INCLUDES = -I include/
OUTPUT = -o main
LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl 

main: $(SRC)
	$(CC) $(SRC) $(OUTPUT) $(INCLUDES) $(LIBS) 
	
