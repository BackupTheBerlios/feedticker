#!make -f

SILENT     = @

SRC_DIR    = .
PARSER_DIR = $(SRC_DIR)/parser
GLADE_DIR  = $(SRC_DIR)/../glade
LAYOUT_DIR = $(SRC_DIR)/layout
UTIL_DIR   = $(SRC_DIR)/util
OBJ_DIR    = $(SRC_DIR)/obj
CC         = gcc

VPATH = $(SRC_DIR):$(GLADE_DIR):$(PARSER_DIR):$(UTIL_DIR):$(LAYOUT_DIR)

CPPFLAGS=-g3 -O0 -I$(SRC_DIR) -I/usr/include/cdk -fvisibility=hidden -pg
LFLAGS  =-g3 -O0 -lstdc++ -lcdk `pkg-config ncurses --libs` -fvisibility=hidden -pg

OBJS=$(OBJ_DIR)/main.o

SRCS=main.cc

all: makeit
.PHONY: all clean parallel makeit

ifeq (.depend,$(wildcard .depend))
  include .depend
endif

makeit:
	test -d $(OBJ_DIR) || mkdir $(OBJ_DIR)
	$(MAKE) .depend
	$(MAKE) linking

.depend: $(GLADES)
	$(CC) $(CPPFLAGS) -M $(SRCS) > .Tdepend
	sed -e "s/\(.*\)\.o/`echo "$(OBJ_DIR)" | sed "s/\//\\\\\\\\\\//g"`\/\1.o/g" <.Tdepend >.depend
	rm -f .Tdepend

linking: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o cfeedticker

clean:
	rm -f *.o
	rm -f *.ui
	rm -f .depend
	rm -f gmon.out
	rm -rf $(OBJ_DIR)
	rm -f cfeedticker

$(OBJ_DIR)/%.o: %.c
	@echo "[C  ] $< is not the way todo."

$(OBJ_DIR)/%.o: %.cc
	@echo "[C++] $<"
	$(SILENT)$(CACHE) $(CC) $(CPPFLAGS) -c $< -o $@
