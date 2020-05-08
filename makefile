CC=g++
OBJDIR=build
OBJS := $(addprefix $(OBJDIR)/,main.o lexer.o lexemes.o timer.o)
BINDIR=bin
BIN=hhfi
STD=c++11

main.o_DEPENDENCIES=main.cpp main.h lexer.h
lexer.o_DEPENDENCIES=lexer.cpp lexer.h lexemes.h lexemestream.h
lexemes.o_DEPENDENCIES=lexemes.cpp lexemes.h
timer.o_DEPENDENCIES=timer.cpp timer.h

ifeq ($(BUILDTYPE), debug)
CFLAGS += -O0 -g
else 
ifeq ($(BUILDTYPE), release)
CFLAGS += -O3 -DNDEBUG
endif
endif

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

$(OBJDIR)/main.o: main.cpp main.h lexer.h
	$(CC) -c $< -o $@ -std=$(STD) $(CFLAGS)

$(OBJDIR)/interpreter.o: interpreter.cpp interpreter.h
	$(CC) -c $< -o $@ -std=$(STD) $(CFLAGS)

$(OBJDIR)/parser.o: parser.cpp parser.h 
	$(CC) -c $< -o $@ -std=$(STD) $(CFLAGS)

$(OBJDIR)/lexer.o: lexer.cpp lexer.h lexemes.h lexemestream.h
	$(CC) -c $< -o $@ -std=$(STD) $(CFLAGS)
	
$(OBJDIR)/lexemes.o: lexemes.cpp lexemes.h
	$(CC) -c $< -o $@ -std=$(STD) $(CFLAGS)

$(OBJDIR)/timer.o: timer.cpp timer.h
	$(CC) -c $< -o $@ -std=$(STD) $(CFLAGS)

clean-full: $(OBJDIR) $(BINDIR)
	rm -r $(OBJDIR) $(BINDIR)

clean: $(OBJDIR) $(BINDIR)
	rm -f $(OBJDIR)/*
	rm -f $(BINDIR)/*

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)