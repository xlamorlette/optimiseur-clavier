# Optimiseur clavier

# R�pertoires
OBJ_DIR	:= obj
SRC_DIR := src

# Binaire
BIN		:= optim_clavier

# Alias
ECHO	:= echo -e
B		:= "\033[1m"
b		:= "\033[0m"
OS		:= $(shell uname -s)

# Include flags
INC_FLAGS	:=

# Flags pr�-processeur et compilateur
CPP_FLAGS	:=
CXX_OPTIM	= -O2
CXX_DEBUG	= 
CXX_FLAGS	= -std=c++11 $(CXX_OPTIM) $(CXX_DEBUG) -Wall $(INC_FLAGS)

# Biblioth�ques
LIBS	:= -lboost_program_options

# Sources
SRCS	:= $(wildcard $(SRC_DIR)/*.cpp)
INCS	:= $(wildcard $(SRC_DIR)/*.hpp)

# Objets
OBJS	:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


# Targets
all: optim_clavier

debug: CXX_DEBUG = -g
debug: CXX_OPTIM =
debug: optim_clavier

clean:
	rm -rf core $(OBJ_DIR) $(BIN)


# R�gles d'inf�rence
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(ECHO) $(B)"Compile: $< "$(b)
	g++ $(CXX_FLAGS) $(CPP_FLAGS) -c -o $@ $<

# D�pendances
$(BIN): $(OBJS)
	@$(ECHO) $(B)"Link: $@ "$(b)
	g++ -o $@ $(OBJS) $(LIBS)

# Note : les d�pendances par rapport aux fichiers headers ne sont pas g�r�es
