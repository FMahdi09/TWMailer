# makefile
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror -g -I $(INC_DIR)
LIBS = -lldap -llber -lcrypto

EXE = $(BIN_DIR)/twmailer-server
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(EXE)

rebuild: clean $(EXE)

$(EXE): $(OBJECTS) | $(BIN_DIR)
	$(CXX) -o $@ $^ ${LIBS}

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
