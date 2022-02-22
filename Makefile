CC      := TOOLS/djgpp/bin/i586-pc-msdosdjgpp-gcc
SRC     := ./SRC
OBJ     := ./OBJS
SRCS    := $(shell find $(SRC)/ -type f -name '*.c')
OBJS    := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
# -O3 for release!!!
CFLAGS  := -s -O3
EXE     := dosword.exe
ALLEG	:= TOOLS/allegro/lib/LIBALLEG.A
IALLEG  := TOOLS/allegro/include
MKDIR   := mkdir

# Set target defines
ifeq ($(TARGET), DOS)
CFLAGS += -DDOS_BUILD
else ifeq ($(TARGET), DOS-DEBUG)
CFLAGS += -g -DDOS_BUILD
else ifeq ($(TARGET), DOS-NOFPU)
CFLAGS += -DDOS_BUILD -lemu
# by default, bulid for DOS
else
CFLAGS += -DDOS_BUILD
endif

# Default
game: $(EXE)

# Link exe
$(EXE): $(OBJS)
	$(CC) $^ -o $@ -Wl,--allow-multiple-definition $(ALLEG)

# Compile all sources to objects
$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) -I$(IALLEG) $(CFLAGS) -c $< -o $@

# Make OBJS directory if nonexistent
$(OBJ):
	$(MKDIR) $@
	$(MKDIR) $@/state

clean:
	find $(OBJ) -type f -name '*.o' -delete
	rm -f $(EXE)
