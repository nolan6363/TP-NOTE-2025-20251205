CC = gcc
HEADER_DIR = include
CFLAGS = -Werror -O3 -I$(HEADER_DIR) -g
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

LIBS = 

# Récupérer tous les fichiers .c dans src/ sauf main.c
LIB_SRCS = $(filter-out $(SRC_DIR)/displaygamegrid.c, $(wildcard $(SRC_DIR)/*.c))
# Générer les noms des fichiers objets correspondants
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB_SRCS))

all: $(BIN_DIR)/displaygamegrid 

# Créer les répertoires nécessaires
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Compiler chaque fichier source de librairie en fichier objet
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Linker le main avec tous les fichiers objets des librairies
$(BIN_DIR)/displaygamegrid: $(SRC_DIR)/displaygamegrid.c $(LIB_OBJS) | $(BIN_DIR)
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

run: $(BIN_DIR)/displaygamegrid
	@./$(BIN_DIR)/displaygamegrid

clean:
	@rm -rf "$(BIN_DIR)/displaygamegrid" "$(OBJ_DIR)"

.PHONY: all run clean
