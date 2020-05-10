# General Variables
CC=g++
OBJDIR=build
OBJS := $(addprefix $(OBJDIR)/,main.o lexer.o lexeme.o timer.o trie.o)
BINDIR=bin
BIN=hhfi
STD=c++11
CFLAGS=-Wall -Wextra -I src

#Test user input variables
ifeq ($(BUILDTYPE), debug)
CFLAGS += -O0 -g
else 
ifeq ($(BUILDTYPE), release)
CFLAGS += -O3 -DNDEBUG
endif
endif

ifeq ($(VERBOSE), true)
CFLAGS+=-v
endif

#Source file dependencies
main.o.dep=main.cpp main.h \
lexer.h lexemestream.h lexeme.h basiclexer.h \
trie.h timer.h testing.h

lexer.o.dep=lexer.cpp lexer.h lexeme.h lexemestream.h trie.h
lexeme.o.dep=lexeme.cpp lexeme.h
timer.o.dep=timer.cpp timer.h
trie.o.dep=trie.cpp trie.h

#Paths to search for dependencies
vpath %.cpp src src/lexer src/util
vpath %.h src src/lexer src/util

#For linking objects
$(BINDIR)/$(BIN): $(OBJDIR) $(BINDIR) $(OBJS)
	$(CC) $(OBJDIR)/*.o $(CFLAGS) -o $@

#Runs Binary after linking
run: $(BINDIR)/$(BIN)
	$(BINDIR)/$(BIN)

test: $(BINDIR)/$(BIN)
	./$(BINDIR)/$(BIN) testfile.txt

#Force a compilation and linkage with certain flags
force: clean $(OBJS) $(BINDIR)/$(BIN)

release:
	make force "BUILDTYPE=release"

debug:
	make force "BUILDTYPE=debug"

#Compiles the source files to objects
#Uses second expansion to determine dependencies from .o.dep variables
.SECONDEXPANSION:
$(OBJDIR)/%.o: $$($$*.o.dep)
	$(CC) -c $< -o $@ -std=$(STD) $(CFLAGS)

*.cpp:
*.h:

#Clean project from objects and binaries
clean-full: $(OBJDIR) $(BINDIR)
	rm -r $(OBJDIR) $(BINDIR)

clean: $(OBJDIR) $(BINDIR)
	rm -f $(OBJDIR)/*
	rm -f $(BINDIR)/*

#Makes folders if don't exist
$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)