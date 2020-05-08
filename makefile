CC=g++
OBJDIR=build
OBJS := $(addprefix $(OBJDIR)/,main.o lexer.o lexemes.o timer.o)
BINDIR=bin
BIN=hhfi
STD=c++11

vpath %.cpp src src/lexer src/misc
vpath %.h src src/lexer src/misc

$(BINDIR)/$(BIN): $(OBJDIR) $(BINDIR) $(OBJS)
	$(CC) $(OBJDIR)/*.o -o $@

run: $(BINDIR)/$(BIN)
	./$(BINDIR)/$(BIN)

test: $(BINDIR)/$(BIN)
	./$(BINDIR)/$(BIN) testfile.txt

force: clean $(OBJS)

$(OBJDIR)/main.o: src/main.h src/main.cpp src/lexer/lexer.h
	$(CC) -c src/main.cpp -o $(OBJDIR)/main.o -std=$(STD)

$(OBJDIR)/interpreter.o: src/interpreter.h src/interpreter.cpp
	$(CC) -c src/interpreter.cpp -o $(OBJDIR)/interpreter.o -std=$(STD)

$(OBJDIR)/parser.o: src/parser.h src/parser.cpp
	$(CC) -c src/parser.cpp -o $(OBJDIR)/parser.o -std=$(STD)

$(OBJDIR)/lexer.o: src/lexer/lexemes.h src/lexer/lexer.h src/lexer/lexer.cpp  src/lexer/lexemestream.h
	$(CC) -c src/lexer/lexer.cpp -o $(OBJDIR)/lexer.o -std=$(STD)
	
$(OBJDIR)/lexemes.o: src/lexer/lexemes.h src/lexer/lexemes.cpp
	$(CC) -c src/lexer/lexemes.cpp -o $(OBJDIR)/lexemes.o -std=$(STD)

$(OBJDIR)/timer.o: src/misc/timer.h src/misc/timer.cpp
	$(CC) -c src/misc/timer.cpp -o $(OBJDIR)/timer.o -std=$(STD)

clean-full: $(OBJDIR) $(BINDIR)
	rm -r $(OBJDIR) $(BINDIR)

clean: $(OBJDIR) $(BINDIR)
	rm -f $(OBJDIR)/*
	rm -f $(BINDIR)/*

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)