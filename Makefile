#!make -f

SILENT     = @

SRC_DIR    = .
PARSER_DIR = $(SRC_DIR)/parser
GLADE_DIR  = $(SRC_DIR)/glade
UTIL_DIR   = $(SRC_DIR)/util
OBJ_DIR    = $(SRC_DIR)/obj

PKG_CFG    = pkg-config gtk+-2.0 gthread-2.0 gconf-2.0 libcurl libxml-2.0

CC         = gcc

VPATH = $(SRC_DIR):$(GLADE_DIR):$(PARSER_DIR):$(UTIL_DIR):$(LAYOUT_DIR)

CPPFLAGS=-g3 -O0 -I$(SRC_DIR) `$(PKG_CFG) --cflags` -fvisibility=hidden -pg
LFLAGS  =-g3 -O0 -lstdc++ `$(PKG_CFG) --libs` -fvisibility=hidden -pg

OBJS=$(OBJ_DIR)/main.o		\
     $(OBJ_DIR)/popupmenu.o	\
     $(OBJ_DIR)/xmlstring.o	\
     $(OBJ_DIR)/parser.o	\
     $(OBJ_DIR)/rdf.o		\
     $(OBJ_DIR)/rss.o		\
     $(OBJ_DIR)/layout.o	\
     $(OBJ_DIR)/feedentry.o	\
     $(OBJ_DIR)/controller.o	\
     $(OBJ_DIR)/config.o	\
     $(OBJ_DIR)/settings.o	\
     $(OBJ_DIR)/browser.o	\
     $(OBJ_DIR)/message.o	\
     $(OBJ_DIR)/callback.o

SRCS=main.cc			\
     popupmenu.cc		\
     layout.cc			\
     feedentry.cc		\
     controller.cc		\
     config.cc			\
     settings.cc		\
     message.cc			\
     callback.cc		\
     $(UTIL_DIR)/xmlstring.cc	\
     $(UTIL_DIR)/browser.cc	\
     $(PARSER_DIR)/parser.cc	\
     $(PARSER_DIR)/rdf.cc	\
     $(PARSER_DIR)/rss.cc

GLADES=$(SRC_DIR)/popupmenu.ui		\
       $(SRC_DIR)/feedsetup.ui		\
       $(SRC_DIR)/globalconfig.ui

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
	$(CC) $(LFLAGS) $(OBJS) -o feedticker

clean:
	rm -f *.o
	rm -f *.ui
	rm -f .depend
	rm -rf $(OBJ_DIR)
	rm -f feedticker

################################################################################
# compiling rules
################################################################################
$(SRC_DIR)/%.ui: %.glade
	gtk-builder-convert $< - | ./bin/convert_glade -o $@

$(OBJ_DIR)/%.o: %.c
	@echo "[C  ] $< is not the way todo."

$(OBJ_DIR)/%.o: %.cc
	@echo "[C++] $<"
	$(SILENT)$(CACHE) $(CC) $(CPPFLAGS) -c $< -o $@
