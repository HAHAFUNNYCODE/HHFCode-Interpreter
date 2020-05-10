CC=g++
OBJDIR=build
OBJS := $(addprefix $(OBJDIR)/,main.o lexer.o lexeme.o timer.o trie.o)
BINDIR=bin
BIN=hhfi
STD=c++11
CFLAGS=-Wall -I src

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

main.o.dep=main.cpp main.h lexer.h trie.h basiclexer.h
lexer.o.dep=lexer.cpp lexer.h lexeme.h lexemestream.h
lexeme.o.dep=lexeme.cpp lexeme.h
timer.o.dep=timer.cpp timer.h
trie.o.dep=trie.cpp trie.h

vpath %.cpp src src/lexer src/util
vpath %.h src src/lexer src/util

$(BINDIR)/$(BIN): $(OBJDIR) $(BINDIR) $(OBJS)
	$(CC) $(OBJDIR)/*.o $(CFLAGS) -o $@

run: $(BINDIR)/$(BIN)
	./$(BINDIR)/$(BIN)

test: $(BINDIR)/$(BIN)
	./$(BINDIR)/$(BIN) testfile.txt

force: clean $(OBJS) $(BINDIR)/$(BIN)

release:
	make force "BUILDTYPE=release"

debug:
	make force "BUILDTYPE=debug"

.SECONDEXPANSION:
$(OBJDIR)/%.o: $$($$*.o.dep)
	$(CC) -c $< -o $@ -std=$(STD) $(CFLAGS)

*.cpp:
*.h:

clean-full: $(OBJDIR) $(BINDIR)
	rm -r $(OBJDIR) $(BINDIR)

clean: $(OBJDIR) $(BINDIR)
	rm -f $(OBJDIR)/*
	rm -f $(BINDIR)/*

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)