BIN := kbbi-gtk

SRCDIR := src
OBJDIR := obj
BINDIR := bin

PKG_CFG_GTK4 := $(shell pkg-config --cflags --libs gtk4)

CFLAGS ?= -Wall -Wextra -g

BIN_SRCS := $(shell find $(SRCDIR) -name *.c)
BIN_OBJS := $(subst $(SRCDIR)/,$(SRCDIR)_,$(BIN_SRCS:%=$(OBJDIR)/%.o))

.PHONY: all

all: $(BINDIR)/$(BIN)

# Link all the objects to create main binary
$(BINDIR)/$(BIN): $(BIN_OBJS)
	@echo LINK $(BIN)
	@$(CC) $(BIN_OBJS) -o $@ $(PKG_CFG_GTK4) $(CFLAGS)

# Compile each main source code
$(OBJDIR)/$(SRCDIR)_%.c.o: $(SRCDIR)/%.c
	@echo CC $<
	@$(CC) -c $< -o $@ $(PKG_CFG_GTK4) $(CFLAGS)
