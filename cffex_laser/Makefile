###############################################################################
GTEST = $(HOME)/local/gtest
CFFEX_FRAME = $(HOME)/01git/01cffex_frame

SRC_DIR  = src/main src/frame src/transfer ../common
DEST     = cffex_laser
OBJ_DIR  = build/
INC_DIR  = -I../json-c/include -I$(CFFEX_FRAME)/src
LINK_LIB = -L../json-c/lib -L$(CFFEX_FRAME) -lpthread -ldl -lcffexd -ljson-c
CC       = g++ -g -O0 -std=c++11 -DTIXML_USE_STL -DLINUX -DGCC -Wno-strict-aliasing
###############################################################################

$(shell sh ./ver.sh>/dev/null 2>&1&)
$(foreach dir, $(SRC_DIR), $(eval INC_DIR += -I$(dir)))

vpath %.cpp $(SRC_DIR)
vpath %.cc  $(SRC_DIR)
vpath %.c   $(SRC_DIR)

SRC = $(shell find $(SRC_DIR) -maxdepth 1 -name "*.cpp")
OBJ = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(notdir $(SRC)))
DEP = $(patsubst %.o, %.d, $(OBJ))

SRC2 = $(shell find $(SRC_DIR) -maxdepth 1 -name "*.cc")
OBJ2 = $(patsubst %.cc, $(OBJ_DIR)%.o, $(notdir $(SRC2)))
DEP += $(patsubst %.o, %.d, $(OBJ2))

SRC3 = $(shell find $(SRC_DIR) -maxdepth 1 -name "*.c")
OBJ3 = $(patsubst %.c, $(OBJ_DIR)%.o, $(notdir $(SRC3)))
DEP += $(patsubst %.o, %.d, $(OBJ3))

$(shell mkdir -p $(dir $(DEST)))
$(shell mkdir -p $(OBJ_DIR))

all: $(DEST)
-include $(DEP)

$(DEST): $(OBJ) $(OBJ2) $(OBJ3)
	@echo -e "\033[32m$(CC) -o $@ $^ $(LINK_LIB)\033[0m"
	@$(CC) -o $@ $^ $(LINK_LIB)

$(OBJ): $(OBJ_DIR)%.o: %.cpp
	@echo -e "\033[32m$(CC) -o $@ -c $< $(INC_DIR)\033[0m"
	@$(CC) -o $@ -c $< $(INC_DIR)
$(OBJ_DIR)%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $< $(INC_DIR) > $@.$$$$; \
	sed 's,.*\.o[ :]*,$(patsubst %.d,%.o,$@) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJ2): $(OBJ_DIR)%.o: %.cc
	@echo -e "\033[32m$(CC) -o $@ -c $< $(INC_DIR)\033[0m"
	@$(CC) -o $@ -c $< $(INC_DIR)
$(OBJ_DIR)%.d: %.cc
	@set -e; rm -f $@; \
	$(CC) $(CC_FLAG) -MM $< $(INC_DIR) > $@.$$$$; \
	sed 's,.*\.o[ :]*,$(patsubst %.d,%.o,$@) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJ3): $(OBJ_DIR)%.o: %.c
	@echo -e "\033[32m$(CC) -o $@ -c $< $(INC_DIR)\033[0m"
	@$(CC) -o $@ -c $< $(INC_DIR)
$(OBJ_DIR)%.d: %.c
	@set -e; rm -f $@; \
	$(CC) $(CC_FLAG) -MM $< $(INC_DIR) > $@.$$$$; \
	sed 's,.*\.o[ :]*,$(patsubst %.d,%.o,$@) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	rm -rf $(DEST)
	rm -rf $(OBJ_DIR)
	rm -rf main/version.h
