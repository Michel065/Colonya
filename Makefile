# Compilateur et options
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Ilib -Isrc -MMD -MP
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Répertoires
SRC_DIR := src
BUILD_DIR := build

# Trouve tous les fichiers .cpp dans src/
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")

# Pour chaque .cpp, crée un chemin .o dans build/
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Pour chaque .o, on aura un fichier .d pour les dépendances
DEPS := $(OBJS:.o=.d)

# Cible par défaut
all: main

# Génération de l'exécutable final
main: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compilation d'un .cpp en .o et génération du .d
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BUILD_DIR) main

run: main
	./main

r: run

# Inclure les fichiers de dépendances si existants
-include $(DEPS)
