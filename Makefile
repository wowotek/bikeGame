
SOURCE_FOLDER := ./source

CFLAGS = -std=c11 -O0 -I./submodules/glm -I./submodules/stb -I./submodules/glad/include -I$(SOURCE_FOLDER)
LDFLAGS = -lGL -lGLU -lglfw -lm -ldl
CC = gcc

BUILD_FOLDER := ./build
BIN_FOLDER := $(BUILD_FOLDER)/bin
OBJ_FOLDER := $(BIN_FOLDER)/o
EXE := ${BIN_FOLDER}/Game

C_FILES := $(wildcard $(SOURCE_FOLDER)/*.c)
C_FILES := $(C_FILES) submodules/glad/src/glad.c

OBJ_FILES := $(C_FILES:$(SOURCE_FOLDER)/%.c=$(OBJ_FOLDER)/%.o)

$(BIN_FOLDER)/%:; @mkdir -p $@
$(BUILD_FOLDER)/%:; @mkdir -p $@

$(OBJ_FOLDER)/%.o: $(SOURCE_FOLDER)/%.c; $(CC) $(CFLAGS) -c $^ -o $@

clean:;
	@rm -r $(BUILD_FOLDER)

all: $(OBJ_FOLDER) $(EXE)
	@cp -r data/ build/

$(EXE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)