CC=g++
OBJDIR=build
OBJS := $(addprefix $(OBJDIR)/,main.o lexer.o lexemes.o timer.o trie.o)
BINDIR=bin
BIN=hhfi
STD=c++11
CFLAGS=-Wall

ifeq ($(BUILDTYPE), debug)
CFLAGS += -O0 -g
else 
ifeq ($(BUILDTYPE), release)
CFLAGS += -O3 -DNDEBUG
endif
endif

main.o.dep=main.cpp main.h lexer.h
lexer.o.dep=lexer.cpp lexer.h lexemes.h lexemestream.h
lexemes.o.dep=lexemes.cpp lexemes.h
timer.o.dep=timer.cpp timer.h
trie.o.dep=trie.cpp trie.h

vpath %.cpp src src/lexer src/misc
vpath %.h src src/lexer src/misc

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