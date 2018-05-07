CXX			:= g++
CXXFLAGS	:= -Wall -g
SRC_SUFFIX	:= cpp
SRCS		:= $(patsubst src/%.$(SRC_SUFFIX), %.$(SRC_SUFFIX), $(wildcard src/*.$(SRC_SUFFIX)))
LIBS		:= -lpthread
OBJ_DIR		:= obj/
BIN_DIR		:= bin/
VPATH		:= src:${OBJ_DIR}

P4: $(SRCS:.$(SRC_SUFFIX)=.o)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)$@ $(patsubst %.$(SRC_SUFFIX), $(OBJ_DIR)%.o, $(SRCS)) $(LIBS)
.PHONY: P4

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(OBJ_DIR)
.PHONY: clean

%.o: %.$(SRC_SUFFIX)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $(OBJ_DIR)$@ $< $(LIBS)

