#OBJS specifies which files to compile as part of the project
OBJS = GameObject.cpp main.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I C:/SDL2-w64/include

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L C:/SDL2-w64/lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -g # -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

# Valgrind command
DRMEMORY_CMD = drmemory


#This is the target that compiles our executable
compile : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

run: compile
	.\main.exe

drmemory: compile
	$(DRMEMORY_CMD) $(OBJ_NAME).exe