BIN := kbbi-gtk

SRCDIR := src
OBJDIR := obj
BINDIR := bin
LIBDIR := lib

LIB_KBBI := libkbbi

PKG_CFG_GTK4 := $(shell pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0)

CFLAGS ?= -Wall -Wextra -g

BIN_SRCS := $(shell find $(SRCDIR) -name *.c)
BIN_OBJS := $(subst $(SRCDIR)/,$(SRCDIR)_,$(BIN_SRCS:%=$(OBJDIR)/%.o))

.PHONY: all

all: $(BINDIR)/$(BIN) $(BINDIR)/$(LIB_KBBI).so

# Link all the objects to create main binary
$(BINDIR)/$(BIN): $(BIN_OBJS)
	@echo LINK $(BIN)
	@$(CC) $(BIN_OBJS) -o $@ $(PKG_CFG_GTK4) $(CFLAGS)

# Compile each main source code
$(OBJDIR)/$(SRCDIR)_%.c.o: $(SRCDIR)/%.c
	@echo CC $<
	@$(CC) -c $< -o $@ $(PKG_CFG_GTK4) $(CFLAGS)

# Move libkbbi.so
$(BINDIR)/$(LIB_KBBI).so: $(LIBDIR)/$(LIB_KBBI)/bin/$(LIB_KBBI).so
	@mv $(LIBDIR)/$(LIB_KBBI)/bin/$(LIB_KBBI).so $(BINDIR)/

# Compile libkbbi.so
$(LIBDIR)/$(LIB_KBBI)/bin/$(LIB_KBBI).so:
	@cd $(LIBDIR)/$(LIB_KBBI); \
	$(MAKE) all
